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

#include "circle_radiusCoordinateSystemConstructOCC.h"
#include "progHelper.h"

#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtOCCCircle.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <logMe/logMe.h>

#include <Geom_Circle.hxx>
#include <Standard_Failure.hxx>
#include <gp_Ax2.hxx>
#include <gp_Pnt.hxx>

namespace dtOO {
circle_radiusCoordinateSystemConstructOCC ::
  circle_radiusCoordinateSystemConstructOCC(
    dtPoint3 const &pp, dtVector3 const &xx, dtReal const &rr
  )
{
  //
  // use standard constructor
  //
  Handle(Geom_Circle) circle = new Geom_Circle(
    gp_Ax2(
      gp_Pnt(
        static_cast<Standard_Real>(pp.x()),
        static_cast<Standard_Real>(pp.y()),
        static_cast<Standard_Real>(pp.z())
      ),
      gp_Dir(
        static_cast<Standard_Real>(xx.x()),
        static_cast<Standard_Real>(xx.y()),
        static_cast<Standard_Real>(xx.z())
      )
    ),
    static_cast<Standard_Real>(rr)
  );

  dtOCCCurveBase base;
  base.setOCC(circle);
  _dtC.reset(new dtOCCCircle(base));
}

circle_radiusCoordinateSystemConstructOCC ::
  circle_radiusCoordinateSystemConstructOCC(
    dtPoint3 const &pp,
    dtVector3 const &nn,
    dtVector3 const &xx,
    dtReal const &rr
  )
{
  //
  // use standard constructor
  //
  Handle(Geom_Circle) circle = new Geom_Circle(
    gp_Ax2(
      gp_Pnt(
        static_cast<Standard_Real>(pp.x()),
        static_cast<Standard_Real>(pp.y()),
        static_cast<Standard_Real>(pp.z())
      ),
      gp_Dir(
        static_cast<Standard_Real>(nn.x()),
        static_cast<Standard_Real>(nn.y()),
        static_cast<Standard_Real>(nn.z())
      ),
      gp_Dir(
        static_cast<Standard_Real>(xx.x()),
        static_cast<Standard_Real>(xx.y()),
        static_cast<Standard_Real>(xx.z())
      )
    ),
    static_cast<Standard_Real>(rr)
  );

  dtOCCCurveBase base;
  base.setOCC(circle);
  _dtC.reset(new dtOCCCircle(base));
}

circle_radiusCoordinateSystemConstructOCC ::
  ~circle_radiusCoordinateSystemConstructOCC()
{
}

dtCurve *circle_radiusCoordinateSystemConstructOCC::result(void)
{
  return _dtC->clone();
}
} // namespace dtOO
