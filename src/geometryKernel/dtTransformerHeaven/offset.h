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
      baseContainer * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF,
      aGPtrVec const * const aG 
    );
    virtual aGPtrVec apply( aGPtrVec const * const aGeoVecP ) const;
    virtual aFPtrVec apply( aFPtrVec const * const sFunP ) const; 
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

