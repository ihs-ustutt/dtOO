#ifndef DTTRANSFORMER_H
#define	DTTRANSFORMER_H

#include <dtLinearAlgebra.h>
#include <dtXmlParserDecorator/dtXmlParserBase.h>
#include <interfaceHeaven/vectorHandling.h>
#include <interfaceHeaven/labelHandling.h>
#include <vector>
#include <string>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticGeometry;
  class analyticFunction;
  
  class dtTransformer : public labelHandling {
  public:
    dt__CLASSNAME(dtTransformer);
    dtTransformer();
    virtual ~dtTransformer();
    dtTransformer(dtTransformer const & orig);
    virtual dtTransformer * clone( void ) const = 0;
    virtual dtTransformer * create( void ) const = 0;    
    virtual void init( 
      QDomElement const * tE, 
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP,
      vectorHandling< analyticGeometry * > const * const depAGeoP 
    ) = 0;
    void init( 
      QDomElement const * tE,
      baseContainer const * const bC, 
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP
    );
    virtual bool isNecessary( void ) const = 0;
    virtual std::vector< dtPoint2 > 
    apply( std::vector< dtPoint2 > const * const toTrans ) const;
    virtual std::vector< dtPoint3 > 
    apply( std::vector< dtPoint3 > const * const toTrans ) const;    
    virtual vectorHandling< analyticGeometry * > 
    apply( vectorHandling< analyticGeometry * > const * const toTrans ) const;
    virtual vectorHandling< analyticFunction * > 
    apply( vectorHandling< analyticFunction * > const * const toTrans ) const;
    std::vector< dtPoint2 * > apply( std::vector< dtPoint2 * > const * const pointVecP ) const;
    std::vector< dtPoint3 * > apply( std::vector< dtPoint3 * > const * const toTrans ) const;        
    dtPoint3 apply(dtPoint3 const & pp) const;    
    dtPoint2 apply(dtPoint2 const & pp) const;
    analyticFunction * apply(analyticFunction const * const sF) const;
    analyticGeometry * apply(analyticGeometry const * const aG) const;
    virtual void handleFloat(std::string const name, float const value);
    virtual void handleInt(std::string const name, int const value);
    virtual void handleAnalyticGeometry(std::string const name, analyticGeometry const * value);
    virtual void handleAnalyticFunction(std::string const name, analyticFunction const * value);
    virtual void handlePoint2d(std::string const name, dtPoint2 const value);
    virtual void handleBool(std::string const name, bool const value);
    virtual void handleDtVector3(std::string const name, dtVector3 const value);
    virtual void handleDtVector2(std::string const name, dtVector2 const value);
  };
}

#endif	/* DTTRANSFORMER_H */

