#include "pOnBlade.h"

#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParser.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
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
#include <interpolationCellPoint.H>

#include <logMe/dtParMacros.h>
#include "dtPluginFactory.h"
#include <meshEngine/dtFoamLibrary.h>

namespace dtOO {  
  bool pOnBlade::_registrated 
  =
  dtPluginFactory::registrate( dt__tmpPtr(pOnBlade, new pOnBlade()) );
  
  pOnBlade::pOnBlade() { 
  }

  pOnBlade::~pOnBlade() {
  }

  void pOnBlade::init( 
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
    _parser = dtXmlParser::ptr();
    
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
        dtFoamLibrary::unsetFpe();

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

        //
        // interpolation
        //
        ::Foam::interpolationCellPoint< ::Foam::scalar > 
        anInterpolation(volField);
        
        dt__pVH(map1dTo3d) bladeCut;
        std::vector< dtReal > percent_bladeStagPoint;
        dt__forAllRefAuto( dtLinearAlgebra::unitGrid(_nP[1]), vCut ) {
          bladeCut.push_back( _blade->segmentConstVPercent(vCut) );
          std::pair< dtReal, dtReal > pairU 
          = 
          pairU_map1dTo3dClosestPointToMap1dTo3d(
            _stagnationLine, &(bladeCut.back()) 
          ).result();
          percent_bladeStagPoint.push_back( 
            bladeCut.back().percent_u( pairU.second ) 
          );
          dt__info(
            apply(),
            << "uPercent_stagPoint on currentBladeCut = " 
            << percent_bladeStagPoint.back() << "."
          );          
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
        << "# 1 i" << std::endl
        << "# 2 j" << std::endl
        << "# 3 length" << std::endl
        << "# 4 value" << std::endl;
        
        int cc = 0;
        dt__forAllRefAuto(bladeCut, aBladeCut) {
          //
          // create vector and value grid
          //
          std::vector< dtReal > grid = dtLinearAlgebra::unitGrid(_nP[0]);
          vectorHandling< dtReal > value(grid.size());
          vectorHandling< dtReal > ll(grid.size());

          //
          // get values
          //
          dtPoint3 xyz0 = aBladeCut.getPointPercent( grid[0] );          
          dtReal uStagCorrect = 0.;
          int uStagCorrectIndex = 0;
          dt__forAllIndex(grid, ii) {            
            dtPoint3 const & xyz1 = aBladeCut.getPointPercent( grid[ii] );
            
            ::Foam::label cId 
            = 
            mesh.findNearestCell( ::Foam::vector(xyz1.x(), xyz1.y(), xyz1.z()) );

            if (cId == -1) {
              dt__warning(
                apply(), 
                << "Cannot find " << dt__point3d(xyz1) << " in mesh."
              );
              continue;
            }
            
//            value[ii] = volField[ cId ];            
            //
            // do interpolation
            //            
            value[ii]
            =
            anInterpolation.interpolate(
              ::Foam::vector(xyz1.x(), xyz1.y(), xyz1.z()), cId
            );
            ll[ii] = dtLinearAlgebra::distance(xyz1, xyz0);
            
            xyz0 = xyz1;            
            
            //
            // stagnation correction
            //
            if ( 
              ( uStagCorrectIndex == 0 )
              &&
              (grid[ii] >= percent_bladeStagPoint[cc]) 
            ) {
              uStagCorrect = percent_bladeStagPoint[cc] / grid[ii-1];
              uStagCorrectIndex = ii-1;
              dt__info(
                apply(),
                << logMe::dtFormat(
                  "Stagnation point correction between %4.3f and %4.3f\n"
                  "l = %f and l = %f\n"
                  "Set uStagCorrect = %f. and uStagCorrectIndex = %d"
                ) % grid[ii-1] % grid[ii] % ll[ii-1] % ll[ii] % uStagCorrect % uStagCorrectIndex
              );                 
            }            
          }
          dt__forFromToIndex(1, ll.size(), ii) {
            ll[ii] = ll[ii-1] + ll[ii];
          }          
          //
          // write values
          //
          dtReal lStagCorrect = uStagCorrect * ll[uStagCorrectIndex];
          dt__forFromToIndex(0, value.size(), ii) {
            of 
            << logMe::dtFormat("%16d, %16d, %16.8e, %16.8e") 
              % ii % cc % ( ll[ii] - lStagCorrect ) % value[ii] 
            << std::endl;
          }
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


