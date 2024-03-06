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

#ifndef circle_radiusCoordinateSystemConstructOCC_H
#define	circle_radiusCoordinateSystemConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  
  class circle_radiusCoordinateSystemConstructOCC {
  public:
    dt__classOnlyName(circle_radiusCoordinateSystemConstructOCC);
    circle_radiusCoordinateSystemConstructOCC( 
      dtPoint3 const & pp, dtVector3 const & xx, dtReal const & rr
    );
    circle_radiusCoordinateSystemConstructOCC( 
      dtPoint3 const & pp, dtVector3 const & nn, dtVector3 const & xx, 
      dtReal const & rr
    );    
    virtual ~circle_radiusCoordinateSystemConstructOCC();
    dtCurve * result( void );
  private:
    ptrHandling<dtCurve> _dtC;
  };
}
#endif	/* circle_radiusCoordinateSystemConstructOCC_H */
