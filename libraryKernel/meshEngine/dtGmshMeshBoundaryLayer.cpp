#include "dtGmshMeshBoundaryLayer.h"

#include <logMe/logMe.h>
#include <progHelper.h>

#include "dtGmshFace.h"
#include "dtOMMesh.h"
#include "dtOMMeshManifold.h"
#include "dtGmshRegion.h"
#include "dtGmshModel.h"
#include "dtOMVertexField.h"
#include "dtOMEdgeField.h"
#include "dtMoabCore.h"
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
		std::vector< dtOMMeshManifold > omM;		
		dt__forFromToIter(omVertexI, om.vertices_begin(), om.vertices_end(), v_it) {
			omM.push_back(dtOMMeshManifold(omFixed, omFixed[om[*v_it]]));			
		}

		//
		// divide manifolds and calculate normals
		// averaging all normals of manifolds
		//
		dtOMVertexField< dtVector3 > nF("nF", omFixed, dtVector3(0.,0.,0.) );
		dt__forAllIter(std::vector< dtOMMeshManifold >, omM, it) {
//			int itC = it - omM.begin();
//			it->writeMesh("omM_"+stringPrimitive::intToString(itC)+".vtk");
			std::vector< dtOMMeshManifold > divOmMs = it->divide(_maxDihedralAngle);
			std::vector< dtVector3 > nnV;
			dt__forAllIter(std::vector< dtOMMeshManifold >, divOmMs, itDiv) {
//				int itDivC = itDiv - divOmMs.begin();
//				itDiv->writeMesh(
//				   "omM_"+stringPrimitive::intToString(itC)+"_"
//				  +stringPrimitive::intToString(itDivC)+".vtk"
//				);
				nnV.push_back(itDiv->normal());
			}
			dtVector3 nn = dtLinearAlgebra::meanAverage(nnV);
			
			nF[it->centerMVertex()] = nn;
		}

		//
		// laplacian smoothing
		//
		for (int ii=0;ii<_nSmoothingSteps;ii++) {
			nF.laplacianSmooth();
			nF.execute( &dtLinearAlgebra::normalize );
		}

		//
		// create new vertices
		//
	  dtOMMesh omT(om);				
		dtOMVertexField< float > tF("tF", omFixed, 0.);
		dtOMVertexField< bool > lockF("lockF", omFixed, true);
		dt__forFromToIter(omVertexI, om.vertices_begin(), om.vertices_end(), it) {
			::MVertex * mv = om[*it];	
			if (!om.vertexIsBoundary(mv)) {
				tF[mv] = _thickness;
				lockF[mv] = false;
				::MVertex * mvNew = new ::MVertex(mv->x(), mv->y(), mv->z(), region);
				omT.replaceMVertex(omT[mv], mvNew);
				region->addMeshVertex(mvNew);
			}
		}
		tF.laplacianSmooth();
		
		//
		// determine thickness
		//

		dt__forFromToIter(omVertexI, om.vertices_begin(), om.vertices_end(), omIt) {
		  ::MVertex * om_mv = om[*omIt];
			if ( !lockF[om_mv] ) {

				dt__THROW_IF(
				  floatHandling::isSmall(dtLinearAlgebra::length(nF[om_mv])), 
					operator()
				);
				
				//
				// get "two-ring" neighbor faces
				//
				std::vector< omFaceH > neighborFace;
				dt__forFromToIter(
				  omVertexVertexI, 
					omFixed.vv_begin(omFixed[om_mv]), 
					omFixed.vv_end(omFixed[om_mv]), 
					vvIt
				) {
//				dt__forFromToIter(
//				  omVertexVertexI, 
//					omFixed.vv_begin(*vvIt), 
//					omFixed.vv_end(*vvIt), 
//					vvvIt
//				) {					
						dt__forFromToIter(
							omConstVertexFaceI, 
							omFixed.cvf_begin(*vvIt), 
							omFixed.cvf_end(*vvIt), 
							fIt
						) {
							if ( !omFixed.contains(*fIt, *omIt) ) neighborFace.push_back(*fIt);
						}
//					}	
				}
		    std::sort( neighborFace.begin(), neighborFace.end() );
        neighborFace.erase( 
				  std::unique( neighborFace.begin(), neighborFace.end() ), 
					neighborFace.end() 
				);

        //
        // check for intersections between target mesh vertex 
				// and neighbor faces
				//
				dtPoint3 start(om_mv->x(), om_mv->y(), om_mv->z());
				for (int ii=0;ii<_nShrinkingSteps;ii++) {
					dtPoint3 target = start + 2.*tF[om_mv]*nF[om_mv];
					if ( !omFixed.intersection(neighborFace, start, target) ) break;
					tF[om_mv] = .9*tF[om_mv];
					tF.laplacianSmooth();
				}
				if (tF[om_mv] != _thickness) {
					DTINFOWF(
				    operator(), 
						<< "Intersection => shrink t = " << _thickness << " -> " << tF[om_mv]
					);
				}
				
				//
				// move vertices of surface mesh om and create new vertices womIth old
				// posomItion in new surface mesh omT
				//
				dtPoint3 target = dtGmshModel::cast2DtPoint3(omT[*omIt]) + tF[om_mv] * nF[om_mv];
				om.replacePosition(*omIt, target);
				omFixed.replaceMVertex(omFixed[om_mv], om_mv);
		  }
		}
	
		//
		// dihedral angle field
		//		
		dtOMEdgeField< float > dAF("dA", omFixed, 0.);
		dtOMVertexField< bool > dABoolF("dABoolF", omFixed, false);
		dt__forFromToIter(omEdgeI, omFixed.edges_begin(), omFixed.edges_end(), eIt) {
		  dAF[*eIt] = omFixed.calc_dihedral_angle(*eIt);
		}
		dt__forFromToIter(omConstVertexI, omFixed.vertices_begin(), omFixed.vertices_end(), vIt) {
			std::vector< omEdgeH > oneRing = omFixed.oneRingEdgeH(*vIt);
			float min = 0.;
			float max = 0.;
			dt__forAllIter(std::vector< omEdgeH >, oneRing, eIt) {
				float const & dA = dAF[*eIt];
				min = std::min(dA, min);
				max = std::max(dA, max);
			}
			if ( fabs(max - min) > M_PI ) {
				dABoolF[*vIt] = true;
			  DTINFOWF(
				  operator(), 
					<< "Possible dihedral angle problem detected." << LOGDEL
				  << DTLOGEVAL(min) << LOGDEL
				  << DTLOGEVAL(max) << LOGDEL
				  << "|max-min| = " << max-min
				);
				
				//
				// move vertices of surface mesh om and create new vertices womIth old
				// posomItion in new surface mesh omT
				//
				MVertex * omFixed_mv = omFixed[*vIt];
				tF[omFixed_mv] = .5*tF[omFixed_mv];
				omVertexH omT_h = om[omFixed_mv];
				dtPoint3 target = dtGmshModel::cast2DtPoint3(omT[omT_h]) + tF[omFixed_mv] * nF[omFixed_mv];
				om.replacePosition(omT_h, target);
				omFixed.replacePosition(omFixed[omFixed_mv], target);				
			}			
//			omVertexH const vH = dAF[*vIt];
//		  float dA = omFixed.calc_dihedral_angle(*eIt);
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
				MVertex * om_mv = om[*vIt];
				MVertex * omT_mv = omT[*vIt];
				if ( lockF[om_mv] ) commonVertices.push_back(om_mv);
				else {
					omVertices.push_back(om_mv);
					omTVertices.push_back(omT_mv);
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
		


		
		//
		// write fields
		//
		dtMoabCore mb(omFixed);
		mb.addVertexField(tF);
		mb.addVertexField(nF);
		mb.addVertexField(lockF);
		mb.addEdgeField(dAF);
		mb.addVertexField(dABoolF);
		mb.write_mesh("dtGmshMeshBoundaryLayer.vtk");
  }
}

