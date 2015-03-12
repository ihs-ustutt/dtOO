#ifndef DTMACROMESHSHEET_H
#define	DTMACROMESHSHEET_H

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/Traits.hh>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

class MVertex;

namespace dtOO {
  struct dtOMMeshTraits : public OpenMesh::DefaultTraits {
    VertexTraits {
    public:
      ::MVertex const * _mv;
    public:
      VertexT() : _mv(NULL) {}
      ::MVertex const * & MVertex( void ) {
        return _mv;
      }
    };
  };

  typedef OpenMesh::PolyMesh_ArrayKernelT< dtOMMeshTraits > omMesh;
  typedef ::OpenMesh::VertexHandle omVertexH;
  typedef omMesh::VertexData omVertexD;
  typedef ::OpenMesh::HalfedgeHandle omHalfEdgeH;
  typedef ::OpenMesh::EdgeHandle omEdgeH;
  typedef ::OpenMesh::FaceHandle omFaceH;
  typedef omMesh::VertexIter omVertexI;
  typedef omMesh::VertexVertexIter omVertexVertexI;   
  typedef omMesh::Point omPoint;
  
  /**
   * @brief Interface to OpenMesh
   * 
   * Manages interface to OpenMesh. Please add new functions of
   * of OpenMesh only in this class.
   * See <a href="http://www.openmesh.org">OpenMesh</a>.
   */    
  class dtOMMesh : public  omMesh {
    public:
      dt__CLASSNAME(dtOMMesh);
    public:
      dtOMMesh();
      dtOMMesh(const dtOMMesh& orig);
      virtual ~dtOMMesh();
      void addVertex( MVertex const * const &mv );
      void addFace(
        ::MVertex const * const &v0, 
        ::MVertex const * const &v1, 
        ::MVertex const * const &v2, 
        ::MVertex const * const &v3
      );
      void addFace(
        ::MVertex const * const &v0, 
        ::MVertex const * const &v1, 
        ::MVertex const * const &v2
      );
      void addFace( std::vector< MVertex * > const & vertices );      
      void writeMesh(std::string const filename) const;
    private:
      std::map< ::MVertex const *, omVertexH > _om_gmsh;
    
  };
}
#endif	/* DTMACROMESHSHEET_H */
