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

#include "float_map1dTo3dPointConstCartesian.h"

#include <interfaceHeaven/staticPropertiesHandler.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <attributionHeaven/pointGeometryOneDDist.h>
#include <gslMinFloatAttr.h>

namespace dtOO {
	float_map1dTo3dPointConstCartesian::float_map1dTo3dPointConstCartesian(
    map1dTo3d const * const m1d, dtInt const & dir, dtReal const & value,
    dtReal const & initGuess
	) {
    gslMinFloatAttr md(
      dt__pH(pointGeometryOneDDist)(
        new pointGeometryOneDDist(value, m1d, dir)
      ),
      initGuess,
      0.01,
      staticPropertiesHandler::getInstance()->getOptionFloat("xyz_resolution")
    );
    md.perform(); 
    _u = m1d->u_percent( md.result()[0] );
    
    dt__info(
      float_map1dTo3dPointConstCartesian(),
      << "Point xyz = (" << m1d->getPoint( _u ) << " / u = " << _u 
      << " ) should have " << value
      << " at cartesian direction " << dir << "." << std::endl
      << "distance = " << m1d->getPoint( _u )[dir] - value
    );
	}

  float_map1dTo3dPointConstCartesian::float_map1dTo3dPointConstCartesian(
    map1dTo3d const * const m1d, dtInt const & dir, dtReal const & value
	) : float_map1dTo3dPointConstCartesian(m1d, dir, value, 0.5) {
      
  }
    
	float_map1dTo3dPointConstCartesian::~float_map1dTo3dPointConstCartesian() {
	}
	
	dtReal float_map1dTo3dPointConstCartesian::result( void ) {
		return _u;
	}
}
