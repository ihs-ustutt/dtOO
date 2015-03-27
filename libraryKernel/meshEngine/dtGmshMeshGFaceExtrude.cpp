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
	  dtGmshRegion * region,
	  std::list< dtGmshFace const * > const & face, 
		std::vector< int > const & ori,
	  std::list< dtGmshFace const * > const & fface, 
		std::vector< int > const & fori 					
	) {
		//
		// define two surface meshes
		//
		dtOMMesh om;
		dtOMMesh omFixed;
	
		//
		// surface mesh to thick
		//
		int itC = 0;
		dt__forAllConstIter( std::list< dtGmshFace const * >, face, it ) {
			dtGmshFace const * const &thisFace = *it;
			//
			// add current surface mesh
			//
			dt__pH(dtOMMesh) tmpOM(thisFace->getOMMesh());
			if (ori[itC] > 0) {
				om.add(*tmpOM);		
				omFixed.add(*tmpOM);		
			}
			else if (ori[itC] < 0) {
				om.addInv(*tmpOM);
				omFixed.addInv(*tmpOM);		
			}
			else {
				dt__THROW(
				  operator(), 
					<< "Undefined orientation of face (tag = " << thisFace->tag() << ")."
				);
			}
			itC++;
		}
		
		//
		// fixed surface mesh
		//
		itC = 0;
		dt__forAllConstIter( std::list< dtGmshFace const * >, fface, it ) {
			dtGmshFace const * const &thisFace = *it;
			//
			// add current surface mesh
			//
			dt__pH(dtOMMesh) tmpOM(thisFace->getOMMesh());
			
			if (fori[itC] > 0) {
				omFixed.add(*tmpOM);		
			}
			else if (fori[itC] < 0) {
				omFixed.addInv(*tmpOM);		
			}
			else {
				dt__THROW(
				  operator(), 
					<< "Undefined orientation of fixedFace (tag = " << thisFace->tag() << ")."
				);
			}
			itC++;
		}		
		
		//
		// create mesh manifolds
		//
		std::vector< dtOMMeshManifold > omMs;		
		dt__forFromToIter(omVertexI, om.vertices_begin(), om.vertices_end(), v_it) {
			omMs.push_back( 
			  dtOMMeshManifold(omFixed, omFixed.requestVertexH(om.requestMVertex(*v_it))) 
			);			
		}

		//
		// divide manifolds and calculate normals
		// averaging all normals of manifolds
		//
		dt__forAllIter(std::vector< dtOMMeshManifold >, omMs, it) {
//			int itC = it - omMs.begin();
//			it->writeMesh("omMs_"+stringPrimitive::intToString(itC)+".vtk");
			std::vector< dtOMMeshManifold > divOmMs = it->divide(_maxDihedralAngle);
			std::vector< dtVector3 > nnV;
			dt__forAllIter(std::vector< dtOMMeshManifold >, divOmMs, itDiv) {
//				int itDivC = itDiv - divOmMs.begin();
//				itDiv->writeMesh(
//				   "omMs_"+stringPrimitive::intToString(itC)+"_"
//				  +stringPrimitive::intToString(itDivC)+".vtk"
//				);
				nnV.push_back(itDiv->normal());
			}
			dtVector3 nn = dtLinearAlgebra::meanAverage(nnV);
			MVertex * mv = it->centerMVertex();
			omFixed.vertexNormal(mv) = nn;
		}

		//
		// laplacian smoothing
		//
		for (int ii=0;ii<_nSmoothingSteps;ii++) omFixed.laplacianSmoothVertexNormal();

		//
		// create new vertices
		//
		dtOMMesh omT(om);
		dt__forFromToIter(omVertexI, om.vertices_begin(), om.vertices_end(), it) {
			MVertex * mv = om.requestMVertex(*it);
			if (!om.vertexIsBoundary(mv)) {
				MVertex * mvNew = new MVertex(mv->x(), mv->y(), mv->z(), region);
				omT.replaceMVertex(omT.requestVertexH(mv), mvNew);
  			region->addMeshVertex(mvNew);
				dtVector3 nT = _thickness*omFixed.vertexNormal(mv);				
				mv->setXYZ(mv->x()+nT.x(), mv->y()+nT.y(), mv->z()+nT.z());
		  }
		}
		
		//
		// create new boundary elements
		//
		dt__forFromToIter(omFaceI, om.faces_begin(), om.faces_end(), fIt) {
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

