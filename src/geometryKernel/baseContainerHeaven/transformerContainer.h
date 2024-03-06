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

#ifndef TRANSFORMERCONTAINER_H
#define	TRANSFORMERCONTAINER_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <string>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/labeledVectorHandling.h>

namespace dtOO {
  class dtTransformer;
  
  class transformerContainer {
    public:    
      dt__classOnlyName(transformerContainer);
      transformerContainer();
      transformerContainer(const transformerContainer& orig);
      virtual ~transformerContainer();
      void add(dtTransformer const * const dtT );
      dtTransformer const * get( std::string const string ) const;
      dtTransformer const * operator[]( std::string const label ) const;
      bool has(std::string const string) const;
      bool isEmpty( void ) const;
      std::vector< std::string > labels( void ) const;
      labeledVectorHandling< dtTransformer * > & lVH( void );
      const labeledVectorHandling< dtTransformer * > & lVH( void ) const;
    private:
      labeledVectorHandling< dtTransformer * > _dtT;
  };
}
#endif	/* TRANSFORMERCONTAINER_H */
