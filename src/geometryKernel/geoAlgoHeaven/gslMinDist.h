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

#ifndef gslMinDist_H
#define	gslMinDist_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "dtPoint3Algo.h"


namespace dtOO { 
  class analyticGeometry;

  class gslMinDist : public dtPoint3Algo {
    public:  
      dt__class(gslMinDist, dtPoint3Algo);
      gslMinDist(
        dtPoint3 const & p3,
        dtPoint2 const & guess,
        dtPoint2 const & step,
        dtReal const & precision,
        dtInt const & maxIterations = 100
      );
      gslMinDist(
        dtPoint3 const & p3,
        dtPoint3 const & guess,
        dtPoint3 const & step,
        dtReal const & precision,
        dtInt const & maxIterations = 100
      );
      gslMinDist( gslMinDist const & orig );
      virtual ~gslMinDist();
      virtual gslMinDist * clone( void ) const;
      virtual void visit( analyticGeometry const * const aG );
  };
  dt__H_addCloneForpVH(gslMinDist);
}
#endif	/* gslMinDist_H */
