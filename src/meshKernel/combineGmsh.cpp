#include "combineGmsh.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <xmlHeaven/dtXmlParserBase.h>

#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshEdge.h>
#include <meshEngine/dtGmshVertex.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <gmsh/MLine.h>
#include <gmsh/MTriangle.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/MTetrahedron.h>
#include <gmsh/MHexahedron.h>
#include <gmsh/MPrism.h>
#include <gmsh/MPyramid.h>

namespace dtOO {
	combineGmsh::combineGmsh() : gmshBoundedVolume() {
	}

	combineGmsh::~combineGmsh() {
	}
	
  void combineGmsh::init( 
    ::QDomElement const & element,
		baseContainer const * const bC,
		cVPtrVec const * const cV,
		aFPtrVec const * const aF,
		aGPtrVec const * const aG,
		bVPtrVec const * const bV
	) {
    //
    // init gmshBoundedVolume
    //
    gmshBoundedVolume::init(element, bC, cV, aF, aG, bV);
		
    //
		// region
		//		
    std::vector< ::QDomElement > wEl 
    = 
    qtXmlPrimitive::getChildVector("boundedVolume", element);

		//
		// set current model
		//
		::GModel::setCurrent( _gm );

    _dtGM.resize( wEl.size() );
    _bV.resize( wEl.size() );
		dt__forAllIndex(wEl, ii) {
      dt__ptrAss(
        _bV[ii], bV->get( qtXmlPrimitive::getAttributeStr("label", wEl[ii]) )
      );
      
      dt__ptrAss(
        _dtGM[ii], dtGmshModel::ConstDownCast( _bV[ii]->getModel() )
      );      
    }
    
    //
    // get precision for removing duplicate vertices
    //
    _relTol = std::numeric_limits<float>::max();
    if ( 
      dtXmlParserBase::hasAttribute("relative_tolerance", element)
    ) {
      _relTol
      =
      dtXmlParserBase::getAttributeFloatMuParse(
        "relative_tolerance", element, cV
      );
    }
    _absTol = std::numeric_limits<float>::max();
    if ( 
      dtXmlParserBase::hasAttribute("absolute_tolerance", element)
    ) {
      _absTol
      =
      dtXmlParserBase::getAttributeFloatMuParse(
        "absolute_tolerance", element, cV
      );
    }    
  }
    
  void combineGmsh::makeGrid(void) {
    if ( !isPreMeshed() ) makePreGrid();
    
    dt__forAllRefAuto(_bV, aBV) if ( !aBV->isMeshed() ) aBV->makeGrid();
        
		//
		// set current model
		//
		::GModel::setCurrent( _gm );

    std::vector< std::map< ::GEntity *, ::GEntity * > > ge_newOldV;    
		dt__forAllIndex(_dtGM, ii) {
      ge_newOldV.push_back( 
        _dtGM[ii]->createTwin(
          _gm->getMaxVertexTag() * ii, 
          _gm->getMaxEdgeTag() * ii, 
          _gm->getMaxFaceTag() * ii, 
          _gm->getMaxRegionTag() * ii
        )
      );

      //
      // add new GEntities to model and duplicate physical entries
      //
      dt__forAllRefAuto( _dtGM[ii]->vertices(), gv ) {
        _gm->add( ge_newOldV[ii][ gv ] );
      }
      dt__forAllRefAuto( _dtGM[ii]->edges(), ge ) {
        _gm->add( ge_newOldV[ii][ ge ] );
      }    
      dt__forAllRefAuto( _dtGM[ii]->faces(), gf ) {
        _gm->add( ge_newOldV[ii][ gf ] );        
        if ( _dtGM[ii]->getPhysicalString(gf) != "" ) {
          _gm->tagPhysical( 
            ge_newOldV[ii][ gf ], 
            _dtGM[ii]->getPhysicalString(gf)
          );
        }        
      }    
      dt__forAllRefAuto( _dtGM[ii]->regions(), gr ) {
        _gm->add( ge_newOldV[ii][ gr ] );
        if ( _dtGM[ii]->getPhysicalString( gr ) != "" ) {        
          _gm->tagPhysical( 
            ge_newOldV[ii][ gr ], 
            _dtGM[ii]->getPhysicalString(gr)
          );
        }
      }
    }
    
    std::map< ::MVertex *, ::MVertex * > mv_newOld;
    
    int cc = 0;
    dt__forAllRefAuto(ge_newOldV, ge_newOld) {
//      std::map< ::GEntity *, ::GEntity * > & ge_newOld = ge_newOldV[cc];    
    //
    // vertex
    //
    dt__forAllRefAuto( _dtGM[cc]->vertices(), gv ) {
      ge_newOld[ gv ]->setModel(  _gm  );
      dt__forAllRefAuto( gv->mesh_vertices, mv ) {
        dtPoint3 pT = dtGmshModel::extractPosition( mv );
        ::MVertex * mv_clone
        = 
        new ::MVertex(pT.x(), pT.y(), pT.z(), ge_newOld[ gv ]);

        ge_newOld[ gv ]->addMeshVertex( mv_clone );

        mv_newOld[ mv ] = mv_clone;
      }
    }
      
    //
    // edge
    //
    dt__forAllRefAuto( _dtGM[cc]->edges(), ge ) {
      dt__ptrAss( dtGmshEdge * edge, dtGmshEdge::DownCast(ge) );
      dt__ptrAss( 
        dtGmshEdge * cloneEdge, dtGmshEdge::DownCast( ge_newOld[ ge ] ) 
      );
      cloneEdge->setModel(  _gm  );
      dt__forAllRefAuto( edge->mesh_vertices, mv ) {
        dtPoint3 pT = dtGmshModel::extractPosition( mv );
        ::MVertex * mv_clone
        = 
        new ::MVertex(pT.x(), pT.y(), pT.z(), cloneEdge);

        cloneEdge->addMeshVertex( mv_clone );

        mv_newOld[ mv ] = mv_clone;
      }

      dt__forAllRefAuto( edge->lines, aLine ) {
        cloneEdge->lines.push_back(
        new ::MLine(
          mv_newOld[ aLine->getVertex(0) ], 
          mv_newOld[ aLine->getVertex(1) ]
          )
        );
      }
    }    

      //
      // face
      //
      dt__forAllRefAuto( _dtGM[cc]->faces(), gf ) {
        dt__ptrAss( dtGmshFace * face, dtGmshFace::DownCast(gf) );
        dt__ptrAss( 
          dtGmshFace * cloneFace, dtGmshFace::DownCast( ge_newOld[ gf ] ) 
        );        
        cloneFace->setModel(  _gm  );
        dt__forAllRefAuto( gf->mesh_vertices, mv ) {
          dtPoint3 pT = dtGmshModel::extractPosition( mv );
          ::MVertex * mv_clone
          = 
          new ::MVertex(pT.x(), pT.y(), pT.z(), cloneFace);

          cloneFace->addMeshVertex( mv_clone );

          mv_newOld[ mv ] = mv_clone;
        }

        dt__forAllRefAuto( face->triangles, aTri ) {
          cloneFace->triangles.push_back(
            new ::MTriangle(
              mv_newOld[ aTri->getVertex(0) ], 
              mv_newOld[ aTri->getVertex(1) ],
              mv_newOld[ aTri->getVertex(2) ]
            )
          );
        }
        dt__forAllRefAuto( face->quadrangles, aQuad ) {
          cloneFace->quadrangles.push_back(
            new ::MQuadrangle(
              mv_newOld[ aQuad->getVertex(0) ], 
              mv_newOld[ aQuad->getVertex(1) ],
              mv_newOld[ aQuad->getVertex(2) ],
              mv_newOld[ aQuad->getVertex(3) ]
            )
          );
        }        
      }

      //
      // region
      //      
      dt__forAllRefAuto( _dtGM[cc]->regions(), gr ) {
        dt__ptrAss( dtGmshRegion * region, dtGmshRegion::DownCast(gr) );
        dt__ptrAss( 
          dtGmshRegion * cloneRegion, dtGmshRegion::DownCast( ge_newOld[ gr ] ) 
        );               
        cloneRegion->setModel(  _gm  );
        dt__forAllRefAuto( gr->mesh_vertices, mv ) {
          dtPoint3 pT = dtGmshModel::extractPosition( mv );
          ::MVertex * mv_clone
          = 
          new ::MVertex(pT.x(), pT.y(), pT.z(), cloneRegion);

          cloneRegion->addMeshVertex( mv_clone );
          
          mv_newOld[ mv ] = mv_clone;
        }
        
        dt__forAllRefAuto( region->tetrahedra, aTet ) {
          cloneRegion->tetrahedra.push_back(
            new ::MTetrahedron(
              mv_newOld[ aTet->getVertex(0) ], 
              mv_newOld[ aTet->getVertex(1) ],
              mv_newOld[ aTet->getVertex(2) ],
              mv_newOld[ aTet->getVertex(3) ]
            )
          );
        }
        dt__forAllRefAuto( region->pyramids, aPyr ) {
          cloneRegion->pyramids.push_back(
            new ::MPyramid(
              mv_newOld[ aPyr->getVertex(0) ], 
              mv_newOld[ aPyr->getVertex(1) ],
              mv_newOld[ aPyr->getVertex(2) ],
              mv_newOld[ aPyr->getVertex(3) ],
              mv_newOld[ aPyr->getVertex(4) ]
            )
          );
        }        
        dt__forAllRefAuto( region->prisms, aPri ) {
          cloneRegion->prisms.push_back(
            new ::MPrism(
              mv_newOld[ aPri->getVertex(0) ], 
              mv_newOld[ aPri->getVertex(1) ],
              mv_newOld[ aPri->getVertex(2) ],
              mv_newOld[ aPri->getVertex(3) ],
              mv_newOld[ aPri->getVertex(4) ],
              mv_newOld[ aPri->getVertex(5) ]
            )
          );
        }
        dt__forAllRefAuto( region->hexahedra, aHex ) {
          cloneRegion->hexahedra.push_back(
            new ::MHexahedron(
              mv_newOld[ aHex->getVertex(0) ], 
              mv_newOld[ aHex->getVertex(1) ],
              mv_newOld[ aHex->getVertex(2) ],
              mv_newOld[ aHex->getVertex(3) ],
              mv_newOld[ aHex->getVertex(4) ],
              mv_newOld[ aHex->getVertex(5) ],
              mv_newOld[ aHex->getVertex(6) ],
              mv_newOld[ aHex->getVertex(7) ]
            )
          );
        }        
      }
      cc++;
    }

    //
    // update bounding box
    //
    gmshBoundedVolume::updateBoundingBox();
    
    SBoundingBox3d bbox = _gm->bounds();
    float lc = bbox.empty() ? 1. : norm(SVector3(bbox.max(), bbox.min()));  
    
    dt__info(
      makeGrid(),
      << "relTol = " << _relTol << std::endl
      << "absTol = " << _absTol << std::endl
      << "lc = " << lc << std::endl
      << "absTol / lc = " << _absTol / lc << std::endl
      << "==> min(absTol / lc, relTol) = " << std::min( _absTol/lc, _relTol )
    );
    
    _gm->removeDuplicateMeshVertices( std::min( _absTol / lc, _relTol ) );
    
    //
    // mark as meshed
    //
    boundedVolume::setMeshed();
    
    //
    // notify observers
    //
    boundedVolume::postNotify();
	}
}
