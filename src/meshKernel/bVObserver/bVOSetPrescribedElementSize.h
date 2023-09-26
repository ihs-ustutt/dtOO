#ifndef bVOSetPrescribedElementSize_H
#define	bVOSetPrescribedElementSize_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "bVOInterface.h"

#include <analyticFunctionHeaven/analyticFunctionCompoundTrojanHorse.h>

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class scaOneDPolyInterface;
  class dtGmshEdge;

  class bVOSetPrescribedElementSize : public bVOInterface {
    public:
      dt__class(bVOSetPrescribedElementSize, bVOInterface);
      dt__classSelfCreate(bVOSetPrescribedElementSize);
      bVOSetPrescribedElementSize();
      virtual ~bVOSetPrescribedElementSize();
      virtual std::vector< std::string > factoryAlias( void ) const;         
      void jInit( 
        jsonPrimitive const & jE,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        boundedVolume * attachTo
      );  
      virtual void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        boundedVolume * attachTo
      );    
      virtual void preUpdate( void );
      double FFirstLast( double const * xx );
      std::vector< dtReal > perform(
        dtGmshEdge * aEdge,
        double (bVOSetPrescribedElementSize::*fPtr)(double const * xx) 
      );
    private:
      analyticFunctionCompoundTrojanHorse< scaOneDPolyInterface > _grading;
      dt__pH(scaOneDPolyInterface) _polyI;
      dtReal _ll;
      dtReal _checkXFirst;
      dtReal _checkXLast;
      static bool _registrated;
  };
}
#endif	/* bVOSetPrescribedElementSize_H */

