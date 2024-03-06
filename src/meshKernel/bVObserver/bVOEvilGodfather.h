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

#ifndef bVOEvilGodfather_H
#define	bVOEvilGodfather_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "bVOInterface.h"

class GRegion;
class MElement;
class MTetrahedron;
class MHexahedron;
class MPyramid;
class MVertex;

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class bVOEvilGodfather : public bVOInterface {
    public:
      dt__class(bVOEvilGodfather, bVOInterface);
      dt__classSelfCreate(bVOEvilGodfather);
      bVOEvilGodfather();
      virtual ~bVOEvilGodfather();
      virtual void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        boundedVolume * attachTo
      );    
      virtual void postUpdate( void );
    private:
      static void perturbElement(
        ::MElement * const jinx, dtReal const & perturbFactor
      );
      static void divideTetrahedron(
        ::MTetrahedron * const jinx, 
        std::pair< std::vector< ::MTetrahedron * >, ::GRegion * > & newTets,
        ::MVertex * & newVertex
      );    
      static void divideHexahedron(
        ::MHexahedron * const jinx,
        std::pair< std::vector< ::MPyramid * >, ::GRegion * > & newPyrs,
        ::MVertex * & newVertex     
      );        
    private:
      dtReal _perHex;
      dtReal _perTet;
      dtReal _perturbTet;
      dtReal _perturbHex;
      static bool _registrated;
  };
}
#endif	/* bVOEvilGodfather_H */
