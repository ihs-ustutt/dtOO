#ifndef DISCRETE3DVECTOR_H
#define	DISCRETE3DVECTOR_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/renderInterface.h>
#include <vector>

namespace dtOO {
  class discrete3dVector : public renderInterface {
    public:
      dt__class(discrete3dVector, renderInterface);
      discrete3dVector();
      discrete3dVector(std::vector< dtVector3 > const & vv, std::vector< dtPoint3 > const & pp);
      discrete3dVector(dtVector3 const & vv, dtPoint3 const & pp);
      virtual ~discrete3dVector();
      std::vector< dtVector3 > const & refV3( void ) const;
      std::vector< dtPoint3 > const & refP3( void ) const;
    private:
      void updateBB( void );
    private:
      std::vector< dtVector3 > _v3;
      std::vector< dtPoint3 > _p3;
  };
}
#endif	/* DISCRETE3DVECTOR_H */

