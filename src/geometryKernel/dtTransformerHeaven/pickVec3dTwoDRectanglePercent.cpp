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

#include "pickVec3dTwoDRectanglePercent.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/rectangularTrimmedSurface_uvBounds.h>
#include "dtTransformerFactory.h"

namespace dtOO {
  bool pickVec3dTwoDRectanglePercent::_registrated 
  =
  dtTransformerFactory::registrate(
    dt__tmpPtr(
      pickVec3dTwoDRectanglePercent, 
      new pickVec3dTwoDRectanglePercent()
    )
  );
  
  pickVec3dTwoDRectanglePercent::pickVec3dTwoDRectanglePercent() 
    : dtTransformer() {
  }

  pickVec3dTwoDRectanglePercent::pickVec3dTwoDRectanglePercent(
    const pickVec3dTwoDRectanglePercent& orig
  ) : dtTransformer(orig) {
  }

  pickVec3dTwoDRectanglePercent::pickVec3dTwoDRectanglePercent( 
    jsonPrimitive const & jE 
  ) : dtTransformer(jE) {
    this->jInit(jE, NULL, NULL, NULL, NULL);
  }

  pickVec3dTwoDRectanglePercent::~pickVec3dTwoDRectanglePercent() {
  }

  dtTransformer * pickVec3dTwoDRectanglePercent::clone( void ) const {
	  return new pickVec3dTwoDRectanglePercent(*this);
	}
	
  dtTransformer * pickVec3dTwoDRectanglePercent::create( void ) const {
		return new pickVec3dTwoDRectanglePercent();
	}
	
  lvH_analyticFunction pickVec3dTwoDRectanglePercent::apply(
    lvH_analyticFunction const * const aFVecP 
  ) const {
    lvH_analyticFunction aFRet;
    dt__forAllRefAuto(*aFVecP, theF) {
      dt__throwIfWithMessage(
        theF->isTransformed(), 
        apply(),
        << theF->dumpToString()
      );
      dtSurface const * const oldSurf 
      = 
      vec3dSurfaceTwoD::MustConstDownCast( theF )->ptrDtSurface();
      dt__pH(dtSurface) newSurf(
        rectangularTrimmedSurface_uvBounds(
          oldSurf, 
          oldSurf->uv_uvPercent(config().lookup<dtPoint2>("_p0")), 
          oldSurf->uv_uvPercent(config().lookup<dtPoint2>("_p1")) 
        ).result()
      );
      aFRet.push_back( new vec3dSurfaceTwoD( newSurf.get() ) );
    }
    return aFRet;
  }

  bool pickVec3dTwoDRectanglePercent::isNecessary( void ) const {
    return true;
  }
 
  void pickVec3dTwoDRectanglePercent::jInit( 
    jsonPrimitive const & jE,
    baseContainer * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG 
	) {
    dtTransformer::jInit(jE, bC, cV, aF, aG);
  }

  void pickVec3dTwoDRectanglePercent::init( 
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
    
    dt__throwIf( wE.size() != 2, init() );
    jsonPrimitive config;    
    config.append<dtPoint2>(
      "_p0", dtXmlParserBase::createDtPoint2( &(wE[0]), bC, cV, aF, aG )
    );
    config.append<dtPoint2>(
      "_p1", dtXmlParserBase::createDtPoint2( &(wE[1]), bC, cV, aF, aG )
    );
    this->jInit( config, bC, cV, aF, aG );
  }  
}
