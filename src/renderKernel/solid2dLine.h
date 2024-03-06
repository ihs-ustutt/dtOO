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

#ifndef SOLID2DLINE_H
#define	SOLID2DLINE_H

#include <dtOOTypeDef.h>

#include "discrete2dPoints.h"

namespace dtOO {
  class solid2dLine : public discrete2dPoints {
    public:
      dt__class(solid2dLine, renderInterface);
      solid2dLine();
      solid2dLine(std::vector< dtPoint2 > const & pp);
      virtual ~solid2dLine();
    private:
      std::vector< dtPoint2 > _p2;
  };
}
#endif	/* SOLID2DLINE_H */
