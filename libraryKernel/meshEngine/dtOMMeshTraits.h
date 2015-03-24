#ifndef DTOMMESHTRAITS_H
#define	DTOMMESHTRAITS_H

#include <gmsh/MFace.h>
#include <dtLinearAlgebra.h>
#include <OpenMesh/Core/Mesh/Traits.hh>

class MVertex;
class MElement;

namespace dtOO {
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
    
    EdgeTraits {
      private:
        float _dihedralAngle;
      public:
        EdgeT() : _dihedralAngle(0.) {
        }
        void dihedralAngle( float const & dihedralAngle ) {
          _dihedralAngle = dihedralAngle;
        }        
        float dihedralAngle( void ) const {
          return _dihedralAngle;
        }
    };
    
    FaceTraits {
      private:
        ::MElement * _me;
        ::MFace _mf;
        dtVector3 _nn;
        bool _inverted;
        bool _mark;
      public:
        FaceT() : _me(NULL), _inverted(false), _mark(false) {
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
        void invert( void ) {
          _inverted = true;
        }            
        bool inverted( void ) const {
          return _inverted;
        }                    
        void mark( void ) {
          _mark = true;
        }            
        bool marked( void ) const {
          return _mark;
        }                            
    };
    //
    // standard attributes
    //
    VertexAttributes( OpenMesh::Attributes::Status );    
    EdgeAttributes( OpenMesh::Attributes::Status );    
    FaceAttributes( OpenMesh::Attributes::Normal | OpenMesh::Attributes::Status );
  };
}

#endif	/* DTOMMESHTRAITS_H */

