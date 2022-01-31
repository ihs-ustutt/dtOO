#ifndef SOLID2DLINE_H
#define	SOLID2DLINE_H

#include <dtOOTypeDef.h>

#include "discrete2dPoints.h"

namespace dtOO {
  class solid2dLine : public discrete2dPoints {
    public:
      dt__class(solid2dLine, renderInterface);
      solid2dLine();
      solid2dLine(vectorHandling< dtPoint2 > const & pp);
      virtual ~solid2dLine();
    private:
      vectorHandling< dtPoint2 > _p2;
  };
}
#endif	/* SOLID2DLINE_H */

