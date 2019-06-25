#ifndef QTXMLBASE_H
#define	QTXMLBASE_H

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
    static float muParseString( std::string const expression ); 
    static std::vector< float > muParseCSString( std::string const expression ); 
    static int muParseStringInt( std::string const expression );
    static std::vector< int > muParseCSStringInt(
      std::string const expression
    );    
  protected:
    qtXmlBase();    
  };
}
#endif	/* QTXMLBASE_H */

