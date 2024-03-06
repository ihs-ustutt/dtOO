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

#ifndef dtMeshGVertex_H
#define	dtMeshGVertex_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMesh0DOperator.h"

namespace dtOO {
  class dtGmshVertex;
  
  class dtMeshGVertex : public dtMesh0DOperator {
    public:
      dt__class(dtMeshGVertex, dtMeshOperator);     
      dt__classSelfCreate(dtMeshGVertex);
      dtMeshGVertex();
      dtMeshGVertex(const dtMeshGVertex& orig);
      virtual ~dtMeshGVertex();
      virtual void operator()( dtGmshVertex * dtgv );    
    private:
      static bool _registrated;
  };
}
#endif	/* dtMeshGVertex_H */
