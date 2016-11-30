#ifndef ROTATE_H
#define	ROTATE_H

#include "dtStrongTransformer.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class rotate : public dtStrongTransformer {
  public:    
    dt__classOnlyName(rotate);
    rotate();
    virtual ~rotate();
    rotate(const rotate& orig);    
    virtual dtStrongTransformer * clone( void ) const;
    virtual dtStrongTransformer * create( void ) const;       
    virtual bool isNecessary( void ) const;
    void init(
      ::QDomElement const * tE, 
      baseContainer const * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF,
      aGPtrVec const * const aG 
    );
    virtual std::vector< dtPoint3 > apply( 
      std::vector< dtPoint3 > const * const toTrans 
    ) const;
    virtual std::vector< dtPoint3 > retract(
      std::vector< dtPoint3 > const * const toRetract
    ) const;
    virtual std::vector< dtVector3 > apply( 
      std::vector< dtVector3 > const * const toTrans 
    ) const;
    virtual std::vector< dtVector3 > retract(
      std::vector< dtVector3 > const * const toRetract
    ) const;        
    virtual aGPtrVec apply( 
      aGPtrVec const * const aGeoVecP 
    ) const;
  private:
    dtPoint3 _origin;
    dtVector3 _rotVector;
    float _angle;
  };
}
#endif	/* ROTATE_H */

