#include "bSplineCurve2d_curveConnectConstructOCC.h"
#include "progHelper.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtOCCCurve2dBase.h>
#include <geometryEngine/dtOCCBSplineCurve2d.h>

#include <Precision.hxx>
#include <Geom2d_BoundedCurve.hxx>
#include <Geom2d_BSplineCurve.hxx>
#include <Geom2dConvert_CompCurveToBSplineCurve.hxx>

namespace dtOO {
	bSplineCurve2d_curveConnectConstructOCC::bSplineCurve2d_curveConnectConstructOCC( 
	  vectorHandling< dtCurve2d const * > const & cc 
	) {
		Geom2dConvert_CompCurveToBSplineCurve conv;
		dt__FORALL(cc, ii,
			dt__PTRASS( dtOCCCurve2d const * occC, dtOCCCurve2d::ConstDownCast(cc[ii]) );
			Handle(Geom2d_BoundedCurve) boundedC
			=
			Handle(Geom2d_BoundedCurve)::DownCast( occC->OCCRef().getOCC() );
      conv.Add(boundedC, Precision::Confusion());//Precision::Confusion());			
    );
		dtOCCCurve2dBase base;
		base.setOCC( conv.BSplineCurve() );
		_dtC2d.reset( new dtOCCBSplineCurve2d(base) );		
	}
	
	bSplineCurve2d_curveConnectConstructOCC::~bSplineCurve2d_curveConnectConstructOCC() {
	}
	
	dtCurve2d * bSplineCurve2d_curveConnectConstructOCC::result( void ) {
		return _dtC2d->clone();
	}
}
