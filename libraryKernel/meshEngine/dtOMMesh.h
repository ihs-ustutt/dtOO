#ifndef dtOMMesh_H
#define	dtOMMesh_H

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
  typedef omMesh::ConstVertexIter omConstVertexI;
  typedef omMesh::VertexVertexIter omVertexVertexI;
  typedef omMesh::ConstVertexVertexIter omConstVertexVertexI;
  typedef omMesh::FaceIter omFaceI;
  typedef omMesh::ConstFaceIter omConstFaceI;
  typedef omMesh::EdgeIter omEdgeI;
  typedef omMesh::ConstEdgeIter omConstEdgeI;  
  typedef omMesh::VertexFaceIter omVertexFaceI;
  typedef omMesh::FaceFaceIter omFaceFaceI;
  typedef omMesh::ConstFaceFaceIter omConstFaceFaceI;  
  typedef omMesh::ConstVertexFaceIter omConstVertexFaceI;
  typedef omMesh::FaceVertexIter omFaceVertexI;
  typedef omMesh::ConstFaceVertexIter omConstFaceVertexI;
  typedef omMesh::VertexEdgeIter omVertexEdgeI;
  typedef omMesh::ConstVertexEdgeIter omConstVertexEdgeI;
  typedef omMesh::VertexOHalfedgeIter omVertexOHalfedgeI;
  typedef omMesh::ConstVertexOHalfedgeIter omConstVertexOHalfedgeI;
  typedef omMesh::VertexIHalfedgeIter omVertexIHalfedgeI;
  typedef omMesh::ConstVertexIHalfedgeIter omConstVertexIHalfedgeI;
  typedef omMesh::FaceEdgeIter omFaceEdgeI;
  typedef omMesh::ConstFaceEdgeIter omConstFaceEdgeI;
  typedef omMesh::FaceHalfedgeIter omFaceHalfedgeI;
  typedef omMesh::ConstFaceHalfedgeIter omConstFaceHalfedgeI;
  
  //
  // misc
  //
  typedef omMesh::Point omPoint;
  typedef omMesh::Scalar omScalar;
  typedef omMesh::Normal omNormal;
  
  class dtOMField;
  /**
   * @brief Interface to OpenMesh
   * 
   * Manages interface to OpenMesh. Please add new functions of
   * of OpenMesh only in this class.
   * See <a href="http://www.openmesh.org">OpenMesh</a>.
   */    
  class dtOMMesh : public omMesh {
    public:
      dt__classOnlyName(dtOMMesh);
      dtOMMesh();
      dtOMMesh( dtOMMesh const & orig );
      virtual ~dtOMMesh();  
      void update( void );                  
      //
      // add new vertices and elements
      //
      omFaceH addFace( omFaceD const & fD );      
      omFaceH addFace( ::MElement const * const me );  
      void add( const dtOMMesh &toAdd );      
      void addInv( const dtOMMesh &toAdd );      
      //
      // access
      //
      ::MVertex * operator[](omVertexH const & vH);
      omVertexH const & at( ::MVertex const * const mv ) const;      
      ::MVertex const * const at(omVertexH const & vH) const; 
      ::MElement const * const at(omFaceH const & fH) const;      
      omFaceH const & at( ::MElement const * const me ) const;      
      //
      // check functions
      //
      bool contains( omFaceH const & fH, omEdgeH const & eH ) const;
      bool contains( omFaceH const & fH, omVertexH const & vH ) const;
      bool intersection( 
        std::vector< omFaceH > const & fH, 
        dtPoint3 const & start, dtPoint3 const & target 
      ) const;      
      std::vector< omEdgeH > oneRingEdgeH( omVertexH const & vH ) const;
      //
      // conversion and replace positions
      //
      static dtPoint3 toDtPoint3( omPoint const & oP);
      static dtVector3 toDtVector3( omNormal const & nP);      
      void replaceMVertex( omVertexH const & vH, ::MVertex * mv );
      void replaceMElement( omFaceH const & fH, ::MElement * me );
      void replacePosition( omVertexH const & vH, dtPoint3 const & pp );      
      //
      // attach fields
      //
      void enqueueField( dtOMField * omField );
      void dequeueField( dtOMField * omField );      
      //
      // output
      //
      void writeMesh(std::string const filename) const;
//      bool vertexIsBoundary(MVertex * mv) const;
//      bool isGeometricalEdge( omEdgeH const & eH) const;
//      std::pair< omVertexH const, omVertexH const >
//      foldVertices( omEdgeH const & eH) const;
//      std::pair< omFaceH, omFaceH > foldFaces( omEdgeH const & eH) const;
      //
      // mesh navigation
      //
      omHalfedgeH sameHalfedgeInNextFace( omHalfedgeH const & heH) const;
      int nVertices( omFaceH const & fH) const;
    private:
      std::map< ::MVertex const *, omVertexH > const & omGmsh( void ) const;      
      omVertexH addVertex( ::MVertex const * const &mv );
      omFaceH addFace( std::vector< ::MVertex * > const & vertices );          
      omFaceH addFace( 
        std::vector< ::MVertex * > const & vertices, 
        ::MElement const * const me 
      );      
      omFaceH addFaceInv( ::MElement const * const me );       
      bool intersection( 
        omFaceH const & fH, dtPoint3 const & start, dtPoint3 const & target 
      ) const;
    private:      
      std::map< ::MVertex const *, omVertexH > _om_gmsh;
      std::map< ::MElement const *, omFaceH > _om_gmshElement;
      std::vector< dtOMField * > _attachedField;
    
  };
}
#endif	/* dtOMMesh_H */
