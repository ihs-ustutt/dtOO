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

#ifndef SOLID3DSURFACE_H
#define	SOLID3DSURFACE_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/renderInterface.h>
#include <interfaceHeaven/twoDArrayHandling.h>

namespace dtOO {
  class solid3dSurface : public renderInterface {
    public:
      dt__class(solid3dSurface, renderInterface);
      solid3dSurface();
      solid3dSurface(twoDArrayHandling< dtPoint3 > const & pp);
      virtual ~solid3dSurface();
      twoDArrayHandling< dtPoint3 > const refP3( void ) const;
    private:
      twoDArrayHandling< dtPoint3 > _p3;
  };
}
#endif	/* SOLID3DSURFACE_H */
