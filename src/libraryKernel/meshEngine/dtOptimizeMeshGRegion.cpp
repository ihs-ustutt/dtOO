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

#include "dtOptimizeMeshGRegion.h"

#include "dtGmshFace.h"
#include "dtGmshModel.h"
#include "dtGmshRegion.h"
#include <gmsh/MVertex.h>
#include <gmsh/meshGRegion.h>
#include <logMe/logMe.h>
// #include <gmsh/Context.h>
#include "dtMeshOperatorFactory.h"
#include <gmsh/MPyramid.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/MTetrahedron.h>
#include <gmsh/MTriangle.h>

namespace dtOO {
bool dtOptimizeMeshGRegion::_registrated = dtMeshOperatorFactory::registrate(
  dt__tmpPtr(dtOptimizeMeshGRegion, new dtOptimizeMeshGRegion())
);

dtOptimizeMeshGRegion::dtOptimizeMeshGRegion() : dtMesh3DOperator()
{
  _gmsh = true;
  _netgen = true;
}

dtOptimizeMeshGRegion::dtOptimizeMeshGRegion(bool gmsh, bool netgen)
  : dtMesh3DOperator()
{
  _gmsh = gmsh;
  _netgen = netgen;
}

dtOptimizeMeshGRegion::~dtOptimizeMeshGRegion() {}

dtOptimizeMeshGRegion::dtOptimizeMeshGRegion(const dtOptimizeMeshGRegion &orig)
  : dtMesh3DOperator(orig)
{
  _gmsh = orig._gmsh;
  _netgen = orig._netgen;
}

void dtOptimizeMeshGRegion::operator()(dtGmshRegion *dtgr)
{
  Msg::Info("dtOptimizeMeshGRegion()() volume %d", dtgr->tag());

  if (_gmsh)
    ::optimizeMeshGRegion()(dtgr);

  if (_netgen)
  {
    //
    // create cloneRegion and cloneFace
    //
    dtGmshRegion cloneRegion(
      dtgr->model(), dtgr->refDtGmshModel().getMaxRegionTag() + 1
    );
    dtGmshFace cloneFace(
      dtgr->model(), dtgr->refDtGmshModel().getMaxFaceTag() + 1
    );
    dtGmshFace cloneFaceApex(
      dtgr->model(), dtgr->refDtGmshModel().getMaxFaceTag() + 1
    );

    //
    // add region to model
    //
    cloneRegion.addGEntity(&cloneFace);
    cloneRegion.addGEntity(&cloneFaceApex);
    dtgr->model()->add(&cloneFace);
    dtgr->model()->add(&cloneFaceApex);
    dtgr->model()->add(&cloneRegion);

    //
    // add mesh entities to cloneFace
    //
    dt__forAllRefAuto(dtgr->vertices(), anEnt)
    {
      dt__forAllRefAuto(anEnt->mesh_vertices, aVert)
      {
        cloneFace.addMeshVertex(aVert);
      }
    }
    dt__forAllRefAuto(dtgr->edges(), anEnt)
    {
      dt__forAllRefAuto(anEnt->mesh_vertices, aVert)
      {
        cloneFace.addMeshVertex(aVert);
      }
    }
    dt__forAllRefAuto(dtgr->faces(), aFace)
    {
      //
      // mesh vertices
      //
      dt__forAllRefAuto(aFace->mesh_vertices, aVert)
      {
        cloneFace.addMeshVertex(aVert);
      }
      //
      // mesh triangles
      //
      if (aFace->triangles.size())
      {
        dt__forAllRefAuto(aFace->triangles, aTri)
        {
          cloneFace.addElement((::MElement *)aTri);
        }
      }
    }

    //
    // tetrahedra
    //
    dt__forAllRefAuto(dtgr->tetrahedra, aTet)
    {
      cloneRegion.addElement((::MElement *)aTet);
    }
    //
    // pyramids
    //
    dt__forAllRefAuto(dtgr->pyramids, aPyr)
    {
      dt__throwIf(
        aPyr->getVertex(0)->onWhat()->dim() > 2 &&
          aPyr->getVertex(1)->onWhat()->dim() > 2 &&
          aPyr->getVertex(2)->onWhat()->dim() > 2 &&
          aPyr->getVertex(3)->onWhat()->dim() > 2,
        operator()()
      );
      aPyr->getVertex(4)->setEntity(&cloneFaceApex);
      cloneFaceApex.addElement(new ::MTriangle(
        aPyr->getVertex(0), aPyr->getVertex(1), aPyr->getVertex(4)
      ));
      cloneFaceApex.addElement(new ::MTriangle(
        aPyr->getVertex(1), aPyr->getVertex(2), aPyr->getVertex(4)
      ));
      cloneFaceApex.addElement(new ::MTriangle(
        aPyr->getVertex(2), aPyr->getVertex(3), aPyr->getVertex(4)
      ));
      cloneFaceApex.addElement(new ::MTriangle(
        aPyr->getVertex(3), aPyr->getVertex(0), aPyr->getVertex(4)
      ));
    }
    //
    // add entities to cloneRegion
    //
    dt__forAllRefAuto(dtgr->mesh_vertices, aVert)
    {
      if (aVert->onWhat() != &cloneFaceApex)
      {
        cloneRegion.addMeshVertex(aVert);
        aVert->setEntity(&cloneRegion);
      }
    }

    //
    // do optimization
    //
    ::optimizeMeshGRegionNetgen()(&cloneRegion);

    //
    // clean and reset dtgr mesh
    //
    std::vector<::MVertex *> newVert;
    dt__forAllRefAuto(dtgr->mesh_vertices, aVert)
    {
      if (aVert->onWhat() == &cloneFaceApex)
      {
        aVert->setEntity(dtgr);
        newVert.push_back(aVert);
      }
    }
    dtgr->mesh_vertices.clear();
    dt__forAllRefAuto(newVert, aVert) dtgr->addMeshVertex(aVert);
    dtgr->tetrahedra.clear();

    //
    // transfer mesh vertices and mesh elements of cloneFace,
    // cloneFaceApex and cloneRegion back to dtgr
    //
    dt__forAllRefAuto(cloneRegion.mesh_vertices, aVert)
    {
      dtgr->addMeshVertex(aVert);
      aVert->setEntity(dtgr);
    }
    dt__forAllRefAuto(cloneRegion.tetrahedra, aTet)
    {
      dtgr->addElement((::MElement *)aTet);
    }
    dt__forAllRefAuto(cloneFaceApex.triangles, aTri) delete aTri;

    //
    // delete GEntities
    //
    cloneFace.mesh_vertices.clear();
    cloneFace.triangles.clear();
    cloneRegion.mesh_vertices.clear();
    cloneRegion.tetrahedra.clear();
    cloneFaceApex.mesh_vertices.clear();
    cloneFaceApex.triangles.clear();

    cloneRegion.deleteFace(&cloneFace);
    cloneRegion.deleteFace(&cloneFaceApex);
    cloneFace.delRegion(&cloneRegion);
    cloneFaceApex.delRegion(&cloneRegion);

    dtgr->model()->remove(&cloneRegion);
    dtgr->model()->remove(&cloneFace);
    dtgr->model()->remove(&cloneFaceApex);
  }
}
} // namespace dtOO
