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
	
  aGPtrVec closeGapsArithmetic::apply( 
    aGPtrVec const * const aG 
  ) const {
    aGPtrVec ret;
		
		dt__pVH(map1dTo3d) m1dV;
    dt__forAllConstIter(aGPtrVec, (*aG), it) {
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

  aFPtrVec closeGapsArithmetic::apply( 
    aFPtrVec const * const aF 
  ) const {
    dt__throwIf(_dir<0, apply());
    dt__throwIf(_nSegments<0, apply());
    
    aFPtrVec ret;
		dt__pVH(vec3dTwoD) v3dV;
    dt__forAllConstIter(aFPtrVec, (*aF), it) {
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
    baseContainer * const bC,
		cVPtrVec const * const cV,
		aFPtrVec const * const aF,
		aGPtrVec const * const aG 
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