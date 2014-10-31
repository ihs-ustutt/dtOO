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
    dt__CLASSNAME(qtXmlBase);  
    qtXmlBase();
    virtual ~qtXmlBase();  
  protected:
    /**
     * replace const values and functions with numeric values
     */  
    std::string replaceUsedFunctions(std::string const expression,
                                vectorHandling< constValue * > const * const cValP,
                                vectorHandling< analyticFunction * > const * const sFunP) const;
    /**
     * evaluate a string with muparser
     */  
    float muParseString( std::string const expression ) const; 
    int muParseStringInt( std::string const expression ) const;
    float getAttributeFloatMuParse( 
      std::string const attName, 
      QDomElement const element, 
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP 
    ) const;
    int getAttributeIntMuParse( 
      std::string const attName, 
      QDomElement const element, 
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP 
    ) const;    
    dtVector3 getDtVector3( 
      QDomElement const element,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP 
    ) const;
  };
}
#endif	/* QTXMLBASE_H */

