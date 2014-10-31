#include "dtCurve2d.h"

#include <logMe/logMe.h>

namespace dtOO {	
	dtCurve2d::dtCurve2d() {

	}
	
	dtCurve2d::~dtCurve2d() {
		
	}
	
  //
  // optional overload
  //	
	float dtCurve2d::minU ( void ) const {
		return minPara(0);
	}
	
	float dtCurve2d::maxU ( void ) const {
		return maxPara(0);
	}
	
	float dtCurve2d::u_uPercent(float const percent) const {
    return minPara(0) + percent * (maxPara(0)-minPara(0));
	}

	float dtCurve2d::uPercent_u(float const uu) const {
    float uMin = minPara(0);
		float uMax = maxPara(0);
		
		return (uu-uMin)/(uMax-uMin);
	}

	
	dtPoint2 dtCurve2d::pointPercent( float const percent ) const {
		return point( u_uPercent(percent) );
	}
	
	dtVector2 dtCurve2d::normalPercent( float const percent) const {
		return normal( u_uPercent(percent) );
	}
	
	dtPoint2 dtCurve2d::lastControlPoint(void) const {
		return controlPoint(nControlPoints()-1);
	}
	
	float dtCurve2d::length(void) const {
		return l_u( maxPara(0) );
	}
	
	float dtCurve2d::l_uPercent(float const percent) const {
		return l_u( u_uPercent(percent) );
	}
	
	float dtCurve2d::u_lPercent( float const percent ) const {
		return u_l( percent*length() );
	}

  dtVector2 dtCurve2d::normal(float const uu) const {

    
		dtVector2 nn = firstDer(uu);
    double xP = nn.x();
    double yP = nn.y();

    std::vector< double > yyV(2,0);		
    if ( (xP == 0.) && (yP == 0.)) {
      dt__THROW(normal(),
              << DTLOGEVAL(xP) << LOGDEL
              << DTLOGEVAL(yP) );      
    } 
    else if ( xP == 0. ) {
      DTWARNINGWF(normal(),
              << DTLOGEVAL(xP) << LOGDEL
              << DTLOGEVAL(yP) << LOGDEL
              << "Adjusting to (1., 0.) ");
      yyV[0] = 1.;
      yyV[1] = 0.;
    }
    else {
      yyV[1] = sqrt( 
                 1.
                 /
                 ( (yP/xP)*(yP/xP) + 1 ) 
               );
      yyV[0] = - yP*yyV[1] / xP;
    }
    
    //
    // check orientation
    //
    double orientation = xP * yyV[1] - yP * yyV[0];
    if (orientation < 0) {
      DTDEBUGWF(normal(),
              << DTLOGEVAL(xP) << LOGDEL
              << DTLOGEVAL(yP) << LOGDEL
              << DTLOGEVAL(yyV[0]) << LOGDEL
              << DTLOGEVAL(yyV[1]) << LOGDEL
              << DTLOGEVAL(orientation) << LOGDEL
              << "Changing direction of normal vector.");
      yyV[0] = -yyV[0];
      yyV[1] = -yyV[1];
    }
    
    return dtVector2( yyV[0], yyV[1] );
  }
	
//	void dtCurve2d::trimPercent( float const uPercentStart, float const uPercentEnd) {
//		trim(u_uPercent(uPercentStart), u_uPercent(uPercentEnd));
//	}
	
	//
	//
	//
	void dtCurve2d::dump(void) {
		DTFUNCTIONNOTI(dump());
	}
	
	void dtCurve2d::connectArithmetic( dtCurve2d const * const toConnect ) {
		DTFUNCTIONNOTI(connectArithmetic());
	}
	
//	void dtCurve2d::closeArithmetic( void ) {
//		DTFUNCTIONNOTI(closeArithmetic());
//	}
//	
//	void dtCurve2d::closeStraight( void ) {
//		DTFUNCTIONNOTI(closeStraight());
//	}
	
	void dtCurve2d::translate( dtVector2 const transVec ) {
		DTFUNCTIONNOTI(translate());
	}
	
//	void dtCurve2d::translate( dtVector3 const transVec ) {
//		DTFUNCTIONNOTI(translate());
//	}
	
//	void dtCurve2d::revert( void ) {
//		DTFUNCTIONNOTI(revert());
//	}
	
//	void dtCurve2d::trim( float const uuStart, float const uuEnd) {
//		DTFUNCTIONNOTI(trim());
//	}
	
	void dtCurve2d::scale( dtPoint2 const norm ) {
		DTFUNCTIONNOTI(scale());
	}
	
	float dtCurve2d::getFunctionValue2d( float const xx ) const {
		DTFUNCTIONNOTI(getFunctionValue2d());
	}
}
