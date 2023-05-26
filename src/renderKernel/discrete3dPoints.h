#ifndef DISCRETE3DPOINTS_H
#define	DISCRETE3DPOINTS_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/renderInterface.h>
#include <vector>

namespace dtOO {
  class discrete3dPoints : public renderInterface {
    public:
      dt__class(discrete3dPoints, renderInterface);
      discrete3dPoints();
      discrete3dPoints(std::vector< dtPoint3 > const & pp);
      virtual ~discrete3dPoints();
      void addPoints( std::vector< dtPoint3 > const & pp );    
      std::vector< dtPoint3 > const & constRefP3( void ) const;
      std::vector< dtPoint3 > & refP3( void );
    private:
      std::vector< dtPoint3 > _p3;
  };
}
#endif	/* DISCRETE3DPOINTS_H */

