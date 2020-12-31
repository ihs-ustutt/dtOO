#include "meshQuality.h"

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
#include <interfaceHeaven/barChart.h>

#include <criticalHeaven/prepareOpenFOAM.h>
#include <argList.H>
#include <Time.H>
#include <polyMesh.H>
#include <cellList.H>
#include <cellShapeList.H>
#include <cellQuality.H>
#include <volFields.H>

#include <logMe/dtParMacros.h>

namespace dtOO {  
  meshQuality::meshQuality() { 
  }

  meshQuality::~meshQuality() {
  }

  void meshQuality::init( 
    ::QDomElement const & element,
    baseContainer * const bC,
    cVPtrVec const * const cV,
    aFPtrVec const * const aF,
    aGPtrVec const * const aG,
    bVPtrVec const * const bV,
    dCPtrVec const * const dC,
    dPPtrVec const * const pL
  ) {   
    dtPlugin::init(element, bC, cV, aF, aG, bV, dC, pL);
    
//	<plugin 
//		name="meshQuality" 
//		label="meshQuality"
//	>
//    <case label="myCase"/>
//  </plugin>  
    dt__throwIf( !dtXmlParser::hasChild("case", element), init() );
    
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
    _parser = dtXmlParser::ptr();
	}
		
  void meshQuality::apply(void) {
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
        ::Foam::fvMesh mesh(
          Foam::IOobject(
            ::Foam::fvMesh::defaultRegion,
            runTime.timeName(),
            runTime,
            ::Foam::IOobject::MUST_READ
          )
        );

        //
        // create quality
        //
        ::Foam::cellQuality cQ( mesh );        
        
        ::Foam::volScalarField skew(
          ::Foam::IOobject(
            "skew",
            runTime.timeName(),
            runTime,
            ::Foam::IOobject::NO_READ,
            ::Foam::IOobject::AUTO_WRITE
          ),
          mesh,
          ::Foam::dimless
        );
        ::Foam::volScalarField nonOrthogonality(
          ::Foam::IOobject(
            "nonOrthogonality",
            runTime.timeName(),
            runTime,
            ::Foam::IOobject::NO_READ,
            ::Foam::IOobject::AUTO_WRITE
          ),
          mesh,
          ::Foam::dimless
        );
        
        skew.internalField() = cQ.skewness();
        nonOrthogonality.internalField() = cQ.nonOrthogonality();
        
        skew.write();
        nonOrthogonality.write();
        
        ::Foam::dimensionedScalar minSkew(0.);
        ::Foam::dimensionedScalar maxSkew(0.);
        minSkew = ::Foam::min(skew.internalField());
        maxSkew = ::Foam::max(skew.internalField());
        ::Foam::dimensionedScalar minNonOrto(0.);
        ::Foam::dimensionedScalar maxNonOrto(0.);
        minNonOrto = ::Foam::min(nonOrthogonality.internalField());
        maxNonOrto = ::Foam::max(nonOrthogonality.internalField());
        
        barChart bcSkew( 
          "skew", minSkew.value(), maxSkew.value(), 10 
        );
        barChart bcNonOrtho( 
          "nonOrtho", minNonOrto.value(), maxNonOrto.value(), 10 
        );
        std::vector< barChart > bcElemSkew(
          4, 
          barChart("bcElemSkew", minSkew.value(), maxSkew.value(), 10)
        );
        std::vector< barChart > bcElemNonOrtho(
          4, 
          barChart("bcElemNonOrtho", minNonOrto.value(), maxNonOrto.value(), 10)
        );
        ::Foam::cellList const & cL = mesh.cells();
        ::Foam::cellShapeList const & cSL = mesh.cellShapes();
        forAll(cL, ii) {
//          dt__info( 
//            apply(), << dt__eval(cSL[ ii ].model().index())
//           );
//          cSL[ii];
          bcSkew( skew[ ii ] );
          bcNonOrtho( nonOrthogonality[ ii ] );
          // hex
          if (cSL[ ii ].model().index() == 3) {
            bcElemSkew[0]( skew[ ii ] );
            bcElemNonOrtho[0]( nonOrthogonality[ ii ] );
          }
          // prism
          else if (cSL[ ii ].model().index() == 5) {
            bcElemSkew[1]( skew[ ii ] );
            bcElemNonOrtho[1]( nonOrthogonality[ ii ] );            
          }
          // pyr
          else if (cSL[ ii ].model().index() == 6) {
            bcElemSkew[2]( skew[ ii ] );
            bcElemNonOrtho[2]( nonOrthogonality[ ii ] );           
          }
          // tet
          else if (cSL[ ii ].model().index() == 7) {
            bcElemSkew[3]( skew[ ii ] );
            bcElemNonOrtho[3]( nonOrthogonality[ ii ] );            
          }
        }
        dt__info(
          apply(), 
          << "Overall" << std::endl
          << bcSkew << std::endl
          << bcNonOrtho << std::endl
          << "Hex" << std::endl
          << bcElemSkew[0] << std::endl
          << bcElemNonOrtho[0] << std::endl
          << "Prism" << std::endl
          << bcElemSkew[1] << std::endl
          << bcElemNonOrtho[1] << std::endl
          << "Pyr" << std::endl
          << bcElemSkew[2] << std::endl
          << bcElemNonOrtho[2] << std::endl
          << "Tet" << std::endl
          << bcElemSkew[3] << std::endl
          << bcElemNonOrtho[3]
        );
        
        //
        // open file
        //
        std::string filename 
        = 
        _case->getDirectory( _parser->currentState() )
        +
        "/"
        +
        "meshQuality.csv";
        
        std::fstream of;
        of.open( filename.c_str(), std::ios::out | std::ios::trunc );        
        
        //
        // write header
        //
        of
        << "# 1 min(skew)" << std::endl
        << "# 2 mean(skew)" << std::endl
        << "# 3 max(skew) " << std::endl
        << "# 4 min(nonOrtho)" << std::endl
        << "# 5 mean(nonOrtho)" << std::endl
        << "# 6 max(nonOrtho)" << std::endl
        << "# [1] type - (all)" << std::endl
        << "# [2] type 3 (hex)" << std::endl
        << "# [3] type 5 (prism)" << std::endl
        << "# [4] type 6 (pyr)" << std::endl
        << "# [5] type 7 (tet)" << std::endl;

        
        of 
        << logMe::dtFormat("%16.8e, %16.8e, %16.8e, %16.8e, %16.8e, %16.8e") 
          % bcSkew.globalMin() 
          % bcSkew.average() 
          % bcSkew.globalMax()
          % bcNonOrtho.globalMin() 
          % bcNonOrtho.average() 
          % bcNonOrtho.globalMax()
        << std::endl
        << logMe::dtFormat("%16.8e, %16.8e, %16.8e, %16.8e, %16.8e, %16.8e") 
          % bcElemSkew[0].globalMin() 
          % bcElemSkew[0].average() 
          % bcElemSkew[0].globalMax()
          % bcElemNonOrtho[0].globalMin() 
          % bcElemNonOrtho[0].average() 
          % bcElemNonOrtho[0].globalMax()      
        << std::endl
        << logMe::dtFormat("%16.8e, %16.8e, %16.8e, %16.8e, %16.8e, %16.8e") 
          % bcElemSkew[1].globalMin() 
          % bcElemSkew[1].average() 
          % bcElemSkew[1].globalMax()
          % bcElemNonOrtho[1].globalMin() 
          % bcElemNonOrtho[1].average() 
          % bcElemNonOrtho[1].globalMax()      
        << std::endl        
        << logMe::dtFormat("%16.8e, %16.8e, %16.8e, %16.8e, %16.8e, %16.8e") 
          % bcElemSkew[2].globalMin() 
          % bcElemSkew[2].average() 
          % bcElemSkew[2].globalMax()
          % bcElemNonOrtho[2].globalMin() 
          % bcElemNonOrtho[2].average() 
          % bcElemNonOrtho[2].globalMax()      
        << std::endl                    
        << logMe::dtFormat("%16.8e, %16.8e, %16.8e, %16.8e, %16.8e, %16.8e") 
          % bcElemSkew[3].globalMin() 
          % bcElemSkew[3].average() 
          % bcElemSkew[3].globalMax()
          % bcElemNonOrtho[3].globalMin() 
          % bcElemNonOrtho[3].average() 
          % bcElemNonOrtho[3].globalMax();            
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


