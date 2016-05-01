#include "bSplineCurve2d_angleRatioDeltaYConstructOCC.h"
#include "progHelper.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtCurve2d.h>
#include "bSplineCurve2d_pointConstructOCC.h"

namespace dtOO {
	bSplineCurve2d_angleRatioDeltaYConstructOCC::bSplineCurve2d_angleRatioDeltaYConstructOCC( 
	  float angleIn, float angleOut, float const & ratio, float const & deltaY
	) {
		//
		// check values
		//
		if ( angleIn < 0.) {
			dt__warning(
				bSplineCurve2d_angleRatioDeltaYConstructOCC(),
				<< "Adjusting angleIn from " << dt__eval(angleIn) << " to " << 0. << "."
			);
			angleIn = 0.;
		}
		if ( angleOut < 0.) {
			dt__warning(
				bSplineCurve2d_angleRatioDeltaYConstructOCC(),
				<< "Adjusting angleOut from " << dt__eval(angleOut) << " to " << 0. << "." 
			);
			angleOut = 0.;        
		}            
		if ( (ratio < 0.) || (ratio > 1.) ) {
			dt__throw(
				bSplineCurve2d_angleRatioDeltaYConstructOCC(),
				<< dt__eval(ratio) << std::endl        
				<< "Ratio is smaller than zero or bigger than one."
			);
		}
		if (angleIn == angleOut) {
			//
			// calculate deltaX
			//
			float deltaX = deltaY * tan(angleIn);
			//
			// calculate points
			//
			std::vector< dtPoint2 > pV;
			pV.push_back( dtPoint2(0.,0.) );
//				if (!mirrorY) {
			pV.push_back( dtPoint2(deltaX, deltaY));
//				}
//				else {
//					pV.push_back( dtPoint2(-deltaX, deltaY));
//				}

			dt__info(
				bSplineCurve2d_angleRatioDeltaYConstructOCC(),
				<< dt__eval(angleIn) << std::endl
				<< dt__eval(angleOut) << std::endl              
				<< dt__eval(deltaX) << std::endl 
				<< dt__eval(deltaY) << std::endl
				<< dt__point2d(pV[0]) << std::endl
				<< dt__point2d(pV[1])
			);
					
			_dtC2d.reset( bSplineCurve2d_pointConstructOCC(pV, 1).result() );
		}
		else {
			//
			// calculate deltaX
			//
			float deltaXMax;
			float deltaXMin;
			if (angleIn >= angleOut) {
				deltaXMax = deltaY / tan(angleOut);
				deltaXMin = deltaY / tan(angleIn);
			}
			else if (angleIn < angleOut) {
				deltaXMin = deltaY / tan(angleOut);
				deltaXMax = deltaY / tan(angleIn);        
			}

			float deltaX = deltaXMin + ratio * (deltaXMax - deltaXMin);

			//
			// calculate points
			//
			std::vector< dtPoint2 > pV;
			pV.push_back( dtPoint2(0.,0.) );
//			int ss = ( deltaY - deltaX * sin(angleIn) / cos(angleIn) )
//							 /
//							 ( sin(angleOut) - (cos(angleOut)*sin(angleIn) / cos(angleIn)) );
			float tt = ( deltaY - deltaX * sin(angleOut) / cos(angleOut) )
							 /
							 ( sin(angleIn) - (cos(angleIn)*sin(angleOut) / cos(angleOut)) );      
//			if (!mirrorY) {
			pV.push_back(
				dtPoint2(
					cos(angleIn) * tt,
					sin(angleIn) * tt
				)
			);      
			pV.push_back( dtPoint2(deltaX, deltaY) );
//			}
//			else {
//				pV.push_back(
//					dtPoint2(
//						-cos(angleIn) * tt,
//						sin(angleIn) * tt
//					)
//				);      
//				pV.push_back( dtPoint2(-deltaX, deltaY) );          
//			}
			dt__info(
				bSplineCurve2d_angleRatioDeltaYConstructOCC(),
				<< dt__eval(angleIn) << std::endl
				<< dt__eval(angleOut) << std::endl              
				<< dt__eval(deltaXMin) << std::endl 
				<< dt__eval(deltaXMax) << std::endl               
				<< dt__eval(ratio) << std::endl
//				<< dt__eval(ss) << std::endl
				<< dt__eval(tt) << std::endl
				<< dt__eval(deltaX) << std::endl 
				<< dt__eval(deltaY) << std::endl
				<< dt__point2d(pV[0]) << std::endl
				<< dt__point2d(pV[1]) << std::endl
				<< dt__point2d(pV[2]) 
			);
			_dtC2d.reset( bSplineCurve2d_pointConstructOCC(pV, 2).result() );
		}
	}
	
	bSplineCurve2d_angleRatioDeltaYConstructOCC::~bSplineCurve2d_angleRatioDeltaYConstructOCC() {
	}
	
	dtCurve2d * bSplineCurve2d_angleRatioDeltaYConstructOCC::result( void ) {
		return _dtC2d->clone();
	}
}