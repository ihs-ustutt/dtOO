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
    virtual ~dtXmlParserBase();
  private:
    static void createBasic(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      dtTransformer const * const dtTransformerP, 
      std::vector< dtPoint3 * > * basicP
    );
    static void createBasic(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      dtTransformer const * const dtTransformerP, 
      std::vector< dtPoint2 * > * basicP
    ); 
    static void createAdvanced( 
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      dtTransformer const * const dtTransformerP,                        
      vectorHandling< analyticGeometry * > * advancedP 
    );       
  public:     
    static dtTransformer * createTransformer(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticGeometry * > const * const depAGeoP
    );       
    static dtTransformer * createTransformer(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP
    );     
    static void createBasic(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      std::vector< dtPoint3 > * basicP
    );
    static dtPoint3 createDtPoint3(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticGeometry * > const * const depAGeoP
    );
    static dtPoint3 createDtPoint3(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP
    );    
    static dtVector2 createDtVector2(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,  
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticGeometry * > const * const depAGeoP
    );       
    static dtVector3 createDtVector3(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,  
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticGeometry * > const * const depAGeoP
    );    
    static dtVector3 createDtVector3(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,  
      vectorHandling< analyticFunction * > const * const sFunP
    );
    
    static dtVector2 getDtVector2(
      QDomElement const * toBuildP,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cValP,  
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticGeometry * > const * const depAGeoP
    );
    static dtPoint3 getDtPoint3(
      QDomElement const * toBuildP,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticGeometry * > const * const depAGeoP
    );
    static dtPoint3 getDtPoint3(
      QDomElement const * toBuildP,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP
    );    
    static dtVector3 getDtVector3(
      QDomElement const * toBuildP,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cValP,  
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticGeometry * > const * const depAGeoP
    );    
    static dtVector3 getDtVector3(
      QDomElement const * toBuildP,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cValP,  
      vectorHandling< analyticFunction * > const * const sFunP
    );
    
    static dtVector3 getDtVector3(
      std::string const & str, baseContainer const * const bC
    );    
    static dtVector3 getDtVector3(
	    QDomElement const * toBuildP, baseContainer const * const bC
	  );
    static dtPoint3 getDtPoint3(
      std::string const & str, baseContainer const * const bC
    );    
    static dtPoint3 getDtPoint3(
	    QDomElement const * toBuildP, baseContainer const * const bC
	  );    
    static std::string createString(
      QDomElement const * toBuildP,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP
    );  
    static void createBasic(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      std::vector< dtPoint2* > * basicP
    );
    static void createBasic(
      QDomElement const * toBuildP,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP,
      std::vector< dtPoint2 * > * basicP
    );
    static void createBasic(
      QDomElement const * toBuildP,
      baseContainer * const bC,      
      vectorHandling< constValue * > const * const cValP, 
      vectorHandling< analyticFunction * > const * const sFunP, 
      std::vector< dtPoint3 > * basicP
    );
    static void createAdvanced( 
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticGeometry * > const * const depAGeoP,            
      vectorHandling< analyticGeometry * > * advancedP 
    );
    /**
     * Creates a clone and returns a pointer.
     */
    static analyticGeometry * createAnalyticGeometry( 
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticGeometry * > const * const depAGeoP
    );
    /**
     * Creates a clone and returns a pointer.
     */    
    static analyticGeometry * createAnalyticGeometry( 
      QDomElement const * toBuildP,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticGeometry * > const * const depAGeoP
    );      
    static void createAdvanced( 
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP, 
      dtTransformer const * const dtTransformerP,                          
      vectorHandling< analyticFunction * > * advancedP 
    );   
    static void createAdvanced( 
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP, 
      vectorHandling< analyticFunction * > * advancedP 
    );
    /**
     * Creates a clone and returns a pointer.
     */    
    static analyticFunction * createAnalyticFunction( 
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP
    ); 
  protected:
    dtXmlParserBase();    
  };
}
#endif	/* DTXMLPARSERBASE_H */

