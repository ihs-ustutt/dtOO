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
    dt__classOnlyName(dtXmlParserBase);
    virtual ~dtXmlParserBase();
  private:
    static void createBasic(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF, 
      vectorHandling< analyticGeometry * > const * const aG,
      dtTransformer const * const dtTransformerP, 
      std::vector< dtPoint3 > * basicP
    );
    static void createBasic(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF, 
      vectorHandling< analyticGeometry * > const * const aG,
      dtTransformer const * const dtTransformerP, 
      std::vector< dtPoint2 > * basicP
    ); 
    static void createAdvanced( 
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF, 
      vectorHandling< analyticGeometry * > const * const aG,
      dtTransformer const * const dtTransformerP,                        
      vectorHandling< analyticGeometry * > * advancedP 
    );       
  public:     
    static dtTransformer * createTransformer(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF, 
      vectorHandling< analyticGeometry * > const * const aG
    );       
    static dtTransformer * createTransformer(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF
    );     
    static void createBasic(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF, 
      vectorHandling< analyticGeometry * > const * const aG,
      std::vector< dtPoint3 > * basicP
    );
    static dtPoint3 createDtPoint3(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF, 
      vectorHandling< analyticGeometry * > const * const aG
    );
    static dtPoint3 createDtPoint3(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF
    );    
    static dtVector2 createDtVector2(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,  
      vectorHandling< analyticFunction * > const * const aF, 
      vectorHandling< analyticGeometry * > const * const aG
    );       
    static dtVector3 createDtVector3(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,  
      vectorHandling< analyticFunction * > const * const aF, 
      vectorHandling< analyticGeometry * > const * const aG
    );    
    static dtVector3 createDtVector3(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,  
      vectorHandling< analyticFunction * > const * const aF
    );
    
    static dtVector2 getDtVector2(
      QDomElement const * toBuildP,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,  
      vectorHandling< analyticFunction * > const * const aF, 
      vectorHandling< analyticGeometry * > const * const aG
    );
    static dtPoint3 getDtPoint3(
      QDomElement const * toBuildP,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF, 
      vectorHandling< analyticGeometry * > const * const aG
    );
    static dtPoint3 getDtPoint3(
      QDomElement const * toBuildP,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF
    );    
    static dtVector3 getDtVector3(
      QDomElement const * toBuildP,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,  
      vectorHandling< analyticFunction * > const * const aF, 
      vectorHandling< analyticGeometry * > const * const aG
    );    
    static dtVector3 getDtVector3(
      QDomElement const * toBuildP,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,  
      vectorHandling< analyticFunction * > const * const aF
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
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF
    );  
    static void createBasic(
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF, 
      vectorHandling< analyticGeometry * > const * const aG,
      std::vector< dtPoint2 > * basicP
    );
    static void createBasic(
      QDomElement const * toBuildP,
      baseContainer * const bC,      
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      std::vector< dtPoint2 > * basicP
    );
    static void createBasic(
      QDomElement const * toBuildP,
      baseContainer * const bC,      
      vectorHandling< constValue * > const * const cV, 
      vectorHandling< analyticFunction * > const * const aF, 
      std::vector< dtPoint3 > * basicP
    );
    /**
     * Creates a clone and put it in advancedP.
     */            
    static void createAdvanced( 
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF, 
      vectorHandling< analyticGeometry * > const * const aG,            
      vectorHandling< analyticGeometry * > * advancedP 
    );
    /**
     * Creates a clone and returns a pointer.
     */
    static analyticGeometry * createAnalyticGeometry( 
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF, 
      vectorHandling< analyticGeometry * > const * const aG
    );
    /**
     * Creates a clone and returns a pointer.
     */    
    static analyticGeometry * createAnalyticGeometry( 
      QDomElement const * toBuildP,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF, 
      vectorHandling< analyticGeometry * > const * const aG
    );      
    /**
     * Creates a clone and put it in advancedP.
     */            
    static void createAdvanced( 
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF, 
      dtTransformer const * const dtTransformerP,                          
      vectorHandling< analyticFunction * > * advancedP 
    );
    /**
     * Creates a clone and put it in advancedP.
     */        
    static void createAdvanced( 
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF, 
      vectorHandling< analyticFunction * > * advancedP 
    );
    /**
     * Creates a clone and returns a pointer.
     */    
    static analyticFunction * createAnalyticFunction( 
      QDomElement const * toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF
    ); 
  protected:
    dtXmlParserBase();    
  };
}
#endif	/* DTXMLPARSERBASE_H */

