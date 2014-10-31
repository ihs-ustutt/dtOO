#ifndef OFFSET_H
#define	OFFSET_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class offset : public dtTransformer {
  public:    
    dt__CLASSNAME(offset);
    offset();
    virtual ~offset();
    virtual bool isNecessary( void ) const;
    void init( QDomElement * transformerElementP, 
               pointContainer * const pointContainerP,
               vectorContainer * const vectorContainerP,    
               vectorHandling< constValue * > const * const cValP,
               vectorHandling< analyticFunction * > const * const sFunP,
               vectorHandling< analyticGeometry * > const * const depAGeoP );
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

