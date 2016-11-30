#ifndef SCA3POINTMEANLINEFUNCTIONDECORATORXY_H
#define	SCA3POINTMEANLINEFUNCTIONDECORATORXY_H

#include <dtXmlParserDecorator/dtXmlParserFunctionDecorator.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

  class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;

  class sca3PointMeanlineFunctionDecoratorXY : public dtXmlParserFunctionDecorator {
  public:
    dt__class(sca3PointMeanlineFunctionDecoratorXY, dtXmlParserFunctionDecorator);
    sca3PointMeanlineFunctionDecoratorXY();
    virtual ~sca3PointMeanlineFunctionDecoratorXY();
    virtual void buildPart(::QDomElement const & toBuildP, 
                                          cVPtrVec const * const cValP, 
                                          aFPtrVec const * const depSFunP,
                                          aFPtrVec * sFunP) const;
  private:
  };
}
#endif	/* SCA3POINTMEANLINEFUNCTIONDECORATORXY_H */

