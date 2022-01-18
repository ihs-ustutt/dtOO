#include "dtOMMeshManifold.h"
#include "progHelper.h"

namespace dtOO {
  dtOMMeshManifold::dtOMMeshManifold(
    dtOMMesh const & om, omVertexH const & vH
  ) : _nTri(0), _nQuad(0) {
		dt__forFromToIter(
      omConstVertexFaceI, om.cvf_begin(vH), om.cvf_end(vH), it
    ) {
      addFace( om.data(*it) );
      if (om.data(*it).nVertices() == 3) _nTri++;
      else if (om.data(*it).nVertices() == 4) _nQuad++;
      else {
        dt__throw(dtOMMeshManifold(), << dt__eval(om.data(*it).nVertices()) );
      }
    }
      
		omVertexD const & my_vD  = om.data(vH);
    _centerVertex = at(my_vD.MVertex());    

		update();
		
//		dtReal minA = minDihedralAngle();
//		dtReal maxA = maxDihedralAngle();
//		dt__info(
//			dtOMMeshManifold(),
//			<< dt__eval(closed()) << std::endl
//			<< dt__eval(divideable()) << std::endl
//			<< "_dihedralAngleV = " << _dihedralAngleV << std::endl
//			<< "_isBoundary = " << _isBoundary << std::endl
//			<< dt__eval(minA) << " rad (" << minA*180./M_PI << " degree )" << std::endl
//			<< dt__eval(maxA) << " rad (" << maxA*180./M_PI << " degree )" << std::endl
//		);
	}
	
	dtOMMeshManifold::dtOMMeshManifold( 
    dtOMMeshManifold const & orig
  ) : dtOMMesh(orig) {
		_centerVertex = at(orig.centerMVertex());
		
		update();
	}
	
	void dtOMMeshManifold::update( void ) {
    //
		// update underlying mesh
		//
		dtOMMesh::update();
		
		_dihedralAngleV.clear();
		_isBoundary.clear();
		
		dt__forFromToIter(
      omVertexEdgeI, ve_begin(_centerVertex), ve_end(_centerVertex), it
    ) {
			_isBoundary.push_back(is_boundary(*it));
			omEdgeD & eD = data(*it);
			eD.dihedralAngle( fabs(calc_dihedral_angle(*it)) );
			_dihedralAngleV.push_back( eD.dihedralAngle() );
		}		
	}

	dtOMMeshManifold::~dtOMMeshManifold() {
	}
	
	dtReal dtOMMeshManifold::minDihedralAngle( void ) const {
		dtReal ret = 0.;
		if (_dihedralAngleV.size() != 0) {
			ret = *(std::min_element(_dihedralAngleV.begin(), _dihedralAngleV.end()));
		}		
		
		return ret;
	}

	dtReal dtOMMeshManifold::maxDihedralAngle( void ) const {
		dtReal ret = 0.;
		if (_dihedralAngleV.size() != 0) {
			ret = *(std::max_element(_dihedralAngleV.begin(), _dihedralAngleV.end()));
		}		
		
		return ret;
	}	
	
  /**
   * @todo Extent divide() subroutine to mixed quad-tri and quad only meshes.
   */
	bool dtOMMeshManifold::divideable( void ) const {
    if (_nQuad>0) return false;
    
		bool isClosed = closed();
		if ( 
		     ((_dihedralAngleV.size()>=3) &&  isClosed)
			|| ((_dihedralAngleV.size()>=3) && !isClosed) 
		) {
			return true;
		}
		else return false;
	}

	bool dtOMMeshManifold::closed( void ) const {
		if (
			std::find(
			  _isBoundary.begin(), _isBoundary.end(), true
			) 
			== 
			_isBoundary.end()
		) {
			return true;
		}
		return false;
	}	

  dtOMMeshManifold dtOMMeshManifold::divide(void) {
		dt__throwIf(!divideable(), divide());
		
		bool isClosed = closed();
		
		if (isClosed) {
			int nEdges = _dihedralAngleV.size();
			//
			// find max dihedral angle and save iterators
			//
      std::vector< dtReal >::iterator maxAngleIt
			=
			std::max_element(_dihedralAngleV.begin(), _dihedralAngleV.end());
		  omVertexEdgeI maxEdgeIt = ve_begin(_centerVertex);
			for (int ii=0;ii<(maxAngleIt-_dihedralAngleV.begin());ii++) maxEdgeIt++;
			*maxAngleIt = 0.;

			//
			// find next max dihedral angle and save iterators
			//			
			std::vector< dtReal >::iterator max2ndAngleIt;
			omVertexEdgeI max2ndEdgeIt;
			dt__forAllIndex(_dihedralAngleV, tmp) {
				max2ndAngleIt
				=
				std::max_element(_dihedralAngleV.begin(), _dihedralAngleV.end());
				max2ndEdgeIt = ve_begin(_centerVertex);
				for (
          dtInt ii=0;ii<(max2ndAngleIt-_dihedralAngleV.begin());ii++
        ) max2ndEdgeIt++;
				*max2ndAngleIt = 0.;
//				if ( 
//					   (abs(max2ndAngleIt-maxAngleIt) > 0) 
//					&& ((nEdges-abs(max2ndAngleIt-maxAngleIt))>0) 
//				) break;
			}
			
			//
			// output
			//
//			dt__info(
//				divide(),
//				<< "Dividing manifold at " << maxAngleIt - _dihedralAngleV.begin() 
//				<< " and " << max2ndAngleIt - _dihedralAngleV.begin() << std::endl
//				<< "Distance between " << max2ndAngleIt-maxAngleIt
//			);
			
			//
			// create pair and return
			//
			if ( (max2ndAngleIt-maxAngleIt)>0 ) {
				return subractManifold(*maxEdgeIt, *max2ndEdgeIt);
			}
			else {
				return subractManifold(*max2ndEdgeIt, *maxEdgeIt);
			}
		}
		else {
			//
			// find max dihedral angle and save iterators
			//			
			std::vector< dtReal >::iterator maxAngleIt;
			omVertexEdgeI maxEdgeIt;
			int maxAngleBegPos;
			int maxAngleEndPos;			
			dt__forAllIndex(_dihedralAngleV, tmp) {
				maxAngleIt
				=
				std::max_element(_dihedralAngleV.begin(), _dihedralAngleV.end());
				maxEdgeIt = ve_begin(_centerVertex);
				maxAngleBegPos = maxAngleIt-_dihedralAngleV.begin();
				maxAngleEndPos = maxAngleIt-_dihedralAngleV.end();
				for (int ii=0;ii<maxAngleBegPos;ii++) maxEdgeIt++;
				*maxAngleIt = 0.;
//				if ( (maxAngleBegPos>0) && (maxAngleEndPos<-0) ) break;
			}
			
			//
			// output
			//
//			dt__info(
//				divide(),
//				<< "Dividing manifold at " << maxAngleBegPos << std::endl
//				<< "Distance " << dt__eval(maxAngleBegPos) 
//				<< " and " << dt__eval(maxAngleEndPos) << std::endl
//				<< "_dihedralAngleV = " << _dihedralAngleV << std::endl
//				<< "_isBoundary = " << _isBoundary << std::endl
//			);
			
			//
			// create pair and return
			//
			return subractManifold( *(ve_begin(_centerVertex)), *maxEdgeIt);
		}
	}

  /**
   * 
   * @todo Should support also quadrangle meshes.
   */
  std::vector< dtOMMeshManifold > dtOMMeshManifold::divide( 
    dtReal const & angle 
  ) {
//    if ( )
		std::vector< dtOMMeshManifold > manifolds;
		
		bool restart = true;
		manifolds.push_back(*this);
		
		while (restart) {
			restart = false;
			dt__forAllIter(std::vector< dtOMMeshManifold >, manifolds, it) {
				if (it->divideable() && (it->maxDihedralAngle()>angle) ) {
					manifolds.push_back( it->divide() );
					restart = true;
					break;
				}
			}
		}
		
		return manifolds;
	}	
	
  dtOMMeshManifold dtOMMeshManifold::subractManifold(
	  omEdgeH const & from, omEdgeH const & to
	) {
		dtOMMesh retMesh;
		//
		// find start halfedge
		//
		omHalfedgeH heH = halfedge_handle(from, 0);
		if ( is_boundary(heH) ) {
			heH = opposite_halfedge_handle(heH);
		}
		omFaceH fH = face_handle(heH);
		dt__forFromToIter(omFaceHalfedgeI, fh_begin(fH), fh_end(fH), fhIt) {
			omHalfedgeH thisHe = *fhIt;
			omHalfedgeH thisOppositeHe = opposite_halfedge_handle(thisHe);
//			if ( is_boundary(thisOppositeHe) && (thisOppositeHe!=heH) ) {
      if ( 
				   is_boundary(thisOppositeHe) 
				&& (from_vertex_handle(thisOppositeHe)!=_centerVertex) 
				&& (to_vertex_handle(thisOppositeHe)!=_centerVertex) 
			) {			
				heH = thisOppositeHe;
				break;
			}
		}
		
		while (true) {
			omFaceH fH = face_handle(opposite_halfedge_handle(heH));

		  retMesh.addFace( data(fH) );
			
			bool lastFace = dtOMMesh::contains(fH, to);
			heH = next_halfedge_handle(heH);
			data(fH).mark();
			
			if (lastFace) break;
		}
		
		dt__forFromToIter(omFaceI, faces_begin(), faces_end(), fIt) {
			if (data(*fIt).marked()) delete_face(*fIt, true);
		}
		
		//
		// get center vertex handle
		//
		omVertexD & vD = retMesh.data(_centerVertex);

		//
		// remove deleted faces and update this manifold
		//
		garbage_collection();
		update();
		
		//
		// create new manifold
		//
		return dtOMMeshManifold( retMesh, retMesh.at(vD.MVertex()) );
	}	
	
	dtVector3 dtOMMeshManifold::normal( void ) const {
		std::vector< dtVector3 > nn;
		dt__forFromToIter(omConstFaceI, faces_begin(), faces_end(), fIt) {		
			omNormal const & omN = omMesh::normal(*fIt);
			nn.push_back( 
        dtLinearAlgebra::normalize(dtVector3(omN[0], omN[1], omN[2])) 
      );
		}
		return dtLinearAlgebra::normalize( dtLinearAlgebra::sum(nn) );
	}
	
	MVertex * dtOMMeshManifold::centerMVertex( void ) const {
		return data(_centerVertex).MVertex();
	}	
}
