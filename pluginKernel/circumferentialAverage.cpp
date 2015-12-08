#include "circumferentialAverage.h"

#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParser.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/dtPoint3_map2dTo3dPoint.h>
#include <boundedVolume.h>
#include <dtCase.h>

#include <xmlHeaven/dtXmlParser.h>
#include <interfaceHeaven/systemHandling.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

#include <criticalHeaven/prepareOpenFOAM.h>
#include <argList.H>
#include <Time.H>
#include <polyMesh.H>
#include <volFields.H>

#include <logMe/dtParMacros.h>

namespace dtOO {  
  circumferentialAverage::circumferentialAverage() { 
  }

  circumferentialAverage::~circumferentialAverage() {
  }

	void circumferentialAverage::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG,
		vectorHandling< boundedVolume * > const * const bV,
    vectorHandling< dtCase * > const * const dC,
		vectorHandling< dtPlugin * > const * const pL
	) {   
	  dtPlugin::init(element, bC, cV, aF, aG, bV, dC, pL);
    
//	<plugin 
//		name="circumferentialAverage" 
//		label="circumferentialAverage"
//    field="URel"   
//    numPoints="{10}{10}"
//	>
//    <case label="myCase"/>
//    <analyticGeometry label="myAnalyticGeometry"
//  </plugin>  
    dt__throwIf(
      !dtXmlParser::hasChild("case", element)
      &&
      !dtXmlParser::hasChild("analyticGeometry", element)
      &&
      !dtXmlParser::hasAttribute("field", element)
      &&
      !dtXmlParser::hasAttribute("numPoints", element), 
      init()
    );
    
    //
    // get parser and case
    //
    _case 
    = 
    dC->get(
      dtXmlParser::getAttributeStr(
        "label", dtXmlParser::getChild("case", element)
      )
    );
    dt__throwIf(cV->empty(), init());
    _parser = &(cV->get(0)->constRefParser());
    
    //
    // get analyticGeometries
    //
    dt__forAllRefAuto(
      dtXmlParser::getChildVector("analyticGeometry", element), anEl
    ) {
      _aG.push_back( 
        aG->get( dtXmlParser::getAttributeStr("label", anEl) )
      );
    }
    
    //
    // get field label
    //
    _field = dtXmlParser::getAttributeStr("field", element);

    //
    // get number of points
    //
    _nP 
    = 
    dtXmlParser::getAttributeIntVectorMuParse("numPoints", element, cV, aF);    
	}
		
  void circumferentialAverage::apply(void) {    
    //
    // get directory
    //
    std::string wDir = _case->getDirectory( _parser->currentState() );

    dt__onlyMaster {
      dt__throwIf( !systemHandling::directoryExists(wDir), apply() );
    
      //
      // enable exception throwing
      //
      ::Foam::FatalError.throwExceptions();    
      ::Foam::FatalIOError.throwExceptions();    

      //
      // argList
      //
      ::Foam::argList::noParallel();
      int argc = 3;
      std::vector< std::string > argvStr(3);
      argvStr[0] = getLabel();
      argvStr[1] = std::string("-case");
      argvStr[2] = wDir;
      char ** argv = new char*[3];
      argv[0] = const_cast< char *>(argvStr[0].c_str());
      argv[1] = const_cast< char *>(argvStr[1].c_str());
      argv[2] = const_cast< char *>(argvStr[2].c_str());

      try {
        // disable floating point exception trapping
        systemHandling::unsetEnv("FOAM_SIGFPE");

        //
        // create rootCase
        //
        ::Foam::argList args(argc, argv);
        if (!args.checkRootCase()) {
          Foam::FatalError.exit();
        }

        //
        // create time
        //
        ::Foam::Time runTime(
          Foam::Time::controlDictName,
          args.rootPath(),
          args.caseName(),
          "system",
          "constant",
          !args.optionFound("noFunctionObjects")
        );
        
        //
        // create mesh
        //
        Foam::fvMesh mesh(
          Foam::IOobject(
            Foam::fvMesh::defaultRegion,
            runTime.timeName(),
            runTime,
            Foam::IOobject::MUST_READ
          )
        );
        
        //
        // lastTime
        //
        runTime.setTime(runTime.times().last(), runTime.times().size()-1);
        
        //
        // read field header
        //
        ::Foam::IOobject fieldHeader(
          _field,
          runTime.timeName(),
          mesh,
          ::Foam::IOobject::MUST_READ
        );
        
        //
        // currently only volVectorField supported
        // 
        dt__throwIf( !fieldHeader.headerOk(), apply());
        dt__throwIf(fieldHeader.headerClassName() != "volVectorField", apply());
        
        //
        // update field 
        //
        mesh.readUpdate();

        //
        // read desired field
        //
        ::Foam::volVectorField volField(fieldHeader, mesh);

        //
        // do circumferential averaging
        //
        dt__forAllRefAuto(_aG, anAG) {
          //
          // only two dimensional mappings
          //
          dt__ptrAss(map2dTo3d const * m2d, map2dTo3d::ConstDownCast(anAG));

          //
          // create surface and value grid
          //
          twoDArrayHandling< dtPoint3 > grid 
          = 
          dtPoint3_map2dTo3dPoint(m2d, _nP[0], _nP[1]).result();
          twoDArrayHandling< dtVector3 > value(grid.size(0), grid.size(1));
          twoDArrayHandling< float > vol(grid.size(0), grid.size(1));

          //
          // get values
          //
          dt__forAllIndex(grid, ii) {
            dt__forAllIndex(grid[ii], jj) {
              dtPoint3 const & xyz = grid[ii][jj];
              ::Foam::vector probePoint(xyz.x(), xyz.y(), xyz.z());
              ::Foam::label cId = mesh.findCell(probePoint);
              vol[ii][jj] = mesh.V()[ cId ];
              ::Foam::vector ofValue = vol[ii][jj] * volField[ cId ];
              value[ii][jj] = dtVector3( ofValue.x(), ofValue.y(), ofValue.z() );
            }
          }

          //
          // open file
          //
          std::string filename 
          = 
          _case->getDirectory( _parser->currentState() )
          +
          "/"
          +
          m2d->getLabel()+"_"+fieldHeader.name()+".csv";
          std::fstream of;
          of.open( filename.c_str(), std::ios::out | std::ios::trunc );
          
          //
          // write header
          //
          of 
          << logMe::dtFormat("# %16i %16i") % _nP[0] % _nP[1] 
          << std::endl;          
          
          //
          // write values
          //
          dt__forFromToIndex(0, value[0].size(), jj) {
            dtVector3 av 
            = 
            dtLinearAlgebra::sum( value.fixJ(jj) ) 
            / 
            dtLinearAlgebra::sum( vol.fixJ(jj) );
            of 
            << logMe::dtFormat("%16.8e %16.8e %16.8e") 
              % av.x() % av.y() % av.z()
            << std::endl;
          }          
          of.close();
        }
      }
      catch (::Foam::error & err) {
        dt__throw(
          apply(), 
          << "Instance of ::Foam::error thrown." << std::endl
          << dt__eval(err.what()) << std::endl
          << dt__eval(err.message())
        );
      }      
    }    
  }
}


