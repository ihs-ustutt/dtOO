#include "volScalarFieldRange.h"

#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParser.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/dtPoint3_map1dTo3dPoint.h>
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
//#include <cellList.H>

#include <logMe/dtParMacros.h>

namespace dtOO {  
  volScalarFieldRange::volScalarFieldRange() { 
  }

  volScalarFieldRange::~volScalarFieldRange() {
  }

	void volScalarFieldRange::init( 
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
//		name="volScalarFieldRange" 
//		label="volScalarFieldRange"
//    field="p"   
//    min="-10."
//    max="10."
//	>
//    <case label="myCase"/>
//  </plugin>  
    dt__throwIf(
      !dtXmlParser::hasChild("case", element)
      &&
      !dtXmlParser::hasAttribute("field", element)
      &&
      !dtXmlParser::hasAttribute("min", element)
      &&
      !dtXmlParser::hasAttribute("max", element), 
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
    // get field label
    //
    _field = dtXmlParser::getAttributeStr("field", element);

    //
    // get min and max
    //
    _min = dtXmlParser::getAttributeFloatMuParse("min", element, cV, aF);
    _max = dtXmlParser::getAttributeFloatMuParse("max", element, cV, aF);
	}
		
  void volScalarFieldRange::apply(void) {    
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
        
        std::vector< dtPoint3 > pXYZ;
        std::vector< float > val;
        std::vector< float > vol;
        //
        // cell list
        //
        forAll(mesh.cells(), ii) {
          float cVal = volField[ ii ];
          if ( (cVal > _min) && (cVal < _max) ) {
            dtPoint3 cPoint( 
              mesh.C()[ ii ].component(0), 
              mesh.C()[ ii ].component(1), 
              mesh.C()[ ii ].component(2) 
            );
            pXYZ.push_back( cPoint );
            val.push_back( cVal );
            vol.push_back( mesh.V()[ ii ] );
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
        virtualClassName()+"_"+getLabel()+"_"+fieldHeader.name()+".csv";
        std::fstream of;
        of.open( filename.c_str(), std::ios::out | std::ios::trunc );

        //
        // write header
        //
        of 
        << "# 1 x" << std::endl
        << "# 2 y" << std::endl
        << "# 3 z" << std::endl
        << "# 4 value" << std::endl
        << "# 5 volume" << std::endl;

        //
        // write values
        //
        dt__forFromToIndex(0, pXYZ.size(), ii) {
          of 
          << logMe::dtFormat("%16.8e, %16.8e, %16.8e, %16.8e, %16.8e") 
            % pXYZ[ii].x() 
            % pXYZ[ii].y() 
            % pXYZ[ii].z() 
            % val[ii]
            % vol[ii]
          << std::endl;
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