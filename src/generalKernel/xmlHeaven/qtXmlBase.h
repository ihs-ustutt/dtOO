#ifndef QTXMLBASE_H
#define	QTXMLBASE_H

#include <string>
#include <vector>
#include <dtLinearAlgebra.h>
#include <QtXml/QDomElement>
#include <logMe/dtMacros.h>
#include "qtXmlPrimitive.h"
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class analyticFunction;
  class constValue;
  
  class qtXmlBase : public qtXmlPrimitive {
  public:  
    dt__classOnlyName(qtXmlBase);  
    virtual ~qtXmlBase();  
    /**
     * replace const values and functions with numeric values
     */  
    static std::string replaceDependencies(
      std::string const expression,
      vectorHandling< constValue * > const * const cValP
    );    
    static std::string replaceDependencies(
      std::string const expression,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF
    );
    /**
     * evaluate a string with muparser
     */  
    static float muParseString( std::string const expression ); 
    static std::vector< float > muParseCSString( std::string const expression ); 
    static int muParseStringInt( std::string const expression );
    static std::vector< int > muParseCSStringInt(
      std::string const expression
    );    
    static float getAttributeFloatMuParse( 
      std::string const attName, 
      ::QDomElement const element, 
      vectorHandling< constValue * > const * const cV
    );
    static float getAttributeFloatMuParse( 
      std::string const attName, 
      ::QDomElement const element, 
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF 
    );
    static int getAttributeIntMuParse( 
      std::string const attName, 
      ::QDomElement const element, 
      vectorHandling< constValue * > const * const cV
    );    
    static int getAttributeIntMuParse( 
      std::string const attName, 
      ::QDomElement const element, 
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF 
    );
    static std::vector< float > getAttributeFloatVectorMuParse( 
      std::string const attName, 
      ::QDomElement const element, 
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF 
    );        
    static std::vector< double > getAttributeDoubleVectorMuParse( 
      std::string const attName, 
      ::QDomElement const element, 
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF 
    );            
    static std::vector< int > getAttributeIntVectorMuParse( 
      std::string const attName, 
      ::QDomElement const element, 
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF 
    );
  protected:
    qtXmlBase();    
  protected:    
    static std::string _CALCSIGN;
    static std::string _POINTSIGN;
    static std::string _DTTSIGN;
    static std::string _CVSIGN;
    static std::string _AFSIGN;
    static std::string _AGSIGN;
  };
}
#endif	/* QTXMLBASE_H */

