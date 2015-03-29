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

  dtOMMesh::dtOMMesh( dtOMMesh const & orig ) : omMesh() {
		dt__forFromToIter(omFaceI, orig.faces_begin(), orig.faces_end(), fIt) {
			addFace( orig.data(*fIt) );
		}
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
		
		return fH;
	}  

  omFaceH dtOMMesh::addFace( omFaceD const & fD ) {
    if ( !fD.inverted() ) {
			return addFace(fD.MElement());
		} 
		else {
			return addFaceInv(fD.MElement());
		}
	}	
	
  omFaceH dtOMMesh::addFace( ::MElement const * const me ) {
		std::vector< ::MVertex * > vertices;
		const_cast< ::MElement * >(me)->getVertices(vertices);
		
    return addFace(vertices, me);
	}

  omFaceH dtOMMesh::addFaceInv( ::MElement const * const me ) {
		std::vector< ::MVertex * > vertices;
		const_cast< ::MElement * >(me)->getVertices(vertices);
		
		std::reverse(vertices.begin(), vertices.end());
    omFaceH fH = addFace(vertices, me);
		data(fH).invert();
		
		return fH;
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
	
	void dtOMMesh::addInv(const dtOMMesh &toAdd) {
		dt__forFromToIter(omFaceI, toAdd.faces_begin(), toAdd.faces_end(), f_it) {
			omFaceD const & fD = toAdd.data(*f_it);
			::MElement const * const me = fD.MElement();
			addFaceInv(me);
		}
	}	
	
	std::map< ::MVertex const *, omVertexH > const & dtOMMesh::omGmsh( void ) const {
		return _om_gmsh;
	}

  bool dtOMMesh::contains( omFaceH const & fH, omEdgeH const & eH ) const {
		dt__forFromToIter(omConstFaceEdgeI, cfe_begin(fH), cfe_end(fH), eIt) {
			if (*eIt == eH) return true;
		}
		return false;
	}
	
  bool dtOMMesh::contains( omFaceH const & fH, omVertexH const & vH ) const {
		dt__forFromToIter(omConstFaceVertexI, cfv_begin(fH), cfv_end(fH), it) {
			if (*it == vH) return true;
		}
		return false;
	}	
	
	bool dtOMMesh::intersection( 
	  omFaceH const & fH, dtPoint3 const & start, dtPoint3 const & target 
	) const {
		std::vector< dtPoint3 > v;
		dt__forFromToIter(omConstFaceVertexI, cfv_begin(fH), cfv_end(fH), it) {
			v.push_back( toDtPoint3(point(*it)) );
		}
		dt__THROW_IF(v.size()!=3, contains());
		
		return dtLinearAlgebra::intersects(
		  dtTriangle3(v[0], v[1], v[2]), dtLine3(start, target)
		);
		
	}
	
	dtVector3 & dtOMMesh::vertexNormal(MVertex * mv) {
		return data(_om_gmsh[mv]).normal();
	}

	void dtOMMesh::replaceMVertex( omVertexH const & vH, ::MVertex * mv ) {
		data(vH).MVertex(mv);
	  point(vH) = omPoint(mv->x(), mv->y(), mv->z());
	}
	
	bool dtOMMesh::vertexIsBoundary(MVertex * mv) const {
		return const_cast<dtOMMesh *>(this)->is_boundary(_om_gmsh.at(mv));
	}
	
	void dtOMMesh::laplacianSmoothVertexNormal( void ) {
		std::vector< dtVector3 > av(_om_gmsh.size());
		int counter = 0;
		dt__forFromToIter(omVertexI, vertices_begin(), vertices_end(), vIt) {
			std::vector< dtVector3 > tmp;
			dt__forFromToIter(omVertexVertexI, vv_begin(*vIt), vv_end(*vIt), vvIt) {			
        tmp.push_back( data(*vvIt).normal() );
			}
			av[counter] = dtLinearAlgebra::meanAverage(tmp);
			counter++;
		}
		
		counter = 0;
		dt__forFromToIter(omVertexI, vertices_begin(), vertices_end(), vIt) {
			data(*vIt).normal() 
			= 
			dtLinearAlgebra::normalize(data(*vIt).normal() + av[counter]);
			counter++;
		}		
	}
	
	bool dtOMMesh::intersection( 
	  std::vector< omFaceH > const & fH, 
		dtPoint3 const & start, dtPoint3 const & target 
	) const {	
		dt__forAllIndex(fH, ii) {
			if (intersection(fH[ii], start, target)) return false;
		}
		return true;
	}
	
	omVertexH const dtOMMesh::requestVertexH( ::MVertex const * mv ) const {
		return _om_gmsh.at(mv);
	}
	
	::MVertex * dtOMMesh::requestMVertex( omVertexH const vH ) const {
		return data(vH).MVertex();
	}	
	
	dtPoint3 dtOMMesh::toDtPoint3( omPoint const & oP) {
		return dtPoint3(oP[0], oP[1], oP[2]);
	}
	
  dtVector3 dtOMMesh::toDtVector3( omNormal const & nP) {
		return dtVector3(nP[0], nP[1], nP[2]);
	}
	
	void dtOMMesh::update( void ) {
		omMesh::update_normals();
	}

  omVertexH const dtOMMesh::operator[]( ::MVertex const * const mv ) {
		return requestVertexH(mv);
	}
	
  ::MVertex * dtOMMesh::operator[](omVertexH const & vH) {
		return requestMVertex(vH);
	}
}
