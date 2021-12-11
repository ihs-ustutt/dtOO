#ifndef bSplineSurface_patchByPatchConstructOCCAFXmlBuilder_H
#define	bSplineSurface_patchByPatchConstructOCCAFXmlBuilder_H

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
        cVPtrVec const * const cV, 
        aFPtrVec const * const aF,
        aFPtrVec * result
      ) const;
    private:
      void createWire(
        ::QDomElement const & toBuild,
        baseContainer * const bC,
        cVPtrVec const * const cV, 
        aFPtrVec const * const aF,              
        std::vector< float > const & wire,              
        twoDArrayHandling< dt__pH(dtCurve) > & cc,
        twoDArrayHandling< dtPoint3 > & pp
      ) const;
      dtSurface * sortAndSkin(
        int const & fixJ, twoDArrayHandling< dt__pH(dtCurve) > const & cc,
        vectorHandling< dtCurve const * > & ccSkin,
        vectorHandling< dtCurve const * > & ccFill,
        int const & skin_nInitCuts, 
        int const & skin_minDeg, 
        int const & skin_maxDeg, 
        int const & skin_nIter 
      ) const;
      dtSurface * fillPatch(
        dtSurface const * const boundSurf_0,
        dtCurve const * const boundCurv_1,
        dtCurve const * const boundCurv_2,
        dtCurve const * const boundCurv_3,
        int const & fill_maxDeg,
        int const & fill_maxSeg
      ) const;
    private: 
      static bool _registrated;      
  };
}
#endif	/* bSplineSurface_patchByPatchConstructOCCAFXmlBuilder_H */

