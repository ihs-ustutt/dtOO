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

#ifndef VECTORCONTAINER_H
#define	VECTORCONTAINER_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <string>
#include <logMe/dtMacros.h>

namespace dtOO {
  class vectorContainer {
    public:    
      dt__classOnlyName(vectorContainer);
      vectorContainer();
      vectorContainer(const vectorContainer& orig);
      virtual ~vectorContainer();
      void add(dtVector3 const vector, std::string const string );
      void add(
        dtVector3 const vector, std::string const string, dtPoint3 const point 
      );
      dtVector3 get(std::string const string ) const;
      dtVector3 operator[]( std::string const label ) const;
      bool has(std::string const string) const;
      bool isEmpty( void ) const;
      std::vector< std::string > labels( void ) const;    
    private:
      std::vector< 
        std::pair <std::pair< dtVector3, dtPoint3 >, std::string > 
      > _pair;
  };
}
#endif	/* VECTORCONTAINER_H */
