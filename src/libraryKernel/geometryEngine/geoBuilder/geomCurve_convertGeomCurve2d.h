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

#ifndef GEOMCURVE_CONVERTGEOMCURVE2D_H
#define	GEOMCURVE_CONVERTGEOMCURVE2D_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  class dtCurve2d;
  
  class geomCurve_convertGeomCurve2d {
    public:
      dt__classOnlyName(geomCurve_convertGeomCurve2d);
      geomCurve_convertGeomCurve2d( 
        dtCurve2d const * const dtC2d, 
        dtPoint3 const & pp, 
        dtVector3 const & vv 
      );
      virtual ~geomCurve_convertGeomCurve2d();
      dtCurve * result( void );
    private:
      ptrHandling<dtCurve> _dtC;
  };
}
#endif	/* GEOMCURVE_CONVERTGEOMCURVE2D_H */
