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

#ifndef BASECONTAINER_H
#define	BASECONTAINER_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>

namespace dtOO {
  class pointContainer;
  class vectorContainer;
  class transformerContainer;
  
  class baseContainer {
    public:
      dt__classOnlyName(baseContainer);    
      baseContainer();
      pointContainer * const ptrPointContainer( void );
  	  pointContainer const * const constPtrPointContainer( void ) const;
      vectorContainer * const ptrVectorContainer( void );
      vectorContainer const * const constPtrVectorContainer( void ) const;
      transformerContainer * const ptrTransformerContainer( void );
      transformerContainer const * const constPtrTransformerContainer( 
        void 
      ) const;
      pointContainer const & dtPoint3( void );
      vectorContainer const & dtVector3( void );
      transformerContainer const & dtTransformer( void );
      std::vector< std::string > labels( void ) const;
      virtual ~baseContainer();
      void clear( void );
    private:
      dt__pH(pointContainer) _pC;
      dt__pH(vectorContainer) _vC;
      dt__pH(transformerContainer) _tC;
  };
}
#endif	/* BASECONTAINER_H */
