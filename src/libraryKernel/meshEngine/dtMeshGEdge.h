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

#ifndef dtMeshGEdge_H
#define	dtMeshGEdge_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMesh1DOperator.h"

namespace dtOO {
  class dtGmshEdge;
  
  class dtMeshGEdge : public dtMesh1DOperator {
    public:
      dt__class(dtMeshGEdge, dtMeshOperator);
      dt__classSelfCreate(dtMeshGEdge);
      dtMeshGEdge();
      dtMeshGEdge(const dtMeshGEdge& orig);
      virtual ~dtMeshGEdge();
      virtual void operator()( dtGmshEdge * dtge );    
    private:
      static bool _registrated;
  };
}
#endif	/* dtMeshGEdge_H */
