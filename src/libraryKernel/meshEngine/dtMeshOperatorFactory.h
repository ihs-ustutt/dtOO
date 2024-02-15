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

#ifndef dtMeshOperatorFactory_H
#define	dtMeshOperatorFactory_H

#include <dtOOTypeDef.h>

#include <string>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtMeshOperator;
  
  class dtMeshOperatorFactory {
    public:
      dt__classOnlyName(dtMeshOperatorFactory);
      virtual ~dtMeshOperatorFactory();
      static bool registrate( dtMeshOperator const * const );
      static dtMeshOperator * create(char const * const str);
      static dtMeshOperator * create( std::string const str );
    private:
      dtMeshOperatorFactory();
    private:
      static dt__pVH(dtMeshOperator) _product;
  };
}

#endif	/* dtMeshOperatorFactory_H */
