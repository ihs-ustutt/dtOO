#include "bSplineCurve2d_angleDeltaXDeltaYConstructOCC.h"
#include "progHelper.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtCurve2d.h>
#include "bSplineCurve2d_pointConstructOCC.h"

namespace dtOO {
	bSplineCurve2d_angleDeltaXDeltaYConstructOCC
    ::bSplineCurve2d_angleDeltaXDeltaYConstructOCC( 
	  float angleIn, float angleOut, float const & deltaX, float const & deltaY
	) {
      //
      // check values
      //
      if ( angleIn < 0.) {
        dt__warning(
				  bSplineCurve2d_angleDeltaXDeltaYConstructOCC(),
          << "Adjusting angleIn from " << dt__eval(angleIn) 
          << " to " << 0. << "." 
			  );
        angleIn = 0.;
      }
      if ( angleOut < 0.) {
        dt__warning(
				  bSplineCurve2d_angleDeltaXDeltaYConstructOCC(),
          << "Adjusting angleOut from " << dt__eval(angleOut) 
          << " to " << 0. << "." 
			  );
        angleOut = 0.;        
      }            
      if (angleIn == angleOut) {
          //
          // calculate points
          //
          std::vector< dtPoint2 > pV;
          pV.push_back( dtPoint2(0.,0.) );
					pV.push_back( dtPoint2(deltaX, deltaY));
          
          dt__info(
            bSplineCurve2d_angleDeltaXDeltaYConstructOCC(),
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
        // calculate points
        //
        std::vector< dtPoint2 > pV;
        pV.push_back( dtPoint2(0.,0.) );
        float ss 
        = 
        ( deltaY - deltaX * sin(angleIn) / cos(angleIn) )
        /
        ( sin(angleOut) - (cos(angleOut)*sin(angleIn) / cos(angleIn)) );
        float tt 
        = 
        ( deltaY - deltaX * sin(angleOut) / cos(angleOut) )
        /
        ( sin(angleIn) - (cos(angleIn)*sin(angleOut) / cos(angleOut)) );      

				pV.push_back(
					dtPoint2(
						cos(angleIn) * tt,
						sin(angleIn) * tt
					)
				);      
				pV.push_back( dtPoint2(deltaX, deltaY) );

        dt__info(
          bSplineCurve2d_angleDeltaXDeltaYConstructOCC(),
          << dt__eval(angleIn) << " rad ( " 
          << angleIn * ( 180./M_PI ) << " deg )" 
          << std::endl
          << dt__eval(angleOut) << " rad ( " 
          << angleOut * ( 180./M_PI ) << " deg )" 
          << std::endl
          << dt__eval(ss) << std::endl
          << dt__eval(tt) << std::endl
          << dt__eval(deltaX) << std::endl 
          << dt__eval(deltaY) << std::endl
          << dt__point2d(pV[0]) << std::endl
          << dt__point2d(pV[1]) << std::endl
          << dt__point2d(pV[2]) 
        );
        
        //
        // calculate deltaX and check solution
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
	      float ratio = (deltaX - deltaXMin) / (deltaXMax - deltaXMin);
        dt__throwIfWithMessage( 
          ratio<0. || ratio>1., 
          bSplineCurve2d_angleDeltaXDeltaYConstructOCC(),
          << dt__eval( deltaXMin ) << std::endl
          << dt__eval( deltaXMax ) << std::endl
          << dt__eval( ratio ) << std::endl
          << "deltaX should be bettween deltaXMin and deltaXMax."
        );        
           
				ptrHandling<dtCurve2d> dtC2d( 
					bSplineCurve2d_pointConstructOCC(pV, 2).result() 
				);

			  _dtC2d.reset( bSplineCurve2d_pointConstructOCC(pV, 2).result() );  
      }
	}
	
	bSplineCurve2d_angleDeltaXDeltaYConstructOCC
    ::~bSplineCurve2d_angleDeltaXDeltaYConstructOCC() {
	}
	
	dtCurve2d * bSplineCurve2d_angleDeltaXDeltaYConstructOCC::result( void ) {
		return _dtC2d->clone();
	}
}