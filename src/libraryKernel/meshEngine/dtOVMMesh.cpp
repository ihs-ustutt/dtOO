/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "dtOVMMesh.h"
#include "dtOMMesh.h"
#include "dtGmshRegion.h"
#include "dtGmshModel.h"

#include <logMe/dtMacros.h>

#include <logMe/logMe.h>
#include <gmsh/MVertex.h>
#include <gmsh/MElement.h>

#include <OpenVolumeMesh/FileManager/FileManager.hh>

namespace dtOO {
  dtOVMMesh::dtOVMMesh(int const & nV, dtInt const & nE) : ovmMesh() {
    ::OpenVolumeMesh::VertexPropertyT< dtInt > mvProp 
    = 
    request_vertex_property< dtInt >("MVertex");
    set_persistent(mvProp); 
    ::OpenVolumeMesh::CellPropertyT< dtInt > meProp 
    = 
    request_cell_property< dtInt >("MElement");
    set_persistent(meProp);     

    if (nV) _mv.reserve(nV);
    if (nE) _me.reserve(nE);
  }

  dtOVMMesh::~dtOVMMesh() {
  }

  ovmVertexH dtOVMMesh::addVertex( ::MVertex * mv ) {
    ovmVertexH vH 
    = 
    ovmMesh::add_vertex( 
      ovmPoint(mv->x(), mv->y(), mv->z()) 
    );
		
		dt__throwIf(!vH.is_valid(), addVertex());
		
    _mv.push_back( mv );
    request_vertex_property< dtInt >("MVertex")[ vH ] = _mv.size()-1;
		
		_ovm_gmsh[mv] = vH;
		
		return vH;
  }
  
  ovmCellH dtOVMMesh::addCell( ::MElement * me ) {
    //
    // check if already in mesh
    //
    if ( _ovm_gmshElement.find( me ) != _ovm_gmshElement.end() ) {
      return _ovm_gmshElement[ me ];
    }
    
    ::MElement * nCMElement = const_cast< ::MElement * >(me);
    std::vector< ovmHalffaceH > handle( nCMElement->getNumFaces() );

    dt__forFromToIndex(0, nCMElement->getNumFaces(), ii) {
      std::vector< ::MVertex * > vv;
      nCMElement->getFace(ii).getOrderedVertices(vv);
      handle[ii] = addFace( vv );
    }
    ovmCellH cH = ovmMesh::add_cell( handle );

    _me.push_back( me );
    request_cell_property< dtInt >("MElement")[ cH ] = _me.size()-1;
    _ovm_gmshElement[ me ] = cH;
    return cH;    
  }  
  
	ovmHalffaceH dtOVMMesh::addFace( 
    ::MVertex * mv0, ::MVertex * mv1, ::MVertex * mv2 
  ) {
    std::vector< ::MVertex * > vv(3);
    vv[0] = mv0;
    vv[1] = mv1;
    vv[2] = mv2;
    return addFace( vv );
  }  
  
	ovmHalffaceH dtOVMMesh::addFace( std::vector< ::MVertex * > const & mv ) {
		std::vector< ovmVertexH > handle(mv.size(), ovmVertexH());
    dtInt already = 0;
    
    dt__forFromToIndex(0, mv.size(), ii) {
      
      if ( _ovm_gmsh.find( mv[ii] ) != _ovm_gmsh.end() ) {
        handle[ii] = _ovm_gmsh[ mv[ii] ];
        already++;
      }
      else handle[ii] = addVertex(mv[ii]);
    }
    
    ovmHalffaceH hfH;
    if ( already <= 1 ) {
		  hfH = halfface_handle( ovmMesh::add_face(handle), 0);
    }
    else {
      hfH = halfface(handle);
      
      if ( !hfH.is_valid() ) {
        hfH = halfface_handle( ovmMesh::add_face(handle), 0);
      }
      else {
        if ( !is_boundary( hfH ) ) hfH = opposite_halfface_handle( hfH );
      }
    }
		dt__throwIfWithMessage(
			!hfH.is_valid(), 
			addFace(),
		  << dt__eval( handle.size() )
		);

		return hfH;
	}	
  
  ovmVertexH const & dtOVMMesh::at( ::MVertex const * const mv ) const {
		return _ovm_gmsh.at(mv);
	}
	
  ::MVertex * dtOVMMesh::operator[](ovmVertexH const & vH) {
		return _mv[ request_vertex_property< dtInt >("MVertex")[vH] ];
	}
	
	::MVertex const * const dtOVMMesh::at(ovmVertexH const & vH) const {
		return 
      _mv.at( 
        const_cast< dtOVMMesh * >(
          this
        )->request_vertex_property< dtInt >(
          "MVertex"
        )[vH] 
    );
	}

  ::MElement * dtOVMMesh::operator[](ovmCellH const & cH) {
    return _me[ request_cell_property< dtInt >("MElement")[cH] ];
  }
  
	::MElement const * const dtOVMMesh::at(ovmCellH const & cH) const {
    return 
      _me.at( 
        const_cast< dtOVMMesh * >(
          this
        )->request_cell_property< dtInt >(
          "MElement"
        )[cH] 
      );
	}

	void dtOVMMesh::replacePosition( 
    ovmVertexH const & vH, dtPoint3 const & pp 
  ) {
		//
		// update position
		//
		::MVertex * mv = this->operator[](vH);
		mv->setXYZ(pp.x(), pp.y(), pp.z());
	  ovmMesh::set_vertex(vH, ovmPoint(pp.x(), pp.y(), pp.z()));
	}	
  
  void dtOVMMesh::makePartition( dtInt const & num ) const {
    dt__forAllRefAuto(_me, anEl) {
      const_cast< ::MElement *>(anEl)->setPartition(num);
    }
  }
  std::vector< dtPoint3 > dtOVMMesh::adjacentVertices( 
    ovmVertexH const & vH
  ) const {
    std::vector< dtPoint3 > pp;
    for (
      ovmVertexOHalfedgeI heIt = voh_iter(vH); heIt.valid(); heIt++
    ) {
      dt__throwIf(halfedge(*heIt).to_vertex()==vH, createPyramids());
      pp.push_back(
        dtGmshModel::extractPosition(
          at( halfedge(*heIt).to_vertex() )
        )
      );
    }
    
    return pp;
  }
}
