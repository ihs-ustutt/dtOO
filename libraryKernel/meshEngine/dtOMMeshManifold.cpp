#include "dtOMMeshManifold.h"
#include "progHelper.h"

namespace dtOO {
  dtOMMeshManifold::dtOMMeshManifold(dtOMMesh const & om, omVertexH const & vH) {
		dt__forFromToIter(omConstVertexFaceI, om.cvf_begin(vH), om.cvf_end(vH), it) {
			addFace( om.data(*it) );
		}
		omVertexD const & my_vD  = om.data(vH);
		_centerVertex = omGmsh().at(my_vD.MVertex());

		update();
		
		float minA = minDihedralAngle();
		float maxA = maxDihedralAngle();
		
		DTINFOWF(
			dtOMMeshManifold(),
			<< DTLOGEVAL(closed()) << LOGDEL
			<< DTLOGEVAL(divideable()) << LOGDEL
			<< "_dihedralAngleV = " << _dihedralAngleV << LOGDEL
			<< "_isBoundary = " << _isBoundary << LOGDEL
			<< DTLOGEVAL(minA) << " rad (" << minA*180./M_PI << " degree )" << LOGDEL
			<< DTLOGEVAL(maxA) << " rad (" << maxA*180./M_PI << " degree )" << LOGDEL
		);
	}
	
	void dtOMMeshManifold::update( void ) {
		_dihedralAngleV.clear();
		_isBoundary.clear();
		
		dt__forFromToIter(omVertexEdgeI, ve_begin(_centerVertex), ve_end(_centerVertex), it) {
			_isBoundary.push_back(is_boundary(*it));
			if ( !_isBoundary.back() ) {
				omEdgeD & eD = data(*it);
				eD.dihedralAngle( fabs(calc_dihedral_angle(*it)) );
				_dihedralAngleV.push_back( eD.dihedralAngle() );
			}
		}		
	}

	dtOMMeshManifold::~dtOMMeshManifold() {
	}
	
	float dtOMMeshManifold::minDihedralAngle( void ) const {
		float ret = 0.;
		if (_dihedralAngleV.size() != 0) {
			ret = *(std::min_element(_dihedralAngleV.begin(), _dihedralAngleV.end()));
		}		
		
		return ret;
	}

	float dtOMMeshManifold::maxDihedralAngle( void ) const {
		float ret = 0.;
		if (_dihedralAngleV.size() != 0) {
			ret = *(std::max_element(_dihedralAngleV.begin(), _dihedralAngleV.end()));
		}		
		
		return ret;
	}	
	
	bool dtOMMeshManifold::divideable( void ) const {
		bool isClosed = closed();
		if ( 
		     ((_dihedralAngleV.size()>=4) &&  isClosed)
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
		dt__THROW_IF(!divideable(), divide());
		
		bool isClosed = closed();
		
		if (isClosed) {
			//
			// find max dihedral angle and save iterators
			//
      std::vector< float >::iterator maxAngleIt
			=
			std::max_element(_dihedralAngleV.begin(), _dihedralAngleV.end());
		  omVertexEdgeI maxEdgeIt = ve_begin(_centerVertex);
			for (int ii=0;ii<(maxAngleIt-_dihedralAngleV.begin());ii++) maxEdgeIt++;
			*maxAngleIt = 0.;

			//
			// find next max dihedral angle and save iterators
			//			
			std::vector< float >::iterator max2ndAngleIt;
			omVertexEdgeI max2ndEdgeIt;
			dt__forAllIndex(_dihedralAngleV, tmp) {
				max2ndAngleIt
				=
				std::max_element(_dihedralAngleV.begin(), _dihedralAngleV.end());
				max2ndEdgeIt = ve_begin(_centerVertex);
				for (int ii=0;ii<(max2ndAngleIt-_dihedralAngleV.begin());ii++) max2ndEdgeIt++;
				*max2ndAngleIt = 0.;
				if ( abs(max2ndAngleIt-maxAngleIt) > 2) break;
			}
			
			//
			// output
			//
			DTINFOWF(
				divide(),
				<< "Dividing manifold at " << maxAngleIt - _dihedralAngleV.begin() 
				<< " and " << max2ndAngleIt - _dihedralAngleV.begin() << LOGDEL
				<< "Distance between " << max2ndAngleIt-maxAngleIt
			);
			
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
			std::vector< float >::iterator maxAngleIt;
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
				if ( (maxAngleBegPos>2) && (maxAngleEndPos<-2) ) break;
			}
			
			//
			// output
			//
			DTINFOWF(
				divide(),
				<< "Dividing manifold at " << maxAngleBegPos << LOGDEL
				<< "Distance " << DTLOGEVAL(maxAngleBegPos) 
				<< " and " << DTLOGEVAL(maxAngleEndPos)
			);
			
			//
			// create pair and return
			//
			return subractManifold( *(ve_begin(_centerVertex)), *maxEdgeIt);
		}
	}

  dtOMMeshManifold dtOMMeshManifold::subractManifold(
	  omEdgeH const & from, omEdgeH const & to
	) {
		dtOMMesh retMesh;
		bool reachStart = false;
		dt__forFromToIter(
			omVertexIHalfedgeI, vih_begin(_centerVertex), vih_end(_centerVertex), 
			vih_it
		) {
			if (edge_handle(*vih_it) == from) {
				reachStart = true;
			}
			if (edge_handle(*vih_it) == to) {
				break;
			}				
			if (reachStart) {
				retMesh.addFace( data(face_handle(*vih_it)) );
				delete_face(face_handle(*vih_it), true);
			}
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
		return dtOMMeshManifold(retMesh, retMesh.omGmsh().at(vD.MVertex()));
	}	
}
