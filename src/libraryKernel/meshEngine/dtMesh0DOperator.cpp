#include "dtMesh0DOperator.h"

#include <xmlHeaven/qtXmlPrimitive.h>
#include "dtGmshVertex.h"
#include <gmsh/MVertex.h>
#include <gmsh/MPoint.h>

namespace dtOO {
  dtMesh0DOperator::dtMesh0DOperator() {
  }

  dtMesh0DOperator::~dtMesh0DOperator() {
  }

  void dtMesh0DOperator::init(
    ::QDomElement const & element,
    baseContainer const * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    labeledVectorHandling< dtMeshOperator * > const * const mO    
  ) {
    dtMeshOperator::init(element, bC, cV, aF, aG, bV, mO);
  }
  
  void dtMesh0DOperator::correspondingVertex( 
    dtGmshVertex * master, dtGmshVertex * slave 
  ) {  
    Msg::Info(
      "Corresponding vertex %d (master) -> %d (slave)", 
      master->tag(), slave->tag()
    );   
    //
    // corresponding vertex
    //
    if( slave->mesh_vertices.empty() ) {
      slave->mesh_vertices.push_back(
        new ::MVertex(slave->x(), slave->y(), slave->z(), slave)
      );
    }
    if (slave->points.empty()) {
      slave->points.push_back(
        new ::MPoint( slave->mesh_vertices.back() )
      );
    }
    
    slave->correspondingVertices[ slave->mesh_vertices[0] ] 
    = 
    master->mesh_vertices[0];

  }
}
