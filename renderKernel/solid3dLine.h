#ifndef SOLID3DLINE_H
#define	SOLID3DLINE_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/renderInterface.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class solid3dLine : public renderInterface {
  public:
    dt__CLASSSTD(solid3dLine, renderInterface);
    solid3dLine();
    solid3dLine(vectorHandling< dtPoint3 > const & pp);
    virtual ~solid3dLine();
	  vectorHandling< dtPoint3 > const & refP3( void ) const;
  private:
    vectorHandling< dtPoint3 > _p3;
  };
}
#endif	/* SOLID3DLINE_H */

