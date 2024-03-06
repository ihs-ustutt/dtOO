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

#include "bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC.h"
#include "progHelper.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtCurve2d.h>
#include "bSplineCurve2d_pointConstructOCC.h"

namespace dtOO {
	bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC
    ::bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC( 
	  dtReal angleIn, dtReal angleOut, 
    dtReal const & deltaX, dtReal const & deltaY, 
    dtReal const & pointOne, dtReal const & pointTwo	
	) {
		//
		// check values
		//
		if ( angleIn < 0.) {
			dt__warning(
				bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC(),
				<< "Adjusting angleIn from " << dt__eval(angleIn) 
        << " to " << 0. << "."
			);
			angleIn = 0.;
		}
		if ( angleOut < 0.) {
			dt__warning(
				bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC(),
				<< "Adjusting angleOut from " << dt__eval(angleOut) 
        << " to " << 0. << "." 
			);
			angleOut = 0.;        
		}            
//		if ( (ratio < 0.) || (ratio > 1.) ) {
//			dt__throw(
//				bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC(),
//				<< dt__eval(ratio) << std::endl        
//				<< "Ratio is smaller than zero or bigger than one."
//			);
//		}
	
		
		if (angleIn == angleOut) {
			//
			// calculate deltaX
			//
			dtReal deltaX = deltaY * tan(angleIn);
			//
			// calculate points
			//
			std::vector< dtPoint2 > pV;
			pV.push_back( dtPoint2(0.,0.) );
			pV.push_back( dtPoint2(deltaX, deltaY));

			dt__info(
				bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC(),
				<< dt__eval(angleIn) << std::endl
				<< dt__eval(angleOut) << std::endl              
				<< dt__eval(deltaX) << std::endl 
				<< dt__eval(deltaY) << std::endl
				<< dt__eval(pointOne) << std::endl
				<< dt__eval(pointTwo) << std::endl
				<< dt__point2d(pV[0]) << std::endl
				<< dt__point2d(pV[1])
			);
					
			_dtC2d.reset( bSplineCurve2d_pointConstructOCC(pV, 1).result() );
		}
		else {
			//
			// calculate deltaX
			//
			dtReal deltaXMax;
			dtReal deltaXMin;
			if (angleIn >= angleOut) {
				deltaXMax = deltaY / tan(angleOut);
				deltaXMin = deltaY / tan(angleIn);
			}
			else if (angleIn < angleOut) {
				deltaXMin = deltaY / tan(angleOut);
				deltaXMax = deltaY / tan(angleIn);        
			}

//			dtReal deltaX = deltaXMin + ratio * (deltaXMax - deltaXMin);
	    dtReal ratio = (deltaX - deltaXMin) / (deltaXMax - deltaXMin);      

			//
			// calculate points
			//
			std::vector< dtPoint2 > pV;
			pV.push_back( dtPoint2(0.,0.) );
//			int ss = ( deltaY - deltaX * sin(angleIn) / cos(angleIn) )
//							 /
//							 ( sin(angleOut) - (cos(angleOut)*sin(angleIn) / cos(angleIn)) );
			dtReal tt = ( deltaY - deltaX * sin(angleOut) / cos(angleOut) )
							 /
							 ( sin(angleIn) - (cos(angleIn)*sin(angleOut) / cos(angleOut)) );  
			dtReal ll = ( deltaY - sin(angleIn) * tt ) / sin(angleOut);

			pV.push_back(
				dtPoint2(
					cos(angleIn) * tt * pointOne,
					sin(angleIn) * tt * pointOne
				)
			); 
			pV.push_back(
				dtPoint2(
					cos(angleIn) * tt,
					sin(angleIn) * tt
				)
			);      
			pV.push_back(
				dtPoint2(
					deltaX - cos(angleOut) * ll * pointTwo,
					deltaY - sin(angleOut) * ll * pointTwo
				)
			); 			
			pV.push_back( dtPoint2(deltaX, deltaY) );

			dt__info(
				bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC(),
				<< dt__eval(angleIn) << std::endl
				<< dt__eval(angleOut) << std::endl              
				<< dt__eval(deltaXMin) << std::endl 
				<< dt__eval(deltaXMax) << std::endl               
				<< dt__eval(ratio) << std::endl
//				<< dt__eval(ss) << std::endl
				<< dt__eval(tt) << std::endl
				<< dt__eval(ll) << std::endl
				<< dt__eval(deltaX) << std::endl 
				<< dt__eval(deltaY) << std::endl
				<< dt__eval(pointOne) << std::endl
				<< dt__eval(pointTwo) << std::endl
				<< dt__point2d(pV[0]) << std::endl
				<< dt__point2d(pV[1]) << std::endl
				<< dt__point2d(pV[2]) << std::endl
				<< dt__point2d(pV[3]) << std::endl
				<< dt__point2d(pV[4]) 
			);
			_dtC2d.reset( bSplineCurve2d_pointConstructOCC(pV, 3).result() );
		}
	}
	
	bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC
    ::~bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC() {
	}
	
	dtCurve2d * bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC
    ::result( void ) {
		return _dtC2d->clone();
	}
}
