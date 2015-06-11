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
    static std::string replaceUsedFunctions(
      std::string const expression,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP
    );
    /**
     * evaluate a string with muparser
     */  
    static float muParseString( std::string const expression ); 
    static int muParseStringInt( std::string const expression );
    static float getAttributeFloatMuParse( 
      std::string const attName, 
      QDomElement const element, 
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP 
    );
    static int getAttributeIntMuParse( 
      std::string const attName, 
      QDomElement const element, 
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP 
    );
    static std::vector< float > getAttributeFloatVectorMuParse( 
		std::string const attName, 
		QDomElement const element, 
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF 
    );        
    static std::vector< int > getAttributeIntVectorMuParse( 
      std::string const attName, 
      QDomElement const element, 
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF 
    );
  protected:
    qtXmlBase();    
  };
}
#endif	/* QTXMLBASE_H */

