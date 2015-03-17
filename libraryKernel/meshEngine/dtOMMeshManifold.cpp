#include "dtOMMeshManifold.h"
#include "progHelper.h"

namespace dtOO {
  dtOMMeshManifold::dtOMMeshManifold(dtOMMesh const & om, omVertexH const & vH) {
		dt__forFromToIter(omConstVertexFaceI, om.cvf_begin(vH), om.cvf_end(vH), it) {
		  omFaceD const & fD = om.data(*it);
			addFace( fD.MElement() );
		}
		omVertexD const & myVD  = om.data(vH);
		omVertexH myVH = omGmsh().at(myVD.MVertex());
		dt__forFromToIter(omVertexEdgeI, ve_begin(vH), ve_end(vH), it) {
//			omHalfedgeD const & heD = data(*it);
			omScalar angle = calc_dihedral_angle(*it);
			DTINFOWF(dtOMMeshManifold(), << DTLOGEVAL(angle) );
		}
	}

	dtOMMeshManifold::~dtOMMeshManifold() {
	}
}
