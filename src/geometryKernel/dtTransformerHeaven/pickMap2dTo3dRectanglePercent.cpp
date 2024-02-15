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

#include "pickMap2dTo3dRectanglePercent.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include "dtTransformerFactory.h"

namespace dtOO {
  bool pickMap2dTo3dRectanglePercent::_registrated 
  =
  dtTransformerFactory::registrate(
    dt__tmpPtr(
      pickMap2dTo3dRectanglePercent, new pickMap2dTo3dRectanglePercent()
    )
  );
  
  pickMap2dTo3dRectanglePercent::pickMap2dTo3dRectanglePercent() 
    : dtTransformer() {
  }

  pickMap2dTo3dRectanglePercent::pickMap2dTo3dRectanglePercent(
    const pickMap2dTo3dRectanglePercent& orig
  ) : dtTransformer(orig) {
  }

  pickMap2dTo3dRectanglePercent::pickMap2dTo3dRectanglePercent( 
    jsonPrimitive const & jE 
  ) : dtTransformer(jE) {
    this->jInit(jE, NULL, NULL, NULL, NULL);
  }
 
  pickMap2dTo3dRectanglePercent::~pickMap2dTo3dRectanglePercent() {
  }

  dtTransformer * pickMap2dTo3dRectanglePercent::clone( void ) const {
	  return new pickMap2dTo3dRectanglePercent(*this);
	}
	
  dtTransformer * pickMap2dTo3dRectanglePercent::create( void ) const {
		return new pickMap2dTo3dRectanglePercent();
	}
	
  lvH_analyticGeometry pickMap2dTo3dRectanglePercent::apply( 
    lvH_analyticGeometry const * const aGeoVecP 
  ) const {
    lvH_analyticGeometry aGeoRet;
    
    dt__forAllRefAuto( *aGeoVecP, aG) {
      aGeoRet.push_back( 
        map2dTo3d::MustConstDownCast( aG )->segmentRectanglePercent(
          config().lookup<dtPoint2>("_p0"), config().lookup<dtPoint2>("_p1")
        ) 
      );
    }
    
    return aGeoRet;
  }

  bool pickMap2dTo3dRectanglePercent::isNecessary( void ) const {
    return true;
  }
  
  void pickMap2dTo3dRectanglePercent::jInit( 
    jsonPrimitive const & jE,
    baseContainer * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG 
	) {
    dtTransformer::jInit(jE, bC, cV, aF, aG);

    dt__throwIfWithMessage(
      !config().contains("_p0") || !config().contains("_p1"),
      jInit(),
      << config().toStdString()
    );
  }  
  void pickMap2dTo3dRectanglePercent::init( 
	  ::QDomElement const * tE, 
    baseContainer * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);
    
    dt__throwIf( !dtXmlParserBase::hasChild("Point_2", *tE), init() );

    std::vector< ::QDomElement > wE 
    = 
    dtXmlParserBase::getChildVector("Point_2", *tE);
       
    jsonPrimitive config;
    
    config.append<dtPoint2>( 
      "_p0", dtXmlParserBase::createDtPoint2( &(wE[0]), bC, cV, aF, aG ) 
    );
    config.append<dtPoint2>( 
      "_p1", dtXmlParserBase::createDtPoint2( &(wE[1]), bC, cV, aF, aG )
    );
    jInit(config, bC, cV, aF, aG);
  }  
}
