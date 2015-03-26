#include "dtGmshMeshGFaceExtrude.h"

#include <logMe/logMe.h>
#include <progHelper.h>

#include "dtGmshFace.h"
#include "dtOMMesh.h"
#include "dtOMMeshManifold.h"
#include "dtGmshRegion.h"
#include <interfaceHeaven/stringPrimitive.h>

namespace dtOO {
	dtGmshMeshGFaceExtrude::dtGmshMeshGFaceExtrude() {
		_thickness = 0.;
	}

	dtGmshMeshGFaceExtrude::~dtGmshMeshGFaceExtrude() {
	}
	
	dtGmshMeshGFaceExtrude::dtGmshMeshGFaceExtrude( 
	  float const & thickness, int const nSmoothingSteps 
	) {
		_thickness = thickness;
		_nSmoothingSteps = nSmoothingSteps;
	}
	
  void dtGmshMeshGFaceExtrude::operator()( 
	  dtGmshRegion const * const region,
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
		
		std::vector< dtOMMeshManifold > omMs;		
		dt__forFromToIter(omVertexI, om.vertices_begin(), om.vertices_end(), v_it) {
			omMs.push_back( dtOMMeshManifold(om, *v_it) );			
		}

//		std::map< MVertex *, dtVector3 > mvN;
		dt__forAllIter(std::vector< dtOMMeshManifold >, omMs, it) {
			std::vector< dtOMMeshManifold > divOmMs = it->divide(M_PI*60./180.);
			std::vector< dtVector3 > nnV;
			dt__forAllIter(std::vector< dtOMMeshManifold >, divOmMs, itDiv) {
				nnV.push_back(itDiv->normal());
			}
			dtVector3 nn = dtLinearAlgebra::meanAverage(nnV);
			MVertex * mv = it->centerMVertex();
			om.vertexNormal(mv) = nn;
//			it->centerMVertex()->setXYZ(
//			  mv->x()+_thickness*nn.x(), 
//			  mv->y()+_thickness*nn.y(), 
//				mv->z()+_thickness*nn.z()
//			);
		}
		
		for (int ii=0;ii<_nSmoothingSteps;ii++) om.laplacianSmoothVertexNormal();
//		om.laplacianSmoothVertexNormal(1.);
		
		dt__forAllIter(std::vector< dtOMMeshManifold >, omMs, it) {
			MVertex * mv = it->centerMVertex();
			dtVector3 nn = om.vertexNormal(mv);
			it->centerMVertex()->setXYZ(
			  mv->x()+_thickness*nn.x(), 
			  mv->y()+_thickness*nn.y(), 
				mv->z()+_thickness*nn.z()
			);
		}	
  }
}

