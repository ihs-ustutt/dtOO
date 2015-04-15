#include "dtCurve.h"

#include <logMe/logMe.h>

namespace dtOO {	
	dtCurve::dtCurve() {

	}

	dtCurve::~dtCurve() {
		
	}
	
  //
  // optional overload
  //	
	float dtCurve::getUMin ( void ) const {
		return minPara(0);
	}
	
	float dtCurve::getUMax ( void ) const {
		return maxPara(0);
	}
	
	float dtCurve::u_uPercent(float const percent) const {
    return minPara(0) + percent * (maxPara(0)-minPara(0));
	}

	float dtCurve::uPercent_u(float const uu) const {
    float uMin = minPara(0);
		float uMax = maxPara(0);
		
		return (uu-uMin)/(uMax-uMin);
	}
	
	dtPoint3 dtCurve::pointPercent( float const percent ) const {
		return point( u_uPercent(percent) );
	}
	
	dtVector3 dtCurve::firstDerPercent( float const percent) const {
		return firstDer( u_uPercent(percent) );
	}
	
	float dtCurve::length(void) const {
		return l_u( maxPara(0) );
	}
	
	float dtCurve::l_uPercent(float const percent) const {
		return l_u( u_uPercent(percent) );
	}
	
	float dtCurve::lPercent_u(float const uu) const {
		return l_u(uu) / length();
	}

	float dtCurve::lPercent_uPercent(float const uuPercent) const {
		return l_uPercent(uuPercent) / length();
	}	
	
	float dtCurve::u_lPercent( float const percent ) const {
		return u_l( percent*length() );
	}

	float dtCurve::uPercent_lPercent( float const percent ) const {
		return uPercent_u( u_l( percent*length() ) );
	}	
	
	float dtCurve::uPercent_l( float const ll ) const {
		return uPercent_u( u_l(ll) );
	}	
	
	//
	//
	//
	void dtCurve::dump(void) {
    dt__info(
		  dump(),
      << "uu = [ " <<  minPara(0) << ", " << maxPara(0) << "]" << std::endl
			<< dt__eval(closed()) << std::endl
		);
	}
}
