#ifndef bVOSetPrescribedFirstElementSize_H
#define	bVOSetPrescribedFirstElementSize_H

#include <logMe/dtMacros.h>
#include "bVOInterface.h"

#include <analyticFunctionHeaven/analyticFunctionCompoundTrojanHorse.h>

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
//  class scaOneD;
  class scaOneDPolyInterface;
  
  class bVOSetPrescribedFirstElementSize : public bVOInterface {
    public:
      dt__class(bVOSetPrescribedFirstElementSize, bVOInterface);
      dt__classSelfCreate(bVOSetPrescribedFirstElementSize);
      bVOSetPrescribedFirstElementSize();
      virtual ~bVOSetPrescribedFirstElementSize();
      virtual void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG,
        bVPtrVec const * const bV,
        boundedVolume * attachTo
      );    
      virtual void preUpdate( void );
      double F( double const * xx );
    private:
      int _type;
      analyticFunctionCompoundTrojanHorse< scaOneDPolyInterface > _grading;
      float _firstElementSize;
      dt__pH(scaOneDPolyInterface) _polyI;
      float _ll;
      float _checkX;
  };
}
#endif	/* bVOSetPrescribedFirstElementSize_H */
