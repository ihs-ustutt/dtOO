#include "dtGmshMeshGFaceExtrude.h"

#include <logMe/logMe.h>
#include <progHelper.h>

#include "dtGmshFace.h"
#include "dtOMMesh.h"

namespace dtOO {
	dtGmshMeshGFaceExtrude::dtGmshMeshGFaceExtrude() {
	}

	dtGmshMeshGFaceExtrude::~dtGmshMeshGFaceExtrude() {
	}
	
	dtGmshMeshGFaceExtrude::dtGmshMeshGFaceExtrude( float const & thickness ) {
		_thickness = thickness;
	}
	
  void dtGmshMeshGFaceExtrude::operator()( std::list< dtGmshFace * > const & face ) {
		dt__FORALLCONSTITER( std::list< dtGmshFace * >, face, it ) {
			dtGmshFace const * const &thisFace = *it;
      DTINFOWF(operator(), << "Working on face = " << thisFace->tag());
			
		  dtOMMesh * om = thisFace->getOMMesh();	
			
			for (
				omVertexI v_it = om->vertices_begin();
				v_it != om->vertices_end();
				++v_it
			) {
				omVertexD omD = om->data(*v_it);
				MVertex const * const mv = omD.MVertex();
				DTINFOWF(
					operator(),
				  << DTLOGEVAL(mv->x()) << LOGDEL
				  << DTLOGEVAL(mv->y()) << LOGDEL
				  << DTLOGEVAL(mv->z())
				);
			}			
			
		}
	}
	
	void dtGmshMeshGFaceExtrude::operator()( dtGmshFace * face ) {
		std::list< dtGmshFace * > faceL;
		faceL.push_back(face);
		
		this->operator()(faceL);
	}
}

