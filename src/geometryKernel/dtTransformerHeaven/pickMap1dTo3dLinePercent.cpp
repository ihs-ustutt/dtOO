/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "pickMap1dTo3dLinePercent.h"
#include "analyticGeometryHeaven/map2dTo3d.h"
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include "dtTransformerFactory.h"

namespace dtOO {
  bool pickMap1dTo3dLinePercent::_registrated 
  =
  dtTransformerFactory::registrate(
    dt__tmpPtr(pickMap1dTo3dLinePercent, new pickMap1dTo3dLinePercent())
  );
  
  pickMap1dTo3dLinePercent::pickMap1dTo3dLinePercent() : dtTransformer() {
  }

  pickMap1dTo3dLinePercent
    ::pickMap1dTo3dLinePercent(const pickMap1dTo3dLinePercent& orig)
    : dtTransformer(orig) {
  }

  pickMap1dTo3dLinePercent::pickMap1dTo3dLinePercent( 
    jsonPrimitive const & jE 
  ) : dtTransformer(jE) {
    this->jInit(jE, NULL, NULL, NULL, NULL);
  }  

  pickMap1dTo3dLinePercent::~pickMap1dTo3dLinePercent() {
  }

  dtTransformer * pickMap1dTo3dLinePercent::clone( void ) const {
	  return new pickMap1dTo3dLinePercent(*this);
	}
	
  dtTransformer * pickMap1dTo3dLinePercent::create( void ) const {
		return new pickMap1dTo3dLinePercent();
	}
	
  lvH_analyticGeometry pickMap1dTo3dLinePercent::apply( 
    lvH_analyticGeometry const * const aGeoVecP 
  ) const {
    lvH_analyticGeometry aGeoRet;
    
    dt__forAllRefAuto(*aGeoVecP, aG) {
      map1dTo3d const * m1d = map1dTo3d::MustConstDownCast( aG );
			
      aGeoRet.push_back( 
        m1d->segmentPercent( 
          config().lookup<dtReal>("_u0"), 
          config().lookup<dtReal>("_u1")
        ) 
      );
    }
    
    return aGeoRet;
  }

  bool pickMap1dTo3dLinePercent::isNecessary( void ) const {
    return true;
  }

  void pickMap1dTo3dLinePercent::jInit( 
    jsonPrimitive const & jE,
    baseContainer * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG 
	) {
    dtTransformer::jInit(jE, bC, cV, aF, aG);

    dt__throwIfWithMessage(
      !config().contains("_u0") || !config().contains("_u1"),
      jInit(),
      << config().toStdString()
    );
  }    
  
  void pickMap1dTo3dLinePercent::init( 
	  ::QDomElement const * tE, 
    baseContainer * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);

    std::vector< ::QDomElement > el 
    = 
    dtXmlParserBase::getChildVector("float", *tE);
    dt__throwIf( el.size()!=2, init() );

    //_u0 = dtXmlParserBase::createFloat(el[0], bC, cV, aF, aG);
    //_u1 = dtXmlParserBase::createFloat(el[1], bC, cV, aF, aG);
    jsonPrimitive config;
    
    config.append<dtReal>( 
      "_u0", dtXmlParserBase::createFloat(el[0], bC, cV, aF, aG) 
    );
    config.append<dtReal>( 
      "_u1", dtXmlParserBase::createFloat(el[1], bC, cV, aF, aG)
    );
    jInit(config, bC, cV, aF, aG);    
  }  
}
