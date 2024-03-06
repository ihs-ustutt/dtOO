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

#ifndef DISCRETE2DPOINTS_H
#define	DISCRETE2DPOINTS_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/renderInterface.h>
#include <vector>

namespace dtOO {
  class discrete2dPoints : public renderInterface {
    public:
      dt__class(discrete2dPoints, renderInterface);
      discrete2dPoints();
      discrete2dPoints(std::vector< dtPoint2 > const & pp);
      virtual ~discrete2dPoints();
      std::vector< dtPoint2 > const & refP2( void ) const;
    private:
      std::vector< dtPoint2 > _p2;
  };
}
#endif	/* DISCRETE2DPOINTS_H */
