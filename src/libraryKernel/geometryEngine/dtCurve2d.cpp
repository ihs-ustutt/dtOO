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
	dtReal dtCurve2d::minU ( void ) const {
		return minPara(0);
	}
	
	dtReal dtCurve2d::maxU ( void ) const {
		return maxPara(0);
	}
	
	dtReal dtCurve2d::u_uPercent(dtReal const percent) const {
    return minPara(0) + percent * (maxPara(0)-minPara(0));
	}

	dtReal dtCurve2d::uPercent_u(dtReal const uu) const {
    dtReal uMin = minPara(0);
		dtReal uMax = maxPara(0);
		
		return (uu-uMin)/(uMax-uMin);
	}

	
	dtPoint2 dtCurve2d::pointPercent( dtReal const percent ) const {
		return point( u_uPercent(percent) );
	}
	
	dtVector2 dtCurve2d::normalPercent( dtReal const percent) const {
		return normal( u_uPercent(percent) );
	}
	
	dtPoint2 dtCurve2d::lastControlPoint(void) const {
		return controlPoint(nControlPoints()-1);
	}
	
	dtReal dtCurve2d::length(void) const {
		return l_u( maxPara(0) );
	}
	
	dtReal dtCurve2d::l_uPercent(dtReal const percent) const {
		return l_u( u_uPercent(percent) );
	}
	
	dtReal dtCurve2d::u_lPercent( dtReal const percent ) const {
		return u_l( percent*length() );
	}

	void dtCurve2d::setControlPoint( dtInt const nPoint, dtPoint2 const point ) {
		dt__throw(setControlPoint(), << "Call on dtCurve2d()");
	}
//  dtVector2 dtCurve2d::normal(dtReal const uu) const {
//		dtVector2 nn = firstDer(uu);
//    double xP = nn.x();
//    double yP = nn.y();
//
//    std::vector< double > yyV(2,0);		
//    if ( (xP == 0.) && (yP == 0.)) {
//      dt__THROW(normal(),
//              << dt__eval(xP) << std::endl
//              << dt__eval(yP) );      
//    } 
//    else if ( xP == 0. ) {
//      dt__warning(normal(),
//              << dt__eval(xP) << std::endl
//              << dt__eval(yP) << std::endl
//              << "Adjusting to (1., 0.) ");
//      yyV[0] = 1.;
//      yyV[1] = 0.;
//    }
//    else {
//      yyV[1] = sqrt( 
//                 1.
//                 /
//                 ( (yP/xP)*(yP/xP) + 1 ) 
//               );
//      yyV[0] = - yP*yyV[1] / xP;
//    }
//    
//    //
//    // check orientation
//    //
//    double orientation = xP * yyV[1] - yP * yyV[0];
//    if (orientation < 0) {
//      dt__debug(normal(),
//              << dt__eval(xP) << std::endl
//              << dt__eval(yP) << std::endl
//              << dt__eval(yyV[0]) << std::endl
//              << dt__eval(yyV[1]) << std::endl
//              << dt__eval(orientation) << std::endl
//              << "Changing direction of normal vector.");
//      yyV[0] = -yyV[0];
//      yyV[1] = -yyV[1];
//    }
//    
//    return dtVector2( yyV[0], yyV[1] );
//  }
	
	//
	//
	//
	void dtCurve2d::dump(void) {
    dt__info(
			dump(), 
		  << dt__eval(virtualClassName()) << std::endl
      << "uu = [ " <<  minPara(0) << ", " << maxPara(0) << "]" << std::endl
			<< dt__eval(closed()) << std::endl
			<< "dumpToString()" << std::endl
			<< dumpToString()
		);
	}
	
	std::string dtCurve2d::dumpToString(void) const {
		std::stringstream ss;
		
		ss << dt__dumpToString( << "" );
		
		return ss.str();
	} 
  
  dt__C_addCloneForpVH(dtCurve2d);    
}
