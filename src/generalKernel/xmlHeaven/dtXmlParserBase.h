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
      ::QDomElement const * toBuildP,
      baseContainer * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF, 
      aGPtrVec const * const aG,
      dtTransformer const * const dtTransformerP, 
      std::vector< dtPoint3 > * basicP
    );
    static void createBasic(
      ::QDomElement const * toBuildP,
      baseContainer * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF, 
      aGPtrVec const * const aG,
      dtTransformer const * const dtTransformerP, 
      std::vector< dtPoint2 > * basicP
    ); 
    static void createAdvanced( 
      ::QDomElement const * toBuildP,
      baseContainer * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF, 
      aGPtrVec const * const aG,
      dtTransformer const * const dtTransformerP,                        
      aGPtrVec * advancedP 
    );      
    static void createAdvanced( 
      ::QDomElement const * toBuildP,
      baseContainer * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF, 
      dtTransformer const * const dtTransformerP,                          
      aFPtrVec * advancedP 
    );    
  public:     
    //
    // replace constValues, analyticFunctions and analyticGeometries
    //
    using qtXmlBase::replaceDependencies;
    static std::string replaceDependencies( 
      std::string const expression,      
      cVPtrVec const * const cV,
      aFPtrVec const * const aF,
      aGPtrVec const * const aG
    );
    static std::string replaceDependencies( 
      std::string const expression,
      baseContainer const * const bC,      
      cVPtrVec const * const cV,
      aFPtrVec const * const aF,
      aGPtrVec const * const aG
    );    
    //
    // transformer
    //
    static dtTransformer * createTransformer(
      ::QDomElement const * toBuildP,
      baseContainer * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF, 
      aGPtrVec const * const aG
    );       
    static dtTransformer * createTransformer(
      ::QDomElement const * toBuildP,
      baseContainer * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF
    );     
    //
    // dtPoint3
    //
    static void createBasic(
      ::QDomElement const * toBuildP,
      baseContainer * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF, 
      aGPtrVec const * const aG,
      std::vector< dtPoint3 > * basicP
    );
    static void createBasic(
      ::QDomElement const * toBuildP,
      baseContainer * const bC,      
      cVPtrVec const * const cV, 
      aFPtrVec const * const aF, 
      std::vector< dtPoint3 > * basicP
    );             
    static dtPoint3 createDtPoint3(
      ::QDomElement const * toBuildP,
      baseContainer * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF, 
      aGPtrVec const * const aG
    );
    static dtPoint3 createDtPoint3(
      ::QDomElement const * toBuildP,
      baseContainer * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF
    );    
    static dtPoint3 getDtPoint3(
      ::QDomElement const * toBuildP,
      baseContainer const * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF, 
      aGPtrVec const * const aG
    );
    static dtPoint3 getDtPoint3(
      ::QDomElement const * toBuildP,
      baseContainer const * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF
    );        
    static dtPoint3 getDtPoint3(
      std::string const & str, baseContainer const * const bC
    );    
    static dtPoint3 getDtPoint3(
	    ::QDomElement const & toBuildP, baseContainer const * const bC
	  );    
    
    //
    // dtPoint2
    //
    static void createBasic(
      ::QDomElement const * toBuildP,
      baseContainer * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF, 
      aGPtrVec const * const aG,
      std::vector< dtPoint2 > * basicP
    );
    static void createBasic(
      ::QDomElement const * toBuildP,
      baseContainer * const bC,      
      cVPtrVec const * const cV,
      aFPtrVec const * const aF,
      std::vector< dtPoint2 > * basicP
    );     
    //
    // dtVector2
    //
    static dtVector2 createDtVector2(
      ::QDomElement const * toBuildP,
      baseContainer * const bC,
      cVPtrVec const * const cV,  
      aFPtrVec const * const aF, 
      aGPtrVec const * const aG
    );       
    static dtVector2 getDtVector2(
      ::QDomElement const * toBuildP,
      baseContainer const * const bC,
      cVPtrVec const * const cV,  
      aFPtrVec const * const aF, 
      aGPtrVec const * const aG
    );    
    //
    // dtVector3
    //
    static dtVector3 createDtVector3(
      ::QDomElement const * toBuildP,
      baseContainer * const bC,
      cVPtrVec const * const cV,  
      aFPtrVec const * const aF, 
      aGPtrVec const * const aG
    );    
    static dtVector3 createDtVector3(
      ::QDomElement const * toBuildP,
      baseContainer * const bC,
      cVPtrVec const * const cV,  
      aFPtrVec const * const aF
    );
    static dtVector3 getDtVector3(
      ::QDomElement const * toBuildP,
      baseContainer const * const bC,
      cVPtrVec const * const cV,  
      aFPtrVec const * const aF, 
      aGPtrVec const * const aG
    );    
    static dtVector3 getDtVector3(
      ::QDomElement const * toBuildP,
      baseContainer const * const bC,
      cVPtrVec const * const cV,  
      aFPtrVec const * const aF
    );    
    static dtVector3 getDtVector3(
      std::string const & str, baseContainer const * const bC
    );    
    static dtVector3 getDtVector3(
	    ::QDomElement const & toBuildP, baseContainer const * const bC
	  );
    //
    // analyticGeometry
    //    
    static void createAdvanced( 
      ::QDomElement const * toBuildP,
      baseContainer * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF, 
      aGPtrVec const * const aG,            
      aGPtrVec * advancedP 
    );
    static analyticGeometry * createAnalyticGeometry( 
      ::QDomElement const * toBuildP,
      baseContainer * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF, 
      aGPtrVec const * const aG
    );
    static analyticGeometry * createAnalyticGeometry( 
      ::QDomElement const & toBuildP,
      baseContainer * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF, 
      aGPtrVec const * const aG
    );    
    static analyticGeometry * createAnalyticGeometry( 
      ::QDomElement const * toBuildP,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF, 
      aGPtrVec const * const aG
    );      
    static analyticGeometry * createAnalyticGeometry( 
      ::QDomElement const & toBuildP,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF, 
      aGPtrVec const * const aG
    );          
    //
    // analyticFunction
    //
    static void createAdvanced( 
      ::QDomElement const * toBuildP,
      baseContainer * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF, 
      aFPtrVec * advancedP 
    );
    static analyticFunction * createAnalyticFunction( 
      ::QDomElement const * toBuildP,
      baseContainer * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF
    ); 
    static analyticFunction * createAnalyticFunction( 
      ::QDomElement const & toBuild,
      baseContainer * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF 
    );
  protected:
    dtXmlParserBase();    
  };
}
#endif	/* DTXMLPARSERBASE_H */

