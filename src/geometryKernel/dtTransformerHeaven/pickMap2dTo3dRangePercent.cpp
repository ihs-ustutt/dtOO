#include "pickMap2dTo3dRangePercent.h"

#include "dtTransformerFactory.h"
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>

namespace dtOO {
  bool pickMap2dTo3dRangePercent::_registrated 
  =
  dtTransformerFactory::registrate(
    dt__tmpPtr(pickMap2dTo3dRangePercent, new pickMap2dTo3dRangePercent())
  );
  
  pickMap2dTo3dRangePercent::pickMap2dTo3dRangePercent() : dtTransformer() {
  }

  pickMap2dTo3dRangePercent
    ::pickMap2dTo3dRangePercent(const pickMap2dTo3dRangePercent& orig)
    : dtTransformer(orig) {
  }

  pickMap2dTo3dRangePercent::pickMap2dTo3dRangePercent( 
    jsonPrimitive const & jE 
  ) : dtTransformer(jE) {
    this->jInit(jE, NULL, NULL, NULL, NULL);
  }
  
  pickMap2dTo3dRangePercent::~pickMap2dTo3dRangePercent() {
  }

  dtTransformer * pickMap2dTo3dRangePercent::clone( void ) const {
	  return new pickMap2dTo3dRangePercent(*this);
	}
	
  dtTransformer * pickMap2dTo3dRangePercent::create( void ) const {
		return new pickMap2dTo3dRangePercent();
	}
	
  aGPtrVec pickMap2dTo3dRangePercent::apply( 
    aGPtrVec const * const aGeoVecP 
  ) const {
    aGPtrVec aGeoRet;
    
    for (int ii=0;ii<aGeoVecP->size();ii++) {
      dt__ptrAss(map2dTo3d const * m2d, map2dTo3d::DownCast(aGeoVecP->at(ii)));
			
			analyticGeometry * aG;
			if (config().contains("_u0")) {
				aG = m2d->segmentConstUPercent(config().lookup<dtReal>("_u0"));
		  }
			else if (config().contains("_v0")) {
				aG = m2d->segmentConstVPercent(config().lookup<dtReal>("_v0"));
		  }
      aGeoRet.push_back( aG );
    }
    
    return aGeoRet;
  }

  bool pickMap2dTo3dRangePercent::isNecessary( void ) const {
    return true;
  }
  
  void pickMap2dTo3dRangePercent::jInit( 
    jsonPrimitive const & jE,
    baseContainer * const bC,
		cVPtrVec const * const cV,
		aFPtrVec const * const aF,
		aGPtrVec const * const aG 
	) {
    dtTransformer::jInit(jE, bC, cV, aF, aG);

    int dirSet = 0;
		if ( config().contains("_u0") ) dirSet = dirSet + 1;
		if ( config().contains("_v0") ) dirSet = dirSet + 1;
    if ( dirSet != 1 ) {
      dt__throw(
        jInit(),
        << config().toStdString() << std::endl
        << "Only one value should be greater than zero. Check input."
      );
    }
  }
  
  void pickMap2dTo3dRangePercent::init( 
	  ::QDomElement const * tE, 
    baseContainer * const bC,
		cVPtrVec const * const cV,
		aFPtrVec const * const aF,
		aGPtrVec const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);

    jsonPrimitive config;

		if (dtXmlParserBase::hasAttribute("parameter_percent_one", *tE)) {
			config.append(
        "_u0",  
        dtXmlParserBase::muParseString( 
          dtXmlParserBase::replaceDependencies(
            dtXmlParserBase::getAttributeStr("parameter_percent_one", *tE),
            cV, 
            aF
          ) 
        )
      );
		}
		if (dtXmlParserBase::hasAttribute("parameter_percent_two", *tE)) {
			config.append(
        "_v0",  
        dtXmlParserBase::muParseString( 
          dtXmlParserBase::replaceDependencies(
            dtXmlParserBase::getAttributeStr("parameter_percent_two", *tE),
            cV, 
            aF
          ) 
        )
      );      
		}
    jInit(config, bC, cV, aF, aG);
  }  
}
