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
		return getMin(0);
	}
	
	float dtCurve::getUMax ( void ) const {
		return getMax(0);
	}
	
	float dtCurve::u_uPercent(float const percent) const {
    return getMin(0) + percent * (getMax(0)-getMin(0));
	}

	float dtCurve::uPercent_u(float const uu) const {
    float uMin = getMin(0);
		float uMax = getMax(0);
		
		return (uu-uMin)/(uMax-uMin);
	}

	
	dtPoint3 dtCurve::getPointLengthPercent3d( float const lPercent) {
		float lMax = l_u( getMax(0) );
		return getPoint3d( u_l(lPercent*lMax) );
	}
	
	dtPoint3 dtCurve::getPointPercent3d( float const percent ) const {
		return getPoint3d( u_uPercent(percent) );
	}
	
	dtVector3 dtCurve::firstDerPercent( float const percent) const {
		return firstDer( u_uPercent(percent) );
	}
	
	dtPoint3 dtCurve::getLastControlPoint3d(void) const {
		return getControlPoint3d(getNControlPoints()-1);
	}
	
	dtPoint3 dtCurve::getPreLastControlPoint3d(void) const {
		return getControlPoint3d(getNControlPoints()-2);
	}
	
	dtPoint3 dtCurve::getFirstControlPoint3d(void) const {
		return getControlPoint3d(0);
	}
	
	dtPoint3 dtCurve::getFirstNextControlPoint3d(void) const {
		return getControlPoint3d(1);
	}
	
	float dtCurve::getLength(void) const {
		return l_u( getMax(0) );
	}
	
	float dtCurve::l_uPercent(float const percent) const {
		return l_u( u_uPercent(percent) );
	}
	
	float dtCurve::lPercent_u(float const uu) const {
		return l_u(uu) / getLength();
	}

	float dtCurve::lPercent_uPercent(float const uuPercent) const {
		return l_uPercent(uuPercent) / getLength();
	}	
	
	float dtCurve::u_lPercent( float const percent ) const {
		return u_l( percent*getLength() );
	}

	float dtCurve::uPercent_lPercent( float const percent ) const {
		return uPercent_u( u_l( percent*getLength() ) );
	}	
	
	float dtCurve::uPercent_l( float const ll ) const {
		return uPercent_u( u_l(ll) );
	}	
	
	void dtCurve::trimPercent( float const uPercentStart, float const uPercentEnd) {
		trim(u_uPercent(uPercentStart), u_uPercent(uPercentEnd));
	}
	
	//
	//
	//
	void dtCurve::dump(void) {
    DTINFOWF(
		  dump(),
      << "uu = [ " <<  getMin(0) << ", " << getMax(0) << "]" << LOGDEL
			<< DTLOGEVAL(closed()) << LOGDEL
		);
	}
	
	void dtCurve::connectArithmetic( dtCurve const * const toConnect ) {
		DTFUNCTIONNOTI(connectArithmetic());
	}
	
	void dtCurve::closeArithmetic( void ) {
		DTFUNCTIONNOTI(closeArithmetic());
	}
	
	void dtCurve::closeStraight( void ) {
		DTFUNCTIONNOTI(closeStraight());
	}
	
	void dtCurve::rotate( dtPoint3 const origin, dtVector3 const vector, float const angle ) {
		DTFUNCTIONNOTI(rotate());
	}
	
	void dtCurve::revert( void ) {
		DTFUNCTIONNOTI(revert());
	}
	
	void dtCurve::trim( float const uuStart, float const uuEnd) {
		DTFUNCTIONNOTI(trim());
	}
	
  dtPoint3 dtCurve::startPoint(void) const {
		return getPointPercent3d(0.);
	}
  
	dtPoint3 dtCurve::endPoint(void) const {
		return getPointPercent3d(1.);
	}
	
}
