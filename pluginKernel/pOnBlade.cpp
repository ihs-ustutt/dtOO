#include "pOnBlade.h"


#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParser.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
//#include <analyticFunctionHeaven/scaCurve2dOneD.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
//#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
//#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/dtPoint3_map1dTo3dPoint.h>
#include <analyticGeometryHeaven/aGBuilder/pairU_map1dTo3dClosestPointToMap1dTo3d.h>
#include <boundedVolume.h>
#include <dtCase.h>

#include <xmlHeaven/dtXmlParser.h>
#include <interfaceHeaven/systemHandling.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

#include <criticalHeaven/prepareOpenFOAM.h>
#include <argList.H>
#include <Time.H>
#include <fvMesh.H>
#include <volFields.H>

#include <logMe/dtParMacros.h>

namespace dtOO {  
  pOnBlade::pOnBlade() { 
  }

  pOnBlade::~pOnBlade() {
  }

	void pOnBlade::init( 
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
//		name="pOnBlade" 
//		label="pOnBlade"
//    field="URel"   
//    numPoints="10"
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
    // get blade
    //
    dt__ptrAss(
      _blade 
      ,
      map2dTo3d::ConstDownCast(
        aG->get( 
          dtXmlParser::getAttributeStr(
            "label", dtXmlParser::getChild("map2dTo3d", element)   
          )
        )
      )
    );
    
    //
    // get channel
    //
    dt__ptrAss(
      _stagnationLine,
      map1dTo3d::ConstDownCast(
        aG->get( 
          dtXmlParser::getAttributeStr(
            "label", dtXmlParser::getChild("map1dTo3d", element)   
          )
        )
      )
    );

    //
    // get number of points
    //
    _nP 
    = 
    dtXmlParser::getAttributeIntVectorMuParse("numPoints", element, cV, aF);   
	}
		
  void pOnBlade::apply(void) {    
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
          "p",
          runTime.timeName(),
          mesh,
          ::Foam::IOobject::MUST_READ
        );
        
        //
        // only volScalarField
        // 
        dt__throwIf( !fieldHeader.headerOk(), apply());
        dt__throwIf(fieldHeader.headerClassName() != "volScalarField", apply());
        
        //
        // update field 
        //
        mesh.readUpdate();

        //
        // read desired field
        //
        ::Foam::volScalarField volField(fieldHeader, mesh);

        dt__pVH(map1dTo3d) bladeCut;
        std::vector< float > bladeStagPoint;
        dt__forAllRefAuto( dtLinearAlgebra::unitGrid(_nP[1]), vCut ) {
          bladeCut.push_back( _blade->segmentConstVPercent(vCut) );
          std::pair< float, float > pairU 
          = 
          pairU_map1dTo3dClosestPointToMap1dTo3d(
            _stagnationLine, &(bladeCut.back()) 
          ).result();
          dt__info(
            apply(),
            << "distance stagPoint on currentBladeCut = " 
            << dtLinearAlgebra::length(
              _stagnationLine->getPoint(pairU.first)
              -
              bladeCut.back().getPoint(pairU.second)
            )
          );
          bladeStagPoint.push_back( bladeCut.back().l_u( pairU.second ) );
    
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
        _blade->getLabel()+"_"+fieldHeader.name()+".csv";
        std::fstream of;
        of.open( filename.c_str(), std::ios::out | std::ios::trunc );
          
        //
        // write header
        //
        of 
        << logMe::dtFormat("# %16i %16i") % _nP[0] % _nP[1] << std::endl
        << logMe::dtFormat("# %16s %16s") % "length" % "value"
        << std::endl;
        
        int cc = 0;
        dt__forAllRefAuto(bladeCut, aBladeCut) {
          //
          // create vector and value grid
          //
          std::vector< float > grid = dtLinearAlgebra::unitGrid(_nP[0]);
          vectorHandling< float > value(grid.size());
          vectorHandling< float > ll(grid.size());

          //
          // get values
          //
          dt__forAllIndex(grid, ii) {
            dtPoint3 const & xyz = aBladeCut.getPointPercent( grid[ii] );
            ;
            ::Foam::label cId 
            = 
            mesh.findNearestCell( ::Foam::vector(xyz.x(), xyz.y(), xyz.z()) );

            if (cId == -1) {
              dt__warning(
                apply(), 
                << "Cannot find " << dt__point3d(xyz) << " in mesh."
              );
              continue;
            }
            value[ii] = volField[ cId ];
            ll[ii] = aBladeCut.l_u( aBladeCut % grid[ii] ) - bladeStagPoint[cc];
          }
                   
          //
          // write values
          //
          dt__forFromToIndex(0, value.size(), ii) {
            of 
            << logMe::dtFormat("%16.8e %16.8e") % ll[ii] % value[ii] 
            << std::endl;
          }
          //
          // gnuplot block
          //
          of << std::endl << std::endl;
          cc++;
        }
        of.close();        
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


