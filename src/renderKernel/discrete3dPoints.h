#ifndef DISCRETE3DPOINTS_H
#define	DISCRETE3DPOINTS_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/renderInterface.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class discrete3dPoints : public renderInterface {
    public:
      dt__class(discrete3dPoints, renderInterface);
      discrete3dPoints();
      discrete3dPoints(vectorHandling< dtPoint3 > const & pp);
      virtual ~discrete3dPoints();
      void addPoints( vectorHandling< dtPoint3 > const & pp );    
      vectorHandling< dtPoint3 > const & constRefP3( void ) const;
      vectorHandling< dtPoint3 > & refP3( void );
    private:
      vectorHandling< dtPoint3 > _p3;
  };
}
#endif	/* DISCRETE3DPOINTS_H */

