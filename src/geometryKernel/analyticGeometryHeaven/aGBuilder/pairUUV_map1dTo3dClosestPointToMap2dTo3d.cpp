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

#include "pairUUV_map1dTo3dClosestPointToMap2dTo3d.h"

#include <interfaceHeaven/staticPropertiesHandler.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <attributionHeaven/geometryGeometryDist.h>
#include <gslMinFloatAttr.h>

namespace dtOO {
	pairUUV_map1dTo3dClosestPointToMap2dTo3d
    ::pairUUV_map1dTo3dClosestPointToMap2dTo3d(
      map1dTo3d const * const m1d, map2dTo3d const * const m2d
	) {
    gslMinFloatAttr md(
      new geometryGeometryDist(m1d, m2d),
      dtPoint3(0.5, 0.5, 0.5),
      dtPoint3(0.001, 0.001, 0.001),
      staticPropertiesHandler::getInstance()->getOptionFloat("xyz_resolution") 
    );
    md.perform();
    _closestUUV.first = m1d->u_percent( md.result()[0] );
    _closestUUV.second = m2d->uv_percent( 
      dtPoint2( md.result()[1], md.result()[2] ) 
    );
	}

	pairUUV_map1dTo3dClosestPointToMap2dTo3d
    ::~pairUUV_map1dTo3dClosestPointToMap2dTo3d() {
	}
	
	std::pair< dtReal, dtPoint2 > 
  pairUUV_map1dTo3dClosestPointToMap2dTo3d::result( void ) {
		return _closestUUV;
	}
}
