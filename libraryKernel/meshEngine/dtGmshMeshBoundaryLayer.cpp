#include "dtGmshMeshBoundaryLayer.h"

#include <logMe/logMe.h>
#include <progHelper.h>

#include "dtGmshFace.h"
#include "dtOMMeshManifold.h"
#include "dtMoabCore.h"
#include "dtGmshVertex.h"
#include "dtGmshEdge.h"
#include "dtGmshFace.h"
#include "dtGmshRegion.h"
#include "dtGmshModel.h"
#include <interfaceHeaven/floatHandling.h>
#include <interfaceHeaven/stringPrimitive.h>

#include <gmsh/MTriangle.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/MPrism.h>
#include <gmsh/MPyramid.h>
#include <gmsh/MTetrahedron.h>

namespace dtOO {
	dtGmshMeshBoundaryLayer::~dtGmshMeshBoundaryLayer() {
	}
	
	dtGmshMeshBoundaryLayer::dtGmshMeshBoundaryLayer(
	  std::list< dtGmshFace const * > const & face, 
		std::vector< int > const & ori,
	  std::list< dtGmshFace const * > const & fface, 
		std::vector< int > const & fori,
	  std::list< dtGmshFace const * > const & mface, 
		std::vector< int > const & mori,  
		float const & thickness, std::vector< float > const & spacing,
		float const & maxDihedralAngle,
		int const nSmoothingSteps, int const nShrinkingSteps 
	) : _fixedF("_fixedF", _omInit, false), 
      _slidableF("_slidableF", _omInit, false),
      _tF("_tF", _omInit, thickness),
      _extrudeF("_extrudeF", _omInit, 1),
      _physicalName("_physicalName", _omInit, ""),
      _thickness(thickness),
      _spacing(spacing),
      _nSmoothingSteps(nSmoothingSteps),
      _nShrinkingSteps(nShrinkingSteps),
      _maxDihedralAngle(maxDihedralAngle) {
    dtOMDynamicVertexField< bool > canSlideF("canSlideF", _omInit, false);
		
		//
		// surface mesh to thick
		//
		int itC = 0;
		dt__forAllConstIter( std::list< dtGmshFace const * >, face, it ) {
			dtGmshFace const * const &thisFace = *it;
			dt__pH(dtOMMesh) tmpOM(thisFace->getOMMesh());
			if (ori[itC] > 0) {
				dt__info(
				  operator(), 
					<< "Adding normal face (tag = " << thisFace->tag() << ") with."
				);        
				_omInit.add(*tmpOM);		
				_omMoved.add(*tmpOM);
			}
			else if (ori[itC] < 0) {
				dt__info(
				  operator(), 
					<< "Adding inverted face (tag = " << thisFace->tag() << ") with."
				);                
				_omInit.addInv(*tmpOM);
				_omMoved.addInv(*tmpOM);		
			}
			else {
				dt__throw(
				  operator(), 
					<< "Undefined orientation of face (tag = " << thisFace->tag() << ")."
				);
			}
			
			itC++;
      
      //
      // store physical name
      //
      _physicalName.assign(
        *tmpOM, 
        dtGmshModel::DownCast(thisFace->model())->getPhysicalString(thisFace)
      );
		}
		
		//
		// slidable surface mesh
		//
		itC = 0;
		dt__forAllConstIter( std::list< dtGmshFace const * >, mface, it ) {
			dtGmshFace const * const &thisFace = *it;
			dt__pH(dtOMMesh) tmpOM(thisFace->getOMMesh());
			
			if (mori[itC] > 0) {
				dt__info(
				  operator(), 
					<< "Adding normal slidableFace (tag = " << thisFace->tag() << ") with."
				);                
				_omInit.add(*tmpOM);						
				_omMoved.add(*tmpOM);		
			}
			else if (mori[itC] < 0) {
				dt__info(
				  operator(), 
					<< "Adding inverted slidableFace (tag = " << thisFace->tag() << ") with."
				);                
				_omInit.addInv(*tmpOM);						
				_omMoved.addInv(*tmpOM);		
			}
			else {
				dt__throw(
				  operator(), 
					<< "Undefined orientation of slidableFace (tag = " 
          << thisFace->tag() << ")."
				);
			}
      
			itC++;
			
			//
			// set fixed and thickness field
			//
			_fixedF.assign(*tmpOM, true);
      canSlideF.assign(*tmpOM, true);
      _extrudeF.assign(*tmpOM, 0);
      
      //
      // store physical name
      //
      _physicalName.assign(
        *tmpOM, 
        dtGmshModel::DownCast(thisFace->model())->getPhysicalString(thisFace)
      );      
		}
    
		//
		// fixed surface mesh
		//
		itC = 0;
		dt__forAllConstIter( std::list< dtGmshFace const * >, fface, it ) {
			dtGmshFace const * const &thisFace = *it;
			dt__pH(dtOMMesh) tmpOM(thisFace->getOMMesh());
			
			if (fori[itC] > 0) {
				dt__info(
				  operator(), 
					<< "Adding normal fixedFace (tag = " << thisFace->tag() << ") with."
				);                
				_omInit.add(*tmpOM);						
				_omMoved.add(*tmpOM);		
			}
			else if (fori[itC] < 0) {
				dt__info(
				  operator(), 
					<< "Adding inverted fixedFace (tag = " << thisFace->tag() << ") with."
				);                
				_omInit.addInv(*tmpOM);						
				_omMoved.addInv(*tmpOM);		
			}
			else {
				dt__throw(
				  operator(), 
					<< "Undefined orientation of fixedFace (tag = " 
          << thisFace->tag() << ")."
				);
			}
      
			itC++;
			
			//
			// set fixed and thickness field
			//
			_fixedF.assign(*tmpOM, true);
			_tF.assign(*tmpOM, 0.);
      _extrudeF.assign(*tmpOM, 0);
      
      //
      // store physical name
      //
      _physicalName.assign(
        *tmpOM, 
        dtGmshModel::DownCast(thisFace->model())->getPhysicalString(thisFace)
      );      
		}		

    //
    // update mesh and fields
    //
		_omInit.update();
		_omMoved.update();    
    
		//
		// detect sliders
		//
		dt__forFromToIter(
      omVertexI, _omInit.vertices_begin(), _omInit.vertices_end(), it
    ) {
      if ( isSlidable(canSlideF, *it) ) {
        _slidableF[*it] = true;
        _fixedF[*it] = false;
      }
    }
    
		//
		// fix boundary nodes
		//
		dt__forFromToIter(
      omVertexI, _omInit.vertices_begin(), _omInit.vertices_end(), it
    ) {
			if ( _omInit.is_boundary(*it) ) {
				_fixedF[*it] = true;
				_tF[*it] = 0.;
			}
		}

		//
		// find slidable faces and mark them with 2
		//
		dt__forFromToIter(
      omFaceI, _omInit.faces_begin(), _omInit.faces_end(), fIt
    ) {
      int slider = 0;
			dt__forFromToIter(
			  omConstFaceVertexI,_omInit.cfv_begin(*fIt), _omInit.cfv_end(*fIt), vIt
			) if (_slidableF.at(*vIt)) slider++;

      //
      // slidable face is extrudable and has sliders
      //
			if ( (slider>0) && _extrudeF.at(*fIt) ) _extrudeF[*fIt] = 2;        
		}
	}
	
  void dtGmshMeshBoundaryLayer::operator() ( 
	  dtGmshModel * const wModel,
	  std::vector< ::MVertex * > & vertex, std::vector< ::MElement * > & element
	) {
		dt__info(operator(), << "Initialized");

		//
		// create mesh manifolds
		//
		std::vector< dtOMMeshManifold > omManifolds;		
		dt__forFromToIter(
		  omVertexI, _omMoved.vertices_begin(), _omMoved.vertices_end(), v_it
		) omManifolds.push_back( dtOMMeshManifold(_omMoved, *v_it) );

		//
		// divide manifolds and calculate normals
		// averaging all normals of manifolds
		//
		dtOMVertexField< dtVector3 > nF("nF", _omInit, dtVector3(0.,0.,0.) );		
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

		dt__info(operator(), << "Normals calculated.");

    //
    // add new GEntities, find relationships, create new mesh vertices
		// and add new mesh elements
    //		
		modifyGEntities(wModel);
		dt__info(operator(), << "New vertices created.");
				
    //
    // create field
    //
    dtOMVertexField< bool > isNew("isNew", _omMoved, false);
    dt__forFromToIter(
      omVertexI, _omMoved.vertices_begin(), _omMoved.vertices_end(), vIt        
    ) if (_omMoved.at(*vIt) != _omInit.at(*vIt)) isNew[*vIt] = true;
    
		//
		// determine thickness
		// modify omMoved and tF
		//
		determineThickness(nF);
		dt__info(operator(), << "Shrinked.");
	
//		//
//		// create dihedral angle field and check for intersections based on
//		// dihedral angles
//		//		
//		dtOMEdgeField< float > dAF("dA", _omInit, 0.);
//		dt__forFromToIter(
//      omEdgeI, _omMoved.edges_begin(), _omMoved.edges_end(), eIt
//    ) dAF[*eIt] = _omMoved.calc_dihedral_angle(*eIt);
//    dihedralAngleIntersection(dAF, nF);
		
		//
		// create new boundary layer elements
		//
		createBoundaryLayerElements(vertex, element);
    dt__info(operator(), << "Elements created");
		
		//
		// write fields
		//
		dtMoabCore mb(_tF.refMesh());
		mb.addVertexField(_tF);
		mb.addVertexField(nF);
		mb.addVertexField(_fixedF);
    mb.addVertexField(_slidableF);
    mb.addFaceField(_extrudeF);
		mb.write_mesh("dtGmshMeshBoundaryLayer.vtk");
	  
    dtMoabCore::writeVtkVertexField(isNew);    
		dt__info(operator(), << "Fields written.");
		dt__info(operator(), << "Done.");
  }
	
	void dtGmshMeshBoundaryLayer::determineThickness(
		dtOMVertexField< dtVector3 > const & nF
	) {
		dt__forFromToIter(
      omConstVertexI, _omInit.vertices_begin(), _omInit.vertices_end(), it
    ) {
      // leave fixed vertex unchanged
			if ( _fixedF.at(*it) ) continue;
      
      ::MVertex const * mv_omInit = _omInit.at(*it);
      dt__throwIf(
        floatHandling::isSmall(dtLinearAlgebra::length(nF.at(*it))), 
        operator()
      );

      //
      // get "two-ring" neighbor faces
      //
      std::vector< omFaceH > neighborFace;
      dt__forFromToIter(
        omConstVertexVertexI, 
        _omMoved.cvv_begin(*it), 
        _omMoved.cvv_end(*it), 
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
          _omMoved.cvf_begin(*vvIt), 
          _omMoved.cvf_end(*vvIt), 
          fIt
        ) if ( !_omMoved.contains(*fIt, *it) ) neighborFace.push_back(*fIt);
//					}	
      }
      progHelper::removeBastardTwins(neighborFace);

      //
      // check for intersections between target mesh vertex 
      // and neighbor faces
      //
      dtPoint3 start(mv_omInit->x(), mv_omInit->y(), mv_omInit->z());
      for (int ii=0;ii<_nShrinkingSteps;ii++) {
        _tF.laplacianSmooth();          
        dtPoint3 target = start + _tF.at(*it)*nF.at(*it);
        if ( !_omMoved.intersection(neighborFace, start, target) ) break;
        _tF[*it] = .95*_tF.at(*it);
      }
      if (_tF.at(*it) != _thickness) {
        dt__info(
          operator(), 
          << "Intersection => shrink t = " << _thickness << " -> " << _tF[*it]
        );
      }

      //
      // move vertices of surface mesh om and create new vertices with old
      // position in new surface mesh omT
      //
      dtPoint3 target 
      = 
      dtGmshModel::extractPosition(_omInit.at(*it)) 
      + 
      _tF.at(*it) * nF.at(*it);
      _omMoved.replacePosition(*it, target);
		}		
	}
	
	void dtGmshMeshBoundaryLayer::modifyGEntities(dtGmshModel * wM) {
		typedef std::map< ::GEntity *, ::GEntity * > GEntGEnt_t;	
		GEntGEnt_t newOld;
    //
    // create new GEntitys and store in newOld mapping
    //
		dt__forFromToIter(
      omConstVertexI, _omInit.vertices_begin(), _omInit.vertices_end(), it
    ) {
      // keep fixed vertex unchanged
			if ( _fixedF.at(*it) ) continue;
      
      // get underlying GEntity
      GEntity * ge = _omInit.at(*it)->onWhat();
      
      // do not modify already modified GEntity
      if ( newOld.find(ge) != newOld.end() ) continue;
      
      // GVertex
      if (ge->dim() == 0) {
        dtGmshVertex * nV = new dtGmshVertex(wM, wM->getMaxVertexTag()+1);
        wM->add(nV);						
        newOld[ ge ] = nV;
      }
      // GEdge
      else if (ge->dim() == 1) {
        dtGmshEdge * nE = new dtGmshEdge(wM, wM->getMaxEdgeTag()+1);
        wM->add(nE);
        newOld[ ge ] = nE;
        nE->meshStatistics.status = GEntity::MeshGenerationStatus::DONE;
      }
      // GFace
      else if (ge->dim() == 2) {
        dt__ptrAss(dtGmshFace * gf, dtGmshFace::DownCast(ge));
        dtGmshFace * nF = new dtGmshFace(wM, wM->getMaxFaceTag()+1);
        wM->add(nF);						
        newOld[gf] = nF;
        std::string pName = wM->getPhysicalString(gf);
        wM->untagPhysical(gf);
        wM->tagPhysical(nF, pName);
        nF->meshStatistics.status = GEntity::MeshGenerationStatus::DONE;
      }
      // GRegion
      else if (ge->dim() == 3) {
        dtGmshRegion * nR = new dtGmshRegion(wM, wM->getMaxRegionTag()+1);
        wM->add(nR);
        newOld[ ge ] = nR;
        nR->_status = GEntity::MeshGenerationStatus::DONE;					
      }
      else dt__throwUnexpected(modifyGEntities());
		}
    
    dt__info(modifyGEntities(),
      << logMe::stringPtrVec( 
        dtGmshModel::cast2DtGmshFace(wM->faces()), 
        &dtGmshFace::getPhysicalString,
        1
      )
    );
    
		//
		// find relationships
		//		
		dt__forAllIter(GEntGEnt_t, newOld, it) {
			::GEntity * oldGE = it->first;
			::GEntity * newGE = it->second;
			
      // GVertex
			if (oldGE->dim() == 0) {
			  dt__ptrAss(dtGmshVertex * gv, dtGmshVertex::DownCast(oldGE));        
        dt__ptrAss(dtGmshVertex * gvn, dtGmshVertex::DownCast(newGE));
				std::list< ::GEdge * > ee = gv->edges();
				dt__forAllIter(std::list< ::GEdge * >, ee, eIt) {
					if (newOld.find(*eIt) != newOld.end()) gvn->addGEntity(newOld[*eIt]);
					else gvn->addGEntity(*eIt);
				}
			}
      // GEdge
			else if (oldGE->dim() == 1) {
        dt__ptrAss(dtGmshEdge * ge, dtGmshEdge::DownCast(oldGE));
        dt__ptrAss(dtGmshEdge * gen, dtGmshEdge::DownCast(newGE));
				std::list< ::GVertex * > vv = ge->vertices();
				dt__forAllIter(std::list< ::GVertex * >, vv, vIt) {
					if (newOld.find(*vIt) != newOld.end()) gen->addGEntity(newOld[*vIt]);
					else gen->addGEntity(*vIt);
				}
				std::list< ::GFace * > ff = ge->faces();
				dt__forAllIter(std::list< ::GFace * >, ff, fIt) {
					if (newOld.find(*fIt) != newOld.end()) gen->addGEntity(newOld[*fIt]);
					else gen->addGEntity(*fIt);
				}				
			}
      // GFace
			else if (oldGE->dim() == 2) {
			  dt__ptrAss(dtGmshFace * gf, dtGmshFace::DownCast(oldGE));        
        dt__ptrAss(dtGmshFace * gfn, dtGmshFace::DownCast(newGE));
				std::list< ::GEdge * > ee = gf->edges();
				dt__forAllIter(std::list< ::GEdge * >, ee, eIt) {
					if (newOld.find(*eIt) != newOld.end()) gfn->addGEntity(newOld[*eIt]);
					else gfn->addGEntity(*eIt);
				}
//				std::list< ::GRegion * > rr = gf->regions();
//				dt__forAllIter(std::list< ::GRegion * >, rr, rIt) {
//					if (newOld.find(*rIt) != newOld.end()) {
//						gfn->addGEntity( newOld[*rIt] );
//						gf->delRegion( dtGmshRegion::SecureCast(newOld[*rIt]) );
//					}
//					else {
//						gfn->addGEntity(*rIt);
//						gf->delRegion(*rIt);
//					}
//				}
			}
			else dt__throwUnexpected(modifyGEntities());
		}
    
		//
		// create new vertices
		//
		dt__forFromToIter(
      omVertexI, _omInit.vertices_begin(), _omInit.vertices_end(), it
    ) {
      // keep fixed vertex unchanged
			if ( _fixedF.at(*it) ) continue;
      
      ::MVertex const * const mv = _omInit.at(*it);
      ::MVertex * mvNew 
      = 
      new ::MVertex(mv->x(), mv->y(), mv->z(), newOld[mv->onWhat()]);

      if ( !_slidableF.at(*it) ) _omInit.replaceMVertex(*it, mvNew);
      else _omMoved.replaceMVertex(*it, mvNew);
      
      newOld[mv->onWhat()]->addMeshVertex(mvNew);
		}		
		
		//
		// create new elements
		//
		dt__forFromToIter(
      omFaceI, _omInit.faces_begin(), _omInit.faces_end(), fIt
    ) {
			std::vector< ::MVertex * > vertices;
			dt__forFromToIter(
        omFaceVertexI, _omInit.fv_begin(*fIt), _omInit.fv_end(*fIt), vIt
      ) vertices.push_back( _omInit[*vIt] );
      
      ::MElement * anEl;
      if (vertices.size() == 3) anEl = new MTriangle(vertices);
      else if (vertices.size() == 4) anEl = new MQuadrangle(vertices);
      else dt__throw(modifyGEntities(), << dt__eval(vertices.size()));
      
			_omInit.replaceMElement(*fIt, anEl);
      dt__ptrAss(
        dtGmshFace * gf,
        dtGmshFace::DownCast( 
          wM->getDtGmshFaceByPhysical( _physicalName.at(*fIt) ) 
        )
      );
      gf->addElement( anEl );
		}		
	}
	
	void dtGmshMeshBoundaryLayer::dihedralAngleIntersection(
		dtOMEdgeField< float > const & dAF, dtOMVertexField< dtVector3 > const & nF
	) {
		dt__forFromToIter(
      omConstVertexI, _omMoved.vertices_begin(), _omMoved.vertices_end(), vIt
    ) {
			std::vector< omEdgeH > oneRing = _omMoved.oneRingEdgeH(*vIt);
			float min = 0.;
			float max = 0.;
			dt__forAllIter(std::vector< omEdgeH >, oneRing, eIt) {
				float const & dA = dAF.at(*eIt);
				min = std::min(dA, min);
				max = std::max(dA, max);
			}
			if ( fabs(max - min) > M_PI ) {
//				dABoolF[*vIt] = true;
			  dt__info(
				  operator(), 
					<< "Possible dihedral angle problem detected." << std::endl
				  << dt__eval(min) << std::endl
				  << dt__eval(max) << std::endl
				  << "|max-min| = " << max-min
				);
							
				//
				// move vertices of surface mesh om and create new vertices with old
				// position in new surface mesh omMoved
				//
				_tF[*vIt] = .5*_tF[*vIt];
				dtPoint3 target 
				= 
				dtGmshModel::extractPosition(_omInit.at(*vIt)) 
        + 
        _tF.at(*vIt) * nF.at(*vIt);
				_omMoved.replacePosition(*vIt, target);				

				//
				// move vertices of surface mesh om and create new vertices with old
				// position in new surface mesh omT
				//
				dt__forFromToIter(
          omConstVertexVertexI, 
          _omMoved.vv_begin(*vIt), 
          _omMoved.vv_end(*vIt), 
          vvIt
        ) {
					if ( !_fixedF.at(*vIt) ) {
						_tF[*vIt] = .75*_tF.at(*vvIt);
						dtPoint3 target 
						= 
						dtGmshModel::extractPosition(_omInit.at(*vvIt)) 
						+ _tF.at(*vvIt) * nF.at(*vvIt);
						_omMoved.replacePosition(*vvIt, target);
					}
				}
				
			}			
//			omVertexH const vH = dAF[*vIt];
//		  float dA = omMoved.calc_dihedral_angle(*eIt);
		}		
	}
	
	void dtGmshMeshBoundaryLayer::createBoundaryLayerElements(
		std::vector< ::MVertex * > & vertex, std::vector< ::MElement * > & element        
	) const {
		int nLayers = _spacing.size()+1;
		dtOMVertexField< std::vector< ::MVertex * > > mvBLayerF(
		  "mvBLayerF", _omInit, std::vector< ::MVertex * >(nLayers-1, NULL)
		);		
		dt__forFromToIter(
      omFaceI, _omInit.faces_begin(), _omInit.faces_end(), fIt
    ) {
      //
      // handle only elements that should be extruded
      //
      if (!_extrudeF.at(*fIt) ) continue;
      
		  //
			// get fix and movable vertices			
			//
			std::vector< ::MVertex * > commonVertices;
			std::vector< ::MVertex * > omFixedVertices;
			std::vector< ::MVertex * > omMovedVertices;
			dt__forFromToIter(
			  omConstFaceVertexI, 
				_omInit.cfv_begin(*fIt), 
				_omInit.cfv_end(*fIt), vIt
			) {
				MVertex const * const mv_omInit = _omInit.at(*vIt);
				MVertex const * const mv_omMoved = _omMoved.at(*vIt);
				if ( _fixedF.at(*vIt) ) {
					commonVertices.push_back(const_cast< ::MVertex *>(mv_omMoved));
				}
				else {
					omFixedVertices.push_back( const_cast< ::MVertex *>(mv_omInit) );
					omMovedVertices.push_back( const_cast< ::MVertex *>(mv_omMoved) );
				}
			}
			
			// create two dimensional arrays of all mesh vertices
			twoDArrayHandling< ::MVertex * > omFixedVerticesL(nLayers, 0);
			twoDArrayHandling< ::MVertex * > omMovedVerticesL(nLayers, 0);			
			omFixedVerticesL[0] = omFixedVertices;
			omMovedVerticesL[nLayers-1] = omMovedVertices;
			dt__forAllIndex(omMovedVertices, jj) {
				dtPoint3 mv0 = dtGmshModel::extractPosition(omFixedVertices[jj]);
				dtPoint3 mv1 = dtGmshModel::extractPosition(omMovedVertices[jj]);
				dtVector3 vv = mv1-mv0;
				dt__toFloat(float nLayersF, nLayers);
				for (int ii=1; ii<nLayers; ii++) {
					dt__toFloat(float iiF, ii);
					dtPoint3 mvXYZ = mv0 + _spacing[ii-1]*vv;					
					::MVertex * mv = mvBLayerF[omFixedVertices[jj]][ii-1];
					if (mv == NULL) {
						mv = new ::MVertex(0., 0., 0., NULL);
						mvBLayerF[omFixedVertices[jj]][ii-1] = mv;
  					vertex.push_back( mv );
					}
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
				    omFixedVerticesL[ii][0], 
            omFixedVerticesL[ii][1], 
            omFixedVerticesL[ii][2],
						omMovedVerticesL[ii][0], 
            omMovedVerticesL[ii][1], 
            omMovedVerticesL[ii][2]
					);
					if (pri->getVolumeSign()<0.) pri->reverse();
					element.push_back(pri);
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
					element.push_back(pyr);
				}
				else if (commonVertices.size() == 2) {
					::MTetrahedron * tet 
					= 
					new ::MTetrahedron(
						omMovedVerticesL[ii][0], omFixedVerticesL[ii][0], 
						commonVertices[0], commonVertices[1]
					);				
					if (tet->getVolumeSign()<0.) tet->reverse();
					element.push_back(tet);
				}
			}
		}		
	}
  
  bool dtGmshMeshBoundaryLayer::isSlidable( 
    dtOMVertexField< bool > const & canSlideF,
    omVertexH const & vH
  ) {
    //
    // check if vertex is slidable itself or there is at least one 
    // vertex in the one-ring-neighborhood that is
    //
    if ( !canSlideF.at(vH) ) return false;
    
    std::vector< bool > localSlide;
    dt__forFromToIter(
      omConstVertexVertexI, 
      canSlideF.refMesh().cvv_begin(vH), 
      canSlideF.refMesh().cvv_end(vH), 
      vvIt
    ) {
      bool isSliding = canSlideF.at(*vvIt);
      dt__forAllConstIter(std::vector< bool >, localSlide, it) {
        if (*it != isSliding) return true;
      }
      localSlide.push_back( isSliding );
    }
    
    return false;
  }
}