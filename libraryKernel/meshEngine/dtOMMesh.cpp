#include "dtOMMesh.h"
#include <progHelper.h>

#include <logMe/logMe.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <gmsh/MVertex.h>
#include <gmsh/MElement.h>
#include <gmsh/MFace.h>
#include <OpenMesh/Core/IO/MeshIO.hh>


namespace dtOO {
  dtOMMesh::dtOMMesh() : omMesh() {
  }

  dtOMMesh::~dtOMMesh() {
  }

  omVertexH dtOMMesh::addVertex( ::MVertex const * const &mv ) {
    omVertexH vH 
    = 
    omMesh::add_vertex( 
      omMesh::Point(mv->x(), mv->y(), mv->z()) 
    );
		
		dt__THROW_IF(!vH.is_valid(), addVertex());
		
    omVertexD & vD = omMesh::data(vH);
    vD.MVertex(mv);
		
		_om_gmsh[mv] = vH;
		
		return vH;
  }
  
/*
 *         v
 *         ^
 *         |
 *   3-----------2
 *   |     |     |
 *   |     |     |
 *   |     +---- | --> u
 *   |           |
 *   |           |
 *   0-----------1
 *
 *   v
 *   ^
 *   |
 *   2
 *   |`\
 *   |  `\
 *   |    `\
 *   |      `\
 *   |        `\
 *   0----------1 --> u
 *
 */		
	omFaceH dtOMMesh::addFace( std::vector< ::MVertex * > const & vertices ) {
		dt__THROW_IF( (vertices.size()!=3)&&(vertices.size()!=4), addFace() );
		
		std::vector< omVertexH > handle;
		handle.reserve(vertices.size());
    dt__forAllConstIter(std::vector< ::MVertex * >, vertices, it) {
			if (_om_gmsh.find(*it)==_om_gmsh.end()) addVertex(*it);
			handle.push_back( _om_gmsh[*it] );
		}
		omFaceH fH = omMesh::add_face(&(handle[0]), handle.size());
		
		dt__THROW_IFWM(
			!fH.is_valid(), 
			addFace(),
		  << DTLOGEVAL( handle.size() ) << LOGDEL
			<< handle
		);
		
		return fH;
	}	
  
  omFaceH dtOMMesh::addFace( 
	  std::vector< ::MVertex * > const & vertices, ::MElement const * const me 
	) {
		omFaceH fH = addFace(vertices);
		omFaceD & fD = omMesh::data( fH );
		fD.MElement(me);

		dt__THROW_IF(const_cast< ::MElement * >(me)->getNumFaces()!=1, addFace());
		
	  fD.MFace( const_cast< ::MElement * >(me)->getFace(0) );
		SVector3 sv = fD.MFace().normal();
		fD.setNormal( dtVector3(sv.x(), sv.y(), sv.z()) );
		
		return fH;
	}  

  omFaceH dtOMMesh::addFace( ::MElement const * const me ) {
		std::vector< ::MVertex * > vertices;
		const_cast< ::MElement * >(me)->getVertices(vertices);
		
    return addFace(vertices, me);
	}  	
	
  void dtOMMesh::writeMesh(std::string const filename) const {
    OpenMesh::IO::write_mesh(*this, filename);
  }
	
	void dtOMMesh::add(const dtOMMesh &toAdd) {
		dt__forFromToIter(omFaceI, toAdd.faces_begin(), toAdd.faces_end(), f_it) {
			omFaceD const & fD = toAdd.data(*f_it);
			::MElement const * const me = fD.MElement();
			addFace(me);
		}
	}
	
	std::map< ::MVertex const *, omVertexH > const & dtOMMesh::omGmsh( void ) const {
		return _om_gmsh;
	}
}
