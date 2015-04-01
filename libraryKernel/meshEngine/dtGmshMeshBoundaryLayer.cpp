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
		DTINFOWF(operator(), << "Start algorithm ...");
		dt__pVH(::MElement) elements;
		//
		// define two surface meshes
		//
		dtOMMesh omInit;
		dtOMMesh omMoved;
		dtOMVertexField< bool > fixedF("fixedF", omInit, false);
	  dtOMVertexField< float > tF("tF", omInit, _thickness);
		
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
				omInit.add(*tmpOM);		
				omMoved.add(*tmpOM);
			}
			else if (ori[itC] < 0) {
				omInit.addInv(*tmpOM);
				omMoved.addInv(*tmpOM);		
			}
			else {
				dt__THROW(
				  operator(), 
					<< "Undefined orientation of face (tag = " << thisFace->tag() << ")."
				);
			}
			//
			// set fixed and thickness field
			//
			fixedF.assign(*tmpOM, false);
			tF.assign(*tmpOM, _thickness);
			
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
				omInit.add(*tmpOM);						
				omMoved.add(*tmpOM);		
			}
			else if (fori[itC] < 0) {
				omInit.addInv(*tmpOM);						
				omMoved.addInv(*tmpOM);		
			}
			else {
				dt__THROW(
				  operator(), 
					<< "Undefined orientation of fixedFace (tag = " << thisFace->tag() << ")."
				);
			}
			itC++;
			
			//
			// set fixed and thickness field
			//
			fixedF.assign(*tmpOM, true);
			tF.assign(*tmpOM, 0.);
		}		
		
		//
		// fix boundary nodes
		//
		dt__forFromToIter(omVertexI, omInit.vertices_begin(), omInit.vertices_end(), it) {
			::MVertex * mv = omInit[*it];	
			if ( omInit.is_boundary(*it) ) {
				fixedF[*it] = true;
				tF[*it] = 0.;
			}
		}
		
		omInit.update();
		omMoved.update();

		DTINFOWF(operator(), << "Initialized");
		//
		// create mesh manifolds
		//
		std::vector< dtOMMeshManifold > omManifolds;		
		dt__forFromToIter(omVertexI, omMoved.vertices_begin(), omMoved.vertices_end(), v_it) {
			omManifolds.push_back( dtOMMeshManifold(omMoved, *v_it) );			
		}

		//
		// divide manifolds and calculate normals
		// averaging all normals of manifolds
		//
		dtOMVertexField< dtVector3 > nF("nF", omInit, dtVector3(0.,0.,0.) );		
		dt__forAllIter(std::vector< dtOMMeshManifold >, omManifolds, it) {
			std::vector< dtOMMeshManifold > divOmMs = it->divide(_maxDihedralAngle);
			std::vector< dtVector3 > nnV;
			dt__forAllIter(std::vector< dtOMMeshManifold >, divOmMs, itDiv) {
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

		DTINFOWF(operator(), << "Normals calculated.");
		
		//
		// create new vertices
		//
		dt__forFromToIter(omVertexI, omInit.vertices_begin(), omInit.vertices_end(), it) {
			if ( !fixedF[*it] ) {
        ::MVertex * mv = omInit[*it];					
				::MVertex * mvNew = new ::MVertex(mv->x(), mv->y(), mv->z(), region);
				omInit.replaceMVertex(*it, mvNew);
				region->addMeshVertex(mvNew);
			}
		}
//		tF.laplacianSmooth();
		//
		// create new elements
		//
		dt__forFromToIter(omFaceI, omInit.faces_begin(), omInit.faces_end(), fIt) {
			std::vector< ::MVertex * > vertices;
			dt__forFromToIter(omFaceVertexI, omInit.fv_begin(*fIt), omInit.fv_end(*fIt), vIt) {
        vertices.push_back( omInit[*vIt] );
			}
			elements.push_back( ::MElement::createElement(MSH_TRI_3, vertices) );
			omInit.data(*fIt).MElement( &(elements.back()) );
		}
		
		DTINFOWF(operator(), << "New vertices created.");
		
		//
		// determine thickness
		//
		dt__forFromToIter(omVertexI, omInit.vertices_begin(), omInit.vertices_end(), it) {
			if ( !fixedF[*it] ) {
        ::MVertex * mv_omInit = omInit[*it];
				dt__THROW_IF(
				  floatHandling::isSmall(dtLinearAlgebra::length(nF[*it])), 
					operator()
				);
				
				//
				// get "two-ring" neighbor faces
				//
				std::vector< omFaceH > neighborFace;
				dt__forFromToIter(
				  omVertexVertexI, 
					omMoved.vv_begin(*it), 
					omMoved.vv_end(*it), 
					vvIt
				) {
//				dt__forFromToIter(
//				  omVertexVertexI, 
//					omMoved.vv_begin(*vvIt), 
//					omMoved.vv_end(*vvIt), 
//					vvvIt
//				) {					
						dt__forFromToIter(
							omConstVertexFaceI, 
							omMoved.cvf_begin(*vvIt), 
							omMoved.cvf_end(*vvIt), 
							fIt
						) if ( !omMoved.contains(*fIt, *it) ) neighborFace.push_back(*fIt);
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
				dtPoint3 start(mv_omInit->x(), mv_omInit->y(), mv_omInit->z());
				for (int ii=0;ii<_nShrinkingSteps;ii++) {
					dtPoint3 target = start + 2.*tF[*it]*nF[*it];
					if ( !omMoved.intersection(neighborFace, start, target) ) break;
					tF[*it] = .9*tF[*it];
//					tF.laplacianSmooth();
				}
				if (tF[*it] != _thickness) {
					DTINFOWF(
				    operator(), 
						<< "Intersection => shrink t = " << _thickness << " -> " << tF[*it]
					);
				}
				
				//
				// move vertices of surface mesh om and create new vertices with old
				// position in new surface mesh omT
				//
				dtPoint3 target = dtGmshModel::cast2DtPoint3(omInit[*it]) + tF[*it] * nF[*it];
				omMoved.replacePosition(*it, target);
		  }
		}
		
		DTINFOWF(operator(), << "Shrinked.");
	
		//
		// dihedral angle field
		//		
		dtOMEdgeField< float > dAF("dA", omInit, 0.);
		dt__forFromToIter(omEdgeI, omMoved.edges_begin(), omMoved.edges_end(), eIt) {
		  dAF[*eIt] = omMoved.calc_dihedral_angle(*eIt);
		}
		dt__forFromToIter(omConstVertexI, omMoved.vertices_begin(), omMoved.vertices_end(), vIt) {
			std::vector< omEdgeH > oneRing = omMoved.oneRingEdgeH(*vIt);
			float min = 0.;
			float max = 0.;
			dt__forAllIter(std::vector< omEdgeH >, oneRing, eIt) {
				float const & dA = dAF[*eIt];
				min = std::min(dA, min);
				max = std::max(dA, max);
			}
			if ( fabs(max - min) > M_PI ) {
//				dABoolF[*vIt] = true;
			  DTINFOWF(
				  operator(), 
					<< "Possible dihedral angle problem detected." << LOGDEL
				  << DTLOGEVAL(min) << LOGDEL
				  << DTLOGEVAL(max) << LOGDEL
				  << "|max-min| = " << max-min
				);
							
				//
				// move vertices of surface mesh om and create new vertices with old
				// position in new surface mesh omT
				//
				tF[*vIt] = .5*tF[*vIt];
				dtPoint3 target = dtGmshModel::cast2DtPoint3(omInit[*vIt]) + tF[*vIt] * nF[*vIt];
				omMoved.replacePosition(*vIt, target);				

				//
				// move vertices of surface mesh om and create new vertices with old
				// position in new surface mesh omT
				//
				dt__forFromToIter(omConstVertexVertexI, omMoved.vv_begin(*vIt), omMoved.vv_end(*vIt), vvIt) {
					if ( !fixedF[*vIt] ) {
						tF[*vIt] = .75*tF[*vvIt];
						dtPoint3 target = dtGmshModel::cast2DtPoint3(omInit[*vvIt]) + tF[*vvIt] * nF[*vvIt];
						omMoved.replacePosition(*vvIt, target);
					}
				}
				
			}			
//			omVertexH const vH = dAF[*vIt];
//		  float dA = omMoved.calc_dihedral_angle(*eIt);
		}
		
		//
		// create new boundary elements
		//
		int nLayers = _spacing.size()+1;
		// calculate element width of each layer sheet
		dt__forFromToIter(omFaceI, omInit.faces_begin(), omInit.faces_end(), fIt) {
			std::vector< ::MVertex * > commonVertices;
			std::vector< ::MVertex * > omFixedVertices;
			std::vector< ::MVertex * > omMovedVertices;
			dt__forFromToIter(omFaceVertexI, omInit.fv_begin(*fIt), omInit.fv_end(*fIt), vIt) {
				MVertex * mv_omInit = omInit[*vIt];
				MVertex * mv_omMoved = omMoved[*vIt];
				if ( fixedF[*vIt] ) commonVertices.push_back(mv_omMoved);
				else {
					omFixedVertices.push_back(mv_omInit);
					omMovedVertices.push_back(mv_omMoved);
				}
			}
			
			// create two dimensional arrays of all mesh vertices
			twoDArrayHandling< ::MVertex * > omFixedVerticesL(nLayers, 0);
			twoDArrayHandling< ::MVertex * > omMovedVerticesL(nLayers, 0);			
			omFixedVerticesL[0] = omFixedVertices;			
			omMovedVerticesL[nLayers-1] = omMovedVertices;
			dt__forAllIndex(omMovedVertices, jj) {
				dtPoint3 mv0 = dtGmshModel::cast2DtPoint3(omFixedVertices[jj]);
				dtPoint3 mv1 = dtGmshModel::cast2DtPoint3(omMovedVertices[jj]);
				dtVector3 vv = mv1-mv0;
				dt__toFloat(float nLayersF, nLayers);
				for (int ii=1; ii<nLayers; ii++) {
					dt__toFloat(float iiF, ii);
					dtPoint3 mvXYZ = mv0 + _spacing[ii-1]*vv;
					::MVertex * mv = new ::MVertex(0., 0., 0., region);
					region->addMeshVertex(mv);
					dtGmshModel::setPosition(mv, mvXYZ);
					omFixedVerticesL[ii].push_back(mv);
					omMovedVerticesL[ii-1].push_back(mv);
				}
			}
			
			// create elements and add it to given region
			for (int ii=0; ii<nLayers; ii++) {
				if (commonVertices.size() == 0) {
					::MPrism * pri 
					= 
					new ::MPrism(
				    omFixedVerticesL[ii][0], omFixedVerticesL[ii][1], omFixedVerticesL[ii][2],
						omMovedVerticesL[ii][0], omMovedVerticesL[ii][1], omMovedVerticesL[ii][2]
					);
					if (pri->getVolumeSign()<0.) pri->reverse();
					region->addPrism(pri);
				}
				else if (commonVertices.size() == 1) {
					::MPyramid * pyr 
					= 
					new ::MPyramid(
						omMovedVerticesL[ii][1], omMovedVerticesL[ii][0], 
						omFixedVerticesL[ii][0], omFixedVerticesL[ii][1], 
						commonVertices[0]
					);
					if (pyr->getVolumeSign()<0.) pyr->reverse();
					region->addPyramid(pyr);		
				}
				else if (commonVertices.size() == 2) {
					::MTetrahedron * tet 
					= 
					new ::MTetrahedron(
						omMovedVerticesL[ii][0], omFixedVerticesL[ii][0], 
						commonVertices[0], commonVertices[1]
					);				
					if (tet->getVolumeSign()<0.) tet->reverse();
					region->addTetrahedron(tet);					
				}
			}
		}
		
    DTINFOWF(operator(), << "Elements created");

		
		//
		// write fields
		//
		dtMoabCore mb(tF.refMesh());
		mb.addVertexField(tF);
		mb.addVertexField(nF);
		mb.addVertexField(fixedF);
		mb.write_mesh("dtGmshMeshBoundaryLayer.vtk");
	  
		DTINFOWF(operator(), << "Fields written.");
		DTINFOWF(operator(), << "Done.");
  }
}

