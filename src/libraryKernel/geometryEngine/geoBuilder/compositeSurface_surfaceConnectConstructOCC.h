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

#ifndef compositeSurface_surfaceConnectConstructOCC_H
#define	compositeSurface_surfaceConnectConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/twoDArrayHandling.h>

namespace dtOO {
  class dtSurface;
  
  class compositeSurface_surfaceConnectConstructOCC {
    public:
      dt__classOnlyName(compositeSurface_surfaceConnectConstructOCC);
      compositeSurface_surfaceConnectConstructOCC( 
        twoDArrayHandling< dtSurface const * > const & ss 
      );
      virtual ~compositeSurface_surfaceConnectConstructOCC();
      dtSurface * result( void );
    private:
      dt__pH(dtSurface) _dtS;
  };
}
#endif	/* BSPLINESURFACE_BSPLINECURVEFILLCONSTRUCTOCC_H */
