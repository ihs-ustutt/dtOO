#include "dtMeshGVertex.h"

#include "dtGmshVertex.h"
//#include <gmsh/meshGEdge.h>
#include <gmsh/MVertex.h>
#include <gmsh/MPoint.h>

namespace dtOO {
  dtMeshGVertex::dtMeshGVertex() : dtMesh0DOperator() {
  }

  dtMeshGVertex::dtMeshGVertex(const dtMeshGVertex& orig) 
    : dtMesh0DOperator(orig) {
  }

  dtMeshGVertex::~dtMeshGVertex() {
  } 

  void dtMeshGVertex::operator()( dtGmshVertex * dtgv) {

    Msg::Info("Meshing vertex %d ( dtGmshVertex )", dtgv->tag());

    //
    // mesh vertex
    //
    if( dtgv->mesh_vertices.empty() ) {
      dtgv->mesh_vertices.push_back(
        new ::MVertex(dtgv->x(), dtgv->y(), dtgv->z(), dtgv)
      );
    }
    if (dtgv->points.empty()) {
      dtgv->points.push_back(
        new ::MPoint( dtgv->mesh_vertices.back() )
      );
    }
    
    dtgv->_status = ::GEntity::MeshGenerationStatus::DONE;
  }
}

