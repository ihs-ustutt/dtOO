#ifndef QTXMLBASE_H
#define	QTXMLBASE_H

#include <dtOOTypeDef.h>

#include <string>
#include <vector>
#include <dtLinearAlgebra.h>
#include <QtXml/QDomElement>
#include <logMe/dtMacros.h>
#include "qtXmlPrimitive.h"
#include <mainConceptFwd.h>

namespace dtOO {  
  class qtXmlBase : public qtXmlPrimitive {
  public:  
    dt__classOnlyName(qtXmlBase);  
    virtual ~qtXmlBase();  
    static dtReal muParseString( std::string const expression ); 
    static std::vector< dtReal > muParseCSString( std::string const expression ); 
    static dtInt muParseStringInt( std::string const expression );
    static std::vector< dtInt > muParseCSStringInt(
      std::string const expression
    );    
  protected:
    qtXmlBase();    
  };
}
#endif	/* QTXMLBASE_H */

