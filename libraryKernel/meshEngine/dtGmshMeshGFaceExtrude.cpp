#include "dtGmshMeshGFaceExtrude.h"

#include <logMe/logMe.h>
#include <progHelper.h>

#include "dtGmshFace.h"
#include "dtOMMesh.h"
#include "dtOMMeshManifold.h"

namespace dtOO {
	dtGmshMeshGFaceExtrude::dtGmshMeshGFaceExtrude() {
		_thickness = 0.;
	}

	dtGmshMeshGFaceExtrude::~dtGmshMeshGFaceExtrude() {
	}
	
	dtGmshMeshGFaceExtrude::dtGmshMeshGFaceExtrude( float const & thickness ) {
		_thickness = thickness;
	}
	
  void dtGmshMeshGFaceExtrude::operator()( 
	  std::list< dtGmshFace const * > const & face, 
		std::vector< int > const & ori 
	) {
		dtOMMesh om;
	
		int itC = 0;
		dt__forAllConstIter( std::list< dtGmshFace const * >, face, it ) {
			dtGmshFace const * const &thisFace = *it;

			dt__pH(dtOMMesh) tmpOM(thisFace->getOMMesh());
			
			if (ori[itC] > 0) om.add(*tmpOM);		
			else if (ori[itC] < 0) om.addInv(*tmpOM);
			else {
				DTWARNINGWF(
				  operator(), 
					<< "Ignoring face (tag = " << thisFace->tag() << ")."
				);
			}
			itC++;
		}
		
		dt__forFromToIter(omVertexI, om.vertices_begin(), om.vertices_end(), v_it) {
			dtOMMeshManifold(om, *v_it);
		}			
	}
}

