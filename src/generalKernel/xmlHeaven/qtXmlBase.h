/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

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
