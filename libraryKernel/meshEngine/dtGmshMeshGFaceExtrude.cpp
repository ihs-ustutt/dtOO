#include "dtGmshMeshGFaceExtrude.h"

#include <logMe/logMe.h>
#include <progHelper.h>

#include "dtGmshFace.h"
#include "dtOMMesh.h"
#include "dtOMMeshManifold.h"
#include "dtGmshRegion.h"
#include <interfaceHeaven/stringPrimitive.h>

#include <gmsh/MPrism.h>
#include <gmsh/MPyramid.h>
#include <gmsh/MTetrahedron.h>

namespace dtOO {
	dtGmshMeshGFaceExtrude::dtGmshMeshGFaceExtrude() {
		_thickness = 0.;
	}

	dtGmshMeshGFaceExtrude::~dtGmshMeshGFaceExtrude() {
	}
	
	dtGmshMeshGFaceExtrude::dtGmshMeshGFaceExtrude( 
	  float const & thickness, float const & maxDihedralAngle,
		int const nSmoothingSteps 
	) {
		_thickness = thickness;
		_nSmoothingSteps = nSmoothingSteps;
		_maxDihedralAngle = maxDihedralAngle;
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
			std::vector< dtOMMeshManifold > divOmMs = it->divide(_maxDihedralAngle);
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
		
//		dt__forAllIter(std::vector< dtOMMeshManifold >, omMs, it) {
		dtOMMesh omT(om);
		dt__forFromToIter(omVertexI, om.vertices_begin(), om.vertices_end(), it) {
			MVertex * mv = om.data(*it).MVertex();//it->centerMVertex();
			if (!om.vertexIsBoundary(mv)) {
				dtVector3 nn = om.vertexNormal(mv);
//				omT.data(omT.omGmsh()[mv]).MVertex()
				MVertex * mvNew = new MVertex(mv->x(), mv->y(), mv->z(), const_cast<dtGmshRegion * >(region));
				omT.replaceMVertex(omT.omGmsh().at(mv), mvNew);
  			const_cast<dtGmshRegion*>(region)->addMeshVertex(mvNew);				
				mv->setXYZ(
					mv->x()+_thickness*nn.x(), 
					mv->y()+_thickness*nn.y(), 
					mv->z()+_thickness*nn.z()//,
				  //const_cast<dtGmshRegion * >(region)
				);
		  }
//			else {
//				omT.omGmsh()[mv] 
//			}
		}
		dt__forFromToIter(omFaceI, om.faces_begin(), om.faces_end(), fIt) {
//			omFaceD & omF = om.data(*fIt);
//			omFaceD & omTF = omT.data(*fIt);
		
			std::vector< MVertex * > commonVertices;
			std::vector< MVertex * > omVertices;
			std::vector< MVertex * > omTVertices;
			dt__forFromToIter(omFaceVertexI, om.fv_begin(*fIt), om.fv_end(*fIt), vIt) {	
				if (om.data(*vIt).MVertex()->getNum() ==  omT.data(*vIt).MVertex()->getNum() ) {
					commonVertices.push_back( om.data(*vIt).MVertex() );
				}
				else {
					omVertices.push_back( om.data(*vIt).MVertex() );
					omTVertices.push_back( omT.data(*vIt).MVertex() );
				}
			}
			
			if (commonVertices.size() == 0) {
				const_cast<dtGmshRegion*>(region)->addPrism( 
				  new MPrism(
				    omVertices[0], omVertices[1], omVertices[2],
						omTVertices[0], omTVertices[1], omTVertices[2]
			    ) 
				);
			}
			else if (commonVertices.size() == 1) {
				const_cast<dtGmshRegion*>(region)->addPyramid( 
				  new MPyramid(
				    omVertices[0], omVertices[1], 
						omTVertices[1], omTVertices[0], 
						commonVertices[0]
			    ) 
				);				
			}
			else if (commonVertices.size() == 2) {
				const_cast<dtGmshRegion*>(region)->addTetrahedron(
				  new MTetrahedron(
				    omVertices[0], omTVertices[0], 
						commonVertices[0], commonVertices[1]
			    ) 
				);					
			}
			
			
		}
  }
}

