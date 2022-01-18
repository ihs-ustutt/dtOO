#ifndef OFFSET_H
#define	OFFSET_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class offset : public dtTransformer {
    public:    
      dt__classOnlyName(offset);
      offset();
      offset( offset const & orig );    
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
      virtual aFPtrVec apply( aFPtrVec const * const toTrans ) const; 
    private:
      dtReal _paraOneOffsetPercent;
      dtReal _paraTwoOffsetPercent;
      bool _xyPercent;
      dtReal _xOffset;
      dtReal _yOffset;
      dtReal _normalOffset;
      dtInt _nPieces;
      static bool _registrated;    
  };    
}

#endif	/* OFFSET_H */

