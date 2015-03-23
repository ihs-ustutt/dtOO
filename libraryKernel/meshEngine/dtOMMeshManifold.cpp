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
				eD.dihedralAngle( calc_dihedral_angle(*it) );
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
		if (_dihedralAngleV.size() >= 3) return true;
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
}
