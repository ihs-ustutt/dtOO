#ifndef DTXMLPARSERBASE_H
#define	DTXMLPARSERBASE_H

#include <string>
#include <interfaceHeaven/vectorHandling.h>
#include "qtXmlBase.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class dtTransformer;
  class baseContainer;
  
  class dtXmlParserBase : public qtXmlBase {
  public:  
    dt__CLASSNAME(dtXmlParserBase);
    dtXmlParserBase();
    virtual ~dtXmlParserBase();
  private:
    void createBasic(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      dtTransformer const * const dtTransformerP, 
      std::vector< dtPoint3 * > * basicP
    ) const;
    void createBasic(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      dtTransformer const * const dtTransformerP, 
      std::vector< dtPoint2 * > * basicP
    ) const; 
    void createAdvanced( 
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      dtTransformer const * const dtTransformerP,                        
      vectorHandling< analyticGeometry * > * advancedP 
    ) const;       
  public:     
    dtTransformer * createTransformer(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticGeometry * > const * const depAGeoP
    ) const;       
    dtTransformer * createTransformer(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP
    ) const;     
    void createBasic(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      std::vector< dtPoint3 > * basicP
    ) const;
    dtPoint3 createDtPoint3(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticGeometry * > const * const depAGeoP
    ) const;
    dtVector3 createDtVector3(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,  
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticGeometry * > const * const depAGeoP
    ) const;    
    dtVector3 createDtVector3(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,  
      vectorHandling< analyticFunction * > const * const sFunP
    ) const;        
    std::string createString(
      QDomElement const * toBuildP,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP
    ) const;  
    void createBasic(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      std::vector< dtPoint2* > * basicP
    ) const;
    void createBasic(
      QDomElement const * toBuildP,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP,
      std::vector< dtPoint2 * > * basicP
    ) const;
    void createBasic(
      QDomElement const * toBuildP,
      baseContainer * const bC,      
      vectorHandling< constValue * > const * const cValP, 
      vectorHandling< analyticFunction * > const * const sFunP, 
      std::vector< dtPoint3 > * basicP
    ) const;    
    void createAdvanced( 
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticGeometry * > const * const depAGeoP,            
      vectorHandling< analyticGeometry * > * advancedP 
    ) const;
    analyticGeometry * createAnalyticGeometry( 
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticGeometry * > const * const depAGeoP
    ) const;  
    void createAdvanced( 
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP, 
      dtTransformer const * const dtTransformerP,                          
      vectorHandling< analyticFunction * > * advancedP 
    ) const;   
    void createAdvanced( 
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticFunction * > * advancedP 
    ) const;
    analyticFunction * createAnalyticFunction( 
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP
    ) const; 
  };
}
#endif	/* DTXMLPARSERBASE_H */

