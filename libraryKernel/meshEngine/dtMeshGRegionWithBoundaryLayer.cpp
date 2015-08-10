#include "dtMeshGRegionWithBoundaryLayer.h"

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

#include <xmlHeaven/qtXmlBase.h>
#include <interfaceHeaven/floatHandling.h>
#include <interfaceHeaven/stringPrimitive.h>

#include <gmsh/MTriangle.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/MPrism.h>
#include <gmsh/MPyramid.h>
#include <gmsh/MTetrahedron.h>

namespace dtOO {
  int dtMeshGRegionWithBoundaryLayer::_NORMAL = 1;
  int dtMeshGRegionWithBoundaryLayer::_SLIDER = 2;
  int dtMeshGRegionWithBoundaryLayer::_FIXER = 3;
  
  dtMeshGRegionWithBoundaryLayer::dtMeshGRegionWithBoundaryLayer(
  ) 
  : 
  _thickness(0.),
  _nSmoothingSteps(0),
  _nShrinkingSteps(0),
  _maxDihedralAngle(0.),
  _spacing(0,0.),  
  _omInit(),
  _omMoved(),    
  _fixedF("_fixedF", _omInit, false),
  _slidableF("_slidableF", _omInit, false),
  _tF("_tF", _omInit, _thickness),
  _extrudeF("_extrudeF", _omInit, 1),
  _typeF("_type", _omInit, 0),
  _buddyF("_buddyF", _omInit, std::vector< ::MVertex * >(0,NULL)) {
    _3D = NULL;
	}

	dtMeshGRegionWithBoundaryLayer::~dtMeshGRegionWithBoundaryLayer() {
    
	}
	
  void dtMeshGRegionWithBoundaryLayer::init(
    ::QDomElement const & element,
    baseContainer const * const bC,
    vectorHandling< constValue * > const * const cV,
    vectorHandling< analyticFunction * > const * const aF,
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< boundedVolume * > const * const bV,
    vectorHandling< dtMeshOperator * > const * const mO
  ) {
    dtMesh3DOperator::init(element, bC, cV, aF, aG, bV, mO);
    
    _spacing 
		= 
		qtXmlBase::getAttributeFloatVectorMuParse("spacing", element, cV, aF);		
    _thickness 
		= 
		qtXmlBase::getAttributeFloatMuParse("thickness", element, cV, aF);
		_nSmoothingSteps 
		= 
		qtXmlBase::getAttributeIntMuParse("nSmoothingSteps", element, cV, aF);		
		_nShrinkingSteps 
		=
		qtXmlBase::getAttributeIntMuParse("nShrinkingSteps", element, cV, aF);		
		_maxDihedralAngle 
		= 
		qtXmlBase::getAttributeFloatMuParse("maxDihedralAngle", element, cV, aF);		
		
		//
		// boundedVolume
		//		
		_faceLabel 
		= 
		qtXmlPrimitive::getAttributeStrVector("faceLabel", element);
		_faceOrientation 
		= 
		qtXmlBase::getAttributeIntVectorMuParse("faceOrientation", element, cV, aF);
		_fixedFaceLabel 
		= 
		qtXmlPrimitive::getAttributeStrVector("fixedFaceLabel", element);
		_fixedFaceOrientation 
		= 
		qtXmlBase::getAttributeIntVectorMuParse(
      "fixedFaceOrientation", element, cV, aF
    );
		_slidableFaceLabel 
		= 
		qtXmlPrimitive::getAttributeStrVector("slidableFaceLabel", element);
		_slidableFaceOrientation 
		= 
		qtXmlBase::getAttributeIntVectorMuParse(
      "slidableFaceOrientation", element, cV, aF
    );    
    
    dt__ptrAss(
      _3D, 
      dtMesh3DOperator::DownCast(
        mO->get(qtXmlBase::getAttributeStr("dtMesh3DOperator", element))
      )
    );
	}
	
  void dtMeshGRegionWithBoundaryLayer::operator() (dtGmshRegion * dtgr) {
		//
		// init faces and regions
		//
		std::list< dtGmshFace const * > faceList = dtgr->constFaceList(_faceLabel);
		std::list< dtGmshFace const * > fixedFaceList 
    = 
    dtgr->constFaceList(_fixedFaceLabel);
		std::list< dtGmshFace const * > slidableFaceList 
    = 
    dtgr->constFaceList(_slidableFaceLabel);    
		
		dt__info(
			postUpdate(), 
			<< "Adding elements to region with label " 
			<< dtgr->refDtGmshModel().getPhysicalString(dtgr)
		);
				
    //
    // delete mesh in region
    //
    dtgr->deleteMesh();
    
    dtOMDynamicVertexField< bool > canSlideF("canSlideF", _omInit, false);
		
    //
    // add different surface meshes and initialize fields
    //
		// normal surface
		int itC = 0;
		dt__forAllConstIter( std::list< dtGmshFace const * >, faceList, it ) {
			dtGmshFace const * const &thisFace = *it;

		  dt__info(
			  dtMeshGRegionWithBoundaryLayer(), 
        << "Normal face tag = " << thisFace->tag() << "." << std::endl
				<< dt__eval(_faceOrientation[itC]) 
      );   
        
			dt__pH(dtOMMesh) tmpOM(thisFace->getOMMesh());      
			if (_faceOrientation[itC] > 0) {     
				_omInit.add(*tmpOM);		
				_omMoved.add(*tmpOM);
			}
			else if (_faceOrientation[itC] < 0) {
				_omInit.addInv(*tmpOM);
				_omMoved.addInv(*tmpOM);		
			}
			else dt__throwUnexpected(dtMeshGRegionWithBoundaryLayer());
			
			itC++;
      
			// set fields
			_tF.assign(*tmpOM, _thickness);       
			_typeF.assign(*tmpOM, dtMeshGRegionWithBoundaryLayer::_NORMAL);
		}
		
		// slidable surface
		itC = 0;
		dt__forAllConstIter( 
      std::list< dtGmshFace const * >, slidableFaceList, it 
    ) {
			dtGmshFace const * const &thisFace = *it;

		  dt__info(
			  dtMeshGRegionWithBoundaryLayer(), 
        << "Sliding face tag = " << thisFace->tag() << "." << std::endl
				<< dt__eval(_slidableFaceOrientation[itC]) 
      );   

      
			dt__pH(dtOMMesh) tmpOM(thisFace->getOMMesh());
			
			if (_slidableFaceOrientation[itC] > 0) {
				_omInit.add(*tmpOM);						
				_omMoved.add(*tmpOM);		
			}
			else if (_slidableFaceOrientation[itC] < 0) {
				_omInit.addInv(*tmpOM);						
				_omMoved.addInv(*tmpOM);		
			}
			else dt__throwUnexpected(dtMeshGRegionWithBoundaryLayer());
      
			itC++;
			
			// set fields
			_fixedF.assign(*tmpOM, true);
			_tF.assign(*tmpOM, _thickness);      
      canSlideF.assign(*tmpOM, true);
      _extrudeF.assign(*tmpOM, 0);
      _typeF.assign(*tmpOM, dtMeshGRegionWithBoundaryLayer::_SLIDER);
		}
    
		// fixed surface
		itC = 0;
		dt__forAllConstIter( std::list< dtGmshFace const * >, fixedFaceList, it ) {
			dtGmshFace const * const &thisFace = *it;

		  dt__info(
			  dtMeshGRegionWithBoundaryLayer(), 
        << "Fixed face tag = " << thisFace->tag() << "." << std::endl
				<< dt__eval(_fixedFaceOrientation[itC]) 
      );   

      
			dt__pH(dtOMMesh) tmpOM(thisFace->getOMMesh());
			
			if (_fixedFaceOrientation[itC] > 0) {
				_omInit.add(*tmpOM);						
				_omMoved.add(*tmpOM);		
			}
			else if (_fixedFaceOrientation[itC] < 0) {
				_omInit.addInv(*tmpOM);
				_omMoved.addInv(*tmpOM);
			}
			else dt__throwUnexpected(dtMeshGRegionWithBoundaryLayer());
      
			itC++;
			
			// set fields
			_fixedF.assign(*tmpOM, true);
			_tF.assign(*tmpOM, 0.);
      _extrudeF.assign(*tmpOM, 0);
      _typeF.assign(*tmpOM, dtMeshGRegionWithBoundaryLayer::_FIXER);
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
      // find slidable faces and mark them with 2
      int slider = 0;
      int nCanSlider = 0;
			dt__forFromToIter(
			  omConstFaceVertexI,_omInit.cfv_begin(*fIt), _omInit.cfv_end(*fIt), vIt
			) {
        if (_slidableF.at(*vIt)) slider++;
        if (canSlideF.at(*vIt)) nCanSlider++;
      }

      //
      // slidable face is extrudable and has sliders
      //
			if ( (slider>0) && _extrudeF.at(*fIt) ) _extrudeF[*fIt] = 2;        
      
      if ( 
        ( (nCanSlider == _omInit.nVertices(*fIt)) ) && (_typeF.at(*fIt) == 1) 
      ) {
        _extrudeF[*fIt] = 2;        
        dt__forFromToIter(
          omConstFaceVertexI, 
          _omInit.cfv_begin(*fIt), 
          _omInit.cfv_end(*fIt), 
          vIt
        ) {
          _slidableF[*vIt] = true;
          _fixedF[*vIt] = false;
        }
      }
		}
    
//		//
//		// write fields
//		//
//		dtMoabCore mb0(_tF.refMesh());
//		mb0.addVertexField(_fixedF);
//    mb0.addVertexField(_slidableF);
//    mb0.addFaceField(_extrudeF);
//    mb0.addFaceField(_typeF);
//    mb0.addVertexField(canSlideF);
//		mb0.write_mesh("dtMeshGRegionWithBoundaryLayer_init_0.vtk");

    std::vector< ::MVertex * > vertex;
    std::vector< ::MElement * > element;
    
		dt__info(operator(), << "Initialized");

		//
		// create mesh manifolds
		//
		std::vector< dtOMMeshManifold > omManifolds;		
		dt__forFromToIter(
		  omVertexI, _omInit.vertices_begin(), _omInit.vertices_end(), v_it
		) omManifolds.push_back( dtOMMeshManifold(_omInit, *v_it) );

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
    
//		//
//		// write fields
//		//
//		dtMoabCore mb1(_tF.refMesh());
//		mb1.addVertexField(_tF);
//		mb1.addVertexField(nF);
//		mb1.write_mesh("dtMeshGRegionWithBoundaryLayer_init_1.vtk");
//
//    dtMoabCore(_omMoved).write_mesh("dtMeshGRegionWithBoundaryLayer_moved_1.vtk");
//		dt__info(operator(), << "Fields written.");
    
		//
		// add elements and vertices to commonReg
		//
    dtgr->mesh_vertices.reserve(vertex.size());
		dt__forAllIter(std::vector< ::MVertex * >, vertex, vIt) {
			(*vIt)->setEntity(dtgr);
			dtgr->addMeshVertex(*vIt);
		}
		dt__forAllIter(std::vector< ::MElement * >, element, eIt) {
      dtgr->addElement(*eIt);
    }
    
    //
    // mark as meshed
    //
    dtgr->_status = ::GEntity::MeshGenerationStatus::DONE;
  }
  
	void dtMeshGRegionWithBoundaryLayer::createLayerVertices(
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
  
  void dtMeshGRegionWithBoundaryLayer::createMovedMesh( void ) {
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
  
	void dtMeshGRegionWithBoundaryLayer::createBoundaryLayerElements(
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
  
  bool dtMeshGRegionWithBoundaryLayer::isSlidable( 
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
  
  omHalfedgeH dtMeshGRegionWithBoundaryLayer::slidableHalfedgeInFace( 
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
        
  
  void dtMeshGRegionWithBoundaryLayer::createRingOfBuddies( 
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
      dt__forFromToIndex(0, _spacing.size(), ii) {
        // mark halfedge
        _omInit.data(
          _omInit.next_halfedge_handle(wHe)
        ).mark();
        
        _buddyF[ 
          _omInit[ _omInit.to_vertex_handle(slidableHe) ] 
        ].push_back(
          _omInit[ 
            _omInit.to_vertex_handle( _omInit.next_halfedge_handle(wHe) ) 
          ]
        );
        
        dt__throwIf( 
          _typeF.at(_omInit.face_handle(wHe))!=_SLIDER, 
          createRingOfBuddies() 
        );
        wHe = _omInit.sameHalfedgeInNextFace(wHe);        
      }
      //
      // mark last halfedge and add last buddy
      //
      _omInit.data(
        _omInit.next_halfedge_handle(wHe)
      ).mark();      
      _buddyF[ 
        _omInit[ _omInit.to_vertex_handle(slidableHe) ] 
      ].push_back(
        _omInit[ 
          _omInit.to_vertex_handle( _omInit.next_halfedge_handle(wHe) ) 
        ]
      );
        
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
  
  void dtMeshGRegionWithBoundaryLayer::meshWithGmsh(
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

    (*_3D)( gr );
    
    element.reserve(gr->getNumMeshElements());
    dt__forFromToIndex(0, gr->getNumMeshElements(), ii) {
      element.push_back( gr->getMeshElement(ii) );
    }
    
    vertex.reserve(gr->getNumMeshVertices());
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