#ifndef SOLID3DLINE_H
#define	SOLID3DLINE_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/renderInterface.h>
#include <vector>

namespace dtOO {
  class solid3dLine : public renderInterface {
    public:
      dt__class(solid3dLine, renderInterface);
      solid3dLine();
      solid3dLine(std::vector< dtPoint3 > const & pp);
      virtual ~solid3dLine();
      std::vector< dtPoint3 > const & refP3( void ) const;
    private:
      std::vector< dtPoint3 > _p3;
  };
}
#endif	/* SOLID3DLINE_H */

