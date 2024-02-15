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

#ifndef aGXmlBuilderFactory_H
#define	aGXmlBuilderFactory_H

#include <dtOOTypeDef.h>

#include <string>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class aGXmlBuilder;
  
  class aGXmlBuilderFactory {
  public:  
    dt__classOnlyName(aGXmlBuilderFactory);
    virtual ~aGXmlBuilderFactory();
    static bool registrate( aGXmlBuilder const * const );        
    static aGXmlBuilder* create( char const * const str );
    static aGXmlBuilder* create( std::string const str );
    static aGXmlBuilderFactory * instance( void );    
  private:
    aGXmlBuilderFactory();
  private:
    static dt__pVH(aGXmlBuilder) _builder;    
  };
}
#endif	/* aGXmlBuilderFactory_H */
