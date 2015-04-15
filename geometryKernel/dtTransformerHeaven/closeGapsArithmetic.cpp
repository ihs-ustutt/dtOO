#include "closeGapsArithmetic.h"
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/map1dTo3d_closeGapsArithmetic.h>
#include <progHelper.h>
#include <logMe/logMe.h>


namespace dtOO {
  closeGapsArithmetic::closeGapsArithmetic() : dtTransformer() {
  }

	closeGapsArithmetic::closeGapsArithmetic( closeGapsArithmetic const & orig ) {
	}
	
  closeGapsArithmetic::~closeGapsArithmetic() {
  }

  dtTransformer * closeGapsArithmetic::clone( void ) const {
	  return new closeGapsArithmetic(*this);
	}
	
  dtTransformer * closeGapsArithmetic::create( void ) const {
		return new closeGapsArithmetic();
	}
	
  vectorHandling< analyticGeometry * > closeGapsArithmetic::apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const {
    vectorHandling< analyticGeometry * > retAGeo;
		
		dt__pVH(map1dTo3d) m1dV;
    dt__forAllConstIter(vectorHandling< analyticGeometry * >, (*aGeoVecP), it) {
			analyticGeometry * const & theAG = *it;
      //
      // cast analyticGeometry
      //
      dt__ptrAss(
			  map1dTo3d const * const m1d, 
				map1dTo3d::ConstDownCast(theAG)
			);			
			
			m1dV.push_back(m1d->clone());
    }

		m1dV = map1dTo3d_closeGapsArithmetic(m1dV).result();
		
		dt__forAllConstIter(dt__pVH(map1dTo3d), m1dV, it) {
			retAGeo.push_back( it->clone() );
//  		retAGeo.back()->setLabel(m1d->getLabel());
		}
    return retAGeo;
  }

  bool closeGapsArithmetic::isNecessary( void ) const {
    return true;
  }

  void closeGapsArithmetic::init( 
	  QDomElement const * tE, 
    baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);
  }
}