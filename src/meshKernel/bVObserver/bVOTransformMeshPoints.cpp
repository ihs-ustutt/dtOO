#include "bVOTransformMeshPoints.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <baseContainerHeaven/baseContainer.h>
#include <baseContainerHeaven/transformerContainer.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshEdge.h>
#include <meshEngine/dtGmshVertex.h>
#include <dtTransformerHeaven/dtTransformer.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <gmsh/MLine.h>
#include <gmsh/MTriangle.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/MTetrahedron.h>
#include <gmsh/MHexahedron.h>
#include <gmsh/MPrism.h>
#include <gmsh/MPyramid.h>

namespace dtOO {  
  bVOTransformMeshPoints::bVOTransformMeshPoints() {
  }

  bVOTransformMeshPoints::~bVOTransformMeshPoints() {
    
  }
  
  void bVOTransformMeshPoints::bVOTransformMeshPoints::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG,
		vectorHandling< boundedVolume * > const * const bV,
		boundedVolume * attachTo
  ) {
    bVOInterface::init(element, bC, cV, aF, aG, bV, attachTo);
    
//		<bVObserver 
//			name="bVOTransformMeshPoints" 
//			relative_tolerance="0.5"     
//			transformer="{rot_0.5pi_NZ}{rot_1.0pi_NZ}{rot_1.5pi_NZ}"
//		/>			
								
    dt__info(init(), << dtXmlParserBase::convertToString(element) );
    
    //
    // trasnformer vector
    //
    dt__forAllRefAuto( 
      dtXmlParserBase::getAttributeStrVector("transformer", element), aLabel
    ) {
      _dtT.push_back( 
        bC->constPtrTransformerContainer()->get( aLabel )
      );
    }
    
    _relTol = 1.0;
    if ( dtXmlParserBase::hasAttribute("relative_tolerance", element) ) {
      _relTol
      =
      dtXmlParserBase::getAttributeFloatMuParse(
        "relative_tolerance", element, cV
      );
    }
  }
  
  void bVOTransformMeshPoints::postUpdate( void ) {
		dt__ptrAss( dtGmshModel * gm, ptrBoundedVolume()->getModel() );
		//
		// set current model
		//
		::GModel::setCurrent(gm);
    
    std::vector< std::map< ::GEntity *, ::GEntity * > > ge_newOldV;
    dt__forAllIndex( _dtT, ii ) {
      ge_newOldV.push_back( 
        gm->createTwin(
          gm->getMaxVertexTag() * (ii+1),
          gm->getMaxEdgeTag() * (ii+1),
          gm->getMaxFaceTag() * (ii+1),
          gm->getMaxRegionTag() * (ii+1)
        )
      );
    }
    std::map< ::MVertex *, ::MVertex * > mv_newOld;
    
    int cc = 0;
    dt__forAllRefAuto(_dtT, aDtT) {
      std::map< ::GEntity *, ::GEntity * > & ge_newOld = ge_newOldV[cc];
      cc = cc+1;
      
      dtTransformer const & currentT = *aDtT;
      
      dt__info( 
        postUpdate(), 
        << "currentT.getLabel() = " << currentT.getLabel()
      );
      
      //
      // vertex
      //
      dt__forAllRefAuto( gm->vertices(), gv ) {
        ge_newOld[ gv ]->setModel( gm );
        dt__forAllRefAuto( gv->mesh_vertices, mv ) {
          dtPoint3 pT = currentT(dtGmshModel::extractPosition( mv ), 1);
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
      // add vertices
      dt__forAllRefAuto( gm->edges(), ge ) {
        dt__ptrAss( dtGmshEdge * edge, dtGmshEdge::DownCast(ge) );
        dt__ptrAss( 
          dtGmshEdge * cloneEdge, dtGmshEdge::DownCast( ge_newOld[ ge ] ) 
        );
        cloneEdge->setModel( gm );
        dt__forAllRefAuto( edge->mesh_vertices, mv ) {
          dtPoint3 pT = currentT(dtGmshModel::extractPosition( mv ), 1);
          ::MVertex * mv_clone
          = 
          new ::MVertex(pT.x(), pT.y(), pT.z(), cloneEdge);

          cloneEdge->addMeshVertex( mv_clone );

          mv_newOld[ mv ] = mv_clone;
        }
      }      
      // add elements
      dt__forAllRefAuto( gm->edges(), ge ) {
        dt__ptrAss( dtGmshEdge * edge, dtGmshEdge::DownCast(ge) );
        dt__ptrAss( 
          dtGmshEdge * cloneEdge, dtGmshEdge::DownCast( ge_newOld[ ge ] ) 
        );        
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
      // add vertices
      dt__forAllRefAuto( gm->faces(), gf ) {
        dt__ptrAss( dtGmshFace * face, dtGmshFace::DownCast(gf) );
        dt__ptrAss( 
          dtGmshFace * cloneFace, dtGmshFace::DownCast( ge_newOld[ gf ] ) 
        );        
        cloneFace->setModel( gm );
        dt__forAllRefAuto( gf->mesh_vertices, mv ) {
          dtPoint3 pT = currentT(dtGmshModel::extractPosition( mv ), 1);
          ::MVertex * mv_clone
          = 
          new ::MVertex(pT.x(), pT.y(), pT.z(), cloneFace);

          cloneFace->addMeshVertex( mv_clone );

          mv_newOld[ mv ] = mv_clone;
        }
      }
      // add elements
      dt__forAllRefAuto( gm->faces(), gf ) {
        dt__ptrAss( dtGmshFace * face, dtGmshFace::DownCast(gf) );
        dt__ptrAss( 
          dtGmshFace * cloneFace, dtGmshFace::DownCast( ge_newOld[ gf ] ) 
        );        
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
      // add vertices
      dt__forAllRefAuto( gm->regions(), gr ) {
        dt__ptrAss( dtGmshRegion * region, dtGmshRegion::DownCast(gr) );
        dt__ptrAss( 
          dtGmshRegion * cloneRegion, dtGmshRegion::DownCast( ge_newOld[ gr ] ) 
        );               
        cloneRegion->setModel( gm );
        dt__forAllRefAuto( gr->mesh_vertices, mv ) {
          dtPoint3 pT = currentT(dtGmshModel::extractPosition( mv ), 1);
          ::MVertex * mv_clone
          = 
          new ::MVertex(pT.x(), pT.y(), pT.z(), cloneRegion);

          cloneRegion->addMeshVertex( mv_clone );
          
          mv_newOld[ mv ] = mv_clone;
        }
      }      
      // add elements
      dt__forAllRefAuto( gm->regions(), gr ) {
        dt__ptrAss( dtGmshRegion * region, dtGmshRegion::DownCast(gr) );
        dt__ptrAss( 
          dtGmshRegion * cloneRegion, dtGmshRegion::DownCast( ge_newOld[ gr ] ) 
        );               
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
    }
    
    //
    // add new GEntities to model and duplicate physical entries
    //
    dt__forAllRefAuto( gm->vertices(), gv ) {
      dt__forAllRefAuto(ge_newOldV, ge_newOld) {
        gm->add( ge_newOld[ gv ] );
      }
    }
    dt__forAllRefAuto( gm->edges(), ge ) {
      dt__forAllRefAuto(ge_newOldV, ge_newOld) {
        gm->add( ge_newOld[ ge ] );
      }
    }    
    dt__forAllRefAuto( gm->faces(), gf ) {
      cc = 0;
      dt__forAllRefAuto(ge_newOldV, ge_newOld) {
        if ( gm->getPhysicalString(gf) != "" ) {
          gm->tagPhysical( 
            ge_newOld[ gf ], 
            gm->getPhysicalString(gf)+"_"+stringPrimitive::intToString(cc)
          );
          cc = cc + 1;
        }        
        gm->add( ge_newOld[ gf ] );
      }
    }    
    dt__forAllRefAuto( gm->regions(), gr ) {
      dt__forAllRefAuto(ge_newOldV, ge_newOld) {
        if ( gm->getPhysicalString(gr) != "" ) {        
          gm->tagPhysical( 
            ge_newOld[ gr ], 
            gm->getPhysicalString(gr)+"_"+stringPrimitive::intToString(cc)
          );
          cc = cc + 1;        
        }
        gm->add( ge_newOld[ gr ] );
      }
    }
    
    gm->removeDuplicateMeshVertices( _relTol );
  }
}
