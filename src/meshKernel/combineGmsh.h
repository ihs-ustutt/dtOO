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

#ifndef combineGmsh_H
#define	combineGmsh_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "gmshBoundedVolume.h"

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class dtGmshModel;
  class map3dTo3d;
   
  class combineGmsh : public gmshBoundedVolume {
    public:
      dt__class(combineGmsh, boundedVolume);
      dt__classSelfCreate(combineGmsh);               
      combineGmsh();
      virtual ~combineGmsh();     
      virtual void init( 
        ::QDomElement const & element,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV
      );
      virtual void makeGrid( void );      
    private:
      lvH_boundedVolume _bV;
      vectorHandling< dtGmshModel const * > _dtGM;
      dtReal _relTol;
      dtReal _absTol;
      static bool _registrated;
  };
}
#endif	/* combineGmsh_H */
