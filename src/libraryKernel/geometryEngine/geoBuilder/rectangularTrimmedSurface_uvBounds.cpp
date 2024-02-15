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

#include "rectangularTrimmedSurface_uvBounds.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <geometryEngine/dtOCCRectangularTrimmedSurface.h>

#include <Precision.hxx>
#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_TypeDef.hxx>
#include <Geom_RectangularTrimmedSurface.hxx>

namespace dtOO {
  rectangularTrimmedSurface_uvBounds::rectangularTrimmedSurface_uvBounds(
	  dtSurface const * dtS, dtPoint2 const & min, dtPoint2 const & max
	) {
    dt__ptrAss(
		  dtOCCSurface const * occS, 
			dtOCCSurface::ConstDownCast(dtS)
		);
		
		Standard_Real U1 = static_cast<Standard_Real>(min.x());
		Standard_Real U2 = static_cast<Standard_Real>(max.x());
		Standard_Real V1 = static_cast<Standard_Real>(min.y());
		Standard_Real V2 = static_cast<Standard_Real>(max.y());
		
		Handle(Geom_RectangularTrimmedSurface) rts;
    
		dt__tryOcc(
      rts
      =
      new Geom_RectangularTrimmedSurface(
        occS->OCCRef().getOCC(), U1, U2, V1, V2
      );
      ,
      << ""
    );
		
		dtOCCSurfaceBase base;
		base.setOCC(rts);

		_dtS.reset( new dtOCCRectangularTrimmedSurface(base) );		
	}

  dtSurface * rectangularTrimmedSurface_uvBounds::result( void ) {
		return _dtS->clone();
	}

	rectangularTrimmedSurface_uvBounds::~rectangularTrimmedSurface_uvBounds() {
	}
}
