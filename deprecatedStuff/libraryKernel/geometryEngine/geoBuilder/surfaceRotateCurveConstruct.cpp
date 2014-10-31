#include "surfaceRotateCurveConstruct.h"
#include "geometryEngine/dtSislCurve.h"
#include "progHelper.h"
#include "geometryEngine/dtSislSurf.h"

#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>

#include <sisl.h>
#include <sisl-copyright.h>
#include <sislP.h>

namespace dtOO {	
  surfaceRotateCurveConstruct::surfaceRotateCurveConstruct(
      dtPoint3 const & origin, 
      dtVector3 const & rotAxis, 
      dtCurve const & curve, 
      float const & angle
    ) {
    int errFlag;
    double point[3];
    double axis[3];
    double angleD = static_cast<double>(angle);
    
    point[0] = origin.x();
    point[1] = origin.y();
    point[2] = origin.z();
    axis[0] = rotAxis.x();
    axis[1] = rotAxis.y();
    axis[2] = rotAxis.z();
		
//    SISLCurve * curve = const_cast< SISLCurve * >( cast2Sisl(curveP)->getSISLCurve());
		dtSislCurve const * sisl;
		dt__MUSTDOWNCAST(&curve, dtSislCurve const, sisl);
    SISLSurf * surf;		
    s1302(const_cast<SISLCurve*>(sisl->getSISLCurve()), //SISLCurve *curve;
          0, //double epsge;
          angleD, //double angle;
          point, //double point[ ];
          axis, //double axis[ ];
          &surf, //SISLSurf **surf;
          &errFlag);//int *stat;
		
    dtSislBase().allRight(errFlag, __FILE__, __LINE__);
    
		_dtS.reset( new dtSislSurf(*surf) );
		
		delete surf;
	}
	
  surfaceRotateCurveConstruct::~surfaceRotateCurveConstruct() {
		
	}
  
	dtSurface * surfaceRotateCurveConstruct::result( void ) {
		return _dtS->clone();
	}

}
