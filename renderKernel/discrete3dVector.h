#ifndef DISCRETE3DVECTOR_H
#define	DISCRETE3DVECTOR_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/renderInterface.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class discrete3dVector : public renderInterface {
  public:
    dt__CLASSSTD(discrete3dVector, renderInterface);
    discrete3dVector();
    discrete3dVector(vectorHandling< dtVector3 > const & vv, vectorHandling< dtPoint3 > const & pp);
    discrete3dVector(dtVector3 const & vv, dtPoint3 const & pp);
    virtual ~discrete3dVector();
    vectorHandling< dtVector3 > const & refV3( void ) const;
	  vectorHandling< dtPoint3 > const & refP3( void ) const;
  private:
    vectorHandling< dtVector3 > _v3;
    vectorHandling< dtPoint3 > _p3;
  };
}
#endif	/* DISCRETE3DVECTOR_H */

