#include "dtOMMesh.h"

#include "dtOMField.h"
#include "dtGmshModel.h"
#include "dtGmshEdge.h"
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
		
		dt__throwIf(!vH.is_valid(), addVertex());
		
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
		dt__throwIf( (vertices.size()!=3)&&(vertices.size()!=4), addFace() );
		
		std::vector< omVertexH > handle;
		handle.reserve(vertices.size());
    dt__forAllConstIter(std::vector< ::MVertex * >, vertices, it) {
			if (_om_gmsh.find(*it)==_om_gmsh.end()) addVertex(*it);
			handle.push_back( _om_gmsh[*it] );
		}
		omFaceH fH = omMesh::add_face(&(handle[0]), handle.size());
		
		dt__throwIfWithMessage(
			!fH.is_valid(), 
			addFace(),
		  << dt__eval( handle.size() ) << std::endl
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

		dt__throwIf(const_cast< ::MElement * >(me)->getNumFaces()!=1, addFace());
		
    _om_gmshElement[ me ] = fH;
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
		
    if ( v.size()==3 ) {
      return dtLinearAlgebra::intersects(
        dtTriangle3(v[0], v[1], v[2]), dtLine3(start, target)
      );      
    }
    else {
      bool intersectsOne 
      =
      dtLinearAlgebra::intersects(
        dtTriangle3(v[0], v[1], v[2]), dtLine3(start, target)
      );      
      bool intersectsTwo
      =
      dtLinearAlgebra::intersects(
        dtTriangle3(v[2], v[3], v[0]), dtLine3(start, target)
      );            
      
      return (intersectsOne || intersectsTwo);
    }
		

	}

	void dtOMMesh::replaceMVertex( omVertexH const & vH, ::MVertex * mv ) {
		//
		// replace vertex in mapping
		//
		_om_gmsh.erase( at(vH) );
		_om_gmsh[mv] = vH;
				
		//
		// replace MVertex on OpenMesh vertex
		//
		data(vH).MVertex(mv);
	  set_point(vH, omPoint(mv->x(), mv->y(), mv->z()));
		
		//
		// update normals
		//
		dt__forFromToIter(
			omVertexFaceI, vf_begin(vH), vf_end(vH), it
		) update_normal(*it);
	}
  
	void dtOMMesh::replaceMElement( omFaceH const & fH, ::MElement * me ) {
		//
		// replace vertex in mapping
		//
		_om_gmshElement.erase( at(fH) );
		_om_gmshElement[me] = fH;
				
		//
		// replace MVertex on OpenMesh vertex
		//
		data(fH).MElement(me);
		
		//
		// update normals
		//
    update_normal(fH);
	}  

	void dtOMMesh::replacePosition( omVertexH const & vH, dtPoint3 const & pp ) {
		//
		// update position
		//
		::MVertex * mv = data(vH).MVertex();
		mv->setXYZ(pp.x(), pp.y(), pp.z());
	  set_point(vH, omPoint(pp.x(), pp.y(), pp.z()));

		//
		// update normals
		//		
		dt__forFromToIter(
			omVertexFaceI, vf_begin(vH), vf_end(vH), it
		) update_normal(*it);
	}	
	
//	bool dtOMMesh::vertexIsBoundary(MVertex * mv) const {
//		return const_cast<dtOMMesh *>(this)->is_boundary(_om_gmsh.at(mv));
//	}
//
//	bool dtOMMesh::isGeometricalEdge( omEdgeH const & eH) const {
//		omVertexH fromH
//		= 
//		from_vertex_handle(const_cast<dtOMMesh*>(this)->halfedge_handle(eH, 0));
//		omVertexH toH
//		= 
//		to_vertex_handle(const_cast<dtOMMesh*>(this)->halfedge_handle(eH, 0));
//		
//		if (
//			   dtGmshEdge::ConstDownCast(at(fromH)->onWhat()) 
//			&& dtGmshEdge::ConstDownCast(at(toH)->onWhat())
//		) return true;
//		return false;
//		
//	}
	
//	std::pair< omVertexH const, omVertexH const >
//	dtOMMesh::foldVertices( omEdgeH const & eH) const {
//		omHalfedgeH he0H = const_cast<dtOMMesh*>(this)->halfedge_handle(eH, 0);
//		omHalfedgeH he1H = const_cast<dtOMMesh*>(this)->halfedge_handle(eH, 1);
//		
//		he0H = const_cast<dtOMMesh*>(this)->next_halfedge_handle(he0H);
//		he1H = const_cast<dtOMMesh*>(this)->next_halfedge_handle(he1H);
//		
//		return std::pair< omVertexH const, omVertexH const >(
//			to_vertex_handle(he0H), to_vertex_handle(he1H)
//		);
//	}
//
//	std::pair< omFaceH, omFaceH > dtOMMesh::foldFaces( omEdgeH const & eH) const {
//		omHalfedgeH he0H = const_cast<dtOMMesh*>(this)->halfedge_handle(eH, 0);
//		omHalfedgeH he1H = const_cast<dtOMMesh*>(this)->halfedge_handle(eH, 1);
//		
//		return std::pair< omFaceH, omFaceH >(
//			const_cast<dtOMMesh*>(this)->face_handle(he0H), 
//			const_cast<dtOMMesh*>(this)->face_handle(he1H)
//		);
//	}
	
	bool dtOMMesh::intersection( 
	  std::vector< omFaceH > const & fH, 
		dtPoint3 const & start, dtPoint3 const & target 
	) const {	
		dt__forAllIndex(fH, ii) {
			if (intersection(fH[ii], start, target)) return true;
		}
		return false;
	}
	
	dtPoint3 dtOMMesh::toDtPoint3( omPoint const & oP) {
		return dtPoint3(oP[0], oP[1], oP[2]);
	}
	
  dtVector3 dtOMMesh::toDtVector3( omNormal const & nP) {
		return dtVector3(nP[0], nP[1], nP[2]);
	}
	
	void dtOMMesh::update( void ) {
		omMesh::update_normals();
    
    dt__forAllIter(std::vector< dtOMField * >, _attachedField, it) {
      (*it)->update();
    }
	}

  omVertexH const & dtOMMesh::at( ::MVertex const * const mv ) const {
		return _om_gmsh.at(mv);
	}
	
  ::MVertex * dtOMMesh::operator[](omVertexH const & vH) {
		return data(vH).MVertex();
	}
	
	::MVertex const * const dtOMMesh::at(omVertexH const & vH) const {
		return data(vH).MVertex();
	}

	::MElement const * const dtOMMesh::at(omFaceH const & fH) const {
		return data(fH).MElement();
	}

  omFaceH const & dtOMMesh::at( ::MElement const * const me ) const {
		return _om_gmshElement.at(me);
	}  
	
	std::vector< omEdgeH > dtOMMesh::oneRingEdgeH( omVertexH const & vH ) const {
	  std::vector< omEdgeH > eHV;	
		dt__forFromToIter(omConstVertexOHalfedgeI, cvoh_begin(vH), cvoh_end(vH), heIt) {
			eHV.push_back( edge_handle( next_halfedge_handle(*heIt) ) );
		}
		return eHV;
	}
  
  void dtOMMesh::enqueueField( dtOMField * omField ) {
    _attachedField.push_back( omField );
  }

  void dtOMMesh::dequeueField( dtOMField * omField ) {
    _attachedField.erase(
      std::find(_attachedField.begin(), _attachedField.end(), omField)
    );
  }  
  
  omHalfedgeH dtOMMesh::sameHalfedgeInNextFace( omHalfedgeH const & heH) const {
    dt__throwIf( 
      nVertices( face_handle(heH) )!=4, 
      sameHalfedgeInNextFace() 
    );
    
    omHalfedgeH ret
    =
    opposite_halfedge_handle( 
      next_halfedge_handle( next_halfedge_handle(heH) ) 
    );

    dt__throwIf( 
      nVertices( face_handle(ret) )!=4, 
      sameHalfedgeInNextFace() 
    );    
    
    return ret;
  }
  
  int dtOMMesh::nVertices( omFaceH const & fH) const {
    int counter = 0;
    dt__forFromToIter(omConstFaceVertexI, cfv_begin(fH), cfv_end(fH), fIt) {
      counter++;
    }
    
    return counter;
  }
}
