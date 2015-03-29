#include "dtGmshMeshBoundaryLayer.h"

#include <logMe/logMe.h>
#include <progHelper.h>

#include "dtGmshFace.h"
#include "dtOMMesh.h"
#include "dtOMMeshManifold.h"
#include "dtGmshRegion.h"
#include "dtGmshModel.h"
#include <interfaceHeaven/floatHandling.h>
#include <interfaceHeaven/stringPrimitive.h>

#include <gmsh/MPrism.h>
#include <gmsh/MPyramid.h>
#include <gmsh/MTetrahedron.h>

namespace dtOO {
	dtGmshMeshBoundaryLayer::dtGmshMeshBoundaryLayer() {
		_thickness = 0.;
	}

	dtGmshMeshBoundaryLayer::~dtGmshMeshBoundaryLayer() {
	}
	
	dtGmshMeshBoundaryLayer::dtGmshMeshBoundaryLayer( 
        float const & thickness, std::vector< float > const & spacing,
        float const & maxDihedralAngle,
        int const nSmoothingSteps, int const nShrinkingSteps 
	) {
		_thickness = thickness;
		_spacing = spacing;
		_nSmoothingSteps = nSmoothingSteps;
		_nShrinkingSteps = nShrinkingSteps;
		_maxDihedralAngle = maxDihedralAngle;
	}
	
  void dtGmshMeshBoundaryLayer::operator()( 
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
		
		om.update();
		omFixed.update();
		
		//
		// create mesh manifolds
		//
		std::vector< dtOMMeshManifold > omMs;		
		dt__forFromToIter(omVertexI, om.vertices_begin(), om.vertices_end(), v_it) {
			omMs.push_back(dtOMMeshManifold(omFixed, omFixed[om[*v_it]]));			
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
			::MVertex * mv = it->centerMVertex();
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
		  ::MVertex * mv = om[*it];
			if (!om.vertexIsBoundary(mv)) {
      	::MVertex * mvNew = new ::MVertex(mv->x(), mv->y(), mv->z(), region);
				omT.replaceMVertex(omT[mv], mvNew);
  			region->addMeshVertex(mvNew);
				
				dtVector3 nn = omFixed.vertexNormal(mv);	
				dt__THROW_IF(floatHandling::isSmall(dtLinearAlgebra::length(nn)), operator());
				
				//
				// shrinking
				//
				std::vector< omFaceH > movingHv;
				dt__forFromToIter(omVertexVertexI, om.vv_begin(*it), om.vv_end(*it), vIt) {
					dt__forFromToIter(omConstVertexFaceI, om.cvf_begin(*vIt), om.cvf_end(*vIt), fIt) {
						if ( !om.contains(*fIt, *it) ) movingHv.push_back(*fIt);
					}
				}				
				float thisThickness = _thickness;
				dtPoint3 target;
				dtPoint3 start(mv->x(), mv->y(), mv->z());
				for (int ii=0;ii<_nShrinkingSteps;ii++) {
					dtVector3 nT = thisThickness*nn;
					target = dtPoint3(mv->x()+nT.x(), mv->y()+nT.y(), mv->z()+nT.z());
					mv->setXYZ(target.x(), target.y(), target.z());
					if ( om.intersection(movingHv, start, target) ) break;
					thisThickness = .5*thisThickness;
				}
				if (thisThickness != _thickness) {
					DTINFOWF(
				    operator(), 
						<< "Shrinking from " << DTLOGEVAL(_thickness) << " to " 
					  << DTLOGEVAL(thisThickness)
					);
				}
		  }
		}
		
		//
		// create new boundary elements
		//
		int nLayers = _spacing.size()+1;
		// calculate element width of each layer sheet
		dt__forFromToIter(omFaceI, om.faces_begin(), om.faces_end(), fIt) {
			std::vector< ::MVertex * > commonVertices;
			std::vector< ::MVertex * > omVertices;
			std::vector< ::MVertex * > omTVertices;
			dt__forFromToIter(omFaceVertexI, om.fv_begin(*fIt), om.fv_end(*fIt), vIt) {
				if ( om.data(*vIt).MVertex()->getNum() ==  omT.data(*vIt).MVertex()->getNum() ) {
					commonVertices.push_back( om.data(*vIt).MVertex() );
				}
				else {
					omVertices.push_back( om.data(*vIt).MVertex() );
					omTVertices.push_back( omT.data(*vIt).MVertex() );
				}
			}

			// create two dimensional arrays of all mesh vertices
			twoDArrayHandling< ::MVertex * > omVerticesL(nLayers, 0);
			twoDArrayHandling< ::MVertex * > omTVerticesL(nLayers, 0);			
			omVerticesL[0] = omVertices;			
			omTVerticesL[nLayers-1] = omTVertices;
			dt__forAllIndex(omTVertices, jj) {
				dtPoint3 mv0 = dtGmshModel::cast2DtPoint3(omVertices[jj]);
				dtPoint3 mv1 = dtGmshModel::cast2DtPoint3(omTVertices[jj]);
				dtVector3 vv = mv1-mv0;
				dt__toFloat(float nLayersF, nLayers);
				for (int ii=1; ii<nLayers; ii++) {
					dt__toFloat(float iiF, ii);
					dtPoint3 mvXYZ = mv0 + _spacing[ii-1]*vv;
					::MVertex * mv = new ::MVertex(0., 0., 0., region);
					region->addMeshVertex(mv);
					dtGmshModel::setPosition(mv, mvXYZ);
					omVerticesL[ii].push_back(mv);
					omTVerticesL[ii-1].push_back(mv);
				}
			}
			
			// create elements and add it to given region
			for (int ii=0; ii<nLayers; ii++) {
				if (commonVertices.size() == 0) {
					::MPrism * pri 
					= 
					new ::MPrism(
				    omVerticesL[ii][0], omVerticesL[ii][1], omVerticesL[ii][2],
						omTVerticesL[ii][0], omTVerticesL[ii][1], omTVerticesL[ii][2]
					);
					if (pri->getVolumeSign()<0.) pri->reverse();
					region->addPrism(pri);
				}
				else if (commonVertices.size() == 1) {
					::MPyramid * pyr 
					= 
					new ::MPyramid(
						omTVerticesL[ii][1], omTVerticesL[ii][0], 
						omVerticesL[ii][0], omVerticesL[ii][1], 
						commonVertices[0]
					);
					if (pyr->getVolumeSign()<0.) pyr->reverse();
					region->addPyramid(pyr);		
				}
				else if (commonVertices.size() == 2) {
					::MTetrahedron * tet 
					= 
					new ::MTetrahedron(
						omTVerticesL[ii][0], omVerticesL[ii][0], 
						commonVertices[0], commonVertices[1]
					);				
					if (tet->getVolumeSign()<0.) tet->reverse();
					region->addTetrahedron(tet);					
				}
			}
		}
  }
}

