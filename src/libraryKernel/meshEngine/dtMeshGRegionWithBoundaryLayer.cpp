#include "dtMeshGRegionWithBoundaryLayer.h"

#include <logMe/logMe.h>
#include <progHelper.h>

#include "dtGmshFace.h"
#include "dtOMMeshManifold.h"
#include "dtOMMeshDivided.h"
#include "dtMoabCore.h"
#include "dtGmshVertex.h"
#include "dtGmshEdge.h"
#include "dtGmshFace.h"
#include "dtGmshRegion.h"
#include "dtGmshModel.h"
#include <interfaceHeaven/optionHandling.h>

#include <xmlHeaven/qtXmlBase.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <interfaceHeaven/labeledVectorHandling.h>

#include <gmsh/MTriangle.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/MPrism.h>
#include <gmsh/MPyramid.h>
#include <gmsh/MTetrahedron.h>
#include <gmsh/Field.h>

namespace dtOO {
  int dtMeshGRegionWithBoundaryLayer::_NORMAL = 1;
  int dtMeshGRegionWithBoundaryLayer::_SLIDER = 2;
  int dtMeshGRegionWithBoundaryLayer::_FIXER = 3;
  
  dtMeshGRegionWithBoundaryLayer::dtMeshGRegionWithBoundaryLayer(
  ) 
  : 
  _nNormalSmoothingSteps(0),
  _nGrowingSmoothingSteps(0),
  _maxGrowingRatePerStep(1.0),
  _maxDihedralAngle(0.),
  _nSpacingSteps(std::vector< int >(0, 0)),
  _omInit(),
  _omMoved(),    
  _fixedF("_fixedF", _omInit, false),
  _slidableF("_slidableF", _omInit, false),
  _tF("_tF", _omInit, 0.),
  _nF("nF", _omInit, dtVector3(0.,0.,0.)),	
  _faceIndex("faceIndex", _omInit, -1),	          
  _extrudeF("_extrudeF", _omInit, 1),
  _typeF("_type", _omInit, 0),
  _nSpacingStepsF("nSpacingStepsF", _omInit, 0),
  _flipNormalF("_flipNormalF", _omInit, false),
  _buddyF("_buddyF", _omInit, std::vector< ::MVertex * >(0,NULL)),
  _realSpacing("_realSpacing", _omInit, std::vector< float >(0,0.)),
  _3D(NULL)
  {
	}

	dtMeshGRegionWithBoundaryLayer::~dtMeshGRegionWithBoundaryLayer() {
    
	}
	
  void dtMeshGRegionWithBoundaryLayer::init(
    ::QDomElement const & element,
    baseContainer const * const bC,
    cVPtrVec const * const cV,
    aFPtrVec const * const aF,
    aGPtrVec const * const aG,
    bVPtrVec const * const bV,
    labeledVectorHandling< dtMeshOperator * > const * const mO
  ) {
    dtMesh3DOperator::init(element, bC, cV, aF, aG, bV, mO);
    
    _maxGrowingRatePerStep 
		= 
		qtXmlBase::getAttributeFloatMuParse(
      "maxGrowingRatePerStep", element, cV, aF
    );
		_nNormalSmoothingSteps 
		= 
		qtXmlBase::getAttributeIntMuParse("nNormalSmoothingSteps", element, cV, aF);		
		_nGrowingSmoothingSteps 
		=
		qtXmlBase::getAttributeIntMuParse(
      "nGrowingSmoothingSteps", element, cV, aF
    );		
		_maxDihedralAngle 
		= 
		qtXmlBase::getAttributeFloatMuParse("maxDihedralAngle", element, cV, aF);		
    
		//
		// boundedVolume
		//		
		_faceLabel 
		= 
		qtXmlPrimitive::getAttributeStrVector("faceLabel", element);
		_fixedFaceLabel 
		= 
		qtXmlPrimitive::getAttributeStrVector("fixedFaceLabel", element);
		_slidableFaceLabel 
		= 
		qtXmlPrimitive::getAttributeStrVector("slidableFaceLabel", element);
    
    _3D
    =
    dtMesh3DOperator::MustDownCast(
      mO->get(qtXmlBase::getAttributeStr("dtMesh3DOperator", element))
    );
    
    if ( qtXmlPrimitive::isAttributeVector("nSpacingSteps", element) ) {
      _nSpacingSteps 
      = 
      qtXmlBase::getAttributeIntVectorMuParse("nSpacingSteps", element, cV, aF);
    }
    else {
      _nSpacingSteps
      =
      std::vector< int >(
        _faceLabel.size(), 
        qtXmlBase::getAttributeIntMuParse("nSpacingSteps", element, cV, aF)
      );
    }
    dt__throwIf( _faceLabel.size()!=_nSpacingSteps.size(), init() );
    _flipNormal = std::vector< bool >(_nSpacingSteps.size(), false);
    dt__forAllIndex(_nSpacingSteps, ii) {
      if ( _nSpacingSteps[ii] < 0 ) {
        _flipNormal[ii] = true;
        _nSpacingSteps[ii] = abs( _nSpacingSteps[ii] );
      }
    }
    
    dt__info(
      init(),
      << "_nSpacingSteps = " << logMe::vecToString(_nSpacingSteps) << std::endl
      << "_flipNormal = " << logMe::vecToString(_flipNormal) << std::endl
    );
	}
	
  void dtMeshGRegionWithBoundaryLayer::operator() (dtGmshRegion * dtgr) {
    dtgr->setOutwardOrientationMeshConstraint();
    
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
		
    dtOMMeshDivided omInitDiv;
    dtOMMeshDivided omMovedDiv;
    dt__forAllRefAuto(faceList, thisFace) {
			dt__pH(dtOMMesh) tmpOM(thisFace->getOMMesh());
      omInitDiv.add(*tmpOM);      
      omMovedDiv.add(*tmpOM);      
    } 
    dt__forAllRefAuto(fixedFaceList, thisFace) {
			dt__pH(dtOMMesh) tmpOM(thisFace->getOMMesh());
      omInitDiv.add(*tmpOM);      
      omMovedDiv.add(*tmpOM);      
    }
    dt__forAllRefAuto(slidableFaceList, thisFace) {
			dt__pH(dtOMMesh) tmpOM(thisFace->getOMMesh());
      omInitDiv.add(*tmpOM);      
      omMovedDiv.add(*tmpOM);      
    } 
    
    _omInit.add( omInitDiv.connect() );
    _omMoved.add( omMovedDiv.connect() );
    
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
    int cc = 0;
    dt__forAllRefAuto(faceList, thisFace) {
			dt__pH(dtOMMesh) tmpOM(thisFace->getOMMesh());      

			// set fields
			_tF.assign(*tmpOM, 0.);       
			_typeF.assign(*tmpOM, dtMeshGRegionWithBoundaryLayer::_NORMAL);
      _realSpacing.assign( 
        *tmpOM, std::vector<float>(_nSpacingSteps[cc], 0.) 
      );
      _flipNormalF.assign( *tmpOM, _flipNormal[cc] );
      _nSpacingStepsF.assign(*tmpOM, _nSpacingSteps[cc] );
      _faceIndex.assign(*tmpOM, cc);
      cc = cc + 1;
		}
		
		// slidable surface
    dt__forAllRefAuto(slidableFaceList, thisFace) {    

			dt__pH(dtOMMesh) tmpOM(thisFace->getOMMesh());
			
			// set fields
			_fixedF.assign(*tmpOM, true);
			_tF.assign(*tmpOM, 0.);      
      canSlideF.assign(*tmpOM, true);
      _extrudeF.assign(*tmpOM, 0);
      _typeF.assign(*tmpOM, dtMeshGRegionWithBoundaryLayer::_SLIDER);
		}
    
		// fixed surface
    dt__forAllRefAuto(fixedFaceList, thisFace) {    
			dt__pH(dtOMMesh) tmpOM(thisFace->getOMMesh());
			
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
        bool prevent = false;
        dt__forFromToIter(
          omConstVertexFaceI,_omInit.cvf_begin(*it), _omInit.cvf_end(*it), fIt
        ) {
          if ( _typeF.at(*fIt) == dtMeshGRegionWithBoundaryLayer::_FIXER ) {
            prevent = true;
            break;
          }
        }        
        if ( !prevent ) {
          _slidableF[*it] = true;
          _fixedF[*it] = false;
        }
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
    
    if ( optionHandling::debugTrue() ) {
      //
      // write fields
      //
      dtMoabCore mb0(_tF.refMesh());
      mb0.addVertexField(_fixedF);
      mb0.addVertexField(_slidableF);
      mb0.addFaceField(_extrudeF);
      mb0.addFaceField(_typeF);
      mb0.addVertexField(canSlideF);
      mb0.addVertexField(_flipNormalF);
      mb0.addFaceField(_nSpacingStepsF);
      mb0.addVertexField(_tF);
      mb0.addVertexField(_nF);      
      mb0.addVertexField(_faceIndex);      
      mb0.write_mesh(
        std::string(
          _3D->getLabel()
          +
          "_"
          +
          this->getLabel()
          +
          "_dtMeshGRegionWithBoundaryLayer_0.vtk"
        ).c_str()
      );
    }
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
		dt__info(operator(), << "Manifolds created.");
    
		//
		// createLayerVertices
		//
		createLayerVertices();
		dt__info(operator(), << "Layer vertices created.");

		//
		// divide manifolds and calculate normals
		// averaging all normals of manifolds
		//
		dt__forAllIter(std::vector< dtOMMeshManifold >, omManifolds, it) {
			std::vector< dtOMMeshManifold > divOmMs = it->divide(_maxDihedralAngle);
			std::vector< dtVector3 > nnV;
			dt__forAllIter(std::vector< dtOMMeshManifold >, divOmMs, itDiv) {
				nnV.push_back(itDiv->normal());
			}
			dtVector3 nn = dtLinearAlgebra::normalize( dtLinearAlgebra::sum(nnV) );
			
      _nF[it->centerMVertex()] = nn;
      if ( _flipNormalF.at(it->centerMVertex()) ) {
        _nF[it->centerMVertex()] = -1. * _nF[it->centerMVertex()];
      }
		}
		dt__info(operator(), << "Normals calculated.");
    
    //
    // adjust thickness
    //
    adjustThickness();
    
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
    meshWithGmsh(_omMoved, vertex, element, dtgr->model()->getFields());
    
		//
		// write fields
		//
    if ( optionHandling::debugTrue() ) {
      dtMoabCore mb1(_tF.refMesh());
      mb1.addVertexField(_fixedF);
      mb1.addVertexField(_slidableF);
      mb1.addFaceField(_extrudeF);
      mb1.addFaceField(_typeF);
      mb1.addVertexField(canSlideF);
      mb1.addVertexField(_flipNormalF);
      mb1.addFaceField(_nSpacingStepsF);
      mb1.addVertexField(_tF);
      mb1.addVertexField(_nF); 
      mb1.addVertexField(_faceIndex);
      mb1.write_mesh(
        std::string(
          _3D->getLabel()
          +
          "_"
          +
          this->getLabel()
          +
          "_dtMeshGRegionWithBoundaryLayer_1.vtk"
        ).c_str()    
      );

      dtMoabCore(_omMoved).write_mesh(
        std::string(
          _3D->getLabel()
          +
          "_"
          +
          this->getLabel()
          +
          "_dtMeshGRegionWithBoundaryLayer_moved_1.vtk"
        ).c_str()    
      );
    }
    
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
  
	void dtMeshGRegionWithBoundaryLayer::createLayerVertices( void ) {
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
      _buddyF[*it].pop_back();
      dt__forFromToIndex(0, _realSpacing.at(*it).size(), ii) {
        _buddyF[*it].push_back( new ::MVertex(0., 0., 0., NULL));
        _realSpacing[*it][ii] = 0.;
      }
      _buddyF[*it].push_back( new ::MVertex(0., 0., 0., NULL) );
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
        dt__throwIf( !_omMoved.is_collapse_ok(*it), createMovedMesh() );
        _omMoved.collapse(*it);
      }
    }
    _omMoved.garbage_collection();    
    _omMoved.updateMap();
  }
  
	void dtMeshGRegionWithBoundaryLayer::createBoundaryLayerElements(
		std::vector< ::MVertex * > & vertex, std::vector< ::MElement * > & element        
	) const {
		dt__forFromToIter(
      omFaceI, _omInit.faces_begin(), _omInit.faces_end(), fIt
    ) {
      // handle only elements that should be extruded
      if (!_extrudeF.at(*fIt) ) continue;
      
			// create elements and add them to given region
      dt__forFromToIndex(0, _nSpacingStepsF.at(*fIt)+1,ii) {
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
        &&
        _slidableF.at(_omInit.to_vertex_handle(*hIt)) 
      ) {
        return *hIt;
      }
    }
    
    dt__throw(
      slidableHalfedgeInFace(), << "No halfedge lies on slidable seam."
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
    //do {
    dt__forFromToIndex( 0, _omInit.n_edges(), ii ) {
      omHalfedgeH wHe = slidableHe;
      dt__forFromToIndex(
        0, 
        _realSpacing.at(_omInit.to_vertex_handle(slidableHe)).size(), 
        ii
      ) {
        // mark halfedge
        _omInit.data( _omInit.next_halfedge_handle(wHe) ).mark();
        
        _buddyF[ _omInit[ _omInit.to_vertex_handle(slidableHe) ] ].push_back(
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
      _omInit.data( _omInit.next_halfedge_handle(wHe) ).mark();      
      _buddyF[ _omInit[ _omInit.to_vertex_handle(slidableHe) ] ].push_back(
        _omInit[ 
          _omInit.to_vertex_handle( _omInit.next_halfedge_handle(wHe) ) 
        ]
      );
      
      
      std::vector< MVertex * > & thisBuddies 
      =
      _buddyF[ _omInit[ _omInit.to_vertex_handle(slidableHe) ] ];
      std::vector< float > & thisRealSpacing 
      =
      _realSpacing[ _omInit[ _omInit.to_vertex_handle(slidableHe) ] ];
      thisRealSpacing.clear();
      float tmpSpace = 0.;
      dt__forAllIndex(thisBuddies, ii) {
        if (ii==0) {
          thisRealSpacing.push_back( 
            dtLinearAlgebra::length(
              dtGmshModel::extractPosition( thisBuddies[ii] )
              -
              dtGmshModel::extractPosition( 
                _omInit[ _omInit.to_vertex_handle(slidableHe) ] 
              )
            )
          );          
        }
        else {
          thisRealSpacing.push_back( 
            dtLinearAlgebra::length(
              dtGmshModel::extractPosition( thisBuddies[ii] )
              -
              dtGmshModel::extractPosition( thisBuddies[ii-1] )
            )
          );
        }
        tmpSpace = tmpSpace + thisRealSpacing.back();
      }
      float tmpASpace = 0.;
      dt__forAllRefAuto(thisRealSpacing, aSpace) {
        tmpASpace = aSpace + tmpASpace;
        aSpace = tmpASpace/tmpSpace;
      }
      _tF[ _omInit.to_vertex_handle(slidableHe) ] = tmpSpace;
      slidableHe 
      = 
      _omInit.prev_halfedge_handle(  
        _omInit.sameHalfedgeInNextFace(
          _omInit.next_halfedge_handle(slidableHe)
        )
      );
      if ( slidableHe == slidableHe_init ) {
        dt__info(
          createRingOfBuddies(), 
          << "Ring created in " << ii << " changes of init halfedge."
        );
        return;
      }
    }
    dt__throw(
      createRingOfBuddies(), 
      << "More than " << _omInit.n_edges() << " changes of inital halfedge."
      << std::endl
      << "More changes than number of edges!"
    );
  }
  
  void dtMeshGRegionWithBoundaryLayer::meshWithGmsh(
    dtOMMesh const & mesh, 
    std::vector< ::MVertex * > & vertex, 
    std::vector< ::MElement * > & element,
    ::FieldManager * fm
  ) const {
    dtGmshModel gm("myModel", fm);

    dtGmshFace * gf = new dtGmshFace(&gm, 1);
    gm.add(gf);
    dtGmshRegion * gr = new dtGmshRegion(&gm, 1);
    gm.add(gr);
    
    //
    // change underlying GEntities
    //
    std::map< ::MVertex *, ::GEntity * > geStore;
    dt__forFromToIter(
      omConstVertexI, mesh.vertices_begin(), mesh.vertices_end(), it
    ) {
      ::MVertex * mv = mesh.data(*it).MVertex();
      
      geStore[ mv ] = mv->onWhat();
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
      }
      else if ( vv.size() == 4 ) {
        gf->addElement( new ::MQuadrangle( vv[0], vv[1], vv[2], vv[3] ) );        
      }      
      else dt__throw(meshWithGmsh(), << dt__eval(vv.size()));
    }
    gf->meshStatistics.status = ::GEntity::MeshGenerationStatus::DONE;

    gr->addFace( gf, 1 );
    gf->addRegion( gr );

    if ( debugTrue()  ) {
      gm.writeMSH(
        this->_3D->getLabel()+"_"+getLabel()+"_0.msh", 4.0, false, true
      );
    }
    
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
    
    //
    // change back underlying GEntities
    //
    dt__forFromToIter(
      omConstVertexI, mesh.vertices_begin(), mesh.vertices_end(), it
    ) {
      ::MVertex * mv = mesh.data(*it).MVertex();
      
      mv->setEntity( geStore[ mv ] );
    }    
  }
  
  void dtMeshGRegionWithBoundaryLayer::adjustThickness( void ) { 
    twoDArrayHandling< float > avSpacing;
    twoDArrayHandling< float > maxSpacing;
    twoDArrayHandling< float > minSpacing;
    vectorHandling< float > avT;
    vectorHandling< float > maxT;
    vectorHandling< float > minT;    
    
    determinMinMaxAverageAtSliders(
      avSpacing, maxSpacing, minSpacing, avT, maxT, minT
    );
    
    dt__info(
      operator(),
      << "avSpacing = " << std::endl
      << logMe::floatMatrixToString(avSpacing) << std::endl
      << "maxSpacing = " << std::endl
      << logMe::floatMatrixToString(maxSpacing) << std::endl
      << "minSpacing = " << std::endl
      << logMe::floatMatrixToString(minSpacing) << std::endl 
      << "avT = " << std::endl
      << logMe::vecToString(avT) << std::endl
      << "maxT = " << std::endl
      << logMe::vecToString(maxT) << std::endl
      << "minT = " << std::endl
      << logMe::vecToString(minT) << std::endl    
    );      
    
    //
    // init thickness and normals
    //
    dt__forFromToIter(
      omVertexI, _omInit.vertices_begin(), _omInit.vertices_end(), v_it
    ) {
      if ( _buddyF.at(*v_it).empty() ) continue;
      
      int fI = _faceIndex.at(*v_it);
      
      if ( !_slidableF.at(*v_it) ) {
        dt__forAllIndex(_realSpacing[ *v_it ], ii) {
          _realSpacing[ *v_it ][ii] = avSpacing[fI][ii];
        }
        _tF[ *v_it ] = minT[fI];
      }
      else {
        _nF[ *v_it ]
        =
        2. * dtLinearAlgebra::normalize(
          dtGmshModel::extractPosition(_buddyF[ *v_it ].back())
          - 
          dtGmshModel::extractPosition(_buddyF[ *v_it ].front())
        );
      }
    }
      
    //
    // smooth normals
    //
    dt__forFromToIndex(0, _nNormalSmoothingSteps, ii) {
      dtOMVertexField< dtVector3 > nFTwin("nFTwin", _omInit, dtVector3(0,0,0));
      dt__forFromToIter(
        omVertexI, _omInit.vertices_begin(), _omInit.vertices_end(), v_it
      ) {    
        if ( _buddyF.at(*v_it).empty() || _slidableF.at(*v_it) ) continue;
        
        nFTwin[ *v_it ] 
        = 
        dtLinearAlgebra::normalize(_nF.oneRingAverage( *v_it ));
      }
      dt__forFromToIter(
        omVertexI, _omInit.vertices_begin(), _omInit.vertices_end(), v_it
      ) {    
        if ( _buddyF.at(*v_it).empty() || _slidableF.at(*v_it) ) continue;
        
        _nF[ *v_it ] = nFTwin[ *v_it ];
      }
    }
    
    //
    // smooth
    //
    dt__forFromToIndex(0, _nGrowingSmoothingSteps, ii) {
      dtOMVertexField< float > tFTwin("tFTwin", _omInit, 0.);
      
      dt__forFromToIter(
        omVertexI, _omInit.vertices_begin(), _omInit.vertices_end(), v_it
      ) {
        if ( _buddyF.at(*v_it).empty() || _slidableF.at(*v_it) ) continue;
        int fI = _faceIndex.at(*v_it);
        tFTwin[ *v_it ] = std::max( _tF.oneRingAverage( *v_it ), minT[fI] );
      }
      dt__forFromToIter(
        omVertexI, _omInit.vertices_begin(), _omInit.vertices_end(), v_it
      ) {    
        if ( _buddyF.at(*v_it).empty() || _slidableF.at(*v_it) ) continue;
               
        int fI = _faceIndex.at(*v_it);
        
        _tF[ *v_it ] 
         = 
        std::min(
          maxT[fI],
          std::min( _maxGrowingRatePerStep * _tF[ *v_it ], tFTwin[ *v_it ] )
        );
      }
    }
      
    //
    // adjust thickness
    //
    dt__forFromToIndex(0, _nGrowingSmoothingSteps, ii) {
      dt__forFromToIter(
        omVertexI, _omInit.vertices_begin(), _omInit.vertices_end(), v_it
      ) {    
        dt__forFromToIter(
          omVertexVertexI, _omInit.vv_begin(*v_it), _omInit.vv_end(*v_it), vv_it
        ) {
          if ( _buddyF.at(*vv_it).empty() || _slidableF.at(*vv_it) ) continue;
          
          std::vector< ::MVertex * > & thisBuddies = _buddyF[ *vv_it ];
          std::vector< float > & thisRealSpacing = _realSpacing[ *vv_it ];
          float & thisThickness = _tF[ *vv_it ];
          dtVector3 & thisNormal = _nF[ *vv_it ];
          dtPoint3 thisStart 
          = 
          dtGmshModel::extractPosition(_omInit[ *vv_it ]);
          dt__forAllIndex(thisRealSpacing, ii) {
            dtGmshModel::setPosition(
              thisBuddies[ii],
              thisStart + thisRealSpacing[ii] * thisThickness * thisNormal
            );
          }
          dtGmshModel::setPosition(
            thisBuddies.back(),
            thisStart + thisThickness * thisNormal
          );          
        }
      } 
    }
    
    determinMinMaxAverageAtSliders(
      avSpacing, maxSpacing, minSpacing, avT, maxT, minT
    );
    
    dt__info(
      operator(),
      << "avSpacing = " << std::endl
      << logMe::floatMatrixToString(avSpacing) << std::endl
      << "maxSpacing = " << std::endl
      << logMe::floatMatrixToString(maxSpacing) << std::endl
      << "minSpacing = " << std::endl
      << logMe::floatMatrixToString(minSpacing) << std::endl 
      << "avT = " << std::endl
      << logMe::vecToString(avT) << std::endl
      << "maxT = " << std::endl
      << logMe::vecToString(maxT) << std::endl
      << "minT = " << std::endl
      << logMe::vecToString(minT) << std::endl    
    );      
  }  
  
  void dtMeshGRegionWithBoundaryLayer::determinMinMaxAverageAtSliders(
      twoDArrayHandling< float > & avSpacing,
      twoDArrayHandling< float > & maxSpacing,
      twoDArrayHandling< float > & minSpacing,
      vectorHandling< float > & avT,
      vectorHandling< float > & maxT,
      vectorHandling< float > & minT
  ) {
    avSpacing.clear();
    maxSpacing.clear();
    minSpacing.clear();
    avT.clear();
    maxT.clear();
    minT.clear();
    dt__forAllRefAuto(_nSpacingSteps, aSpace) {
      avSpacing.push_back( std::vector< float >(aSpace, 0.) );
      maxSpacing.push_back( 
        std::vector< float >(aSpace, std::numeric_limits<float>::min()) 
      );
      minSpacing.push_back( 
        std::vector< float >(aSpace, std::numeric_limits<float>::max()) 
      );
      avT.push_back(0.);
      maxT.push_back( std::numeric_limits<float>::min() );
      minT.push_back( std::numeric_limits<float>::max() );
    }
    std::vector< float > nodeCount 
    = 
    std::vector< float >(_nSpacingSteps.size(), 0.);
    
		dt__forFromToIter(
		  omConstVertexI, _omInit.vertices_begin(), _omInit.vertices_end(), v_it
		) {
      if ( _buddyF.at( *v_it ).empty() || !_slidableF.at( *v_it ) ) continue;
      int fI = _faceIndex.at(*v_it);
      nodeCount[fI] = nodeCount[fI] + 1.;
      
      avT[fI] = avT[fI] + _tF.at( *v_it );
      minT[fI] = std::min( minT[fI], _tF.at(*v_it) );
      maxT[fI] = std::max( maxT[fI], _tF.at(*v_it) );
      dt__forAllIndex(_realSpacing.at( *v_it ), ii) {
        avSpacing[fI][ii] = avSpacing[fI][ii] + _realSpacing.at( *v_it )[ii];
        minSpacing[fI][ii] 
        =
        std::min(minSpacing[fI][ii], _realSpacing.at( *v_it )[ii]);
        maxSpacing[fI][ii] 
        =
        std::max(maxSpacing[fI][ii], _realSpacing.at( *v_it )[ii]);
      }
    }
    dt__forAllIndex(_nSpacingSteps, fI) {
      avT[fI] = avT[fI]/nodeCount[fI];
      dt__forAllRefAuto(avSpacing[fI], aSpacing) {
        aSpacing = aSpacing/nodeCount[fI];
      }
    }
  }
}