#ifndef bSplineSurface_patchByPatchConstructOCCAFXmlBuilder_H
#define	bSplineSurface_patchByPatchConstructOCCAFXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;
  class dtCurve;
  class dtSurface;
  
  class bSplineSurface_patchByPatchConstructOCCAFXmlBuilder 
    : public aFXmlBuilder {
    public:
      dt__classOnlyName(bSplineSurface_patchByPatchConstructOCCAFXmlBuilder);    
      dt__classSelfCreate(bSplineSurface_patchByPatchConstructOCCAFXmlBuilder);    
      bSplineSurface_patchByPatchConstructOCCAFXmlBuilder();
      virtual ~bSplineSurface_patchByPatchConstructOCCAFXmlBuilder();
      virtual void buildPart(
        ::QDomElement const & toBuild, 
        baseContainer * const bC,
        lvH_constValue const * const cV, 
        lvH_analyticFunction const * const aF,
        lvH_analyticFunction * result
      ) const;
    private:
      void createWire(
        ::QDomElement const & toBuild,
        baseContainer * const bC,
        lvH_constValue const * const cV, 
        lvH_analyticFunction const * const aF,              
        std::vector< dtReal > const & wire,              
        twoDArrayHandling< dt__pH(dtCurve) > & cc,
        twoDArrayHandling< dtPoint3 > & pp
      ) const;
      dt__pH(dtSurface) sortAndInit(
        dtInt const & fixJ, twoDArrayHandling< dt__pH(dtCurve) > const & cc,
        vectorHandling< dtCurve const * > & ccFill,
        dtInt const & skin_nInitCuts, 
        dtInt const & skin_minDeg, 
        dtInt const & skin_maxDeg, 
        dtInt const & skin_nIter
      ) const;
      dtSurface * fillPatch(
        dtSurface const * const boundSurf_0,
        dtCurve const * const boundCurv_1,
        dtCurve const * const boundCurv_2,
        dtCurve const * const boundCurv_3,
        dtInt const & fill_maxDeg,
        dtInt const & fill_maxSeg
      ) const;
    private: 
      static bool _registrated;      
  };
}
#endif	/* bSplineSurface_patchByPatchConstructOCCAFXmlBuilder_H */

