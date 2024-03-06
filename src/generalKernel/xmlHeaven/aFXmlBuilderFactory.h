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

#ifndef aFXmlBuilderFactory_H
#define	aFXmlBuilderFactory_H

#include <dtOOTypeDef.h>

#include <string>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class aFXmlBuilder;
  
  class aFXmlBuilderFactory {
  public:  
    dt__classOnlyName(aFXmlBuilderFactory);
    virtual ~aFXmlBuilderFactory();
    static bool registrate( aFXmlBuilder const * const );    
    static aFXmlBuilder * create( char const * const str );
    static aFXmlBuilder * create( std::string const str );
  private:
    aFXmlBuilderFactory();
  private:
    static dt__pVH(aFXmlBuilder) _builder;
  };
}
#endif	/* aFXmlBuilderFactory_H */
