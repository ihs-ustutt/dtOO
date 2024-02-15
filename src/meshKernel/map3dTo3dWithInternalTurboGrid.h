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

#ifndef MAP3DTO3DWITHINTERNALTURBOGRID_H
#define	MAP3DTO3DWITHINTERNALTURBOGRID_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "gmshBoundedVolume.h"
#include <interfaceHeaven/twoDArrayHandling.h>


class MVertex;
class MElement;

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class map2dTo3d;
  class map3dTo3d;
  class dtGmshFace;
  class dtGmshRegion;
  
  class map3dTo3dWithInternalTurboGrid : public gmshBoundedVolume {
    public:
      dt__class(map3dTo3dWithInternalTurboGrid, boundedVolume);    
      dt__classSelfCreate(map3dTo3dWithInternalTurboGrid);
      map3dTo3dWithInternalTurboGrid();
      virtual ~map3dTo3dWithInternalTurboGrid();
      virtual void init( 
        ::QDomElement const & element,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV
      );    
      virtual void makeGrid(void);
      virtual void makePreGrid(void);
    private:
      std::string _meshFileName;
      dt__pH(map3dTo3d) _channel;
      dt__pH(map2dTo3d) _internal;
      dtInt _nInternalCuts;
      dtInt _nPoints;
      dtInt _nInternals;
      dtVector3 _vv;
      std::string _directory;
      std::string _script;   
      static bool _registrated;
  };
}

#endif	/* MAP3DTO3DWITHINTERNALTURBOGRID_H */
