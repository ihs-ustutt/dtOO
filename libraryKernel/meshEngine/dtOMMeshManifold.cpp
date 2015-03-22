#include "dtOMMeshManifold.h"
#include "progHelper.h"

namespace dtOO {
  dtOMMeshManifold::dtOMMeshManifold(dtOMMesh const & om, omVertexH const & vH) {
		dt__forFromToIter(omConstVertexFaceI, om.cvf_begin(vH), om.cvf_end(vH), it) {
			addFace( om.data(*it) );
		}
		omVertexD const & myVD  = om.data(vH);
		omVertexH myVH = omGmsh().at(myVD.MVertex());
		std::vector< float > itVal;
		dt__forFromToIter(omVertexEdgeI, ve_begin(myVH), ve_end(myVH), it) {
			omScalar angle = calc_dihedral_angle(*it);
			itVal.push_back( angle );
			itVal.push_back( angle*180./M_PI );
		}
		std::vector< std::string > header;
		header.push_back("rad");
		header.push_back("degree");
		DTINFOWF(
		  dtOMMeshManifold(),
			<< logMe::floatVecToTable(header, itVal)
		);
	}

	dtOMMeshManifold::~dtOMMeshManifold() {
	}
}
