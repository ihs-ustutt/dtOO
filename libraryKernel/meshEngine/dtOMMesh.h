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
  typedef ::OpenMesh::HalfedgeHandle omHalfedgeH;
  typedef ::OpenMesh::EdgeHandle omEdgeH;
  typedef ::OpenMesh::FaceHandle omFaceH;
  //
  // data
  //
  typedef omMesh::VertexData omVertexD;  
  typedef omMesh::HalfedgeData omHalfedgeD;
  typedef omMesh::EdgeData omEdgeD;
  typedef omMesh::FaceData omFaceD;
  //
  // iterators
  //
  typedef omMesh::VertexIter omVertexI;
  typedef omMesh::VertexVertexIter omVertexVertexI;
  typedef omMesh::FaceIter omFaceI;
  typedef omMesh::VertexFaceIter omVertexFaceI;
  typedef omMesh::ConstVertexFaceIter omConstVertexFaceI;
  typedef omMesh::FaceVertexIter omFaceVertexI;
  typedef omMesh::ConstFaceVertexIter omConstFaceVertexI;
  typedef omMesh::VertexEdgeIter omVertexEdgeI;
  typedef omMesh::ConstVertexEdgeIter omConstVertexEdgeI;
  typedef omMesh::VertexOHalfedgeIter omVertexOHalfedgeI;
  typedef omMesh::ConstVertexOHalfedgeIter omConstVertexOHalfedgeI;
  //
  // misc
  //
  typedef omMesh::Point omPoint;
  typedef omMesh::Scalar omScalar;
  
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
      omFaceH addFace( ::MElement const * const me );  
      void writeMesh(std::string const filename) const;
      void add( const dtOMMesh &toAdd );      
      std::map< ::MVertex const *, omVertexH > const & omGmsh( void ) const;
    private:
      omVertexH addVertex( ::MVertex const * const &mv );      
      omFaceH addFace( std::vector< ::MVertex * > const & vertices );          
    private:      
      std::map< ::MVertex const *, omVertexH > _om_gmsh;
    
  };
}
#endif	/* DTMACROMESHSHEET_H */
