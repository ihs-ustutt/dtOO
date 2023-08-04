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

