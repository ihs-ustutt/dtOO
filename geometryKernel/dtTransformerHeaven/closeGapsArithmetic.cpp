#include "closeGapsArithmetic.h"
#include "analyticFunctionHeaven/analyticFunction.h"
#include "analyticFunctionHeaven/vec3dTwoD.h"
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/map1dTo3d_closeGapsArithmetic.h>
#include <analyticFunctionHeaven/aFBuilder/vec3dTwoD_closeArithmetic.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>


namespace dtOO {
  closeGapsArithmetic::closeGapsArithmetic() : dtTransformer() {
    _nSegments = -1;
    _dir = -1;
  }

	closeGapsArithmetic::closeGapsArithmetic( closeGapsArithmetic const & orig ) {
    _dir = orig._dir;
    _nSegments = orig._nSegments;
	}
	
  closeGapsArithmetic::~closeGapsArithmetic() {
  }

  dtTransformer * closeGapsArithmetic::clone( void ) const {
	  return new closeGapsArithmetic(*this);
	}
	
  dtTransformer * closeGapsArithmetic::create( void ) const {
		return new closeGapsArithmetic();
	}
	
  vectorHandling< analyticGeometry * > closeGapsArithmetic::apply( 
    vectorHandling< analyticGeometry * > const * const aG 
  ) const {
    vectorHandling< analyticGeometry * > ret;
		
		dt__pVH(map1dTo3d) m1dV;
    dt__forAllConstIter(vectorHandling< analyticGeometry * >, (*aG), it) {
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
			ret.push_back( it->clone() );
		}
    return ret;
  }

  vectorHandling< analyticFunction * > closeGapsArithmetic::apply( 
    vectorHandling< analyticFunction * > const * const aF 
  ) const {
    dt__throwIf(_dir<0, apply());
    dt__throwIf(_nSegments<0, apply());
    
    vectorHandling< analyticFunction * > ret;
		dt__pVH(vec3dTwoD) v3dV;
    dt__forAllConstIter(vectorHandling< analyticFunction * >, (*aF), it) {
			analyticFunction * const & theAF = *it;
      //
      // cast analyticFunction
      //
      dt__ptrAss(
			  vec3dTwoD const * const v3d, 
				vec3dTwoD::ConstDownCast(theAF)
			);			
			v3dV.push_back(v3d->clone());
    }

    //
    // close analyticFunction
    //
		v3dV = vec3dTwoD_closeArithmetic(v3dV, _dir, _nSegments).result();
		
		dt__forAllConstIter(dt__pVH(vec3dTwoD), v3dV, it) {
			ret.push_back( it->clone() );
		}
    return ret;
  }
  
  bool closeGapsArithmetic::isNecessary( void ) const {
    return true;
  }

  void closeGapsArithmetic::init( 
	  ::QDomElement const * tE, 
    baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);
    
    if (dtXmlParserBase::hasAttribute("number_segments", *tE)) {
      _nSegments 
      = 
      dtXmlParserBase::getAttributeIntMuParse("number_segments", *tE, cV, aF);
    }
    if (dtXmlParserBase::hasAttribute("direction", *tE)) {
      _dir = dtXmlParserBase::getAttributeInt("direction", *tE);
    }
  }
}