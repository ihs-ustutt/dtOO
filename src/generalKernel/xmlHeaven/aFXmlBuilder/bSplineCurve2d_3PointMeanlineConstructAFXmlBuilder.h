#ifndef bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder_H
#define	bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder_H

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;
  
  class bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder : public aFXmlBuilder {
  public:
    dt__classOnlyName(bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder); 
    dt__classSelfCreate(bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder);
    bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder();
    virtual ~bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder();
    virtual std::vector< std::string > factoryAlias( void ) const;    
    virtual void buildPart(
      ::QDomElement const & toBuildP, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP, 
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP
    ) const;
  };
}
#endif	/* bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder_H */
