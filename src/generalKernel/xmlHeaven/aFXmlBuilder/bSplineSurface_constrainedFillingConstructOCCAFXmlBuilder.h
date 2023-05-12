#ifndef bSplineSurface_constrainedFillingConstructOCCAFXmlBuilder_H
#define	bSplineSurface_constrainedFillingConstructOCCAFXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineSurface_constrainedFillingConstructOCCAFXmlBuilder 
    : public aFXmlBuilder {
    public:
      dt__classOnlyName(bSplineSurface_constrainedFillingConstructOCCAFXmlBuilder);    
      dt__classSelfCreate(bSplineSurface_constrainedFillingConstructOCCAFXmlBuilder);    
      bSplineSurface_constrainedFillingConstructOCCAFXmlBuilder();
      virtual ~bSplineSurface_constrainedFillingConstructOCCAFXmlBuilder();
      virtual void buildPart(
        ::QDomElement const & toBuildP, 
        baseContainer * const bC,
        lvH_constValue const * const cValP, 
        lvH_analyticFunction const * const depSFunP,
        lvH_analyticFunction * sFunP
      ) const;
    private: 
      static bool _registrated;      
  };
}
#endif	/* bSplineSurface_constrainedFillingConstructOCCAFXmlBuilder_H */

