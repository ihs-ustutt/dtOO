#include "circle_radiusCoordinateSystemConstructOCC.h"
#include "progHelper.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCCircle.h>

#include <Standard_Failure.hxx>
#include <Geom_Circle.hxx>
#include <gp_Ax2.hxx>
#include <gp_Pnt.hxx>

namespace dtOO {
	circle_radiusCoordinateSystemConstructOCC::circle_radiusCoordinateSystemConstructOCC(
    dtPoint3 const & pp, dtVector3 const & xx, float const & rr 
  ) {
		//
		// use standard constructor
		//
		Handle(Geom_Circle) circle 
		= 
		new Geom_Circle( 
      gp_Ax2(
        gp_Pnt(
          static_cast< Standard_Real >(pp.x()), 
          static_cast< Standard_Real >(pp.y()), 
          static_cast< Standard_Real >(pp.z())
        ),
        gp_Dir(
          static_cast< Standard_Real >(xx.x()), 
          static_cast< Standard_Real >(xx.y()), 
          static_cast< Standard_Real >(xx.z())
        )
      ),
      static_cast< Standard_Real >(rr)
    );

		dtOCCCurveBase base;
		base.setOCC( circle );
		_dtC.reset( new dtOCCCircle(base) );
	}
	
	circle_radiusCoordinateSystemConstructOCC::circle_radiusCoordinateSystemConstructOCC(
    dtPoint3 const & pp, dtVector3 const & nn, dtVector3 const & xx, float const & rr 
  ) {
		//
		// use standard constructor
		//
		Handle(Geom_Circle) circle 
		= 
		new Geom_Circle( 
      gp_Ax2(
        gp_Pnt(
          static_cast< Standard_Real >(pp.x()), 
          static_cast< Standard_Real >(pp.y()), 
          static_cast< Standard_Real >(pp.z())
        ),
        gp_Dir(
          static_cast< Standard_Real >(nn.x()), 
          static_cast< Standard_Real >(nn.y()), 
          static_cast< Standard_Real >(nn.z())
        ),      
        gp_Dir(
          static_cast< Standard_Real >(xx.x()), 
          static_cast< Standard_Real >(xx.y()), 
          static_cast< Standard_Real >(xx.z())
        )
      ),
      static_cast< Standard_Real >(rr)
    );

		dtOCCCurveBase base;
		base.setOCC( circle );
		_dtC.reset( new dtOCCCircle(base) );
	}
  
	circle_radiusCoordinateSystemConstructOCC::~circle_radiusCoordinateSystemConstructOCC() {
	}
	
	dtCurve * circle_radiusCoordinateSystemConstructOCC::result( void ) {
		return _dtC->clone();
	}
}