#ifndef POINTCLOUD_H
#define	POINTCLOUD_H

#include <logMe/dtMacros.h>

#include "scaFunction.h"
#include <dtLinearAlgebra.h>

namespace covise {
  class coDoVec2;
}

namespace dtOO {
  class pointCloud : public scaFunction {
  public:
    dt__CLASSNAME(pointCloud);
    pointCloud();
    pointCloud(std::vector< dtPoint2> const * point);
    pointCloud(std::vector< dtPoint2> const & point);
    pointCloud(const pointCloud& orig);
    pointCloud * clone( void ) const;
    pointCloud * create( void ) const;        
    virtual ~pointCloud();
    virtual void dump(void);
    virtual float getValue(float const xx) const;
    virtual std::vector< float > getVectorValue(float const xx) const;
    virtual covise::coDoSet* toCoDoSet(char const * const str) const;
    virtual covise::coDoSet * packToCoDoSet( char const * const str) const;
    virtual void unpackFromCoDoSet( covise::coDoSet const * const set );       
  private:
    std::vector< dtPoint2 > _point;
  private:
    int getSurroundingPoints(
          float const xx, float const yy, float const rX, float const rY,
          float ** xxSur, float ** yySur
    );
    virtual covise::coDoVec2* toCoDoVec2(char const * const str);

  };
}
#endif	/* POINTCLOUD_H */

