#include "helloWorldTransformer.h"

#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <xmlHeaven/qtXmlBase.h>

namespace dtOO {
  helloWorldTransformer::helloWorldTransformer() : dtTransformerInvThreeD() {
  }

  helloWorldTransformer::~helloWorldTransformer() {
  }

  helloWorldTransformer::helloWorldTransformer(
    helloWorldTransformer const & orig
  ) : dtTransformerInvThreeD(orig) {
    _float = orig._float;
  }
   
  helloWorldTransformer * helloWorldTransformer::clone( void ) const {
    return new helloWorldTransformer( *this );  
  }
  
  helloWorldTransformer * helloWorldTransformer::create( void ) const {
    return new helloWorldTransformer();  
  }
  
  void helloWorldTransformer::init( 
    ::QDomElement const * tE, 
    baseContainer * const bC,
    cVPtrVec const * const cV,
    aFPtrVec const * const aF,
    aGPtrVec const * const aG 
  ) {
    dtTransformer::init( tE, bC, cV, aF, aG );

    dt__info(
      init(), 
      << "Init helloWorldTransformer"
    );    
    
    dt__throwIf( !qtXmlBase::hasAttribute( "float", *tE ), init() );
    _float = qtXmlBase::getAttributeFloatMuParse( "float", *tE, cV, aF );
  }

  bool helloWorldTransformer::isNecessary( void ) const {
    return true;
  }
  
  std::vector< dtPoint3 > helloWorldTransformer::apply( 
    std::vector< dtPoint3 > const * const toTrans 
  ) const {
    std::vector< dtPoint3 > ret;
    dt__forAllRefAuto(*toTrans, aTrans) {
      ret.push_back( 
        dtPoint3( 
          _float * aTrans.x(), 
          _float * aTrans.y(), 
          _float * aTrans.z() 
        ) 
      );
    }
    
    return ret;
  }
  
  std::vector< dtPoint3 > helloWorldTransformer::retract( 
    std::vector< dtPoint3 > const * const toTrans 
  ) const {
    std::vector< dtPoint3 > ret;
    dt__forAllRefAuto(*toTrans, aTrans) {
      ret.push_back( 
        dtPoint3( 
          (1./_float) * aTrans.x(), 
          (1./_float) * aTrans.y(), 
          (1./_float) * aTrans.z() 
        ) 
      );
    }
    
    return ret;
  }  

  std::vector< dtVector3 > helloWorldTransformer::apply( 
    std::vector< dtVector3 > const * const toTrans 
  ) const {
    std::vector< dtVector3 > ret;
    dt__forAllRefAuto(*toTrans, aTrans) {
      ret.push_back( 
        dtVector3( 
          _float * aTrans.x(), 
          _float * aTrans.y(), 
          _float * aTrans.z() 
        ) 
      );
    }
    
    return ret;
  }
  
  std::vector< dtVector3 > helloWorldTransformer::retract( 
    std::vector< dtVector3 > const * const toTrans 
  ) const {
    std::vector< dtVector3 > ret;
    dt__forAllRefAuto(*toTrans, aTrans) {
      ret.push_back( 
        dtVector3( 
          (1./_float) * aTrans.x(), 
          (1./_float) * aTrans.y(), 
          (1./_float) * aTrans.z() 
        ) 
      );
    }
    
    return ret;
  }  
  
  aGPtrVec helloWorldTransformer::apply( aGPtrVec const * const toTrans ) const {
    aGPtrVec retAGeo;
    dt__forAllRefAuto(*toTrans, aG) {
  		//
			// clone and cast analyticGeometry
			//
      map1dTo3d const * const m1d = map1dTo3d::ConstDownCast(aG);      
      map2dTo3d const * const m2d = map2dTo3d::ConstDownCast(aG);
      map3dTo3d const * const m3d = map3dTo3d::ConstDownCast(aG);
      
      if (m1d) {
        retAGeo.push_back( m1d->cloneTransformed(this) );
      }
			else if (m2d) {
        retAGeo.push_back( m2d->cloneTransformed(this) );
      }
			else if (m3d) {
        retAGeo.push_back( m3d->cloneTransformed(this) );
      }
    }
    
    return retAGeo;
  }
}

//
// pugg driver
//
helloWorldTransformerDriver::helloWorldTransformerDriver() : dtTransformerDriver(
  std::string("helloWorldTransformerDriver"), dtOO::dtTransformer::version
) {

}

//
// pugg plugin creation via driver
//
dtOO::dtTransformer * helloWorldTransformerDriver::create() {
  return new dtOO::helloWorldTransformer();
}

//
// registrate plugin in driver
//
#include <pluginEngine/pugg/Kernel.h>
extern "C" void register_pugg_plugin(pugg::Kernel* kernel) {
  kernel->add_driver(new helloWorldTransformerDriver());
}


