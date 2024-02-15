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

#ifndef geomSurface_closeGapsArithmeticOCC_H
#define	geomSurface_closeGapsArithmeticOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtSurface;
  
  class geomSurface_closeGapsArithmeticOCC {
  public:
    dt__classOnlyName(geomSurface_closeGapsArithmeticOCC);    
    geomSurface_closeGapsArithmeticOCC();
    geomSurface_closeGapsArithmeticOCC(
      dt__pVH(dtSurface) const & dtS, dtInt const & dir, dtInt const & nSegments
    );
    virtual ~geomSurface_closeGapsArithmeticOCC();
    dt__pVH(dtSurface) result( void );    
  private:
    dt__pVH(dtSurface) _dtS;
  };
}
#endif	/* geomSurface_closeGapsArithmeticOCC_H */
