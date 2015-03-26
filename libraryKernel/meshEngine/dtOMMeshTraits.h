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
        dtVector3 _nn;
      public:
        VertexT() : _mv(NULL), _nn(dtVector3(0, 0, 0)) {}
        void MVertex( ::MVertex const * const mv ) {
          _mv = const_cast< ::MVertex * >(mv);
        }
        ::MVertex * MVertex( void ) const {
          return _mv;
        }             
        dtVector3 & normal( void ) {
          return _nn;
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

