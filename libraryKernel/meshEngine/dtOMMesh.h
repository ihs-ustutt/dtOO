#ifndef DTMACROMESHSHEET_H
#define	DTMACROMESHSHEET_H

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/Traits.hh>

#include <gmsh/MFace.h>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

class MVertex;
class MElement;

namespace dtOO {
  class dtGmshFace;
  
  struct dtOMMeshTraits : public OpenMesh::DefaultTraits {
    VertexTraits {
      private:
        ::MVertex * _mv;
      public:
        VertexT() : _mv(NULL) {}
        void MVertex( ::MVertex const * const mv ) {
          _mv = const_cast< ::MVertex * >(mv);
        }
        ::MVertex * MVertex( void ) const {
          return _mv;
        }              
    };
    FaceTraits {
      private:
        ::MElement * _me;
        ::MFace _mf;
        dtVector3 _nn;
      public:
        FaceT() : _me(NULL) {
        }
        void MElement( ::MElement const * const me ) {
          _me = const_cast< ::MElement * >(me);
        }        
        ::MElement * MElement( void ) const {
          return _me;
        }                
        void MFace( ::MFace const & mf ) {
          _mf = mf;
        }        
        ::MFace & MFace( void ) {
          return _mf;
        }            
        dtVector3 const & normal( void ) {
          return _nn;
        }
        void setNormal( dtVector3 const & nn ) {
          _nn = nn;
        }        
    };
  };

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
