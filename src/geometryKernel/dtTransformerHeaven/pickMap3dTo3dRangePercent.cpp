#include "pickMap3dTo3dRangePercent.h"
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include "analyticGeometryHeaven/map2dTo3d.h"
#include <analyticGeometryHeaven/map3dTo3d.h>
#include "dtTransformerFactory.h"

namespace dtOO {
  bool pickMap3dTo3dRangePercent::_registrated 
  =
  dtTransformerFactory::registrate(
    dt__tmpPtr(pickMap3dTo3dRangePercent, new pickMap3dTo3dRangePercent())
  );
  
  pickMap3dTo3dRangePercent::pickMap3dTo3dRangePercent() : dtTransformer() {
  }

  pickMap3dTo3dRangePercent::pickMap3dTo3dRangePercent(
    const pickMap3dTo3dRangePercent& orig
  ) : dtTransformer(orig) {
  }

  pickMap3dTo3dRangePercent::pickMap3dTo3dRangePercent( 
    jsonPrimitive const & jE 
  ) : dtTransformer(jE) {
    this->jInit(jE, NULL, NULL, NULL, NULL);
  }

  pickMap3dTo3dRangePercent::~pickMap3dTo3dRangePercent() {
  }

  dtTransformer * pickMap3dTo3dRangePercent::clone( void ) const {
	  return new pickMap3dTo3dRangePercent(*this);
	}
	
  dtTransformer * pickMap3dTo3dRangePercent::create( void ) const {
		return new pickMap3dTo3dRangePercent();
	}
	
  lvH_analyticGeometry pickMap3dTo3dRangePercent::apply( 
    lvH_analyticGeometry const * const aGeoVecP 
  ) const {
    lvH_analyticGeometry aGeoRet;
    
    for (int ii=0;ii<aGeoVecP->size();ii++) {
      dt__ptrAss(map3dTo3d const * m3d, map3dTo3d::DownCast(aGeoVecP->at(ii)));
			
			analyticGeometry * aG;
			if (config().contains("_u0")) {
				aG = m3d->segmentConstUPercent(config().lookup<dtReal>("_u0"));
		  }
			else if (config().contains("_v0")) {
				aG = m3d->segmentConstVPercent(config().lookup<dtReal>("_v0"));
		  }
			else if (config().contains("_w0")) {
				aG = m3d->segmentConstWPercent(config().lookup<dtReal>("_w0"));
		  }
      aGeoRet.push_back( aG );
    }
    
    return aGeoRet;
  }

  bool pickMap3dTo3dRangePercent::isNecessary( void ) const {
    return true;
  }
  
  void pickMap3dTo3dRangePercent::jInit( 
    jsonPrimitive const & jE,
    baseContainer * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG 
	) {
    dtTransformer::jInit(jE, bC, cV, aF, aG);

    int dirSet = 0;
		if ( config().contains("_u0") ) dirSet = dirSet + 1;
		if ( config().contains("_v0") ) dirSet = dirSet + 1;
		if ( config().contains("_w0") ) dirSet = dirSet + 1;
    if ( dirSet != 1 ) {
      dt__throw(
        jInit(),
        << config().toStdString() << std::endl
        << "Only one value should be greater than zero. Check input."
      );
    }
  }
  
  void pickMap3dTo3dRangePercent::init( 
	  ::QDomElement const * tE, 
    baseContainer * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG 
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
		if (dtXmlParserBase::hasAttribute("parameter_percent_three", *tE)) {
			config.append(
        "_w0",  
        dtXmlParserBase::muParseString( 
          dtXmlParserBase::replaceDependencies(
            dtXmlParserBase::getAttributeStr("parameter_percent_three", *tE),
            cV, 
            aF
          ) 
        )
      );          
		}
    jInit(config, bC, cV, aF, aG);
  }  
}