#include "ellipse_radiusRadiusCoordinateSystemConstructOCC.h"
#include "progHelper.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCEllipse.h>

#include <Standard_Failure.hxx>
#include <Geom_Ellipse.hxx>
#include <gp_Ax2.hxx>
#include <gp_Pnt.hxx>

namespace dtOO {
	ellipse_radiusRadiusCoordinateSystemConstructOCC
    ::ellipse_radiusRadiusCoordinateSystemConstructOCC(
    dtPoint3 const & pp, dtVector3 const & xx, 
    float const & r0, float const & r1
  ) {
		//
		// use standard constructor
		//
		Handle(Geom_Ellipse) ellipse 
		= 
		new Geom_Ellipse( 
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
      static_cast< Standard_Real >(r0), static_cast< Standard_Real >(r1)
    );

		dtOCCCurveBase base;
		base.setOCC( ellipse );
		_dtC.reset( new dtOCCEllipse(base) );
	}
	
	ellipse_radiusRadiusCoordinateSystemConstructOCC
    ::ellipse_radiusRadiusCoordinateSystemConstructOCC(
    dtPoint3 const & pp, dtVector3 const & nn, dtVector3 const & xx, 
    float const & r0, float const & r1 
  ) {
		//
		// use standard constructor
		//
		Handle(Geom_Ellipse) ellipse 
		= 
		new Geom_Ellipse( 
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
      static_cast< Standard_Real >(r0), static_cast< Standard_Real >(r1)
    );

		dtOCCCurveBase base;
		base.setOCC( ellipse );
		_dtC.reset( new dtOCCEllipse(base) );
	}
  
	ellipse_radiusRadiusCoordinateSystemConstructOCC
    ::~ellipse_radiusRadiusCoordinateSystemConstructOCC() {
	}
	
	dtCurve * ellipse_radiusRadiusCoordinateSystemConstructOCC::result( void ) {
		return _dtC->clone();
	}
}