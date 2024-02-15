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

#include "bezierSurface_pointConstructOCC.h"
#include "progHelper.h"
#include "interfaceHeaven/twoDArrayHandling.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <geometryEngine/dtOCCBezierSurface.h>

#include <Geom_BezierSurface.hxx>
#include <TColgp_Array2OfPnt.hxx>

namespace dtOO {
	bezierSurface_pointConstructOCC::bezierSurface_pointConstructOCC( twoDArrayHandling< dtPoint3 > const & pp ) {
		// number of points
		//
		int nP0 = pp.size(0);
		int nP1 = pp.size(1);
		
		//
		// allocate arraies
		//
		TColgp_Array2OfPnt arr(1, nP0, 1, nP1);

		//
		// set arraies
		//
		dt__forAllIndex(pp, ii) {
		  dt__forAllIndex(pp[ii], jj) {
		    arr.SetValue( ii+1, jj+1, gp_Pnt(pp[ii][jj].x(), pp[ii][jj].y(), pp[ii][jj].z()) );
		  }
	  }

		Handle(Geom_BezierSurface) surface = new Geom_BezierSurface( arr );
		dtOCCSurfaceBase base;
		base.setOCC( surface );
		_dtS.reset( new dtOCCBezierSurface(base) );
	}
	
	bezierSurface_pointConstructOCC::~bezierSurface_pointConstructOCC() {
	}
	
	dtSurface * bezierSurface_pointConstructOCC::result( void ) {
		return _dtS->clone();
	}
}
