#include "dtGmshMeshBoundaryLayer.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>

#include "dtGmshFace.h"
#include "dtOMMeshManifold.h"
#include "dtMoabCore.h"
#include "dtGmshVertex.h"
#include "dtGmshEdge.h"
#include "dtGmshFace.h"
#include "dtGmshRegion.h"
#include "dtGmshModel.h"
#include <meshEngine/dtMeshGRegion.h>
#include <interfaceHeaven/floatHandling.h>
#include <interfaceHeaven/stringPrimitive.h>

#include <gmsh/MTriangle.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/MPrism.h>
#include <gmsh/MPyramid.h>
#include <gmsh/MTetrahedron.h>

namespace dtOO {
  int dtGmshMeshBoundaryLayer::_NORMAL = 1;
  int dtGmshMeshBoundaryLayer::_SLIDER = 2;
  int dtGmshMeshBoundaryLayer::_FIXER = 3;
  
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
      _thickness(thickness),
      _spacing(spacing),
      _nSmoothingSteps(nSmoothingSteps),
      _nShrinkingSteps(nShrinkingSteps),
      _maxDihedralAngle(maxDihedralAngle),
      _typeF("_type", _omInit, 0),
      _buddyF("_buddyF", _omInit, std::vector< ::MVertex * >(0,NULL)) {
    dtOMDynamicVertexField< bool > canSlideF("canSlideF", _omInit, false);
		
    //
    // add different surface meshes and initialize fields
    //
		// normal surface
		int itC = 0;
		dt__forAllConstIter( std::list< dtGmshFace const * >, face, it ) {
			dtGmshFace const * const &thisFace = *it;

		  dt__info(
			  dtGmshMeshBoundaryLayer(), 
        << "Normal face tag = " << thisFace->tag() << "." << std::endl
				<< dt__eval(ori[itC]) 
      );   
        
			dt__pH(dtOMMesh) tmpOM(thisFace->getOMMesh());      
			if (ori[itC] > 0) {     
				_omInit.add(*tmpOM);		
				_omMoved.add(*tmpOM);
			}
			else if (ori[itC] < 0) {
				_omInit.addInv(*tmpOM);
				_omMoved.addInv(*tmpOM);		
			}
			else dt__throwUnexpected(dtGmshMeshBoundaryLayer());
			
			itC++;
      
			// set fields
			_typeF.assign(*tmpOM, dtGmshMeshBoundaryLayer::_NORMAL);
		}
		
		// slidable surface
		itC = 0;
		dt__forAllConstIter( std::list< dtGmshFace const * >, mface, it ) {
			dtGmshFace const * const &thisFace = *it;

		  dt__info(
			  dtGmshMeshBoundaryLayer(), 
        << "Sliding face tag = " << thisFace->tag() << "." << std::endl
				<< dt__eval(ori[itC]) 
      );   

      
			dt__pH(dtOMMesh) tmpOM(thisFace->getOMMesh());
			
			if (mori[itC] > 0) {
				_omInit.add(*tmpOM);						
				_omMoved.add(*tmpOM);		
			}
			else if (mori[itC] < 0) {
				_omInit.addInv(*tmpOM);						
				_omMoved.addInv(*tmpOM);		
			}
			else dt__throwUnexpected(dtGmshMeshBoundaryLayer());
      
			itC++;
			
			// set fields
			_fixedF.assign(*tmpOM, true);
      canSlideF.assign(*tmpOM, true);
      _extrudeF.assign(*tmpOM, 0);
      _typeF.assign(*tmpOM, dtGmshMeshBoundaryLayer::_SLIDER);
		}
    
		// fixed surface
		itC = 0;
		dt__forAllConstIter( std::list< dtGmshFace const * >, fface, it ) {
			dtGmshFace const * const &thisFace = *it;

		  dt__info(
			  dtGmshMeshBoundaryLayer(), 
        << "Fixed face tag = " << thisFace->tag() << "." << std::endl
				<< dt__eval(ori[itC]) 
      );   

      
			dt__pH(dtOMMesh) tmpOM(thisFace->getOMMesh());
			
			if (fori[itC] > 0) {
				_omInit.add(*tmpOM);						
				_omMoved.add(*tmpOM);		
			}
			else if (fori[itC] < 0) {
				_omInit.addInv(*tmpOM);
				_omMoved.addInv(*tmpOM);
			}
			else dt__throwUnexpected(dtGmshMeshBoundaryLayer());
      
			itC++;
			
			// set fields
			_fixedF.assign(*tmpOM, true);
			_tF.assign(*tmpOM, 0.);
      _extrudeF.assign(*tmpOM, 0);
      _typeF.assign(*tmpOM, dtGmshMeshBoundaryLayer::_FIXER);
		}

    //
    // update mesh and fields
    //
		_omInit.update();
		_omMoved.update();    
    
    
    // vertex fields
		dt__forFromToIter(
      omVertexI, _omInit.vertices_begin(), _omInit.vertices_end(), it
    ) {
      //
      // detect sliders
      //      
      if ( isSlidable(canSlideF, *it) ) {
        _slidableF[*it] = true;
        _fixedF[*it] = false;
      }
      
      //
      // fix boundary nodes
      //      
			if ( _omInit.is_boundary(*it) ) {
				_fixedF[*it] = true;
				_tF[*it] = 0.;
			}
		}
        
    // face fields
		dt__forFromToIter(
      omFaceI, _omInit.faces_begin(), _omInit.faces_end(), fIt
    ) {
      //
      // find slidable faces and mark them with 2
      //      
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
		// createLayerVertices
		//
		createLayerVertices(nF);
		dt__info(operator(), << "Layer vertices created.");
		
		//
		// createMovedMesh
		//
		createMovedMesh();
		dt__info(operator(), << "Moved mesh created.");
    
		//
		// create new boundary layer elements
		//
		createBoundaryLayerElements(vertex, element);
    dt__info(operator(), << "Elements created");

		//
		// create internal unstructured mesh
		//		
    meshWithGmsh(_omMoved, vertex, element);
    
		//
		// write fields
		//
		dtMoabCore mb(_tF.refMesh());
		mb.addVertexField(_tF);
		mb.addVertexField(nF);
		mb.addVertexField(_fixedF);
    mb.addVertexField(_slidableF);
    mb.addFaceField(_extrudeF);
    mb.addFaceField(_typeF);
		mb.write_mesh("dtGmshMeshBoundaryLayer_init.vtk");

    dtMoabCore(_omMoved).write_mesh("dtGmshMeshBoundaryLayer_moved.vtk");
    
		dt__info(operator(), << "Fields written.");
		dt__info(operator(), << "Done.");
  }
  
	void dtGmshMeshBoundaryLayer::createLayerVertices(
		dtOMVertexField< dtVector3 > const & nF
	) {
		//
		// create new vertices
		//
		dt__forFromToIter(
      omVertexI, _omInit.vertices_begin(), _omInit.vertices_end(), it
    ) {
      // keep fixed and already modified vertex unchanged
			if ( _fixedF.at(*it) || !(_buddyF.at(*it).empty()) ) continue;

      if ( _slidableF.at(*it) ) {
        createRingOfBuddies(_omInit.at(*it));
      }
      else {
        ::MVertex const * const mv = _omInit.at(*it);
        _buddyF[*it].push_back( 
          new ::MVertex( mv->x(), mv->y(), mv->z() ) 
        );     
      }
		}
    
		dt__forFromToIter(
      omConstVertexI, _omInit.vertices_begin(), _omInit.vertices_end(), it
    ) {
      // keep fixed or slidable vertex unchanged
			if ( _fixedF.at(*it) || _slidableF.at(*it) ) continue;      

      //
      // move vertices of surface mesh om and create new vertices with old
      // position in new surface mesh omT
      //      
      ::MVertex * mv = _buddyF[*it].front();
      _buddyF[*it].pop_back();
      
      dt__forAllIndex(_spacing, ii) {
        dtPoint3 newPos
        =
        dtGmshModel::extractPosition(mv) 
        + 
        _spacing.at(ii) * _tF.at(*it) * nF.at(*it);
        _buddyF[*it].push_back(
          new ::MVertex( newPos.x(), newPos.y(), newPos.z() )
        );
      }
      dtPoint3 newPos
      =
      dtGmshModel::extractPosition(mv) 
      + 
      _tF.at(*it) * nF.at(*it);
      _buddyF[*it].push_back(
        new ::MVertex( newPos.x(), newPos.y(), newPos.z() )
      );       
		}	 
	}
  
  void dtGmshMeshBoundaryLayer::createMovedMesh( void ) {
		dt__forFromToIter(
      omConstVertexI, _omInit.vertices_begin(), _omInit.vertices_end(), it
    ) {
      if ( !(_buddyF.at(*it).empty()) ) {
        _omMoved.replaceMVertex(*it, _buddyF.at(*it).back());
      }
    }
    
    dt__forFromToIter(
      omHalfedgeI, _omInit.halfedges_begin(), _omInit.halfedges_end(), it
    ) {
      if ( _omInit.data(*it).marked() ) {
        dt__throwIf( !_omMoved.is_collapse_ok(*it), createLayerVertices() );
        _omMoved.collapse(*it);
      }
    }
    _omMoved.garbage_collection();    
  }
  
	void dtGmshMeshBoundaryLayer::createBoundaryLayerElements(
		std::vector< ::MVertex * > & vertex, std::vector< ::MElement * > & element        
	) const {
		dt__forFromToIter(
      omFaceI, _omInit.faces_begin(), _omInit.faces_end(), fIt
    ) {
      // handle only elements that should be extruded
      if (!_extrudeF.at(*fIt) ) continue;
      
			// create elements and add it to given region
      dt__forFromToIndex(0, _spacing.size()+1, ii) {
        std::vector< ::MVertex * > fixedVertices;
        std::vector< ::MVertex * > movedVertices;		      
        std::vector< ::MVertex * > commonVertices;		              
        dt__forFromToIter(
          omConstFaceVertexI, 
          _omInit.cfv_begin(*fIt), 
          _omInit.cfv_end(*fIt), 
          vIt
        ) {
          if ( _fixedF.at(*vIt) ) {
            commonVertices.push_back( 
              const_cast< ::MVertex * >(_omInit.at(*vIt)) 
            );
          }
          else {
            if (ii==0) {
              fixedVertices.push_back( 
                const_cast< ::MVertex * >(_omInit.at(*vIt)) 
              );
            }
            else {
              fixedVertices.push_back( 
                const_cast< ::MVertex * >(_buddyF.at(*vIt).at(ii-1)) 
              );
              if ( !_slidableF.at(*vIt) ) {
                vertex.push_back( fixedVertices.back() );
              }
            }
            movedVertices.push_back( 
              const_cast< ::MVertex * >(_buddyF.at(*vIt).at(ii)) 
            );
            if ( !_slidableF.at(*vIt) ) {
              vertex.push_back( movedVertices.back() );
            }            
          }
        }
              
				if (commonVertices.size() == 0) {
					::MPrism * pri 
					= 
					new ::MPrism(
				    fixedVertices[0], 
            fixedVertices[1], 
            fixedVertices[2],
						movedVertices[0], 
            movedVertices[1], 
            movedVertices[2]
					);
					if (pri->getVolumeSign()<0.) pri->reverse();
					element.push_back(pri);
				}
				else if (commonVertices.size() == 1) {
					::MPyramid * pyr 
					= 
					new ::MPyramid(
						movedVertices[1], movedVertices[0], 
						fixedVertices[0], fixedVertices[1], 
						commonVertices[0]
					);
					if (pyr->getVolumeSign()<0.) pyr->reverse();
					element.push_back(pyr);
				}
				else if (commonVertices.size() == 2) {
					::MTetrahedron * tet 
					= 
					new ::MTetrahedron(
						movedVertices[0], fixedVertices[0], 
						commonVertices[0], commonVertices[1]
					);				
					if (tet->getVolumeSign()<0.) tet->reverse();
					element.push_back(tet);
				}
      }
	  }	
    progHelper::removeBastardTwins( vertex );
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
  
  omHalfedgeH dtGmshMeshBoundaryLayer::slidableHalfedgeInFace( 
    omFaceH const & fH 
  ) const {
    dt__forFromToIter(
      omConstFaceHalfedgeI, _omInit.cfh_begin(fH), _omInit.cfh_end(fH), hIt
    ) {
      if ( 
           _slidableF.at(_omInit.from_vertex_handle(*hIt)) 
        && _slidableF.at(_omInit.to_vertex_handle(*hIt)) 
      ) return *hIt;
    }
    
    dt__throw(
      slidableHalfedgeInFace(), 
      << "No halfedge lies on slidable seam."
    );
  }
        
  
  void dtGmshMeshBoundaryLayer::createRingOfBuddies( 
    MVertex const * const mv0 
  ) {
    omFaceH face;
    dt__forFromToIter(
      omConstVertexFaceI, 
      _omInit.cvf_begin(_omInit.at(mv0)),
      _omInit.cvf_end(_omInit.at(mv0)),
      fIt
    ) {
      if (_typeF.at(*fIt) == _SLIDER ) {
        face = *fIt;
        break;
      }
    }
    
    dt__throwIf(!face.is_valid(), createRingOfBuddies());
    
    
    omHalfedgeH slidableHe_init = slidableHalfedgeInFace(face);
    
    omHalfedgeH slidableHe = slidableHe_init;
    do {
      omHalfedgeH wHe = slidableHe;
      dt__forFromToIndex(0, _spacing.size()+1, ii) {
        wHe = _omInit.sameHalfedgeInNextFace(wHe);
        
        // mark halfedge
        _omInit.data(
          _omInit.next_halfedge_handle(wHe)
        ).mark();
        
        _buddyF[ 
          _omInit[ _omInit.to_vertex_handle(slidableHe) ] 
        ].push_back(
          _omInit[ _omInit.to_vertex_handle( wHe ) ]
        );
        
        dt__throwIf( 
          _typeF.at(_omInit.face_handle(wHe))!=_SLIDER, 
          createRingOfBuddies() 
        );
      }
      
      slidableHe 
      = 
      _omInit.prev_halfedge_handle(  
        _omInit.sameHalfedgeInNextFace(
          _omInit.next_halfedge_handle(slidableHe)
        )
      );
    }
    while (slidableHe != slidableHe_init);
  }
  
  void dtGmshMeshBoundaryLayer::meshWithGmsh(
    dtOMMesh const & mesh, 
    std::vector< ::MVertex * > & vertex, 
    std::vector< ::MElement * > & element  
  ) const {
    dtGmshModel gm("myModel");

    dtGmshFace * gf = new dtGmshFace(&gm, 1);
    gm.add(gf);
    dtGmshRegion * gr = new dtGmshRegion(&gm, 1);
    gm.add(gr);
    
    dt__forFromToIter(
      omConstVertexI, mesh.vertices_begin(), mesh.vertices_end(), it
    ) {
      ::MVertex * mv = mesh.data(*it).MVertex();
      mv->setEntity(gf); 
      gf->addMeshVertex( mv );
    }
    
    dt__forFromToIter(omConstFaceI, mesh.faces_begin(), mesh.faces_end(), fit) {
      std::vector< ::MVertex * > vv;
      dt__forFromToIter(
        omConstFaceVertexI, mesh.cfv_begin(*fit), mesh.cfv_end(*fit), it
      ) vv.push_back( const_cast< ::MVertex * >(mesh.at(*it)) );
      if ( vv.size() == 3 ) {
        gf->addElement( new ::MTriangle(vv[0], vv[1], vv[2]) );
        if ( mesh.data(*fit).inverted() ) gf->triangles.back()->reverse();
      }
      else if ( vv.size() == 4 ) {
        gf->addElement( new ::MQuadrangle( vv[0], vv[1], vv[2], vv[3] ) );        
        if ( mesh.data(*fit).inverted() ) gf->quadrangles.back()->reverse();
      }      
      else dt__throw(meshWithGmsh(), << dt__eval(vv.size()));
    }
    gf->meshStatistics.status = ::GEntity::MeshGenerationStatus::DONE;

    gr->addFace( gf, 1 );
    gf->addRegion( gr );

    dtMeshGRegion()( gr );
    
    dt__forFromToIndex(0, gr->getNumMeshElements(), ii) {
      element.push_back( gr->getMeshElement(ii) );
    }
    
    dt__forFromToIndex(0, gr->getNumMeshVertices(), ii) {
      vertex.push_back( gr->getMeshVertex(ii) );
    }
    //
    // do not destroy currently created mesh elements
    //
    gr->tetrahedra.clear();
    gr->hexahedra.clear();
    gr->pyramids.clear();    
    gr->mesh_vertices.clear();
    
    gf->mesh_vertices.clear();
  }
}