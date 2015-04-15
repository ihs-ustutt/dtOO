#include "cardinalDirGModel.h"

#include <logMe/logMe.h>

#include "cardinalDirTransfiniteVolume.h"

#include <gmsh/GFace.h>

#include <dtGmshRegion.h>
#include <dtGmshFace.h>
#include <dtGmshEdge.h>
#include <dtGmshVertex.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {  
  cardinalDirGModel::cardinalDirGModel(std::string name) : dtGmshModel(name){
  }

  cardinalDirGModel::~cardinalDirGModel() {
  }
  
  void cardinalDirGModel::attachToBoundedVolume( boundedVolume const * const bV) {
    dt__mustCast(bV, cardinalDirTransfiniteVolume const, _cardDir);
  }

  boundedVolume const * cardinalDirGModel::getAttachedBoundedVolume( void ) {
    return _cardDir;
  }
    
  void cardinalDirGModel::convertOuterToGmshModel( cardinalDirVolumeDefines const * cDDef) {
    cardinalDirVolumeDefines const * def = cDDef;
    if (cDDef == NULL) {
      def = new cardinalDirVolumeDefines(1);
    }
    
    /* -------------------------------------------------------------------- */
    //
    // SOUTH
    //
    /* -------------------------------------------------------------------- */
    //
    // vertices
    //
    dtGmshVertex * vertex;
    addVertexToGmshModel( _cardDir->getPtrToVertex(_cardDir->vStrToId("dtSouthWestFront")), def->transId(_cardDir->vStrToId("dtSouthWestFront")) );
    addVertexToGmshModel( _cardDir->getPtrToVertex(_cardDir->vStrToId("dtSouthEastFront")), def->transId(_cardDir->vStrToId("dtSouthEastFront")) );
    addVertexToGmshModel( _cardDir->getPtrToVertex(_cardDir->vStrToId("dtSouthEastBack")), def->transId(_cardDir->vStrToId("dtSouthEastBack")) );
    addVertexToGmshModel( _cardDir->getPtrToVertex(_cardDir->vStrToId("dtSouthWestBack")), def->transId(_cardDir->vStrToId("dtSouthWestBack")) );

    //
    // edges
    //
    dtGmshEdge * edge;
    addEdgeToGmshModel(
      _cardDir->getPtrToMap1dTo3d(_cardDir->eStrToId("dtSouthFront")), 
      def->transId(_cardDir->eStrToId("dtSouthFront")), 
      def->transId(_cardDir->vStrToId("dtSouthWestFront")), 
      def->transId(_cardDir->vStrToId("dtSouthEastFront"))
    );
    addEdgeToGmshModel(
      _cardDir->getPtrToMap1dTo3d(_cardDir->eStrToId("dtSouthEast")), 
      def->transId(_cardDir->eStrToId("dtSouthEast")), 
      def->transId(_cardDir->vStrToId("dtSouthEastFront")), 
      def->transId(_cardDir->vStrToId("dtSouthEastBack"))
    );
    addEdgeToGmshModel(
      _cardDir->getPtrToMap1dTo3d(_cardDir->eStrToId("dtSouthBack")), 
      def->transId(_cardDir->eStrToId("dtSouthBack")), 
      def->transId(_cardDir->vStrToId("dtSouthWestBack")), 
      def->transId(_cardDir->vStrToId("dtSouthEastBack"))
    );
    addEdgeToGmshModel(
      _cardDir->getPtrToMap1dTo3d(_cardDir->eStrToId("dtSouthWest")), 
      def->transId(_cardDir->eStrToId("dtSouthWest")), 
      def->transId(_cardDir->vStrToId("dtSouthWestFront")), 
      def->transId(_cardDir->vStrToId("dtSouthWestBack"))
    );    

    //
    // faces
    //
    std::list< ::GEdge * > edges;
    std::vector< int > ori;
    edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("dtSouthFront"))) );
    ori.push_back(1);
    edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("dtSouthEast"))) );
    ori.push_back(1);
    edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("dtSouthBack"))) );
    ori.push_back(-1);
    edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("dtSouthWest"))) );
    ori.push_back(-1);
    add( new dtGmshFace(this, def->transId(_cardDir->fStrToId("dtSouth")), edges, ori) );
    dtGmshFace * face = getDtGmshFaceByTag( def->transId(_cardDir->fStrToId("dtSouth")) );
    face->setMap2dTo3d( _cardDir->getPtrToMap2dTo3d(_cardDir->fStrToId("dtSouth")) );

    /* -------------------------------------------------------------------- */
    //
    // NORTH
    //
    /* -------------------------------------------------------------------- */

    //
    // vertices
    //
    add( new dtGmshVertex(this, def->transId(_cardDir->vStrToId("dtNorthWestFront"))) );
    add( new dtGmshVertex(this, def->transId(_cardDir->vStrToId("dtNorthEastFront"))) );
    add( new dtGmshVertex(this, def->transId(_cardDir->vStrToId("dtNorthEastBack"))) );
    add( new dtGmshVertex(this, def->transId(_cardDir->vStrToId("dtNorthWestBack"))) );

    vertex = getDtGmshVertexByTag(def->transId(_cardDir->vStrToId("dtNorthWestFront")));
    vertex->setPosition( _cardDir->getPtrToVertex(_cardDir->vStrToId("dtNorthWestFront")) );
    vertex = getDtGmshVertexByTag(def->transId(_cardDir->vStrToId("dtNorthEastFront")));
    vertex->setPosition( _cardDir->getPtrToVertex(_cardDir->vStrToId("dtNorthEastFront")) );
    vertex = getDtGmshVertexByTag(def->transId(_cardDir->vStrToId("dtNorthEastBack")));
    vertex->setPosition( _cardDir->getPtrToVertex(_cardDir->vStrToId("dtNorthEastBack")) );
    vertex = getDtGmshVertexByTag(def->transId(_cardDir->vStrToId("dtNorthWestBack")));
    vertex->setPosition( _cardDir->getPtrToVertex(_cardDir->vStrToId("dtNorthWestBack")) );

    //
    // edges
    //
    add( 
      new dtGmshEdge(
            this, 
            def->transId(_cardDir->eStrToId("dtNorthFront")), 
            getVertexByTag(def->transId(_cardDir->vStrToId("dtNorthWestFront"))), 
            getVertexByTag(def->transId(_cardDir->vStrToId("dtNorthEastFront")))
      )
    );
    add( 
      new dtGmshEdge(
            this, 
            def->transId(_cardDir->eStrToId("dtNorthEast")), 
            getVertexByTag(def->transId(_cardDir->vStrToId("dtNorthEastFront"))), 
            getVertexByTag(def->transId(_cardDir->vStrToId("dtNorthEastBack")))
      )
    );
    add( 
      new dtGmshEdge(
            this, 
            def->transId(_cardDir->eStrToId("dtNorthBack")), 
            getVertexByTag(def->transId(_cardDir->vStrToId("dtNorthWestBack"))), 
            getVertexByTag(def->transId(_cardDir->vStrToId("dtNorthEastBack")))
      )
    );
    add( 
      new dtGmshEdge(
            this, 
            def->transId(_cardDir->eStrToId("dtNorthWest")), 
            getVertexByTag(def->transId(_cardDir->vStrToId("dtNorthWestFront"))), 
            getVertexByTag(def->transId(_cardDir->vStrToId("dtNorthWestBack")))
      )
    );

    edge = getDtGmshEdgeByTag(def->transId(_cardDir->eStrToId("dtNorthFront")));
    edge->setMap1dTo3d( _cardDir->getPtrToMap1dTo3d(_cardDir->eStrToId("dtNorthFront")) );
    edge = getDtGmshEdgeByTag(def->transId(_cardDir->eStrToId("dtNorthEast")));
    edge->setMap1dTo3d( _cardDir->getPtrToMap1dTo3d(_cardDir->eStrToId("dtNorthEast")) );
    edge = getDtGmshEdgeByTag(def->transId(_cardDir->eStrToId("dtNorthBack")));
    edge->setMap1dTo3d( _cardDir->getPtrToMap1dTo3d(_cardDir->eStrToId("dtNorthBack")) );
    edge = getDtGmshEdgeByTag(def->transId(_cardDir->eStrToId("dtNorthWest")));
    edge->setMap1dTo3d( _cardDir->getPtrToMap1dTo3d(_cardDir->eStrToId("dtNorthWest")) );

    //
    // faces
    //
    edges.clear();
    ori.clear();
    edges.push_back( ::GModel::getEdgeByTag(def->transId(_cardDir->eStrToId("dtNorthFront"))) );
    ori.push_back(1);
    edges.push_back( ::GModel::getEdgeByTag(def->transId(_cardDir->eStrToId("dtNorthEast"))) );
    ori.push_back(1);
    edges.push_back( ::GModel::getEdgeByTag(def->transId(_cardDir->eStrToId("dtNorthBack"))) );
    ori.push_back(-1);
    edges.push_back( ::GModel::getEdgeByTag(def->transId(_cardDir->eStrToId("dtNorthWest"))) );
    ori.push_back(-1);
    add( new dtGmshFace(this, def->transId(_cardDir->fStrToId("dtNorth")), edges, ori) );
    face = getDtGmshFaceByTag(def->transId(_cardDir->fStrToId("dtNorth")));
    face->setMap2dTo3d( _cardDir->getPtrToMap2dTo3d(_cardDir->fStrToId("dtNorth")) );

    /* -------------------------------------------------------------------- */
    //
    // FRONT
    //
    /* -------------------------------------------------------------------- */

    //
    // edges
    //
    add( 
      new dtGmshEdge(
            this, 
            def->transId(_cardDir->eStrToId("dtWestFront")), 
            getVertexByTag(def->transId(_cardDir->vStrToId("dtSouthWestFront"))), 
            getVertexByTag(def->transId(_cardDir->vStrToId("dtNorthWestFront")))
      )
    );
    add( 
      new dtGmshEdge(
            this, 
            def->transId(_cardDir->eStrToId("dtEastFront")), 
            getVertexByTag(def->transId(_cardDir->vStrToId("dtSouthEastFront"))), 
            getVertexByTag(def->transId(_cardDir->vStrToId("dtNorthEastFront")))
      )
    );

    edge = getDtGmshEdgeByTag(def->transId(_cardDir->eStrToId("dtWestFront")));
    edge->setMap1dTo3d( _cardDir->getPtrToMap1dTo3d(_cardDir->eStrToId("dtWestFront")) );
    edge = getDtGmshEdgeByTag(def->transId(_cardDir->eStrToId("dtEastFront")));
    edge->setMap1dTo3d( _cardDir->getPtrToMap1dTo3d(_cardDir->eStrToId("dtEastFront")) );

    //
    // faces
    //
    edges.clear();
    ori.clear();
    edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("dtSouthFront"))) );
    ori.push_back(1);
    edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("dtEastFront"))) );
    ori.push_back(1);
    edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("dtNorthFront"))) );
    ori.push_back(-1);
    edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("dtWestFront"))) );
    ori.push_back(-1);
    add( new dtGmshFace(this, def->transId(_cardDir->fStrToId("dtFront")), edges, ori) );
    face = getDtGmshFaceByTag(def->transId(_cardDir->fStrToId("dtFront")));
    face->setMap2dTo3d( _cardDir->getPtrToMap2dTo3d(_cardDir->fStrToId("dtFront")) );

    /* -------------------------------------------------------------------- */
    //
    // BACK
    //
    /* -------------------------------------------------------------------- */
    //
    // edges
    //
    add( 
      new dtGmshEdge(
            this, 
            def->transId(_cardDir->eStrToId("dtWestBack")), 
            getVertexByTag(def->transId(_cardDir->vStrToId("dtSouthWestBack"))), 
            getVertexByTag(def->transId(_cardDir->vStrToId("dtNorthWestBack")))
      )
    );
    add( 
      new dtGmshEdge(
            this, 
            def->transId(_cardDir->eStrToId("dtEastBack")), 
            getVertexByTag(def->transId(_cardDir->vStrToId("dtSouthEastBack"))), 
            getVertexByTag(def->transId(_cardDir->vStrToId("dtNorthEastBack")))
      )
    );

    edge = getDtGmshEdgeByTag(def->transId(_cardDir->eStrToId("dtWestBack")));
    edge->setMap1dTo3d( _cardDir->getPtrToMap1dTo3d(_cardDir->eStrToId("dtWestBack")) );
    edge = getDtGmshEdgeByTag(def->transId(_cardDir->eStrToId("dtEastBack")));
    edge->setMap1dTo3d( _cardDir->getPtrToMap1dTo3d(_cardDir->eStrToId("dtEastBack")) );

    //
    // faces
    //
    edges.clear();
    ori.clear();
    edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("dtSouthBack"))) );
    ori.push_back(1);
    edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("dtEastBack"))) );
    ori.push_back(1);
    edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("dtNorthBack"))) );
    ori.push_back(-1);
    edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("dtWestBack"))) );
    ori.push_back(-1);
    add( new dtGmshFace(this, def->transId(_cardDir->fStrToId("dtBack")), edges, ori) );
    face = getDtGmshFaceByTag(def->transId(_cardDir->fStrToId("dtBack")));
    face->setMap2dTo3d( _cardDir->getPtrToMap2dTo3d(_cardDir->fStrToId("dtBack")) );

    /* -------------------------------------------------------------------- */
    //
    // WEST
    //
    /* -------------------------------------------------------------------- */
    //
    // faces
    //
    edges.clear();
    ori.clear();
    edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("dtSouthWest"))) );
    ori.push_back(1);
    edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("dtWestBack"))) );
    ori.push_back(1);
    edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("dtNorthWest"))) );
    ori.push_back(-1);
    edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("dtWestFront"))) );
    ori.push_back(-1);
    add( new dtGmshFace(this, def->transId(_cardDir->fStrToId("dtWest")), edges, ori) );
    face = getDtGmshFaceByTag(def->transId(_cardDir->fStrToId("dtWest")));
    face->setMap2dTo3d( _cardDir->getPtrToMap2dTo3d(_cardDir->fStrToId("dtWest")) );

    /* -------------------------------------------------------------------- */
    //
    // EAST
    //
    /* -------------------------------------------------------------------- */
    //
    // faces
    //
    edges.clear();
    ori.clear();
    edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("dtSouthEast"))) );
    ori.push_back(1);
    edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("dtEastBack"))) );
    ori.push_back(1);
    edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("dtNorthEast"))) );
    ori.push_back(-1);
    edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("dtEastFront"))) );
    ori.push_back(-1);
    add( new dtGmshFace(this, def->transId(_cardDir->fStrToId("dtEast")), edges, ori) );
    face = getDtGmshFaceByTag(def->transId(_cardDir->fStrToId("dtEast")));
    face->setMap2dTo3d( _cardDir->getPtrToMap2dTo3d(_cardDir->fStrToId("dtEast")) );
  }
  
  void cardinalDirGModel::convertInternalToGmshModel( float const divideInt, cardinalDirVolumeDefines const * cDDef) {
    cardinalDirVolumeDefines const * def = cDDef;
    if (cDDef == NULL) {
      def = new cardinalDirVolumeDefines(1);
    }    
    /* -------------------------------------------------------------------- */
    //
    // INTERNAL
    //
    /* -------------------------------------------------------------------- */
    if ( _cardDir->hasInternal() ) {
      std::vector< int > ori;
      dtGmshVertex * vertex;
      dtGmshEdge * edge;
      dtGmshFace * face;
      std::list< ::GEdge * > edges;
      //
      // vertices
      //
      add( new dtGmshVertex(this, def->transId(_cardDir->vStrToId("vInternalSouthBack"))) );
      add( new dtGmshVertex(this, def->transId(_cardDir->vStrToId("vInternalSouthFront"))) );
      add( new dtGmshVertex(this, def->transId(_cardDir->vStrToId("vInternalNorthBack"))) );
      add( new dtGmshVertex(this, def->transId(_cardDir->vStrToId("vInternalNorthFront"))) );

      vertex = getDtGmshVertexByTag(def->transId(_cardDir->vStrToId("vInternalSouthBack")));
      vertex->setPosition( _cardDir->getPtrToMap2dTo3d(_cardDir->fStrToId("dtInternal"))->getPointPercent(0.0,0.0) );
      vertex = getDtGmshVertexByTag(def->transId(_cardDir->vStrToId("vInternalSouthFront")));
      vertex->setPosition( _cardDir->getPtrToMap2dTo3d(_cardDir->fStrToId("dtInternal"))->getPointPercent(divideInt, 0.0) );
      vertex = getDtGmshVertexByTag(def->transId(_cardDir->vStrToId("vInternalNorthBack")));
      vertex->setPosition( _cardDir->getPtrToMap2dTo3d(_cardDir->fStrToId("dtInternal"))->getPointPercent(0.0,1.0) );
      vertex = getDtGmshVertexByTag(def->transId(_cardDir->vStrToId("vInternalNorthFront")));
      vertex->setPosition( _cardDir->getPtrToMap2dTo3d(_cardDir->fStrToId("dtInternal"))->getPointPercent(divideInt, 1.0) );

      //
      // edges
      //
      add( 
        new dtGmshEdge(
              this, 
              def->transId(_cardDir->eStrToId("_internalSouthEast")), 
              getVertexByTag(def->transId(_cardDir->vStrToId("vInternalSouthBack"))), 
              getVertexByTag(def->transId(_cardDir->vStrToId("vInternalSouthFront")))
        )
      );
      add( 
        new dtGmshEdge(
              this, 
              def->transId(_cardDir->eStrToId("_internalSouthWest")), 
              getVertexByTag(def->transId(_cardDir->vStrToId("vInternalSouthFront"))), 
              getVertexByTag(def->transId(_cardDir->vStrToId("vInternalSouthBack")))
        )
      );
      add( 
        new dtGmshEdge(
              this, 
              def->transId(_cardDir->eStrToId("_internalNorthEast")),
              getVertexByTag(def->transId(_cardDir->vStrToId("vInternalNorthBack"))), 
              getVertexByTag(def->transId(_cardDir->vStrToId("vInternalNorthFront")))
        )
      );
      add( 
        new dtGmshEdge(
              this, 
              def->transId(_cardDir->eStrToId("_internalNorthWest")), 
              getVertexByTag(def->transId(_cardDir->vStrToId("vInternalNorthFront"))), 
              getVertexByTag(def->transId(_cardDir->vStrToId("vInternalNorthBack")))
        )
      );
      add( 
        new dtGmshEdge(
              this, 
              def->transId(_cardDir->eStrToId("_internalFront")), 
              getVertexByTag(def->transId(_cardDir->vStrToId("vInternalSouthFront"))), 
              getVertexByTag(def->transId(_cardDir->vStrToId("vInternalNorthFront")))
        )
      );
      add( 
        new dtGmshEdge(
              this, 
              def->transId(_cardDir->eStrToId("_internalBack")), 
              getVertexByTag(def->transId(_cardDir->vStrToId("vInternalSouthBack"))), 
              getVertexByTag(def->transId(_cardDir->vStrToId("vInternalNorthBack")))
        )
      );    

      map1dTo3d const * tmpMap;
      edge = getDtGmshEdgeByTag(def->transId(_cardDir->eStrToId("_internalSouthEast")));
      tmpMap = _cardDir->getPtrToMap2dTo3d(_cardDir->fStrToId("dtInternal"))->pickConstVPercent(0,0,divideInt);
      edge->setMap1dTo3d(tmpMap);

      edge = getDtGmshEdgeByTag(def->transId(_cardDir->eStrToId("_internalSouthWest")));
      tmpMap = _cardDir->getPtrToMap2dTo3d(_cardDir->fStrToId("dtInternal"))->pickConstVPercent(0,divideInt,1.0);
      edge->setMap1dTo3d( tmpMap );

      edge = getDtGmshEdgeByTag(def->transId(_cardDir->eStrToId("_internalNorthEast")));
      tmpMap = _cardDir->getPtrToMap2dTo3d(_cardDir->fStrToId("dtInternal"))->pickConstVPercent(1.,0,divideInt);
      edge->setMap1dTo3d( tmpMap );

      edge = getDtGmshEdgeByTag(def->transId(_cardDir->eStrToId("_internalNorthWest")));
      tmpMap = _cardDir->getPtrToMap2dTo3d(_cardDir->fStrToId("dtInternal"))->pickConstVPercent(1.,divideInt,1.0);
      edge->setMap1dTo3d( tmpMap );

      edge = getDtGmshEdgeByTag(def->transId(_cardDir->eStrToId("_internalFront")));
      tmpMap = _cardDir->getPtrToMap2dTo3d(_cardDir->fStrToId("dtInternal"))->pickConstUPercent(divideInt,0.,1.);
      edge->setMap1dTo3d( tmpMap );

      edge = getDtGmshEdgeByTag(def->transId(_cardDir->eStrToId("_internalBack")));
      tmpMap = _cardDir->getPtrToMap2dTo3d(_cardDir->fStrToId("dtInternal"))->pickConstUPercent(0.,0.,1.);
      edge->setMap1dTo3d( tmpMap );

      //
      // faces
      //
      edges.clear();
      ori.clear();
      edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("_internalSouthEast"))) );
      ori.push_back(1);
      edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("_internalFront"))) );
      ori.push_back(1);      
      edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("_internalNorthEast"))) );
      ori.push_back(-1);
      edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("_internalBack"))) );
      ori.push_back(-1);      
      add( new dtGmshFace(this, def->transId(_cardDir->fStrToId("sInternalEast")), edges, ori) );
      face = getDtGmshFaceByTag(def->transId(_cardDir->fStrToId("sInternalEast")));
      ptrHandling< map2dTo3d > mm2d(
        _cardDir->getPtrToMap2dTo3d(
          _cardDir->fStrToId("dtInternal")
        )->pickPercent(0., divideInt, 0., 1.)
      );
      face->setMap2dTo3d( mm2d.get() );

      edges.clear();
      ori.clear();
      edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("_internalSouthWest"))) );
      ori.push_back(1);
      edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("_internalBack"))) );
      ori.push_back(1);      
      edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("_internalNorthWest"))) );
      ori.push_back(-1);
      edges.push_back( getEdgeByTag(def->transId(_cardDir->eStrToId("_internalFront"))) );
      ori.push_back(-1);      
      ::GModel::add( new dtGmshFace(this, def->transId(_cardDir->fStrToId("sInternalWest")), edges, ori) );
      face = getDtGmshFaceByTag(def->transId(_cardDir->fStrToId("sInternalWest")));
      mm2d.reset(
        _cardDir->getPtrToMap2dTo3d(
          _cardDir->fStrToId("dtInternal")
        )->pickPercent(divideInt, 1.0, 0, 1.) 
      );
      face->setMap2dTo3d( mm2d.get() );

      //
      // internal edges
      //
      ::GFace * gfP;
      ::GEdge * geP;
      ::GVertex * gvP;

      gfP = getFaceByTag(def->transId(_cardDir->fStrToId("dtSouth")));
      geP = getEdgeByTag(def->transId(_cardDir->eStrToId("_internalSouthEast")));
      dynamic_cast< dtGmshFace * >(gfP)->addEdge(geP, 1);
      geP = getEdgeByTag(def->transId(_cardDir->eStrToId("_internalSouthWest")));
      dynamic_cast< dtGmshFace * >(gfP)->addEdge(geP, 1);

      gfP = getFaceByTag(def->transId(_cardDir->fStrToId("dtNorth")));
      geP = getEdgeByTag(def->transId(_cardDir->eStrToId("_internalNorthEast")));
      dynamic_cast< dtGmshFace * >(gfP)->addEdge(geP, 1);
      geP = getEdgeByTag(def->transId(_cardDir->eStrToId("_internalNorthWest")));
      dynamic_cast< dtGmshFace * >(gfP)->addEdge(geP, 1);
    }    
  }
    
  void cardinalDirGModel::convertToGmshModel( cardinalDirVolumeDefines const * cDDef ) {
    cardinalDirVolumeDefines const * def = cDDef;
    if (cDDef == NULL) {
      def = new cardinalDirVolumeDefines(1);
    }    
    //
    // push outer and internal to model
    //
    convertOuterToGmshModel();
    convertInternalToGmshModel();
    
    /* -------------------------------------------------------------------- */
    //
    // VOLUME
    //
    /* -------------------------------------------------------------------- */
    std::vector< int > ori;
    std::list< ::GFace * > faces;
    faces.push_back( getFaceByTag(def->transId(_cardDir->fStrToId("dtNorth"))) );
    ori.push_back(1);
    faces.push_back( getFaceByTag(def->transId(_cardDir->fStrToId("dtWest"))) );
    ori.push_back(1);
    faces.push_back( getFaceByTag(def->transId(_cardDir->fStrToId("dtSouth"))) );
    ori.push_back(1);
    faces.push_back( getFaceByTag(def->transId(_cardDir->fStrToId("dtEast"))) );
    ori.push_back(1);
    faces.push_back( getFaceByTag(def->transId(_cardDir->fStrToId("dtFront"))) );
    ori.push_back(1);
    faces.push_back( getFaceByTag(def->transId(_cardDir->fStrToId("dtBack"))) );
    ori.push_back(1);
    if ( _cardDir->hasInternal() ) {
      faces.push_back( getFaceByTag(def->transId(_cardDir->fStrToId("sInternalEast"))) );
      ori.push_back(-1);
      faces.push_back( getFaceByTag(def->transId(_cardDir->fStrToId("sInternalWest"))) );
      ori.push_back(-1);    
    }
    
    dtGmshModel::add( new dtGmshRegion(this, 1, faces, ori) );
  }
   
  void cardinalDirGModel::addEdgeToGmshModel( 
    analyticGeometry const * const edge, 
    int const tag, 
    int const from, 
    int const to
  ) {
    map1dTo3d const * sC;
    dt__mustCast(edge, map1dTo3d const, sC);
    addEdgeToGmshModel(sC, tag, from, to);
  }  
}
