#ifndef DTMACROMESHSHEET_H
#define	DTMACROMESHSHEET_H

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>


#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

#include "dtOMMeshTraits.h"

class MVertex;
class MElement;

namespace dtOO {  
  //
  // typedefs
  //
  typedef OpenMesh::PolyMesh_ArrayKernelT< dtOMMeshTraits > omMesh;
  //
  // handles
  //
  typedef ::OpenMesh::VertexHandle omVertexH;  
  typedef ::OpenMesh::HalfedgeHandle omHalfEdgeH;
  typedef ::OpenMesh::EdgeHandle omEdgeH;
  typedef ::OpenMesh::FaceHandle omFaceH;
  //
  // data
  //
  typedef omMesh::VertexData omVertexD;  
  typedef omMesh::HalfedgeData omHalfEdgeD;
  typedef omMesh::EdgeData omEdgeD;
  typedef omMesh::FaceData omFaceD;
  //
  // iterators
  //
  typedef omMesh::VertexIter omVertexI;
  typedef omMesh::VertexVertexIter omVertexVertexI;
  typedef omMesh::FaceIter omFaceI;
  typedef omMesh::VertexFaceIter omVertexFaceI;
  typedef omMesh::ConstFaceVertexIter omConstFaceVertexI;
  //
  // misc
  //
  typedef omMesh::Point omPoint;
  
  /**
   * @brief Interface to OpenMesh
   * 
   * Manages interface to OpenMesh. Please add new functions of
   * of OpenMesh only in this class.
   * See <a href="http://www.openmesh.org">OpenMesh</a>.
   */    
  class dtOMMesh : public omMesh {
    public:
      dt__CLASSNAME(dtOMMesh);
    public:
      dtOMMesh();
      virtual ~dtOMMesh();
      omFaceH addFace( std::vector< ::MVertex * > const & vertices, ::MElement const * const me );  
      void writeMesh(std::string const filename) const;
      void add( const dtOMMesh &toAdd );      
    private:
      omVertexH addVertex( ::MVertex const * const &mv );      
      omFaceH addFace( std::vector< ::MVertex * > const & vertices );          
    private:      
      std::map< ::MVertex const *, omVertexH > _om_gmsh;
    
  };
}
#endif	/* DTMACROMESHSHEET_H */
