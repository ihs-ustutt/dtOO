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
    dt__FORALLCONSTITER(std::vector< ::MVertex * >, vertices, it) {
			if (_om_gmsh.find(*it)==_om_gmsh.end()) addVertex(*it);
			handle.push_back( _om_gmsh[*it] );
		}
		return omMesh::add_face(&(handle[0]), handle.size());
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
		for (
			omFaceI f_it = toAdd.faces_begin();
			f_it != toAdd.faces_end();
			++f_it
		) {
			std::vector< ::MVertex * > vertices;
			for (
				omConstFaceVertexI fv_it = toAdd.cfv_begin(*f_it);
				fv_it != toAdd.cfv_end(*f_it);
				++fv_it
			) {
  		  omVertexD const & vD = toAdd.data(*fv_it);
				vertices.push_back( const_cast< ::MVertex * >(vD.MVertex()) );
			}
			omFaceD const & fD = toAdd.data(*f_it);
			::MElement const * const me = fD.MElement();
			addFace(vertices, me);
		}
	}
	
	std::map< ::MVertex const *, omVertexH > const & dtOMMesh::omGmsh( void ) const {
		return _om_gmsh;
	}
}
