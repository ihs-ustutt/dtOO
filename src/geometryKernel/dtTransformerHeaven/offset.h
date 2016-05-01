#ifndef OFFSET_H
#define	OFFSET_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class offset : public dtTransformer {
  public:    
    dt__classOnlyName(offset);
    offset();
    virtual ~offset();
    virtual dtTransformer * clone( void ) const;
    virtual dtTransformer * create( void ) const;     
    virtual bool isNecessary( void ) const;
    void init( 
      ::QDomElement const * tE, 
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG 
    );
    virtual vectorHandling< analyticGeometry * > apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const;
    virtual vectorHandling< analyticFunction * > apply( vectorHandling< analyticFunction * > const * const sFunP ) const; 
  private:
    float _paraOneOffsetPercent;
    float _paraTwoOffsetPercent;
    bool _xyPercent;
    float _xOffset;
    float _yOffset;
    float _normalOffset;
    int _nPieces;
  };    
}

#endif	/* OFFSET_H */

