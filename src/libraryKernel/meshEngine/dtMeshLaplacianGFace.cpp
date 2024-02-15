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

#include "dtMeshLaplacianGFace.h"
#include "dtMeshGFace.h"

#include <xmlHeaven/qtXmlPrimitive.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshModel.h>
#include <gmsh/meshGFace.h>
#include <gmsh/GmshDefines.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include "dtMeshOperatorFactory.h"

namespace dtOO {
  bool dtMeshLaplacianGFace::_registrated 
  =
  dtMeshOperatorFactory::registrate(
    dt__tmpPtr(dtMeshLaplacianGFace, new dtMeshLaplacianGFace())
  );
 
  dtMeshLaplacianGFace::dtMeshLaplacianGFace() : dtMeshTransfinite2DOperator() {
    
  }

  dtMeshLaplacianGFace::dtMeshLaplacianGFace(
    const dtMeshLaplacianGFace& orig
  ) : dtMeshTransfinite2DOperator(orig) {
  }

  dtMeshLaplacianGFace::~dtMeshLaplacianGFace() {
    
  }
  
  void dtMeshLaplacianGFace::operator()( dtGmshFace * dtgf ) {

    dtgf->model()->setCurrentMeshEntity(dtgf);

    if(dtgf->meshAttributes.method != MESH_TRANSFINITE) {
      dtMeshGFace()(dtgf);
      return;
    }

    // destroy the mesh if it exists
    deMeshGFace()(dtgf);    
    Msg::Info(
      "Meshing surface %d / %s ( dtMeshLaplacianGFace )", 
      dtgf->tag(), dtgf->getPhysicalString().c_str()
    );


    //
    // reparam vertices
    //
    twoDArrayHandling< dtPoint2 > pUV
    = 
    dtMeshTransfinite2DOperator::computeEdgeLoops(dtgf);
    
    //
    // get transfinite vertices
    //
    std::vector< std::vector< ::MVertex * > > & tab = dtgf->transfinite_vertices;
    dtInt L = pUV.size(0)-1;
    dtInt H = pUV.size(1)-1;
    
    dt__forInnerIndex(pUV, ii) {
      dt__forInnerIndex(pUV[ii], jj) {
        pUV[ii][jj]
        =
        dtLinearAlgebra::intersectionPoint( 
          dtLine2(pUV[ii][0], pUV[ii][H]), 
          dtLine2(pUV[0][jj], pUV[L][jj])
        );
      }
    }
    

    //
    // create vertices and elements
    //     
    for(int i = 1; i < L; i++) {
      for(int j = 1; j < H; j++) {
        GPoint gp = dtgf->point( pUV[i][j].x(), pUV[i][j].y() );
        tab[i][j]
        = 
        new MFaceVertex(
          gp.x(), gp.y(), gp.z(), dtgf, pUV[i][j].x(), pUV[i][j].y()
        );
        dtgf->mesh_vertices.push_back(tab[i][j]);
      }
    }
    
    dtMeshTransfinite2DOperator::createTransfiniteElements( dtgf );
    
    dtgf->meshStatistics.status = GFace::DONE;
  }
}
