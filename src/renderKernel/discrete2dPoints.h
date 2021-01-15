#ifndef DISCRETE2DPOINTS_H
#define	DISCRETE2DPOINTS_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/renderInterface.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class discrete2dPoints : public renderInterface {
    public:
      dt__class(discrete2dPoints, renderInterface);
      discrete2dPoints();
      discrete2dPoints(vectorHandling< dtPoint2 > const & pp);
      virtual ~discrete2dPoints();
      vectorHandling< dtPoint2 > const & refP2( void ) const;
    private:
      vectorHandling< dtPoint2 > _p2;
  };
}
#endif	/* DISCRETE2DPOINTS_H */

