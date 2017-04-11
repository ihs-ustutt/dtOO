#include "aGXmlBuilderFactory.h"

#include "aGXmlBuilder.h"
#include <logMe/logMe.h>
#include <string.h>
#include "aGXmlBuilder/baseContainerAGXmlBuilder.h"
#include "aGXmlBuilder/analyticGeometryAGXmlBuilder.h"
#include "aGXmlBuilder/map2dTo3dGroupAGXmlBuilder.h"
#include "aGXmlBuilder/map3dTo3dGroupAGXmlBuilder.h"
#include "aGXmlBuilder/trans6SidedCubeAGXmlBuilder.h"
#include "aGXmlBuilder/scaInMap1dTo3dAGXmlBuilder.h"
#include "aGXmlBuilder/vec2dInMap2dTo3dAGXmlBuilder.h"
#include "aGXmlBuilder/vec3dInMap3dTo3dAGXmlBuilder.h"
#include "aGXmlBuilder/rotatingMap2dTo3dAGXmlBuilder.h"
#include "aGXmlBuilder/rotatingMap1dTo3dAGXmlBuilder.h"
#include "aGXmlBuilder/infinityMap3dTo3dAGXmlBuilder.h"
#include "aGXmlBuilder/makeCompoundAGXmlBuilder.h"
#include "aGXmlBuilder/vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder.h"
#include "aGXmlBuilder/map2dTo3dApproximateInMap3dTo3dAGXmlBuilder.h"
#include "aGXmlBuilder/bezierCurve_pointConstructOCCAGXmlBuilder.h"
#include "aGXmlBuilder/bSplineCurve_curveConnectConstructOCCAGXmlBuilder.h"
#include "aGXmlBuilder/bSplineSurface_skinConstructOCCAGXmlBuilder.h"
#include "aGXmlBuilder/bSplineSurface_pipeConstructOCCAGXmlBuilder.h"
#include "aGXmlBuilder/surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder.h"
#include "aGXmlBuilder/bezierCurvePointPickFromPartConstructOCCAGXmlBuilder.h"
#include "aGXmlBuilder/bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder.h"
#include "aGXmlBuilder/bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder.h"
#include "aGXmlBuilder/bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder.h"
#include "aGXmlBuilder/bSplineCurve_pointConstructOCCAGXmlBuilder.h"
#include "aGXmlBuilder/bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder.h"
#include "aGXmlBuilder/bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder.h"
#include "aGXmlBuilder/bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder.h"
#include "aGXmlBuilder/bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder.h"
#include "aGXmlBuilder/vec2dOneDInMap2dTo3d_closestCurveAGXmlBuilder.h"
#include "aGXmlBuilder/geomSurface_readStepOCCAGXmlBuilder.h"
#include "aGXmlBuilder/geomSurface_readStlOCCAGXmlBuilder.h"
#include "aGXmlBuilder/geomShape_readIgesOCCAGXmlBuilder.h"
#include "aGXmlBuilder/bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder.h"
#include "aGXmlBuilder/geomCurve_map2dTo3dIntersectAGXmlBuilder.h"
#include "aGXmlBuilder/circle_radiusCoordinateSystemOCCAGXmlBuilder.h"
#include "aGXmlBuilder/ellipse_radiusRadiusCoordinateSystemOCCAGXmlBuilder.h"

namespace dtOO {
  dt__pH(aGXmlBuilderFactory) aGXmlBuilderFactory::_instance(NULL);
  
  aGXmlBuilderFactory::~aGXmlBuilderFactory() {
    _builder.destroy();    
  }

  aGXmlBuilder * aGXmlBuilderFactory::create(char const * const str) {
    return create( std::string(str) );
  }

  aGXmlBuilder* aGXmlBuilderFactory::create( std::string const str ) {
    dt__forAllRefAuto( instance()->_builder, aBuilder ) {
      //
      // check virtual class name
      //
      if ( aBuilder->virtualClassName() == str ) {
        return aBuilder->create();
      }
      
      //
      // check virtual class name without postFix
      //
      if ( 
        stringPrimitive::replaceStringInString(
          "AGXmlBuilder", "", aBuilder->virtualClassName() 
        ) 
        == 
        str
      ) {
        return aBuilder->create();
      }
      
      //
      // check alias
      //
      dt__forAllRefAuto(aBuilder->factoryAlias(), anAlias) {
        if ( anAlias == str ) return aBuilder->create();
      }
    }

    std::vector< std::string > av;
    dt__forAllRefAuto( instance()->_builder, aBuilder ) {
      av.push_back( aBuilder->virtualClassName() );
      dt__forAllRefAuto(aBuilder->factoryAlias(), anAlias) {
        av.push_back("  -> "+anAlias); 
      }      
    }
    dt__throw(
      create(), 
      << str <<  " could not be created." << std::endl
      << "Implemented builder:" << std::endl
      << logMe::vecToString(av,1) << std::endl
    );          
  }

  aGXmlBuilderFactory * aGXmlBuilderFactory::instance( void ) {
    if ( !_instance.isNull() ) return _instance.get();
    
    _instance.reset( new aGXmlBuilderFactory() );
    
    //
    // add builder
    //
    _instance->_builder.push_back( new baseContainerAGXmlBuilder() );
    _instance->_builder.push_back( new analyticGeometryAGXmlBuilder() );
    _instance->_builder.push_back( new map2dTo3dGroupAGXmlBuilder() );
    _instance->_builder.push_back( new map3dTo3dGroupAGXmlBuilder() );
    _instance->_builder.push_back( new trans6SidedCubeAGXmlBuilder() );
    _instance->_builder.push_back( new scaInMap1dTo3dAGXmlBuilder() );
    _instance->_builder.push_back( new vec2dInMap2dTo3dAGXmlBuilder() );
    _instance->_builder.push_back( new vec3dInMap3dTo3dAGXmlBuilder() );
    _instance->_builder.push_back( new rotatingMap2dTo3dAGXmlBuilder() );
    _instance->_builder.push_back( new rotatingMap1dTo3dAGXmlBuilder() );
    _instance->_builder.push_back( new infinityMap3dTo3dAGXmlBuilder() );
    _instance->_builder.push_back( new makeCompoundAGXmlBuilder() );
    _instance->_builder.push_back( 
      new vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder() 
    );
    _instance->_builder.push_back( 
      new map2dTo3dApproximateInMap3dTo3dAGXmlBuilder() 
    );
    _instance->_builder.push_back( 
      new bezierCurve_pointConstructOCCAGXmlBuilder() 
    );
    _instance->_builder.push_back( 
      new bSplineCurve_curveConnectConstructOCCAGXmlBuilder() 
    );
    _instance->_builder.push_back( 
      new bSplineSurface_skinConstructOCCAGXmlBuilder() 
    );
    _instance->_builder.push_back( 
      new bSplineSurface_pipeConstructOCCAGXmlBuilder()
    );
    _instance->_builder.push_back( 
      new surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder() 
    );
    _instance->_builder.push_back( 
      new bezierCurvePointPickFromPartConstructOCCAGXmlBuilder() 
    );
    _instance->_builder.push_back( 
      new bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder() 
    );
    _instance->_builder.push_back( 
      new bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder() 
    );
    _instance->_builder.push_back( 
      new bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder() 
    );
    _instance->_builder.push_back( 
      new bSplineCurve_pointConstructOCCAGXmlBuilder() 
    );
    _instance->_builder.push_back( 
      new bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder() 
    );
    _instance->_builder.push_back( 
      new bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder() 
    );
    _instance->_builder.push_back( 
      new bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder() 
    );
    _instance->_builder.push_back( 
      new bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder() 
    );
    _instance->_builder.push_back( 
      new vec2dOneDInMap2dTo3d_closestCurveAGXmlBuilder() 
    );
    _instance->_builder.push_back( new geomSurface_readStepOCCAGXmlBuilder() );
    _instance->_builder.push_back( new geomSurface_readStlOCCAGXmlBuilder() );
    _instance->_builder.push_back( new geomShape_readIgesOCCAGXmlBuilder() );
    _instance->_builder.push_back( 
      new bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder() 
    );
    _instance->_builder.push_back( 
      new geomCurve_map2dTo3dIntersectAGXmlBuilder() 
    );
    _instance->_builder.push_back(
      new circle_radiusCoordinateSystemOCCAGXmlBuilder()
    );
    _instance->_builder.push_back(
      new ellipse_radiusRadiusCoordinateSystemOCCAGXmlBuilder()
    );
    
    return _instance.get();
  }  

  aGXmlBuilderFactory::aGXmlBuilderFactory() {
  }  
}
