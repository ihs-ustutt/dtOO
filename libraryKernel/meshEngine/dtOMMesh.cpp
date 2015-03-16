#include "dtOMMesh.h"
#include "progHelper.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <gmsh/MVertex.h>

//#define __forAllVert(in) \
//    for ( \
//      omVertexI v_it = this->vertices_begin(); \
//      v_it != this->vertices_end(); \
//      ++v_it \
//    ) { \
//      in \
//    }

namespace dtOO {
  dtOMMesh::dtOMMesh() : omMesh() {
  }

  dtOMMesh::~dtOMMesh() {
  }

  void dtOMMesh::addVertex( MVertex const * const &mv ) {
    omVertexH vH 
    = 
    omMesh::add_vertex( 
      omMesh::Point(mv->x(), mv->y(), mv->z()) 
    );
    omVertexD & vD = omMesh::data(vH);
    vD.MVertex() = mv;
		
		_om_gmsh[mv] = vH;
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
 */  	
  void dtOMMesh::addFace(
		::MVertex const * const &v0, 
		::MVertex const * const &v1, 
		::MVertex const * const &v2, 
		::MVertex const * const &v3
  ) {
		if (_om_gmsh.find(v0)==_om_gmsh.end()) addVertex(v0);
		if (_om_gmsh.find(v1)==_om_gmsh.end()) addVertex(v1);
		if (_om_gmsh.find(v2)==_om_gmsh.end()) addVertex(v2);
		if (_om_gmsh.find(v3)==_om_gmsh.end()) addVertex(v3);
		
    omMesh::add_face(_om_gmsh[v0], _om_gmsh[v1], _om_gmsh[v2], _om_gmsh[v3]);
  }

/*
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
  void dtOMMesh::addFace(
		::MVertex const * const &v0, 
		::MVertex const * const &v1, 
		::MVertex const * const &v2
  ) {
		if (_om_gmsh.find(v0)==_om_gmsh.end()) addVertex(v0);
		if (_om_gmsh.find(v1)==_om_gmsh.end()) addVertex(v1);
		if (_om_gmsh.find(v2)==_om_gmsh.end()) addVertex(v2);
		
    omMesh::add_face(_om_gmsh[v0], _om_gmsh[v1], _om_gmsh[v2]);
  }  

	
	void dtOMMesh::addFace( std::vector< MVertex * > const & vertices ) {
		std::vector< omVertexH > handle;
		handle.reserve(vertices.size());
    dt__FORALLCONSTITER(std::vector< MVertex * >, vertices, it) {
			if (_om_gmsh.find(*it)==_om_gmsh.end()) addVertex(*it);
			handle.push_back( _om_gmsh[*it] );
		}
		omMesh::add_face(&(handle[0]), handle.size());
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
			std::vector< MVertex * > vertices;
			for (
				omConstFaceVertexI fv_it = toAdd.cfv_begin(*f_it);
				fv_it != toAdd.cfv_end(*f_it);
				++fv_it
			) {
  		  omVertexD omVD = toAdd.data(*fv_it);
				vertices.push_back( const_cast< MVertex * >(omVD.MVertex()) );
			}
			addFace(vertices);
		}
	}
}
