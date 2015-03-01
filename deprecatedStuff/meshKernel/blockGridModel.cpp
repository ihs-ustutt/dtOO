#include "blockGridModel.h"

#include <logMe/logMe.h>
#include "boundedVolume.h"
#include "gmsh/MQuadrangle.h"
#include "alg/coVectField.h"
#include <functionHeaven/vec3dOneD.h>
#include <functionHeaven/vec3dCurveOneD.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <analyticGeometryHeaven/vec3dOneDInMap3dTo3d.h>
#include <geometryEngine/geoBuilder/trimmedCurve_twoPointsConnectConstructOCC.h>
#include <geometryEngine/dtCurve.h>
#include <dtGmshVertex.h>
#include <dtGmshEdge.h>
#include <dtGmshFace.h>
#include <dtGmshTransPartArea.h>
#include <dtGmshTransFace.h>
#include <gmsh/MElement.h>
#include <gmsh/MTetrahedron.h>
#include <gmsh/MHexahedron.h>
#include <vector>
#include <interfaceHeaven/ptrHandling.h>
#include <do/coDoSet.h>
#include <do/coDoUnstructuredGrid.h>
#include <do/coDoIntArr.h>
#include <interfaceHeaven/coDoSetHandling.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <gmsh/MVertex.h>

#define __caCThis \
  const_cast< blockGridModel * >(this)

namespace dtOO { 
  //float blockGridModel::_snapTol = 1.e-08;
    
  blockGridModel::blockGridModel(std::string name) : cardinalDirGModel(name) {
    _snapTol 
    =
    staticPropertiesHandler::getInstance()->getOptionFloat("snapTol");    
  }

  blockGridModel::~blockGridModel() {
  }
  
  void blockGridModel::addDtGmshVertex(int const vId) {
    GModel::add( new dtGmshVertex(this, vId) );    
  }

  void blockGridModel::snapVertex(int const vId, int const aSId) {
    if (!getAttachedBoundedVolume()) {
      dt__THROW(snapVertex(),
              << "blockGridModel is not attached." << LOGDEL
              << DTLOGEVAL(getAttachedBoundedVolume()));
    }
    ::GVertex * gv = ::GModel::getVertexByTag(vId);
    dtGmshVertex * dtGV;
    dt__MUSTDOWNCAST(gv, dtGmshVertex, dtGV);
    dtGV->snapToMap2dTo3d( getAttachedBoundedVolume()->getPtrToMap2dTo3d(aSId) );
  }

  void blockGridModel::snap3dVertex(int const vId, int const rId) {
    if (!getAttachedBoundedVolume()) {
      dt__THROW(snap3dVertex(),
              << "blockGridModel is not attached." << LOGDEL
              << DTLOGEVAL(getAttachedBoundedVolume()));
    }
    ::GVertex * gv = ::GModel::getVertexByTag(vId);
    dtGmshVertex * dtGV;
    dt__MUSTDOWNCAST(gv, dtGmshVertex, dtGV);
    dtGV->snapToMap3dTo3d( getAttachedBoundedVolume()->getPtrToMap3dTo3d(rId) );
  }
  
  void blockGridModel::snapVertexPercent(int const vId, int const aSId, dtPoint2 const ppUV) {
    if (!getAttachedBoundedVolume()) {
      dt__THROW(snapVertexPercent(),
              << "blockGridModel is not attached." << LOGDEL
              << DTLOGEVAL(getAttachedBoundedVolume()));
    }
    ::GVertex * gv = ::GModel::getVertexByTag(vId);
    dtGmshVertex * dtGV;
    dt__MUSTDOWNCAST(gv, dtGmshVertex, dtGV);
    dtGV->snapToMap2dTo3d( getAttachedBoundedVolume()->getPtrToMap2dTo3d(aSId) );
    dtGV->setPosition( 
      getAttachedBoundedVolume()->getPtrToMap2dTo3d(aSId)->getPointPercent( ppUV.x(), ppUV.y() ) 
    );
  }

  void blockGridModel::addDtGmshEdge(int const eId, int const from, int const to) {
    dtGmshEdge * dtGE = new dtGmshEdge(
                              this, 
                              eId, 
                              ::GModel::getVertexByTag(from), 
                              ::GModel::getVertexByTag(to)
                            );
    ::GModel::add(dtGE);  
  }

  void blockGridModel::addDtGmshFace(int const fId, int const e0, int const e1, int const e2, int const e3) {
    if ( ::GModel::getFaceByTag(fId) ) {
      return;
    }
    else {
      std::vector< ::GEdge * > edgesV;
      edgesV.push_back( getDtGmshEdgeByTag(e0) );
      edgesV.push_back( getDtGmshEdgeByTag(e1) );
      edgesV.push_back( getDtGmshEdgeByTag(e2) );
      edgesV.push_back( getDtGmshEdgeByTag(e3) );
      
      dtGmshVertex * dGV0 = getDtGmshVertexByTag( edgesV[0]->getBeginVertex()->tag() );
      dtGmshVertex * dGV1 = getDtGmshVertexByTag( edgesV[0]->getEndVertex()->tag() );
      dtGmshVertex * dGV2 = getDtGmshVertexByTag( edgesV[2]->getBeginVertex()->tag() );
      dtGmshVertex * dGV3 = getDtGmshVertexByTag( edgesV[2]->getEndVertex()->tag() );
      std::vector< map2dTo3d const * > const & snap0 = dGV0->getRefToSnapMap2dTo3d();
      std::vector< map2dTo3d const * > const & snap1 = dGV1->getRefToSnapMap2dTo3d();
      std::vector< map2dTo3d const * > const & snap2 = dGV2->getRefToSnapMap2dTo3d();
      std::vector< map2dTo3d const * > const & snap3 = dGV3->getRefToSnapMap2dTo3d();
      
      std::vector< int > snap0Index;
      for (int ii=0; ii<snap0.size(); ii++) {
        for (int jj=0; jj<snap1.size(); jj++) {
          if (snap0[ii] == snap1[jj]) {
            for (int kk=0; kk<snap2.size(); kk++) {
              if (snap2[kk] == snap1[jj]) {
                for (int ll=0; ll<snap3.size(); ll++) {
                  if ( (snap2[kk] == snap3[ll]) ) {
                    snap0Index.push_back(ii);
                  }
                }
              }
            }
          } 
        } 
      }
      
      std::list< ::GEdge * > edges;
      std::vector< int > ori;
      edges.push_back( edgesV[0] );
      edges.push_back( edgesV[1] );
      edges.push_back( edgesV[2] );
      edges.push_back( edgesV[3] );
//      if ( edgesV[3]->getEndVertex()->tag() == edgesV[0]->getBeginVertex()->tag() ) {
//        ori.push_back(1);
//      }
//      else {
//        ori.push_back(-1);
//      }
      int tmpId;
      if ( edgesV[0]->getEndVertex()->tag() == edgesV[1]->getBeginVertex()->tag() ) {
        ori.push_back(1);
        ori.push_back(1);
        tmpId = edgesV[1]->getEndVertex()->tag();
      }
      else if ( edgesV[0]->getEndVertex()->tag() == edgesV[1]->getEndVertex()->tag() ) {
        ori.push_back(1);
        ori.push_back(-1);          
        tmpId = edgesV[1]->getBeginVertex()->tag();
      }
      else if ( edgesV[0]->getBeginVertex()->tag() == edgesV[1]->getBeginVertex()->tag() ) {
        ori.push_back(-1);
        ori.push_back(1);    
        tmpId = edgesV[1]->getEndVertex()->tag();        
      }
      else if ( edgesV[0]->getBeginVertex()->tag() == edgesV[1]->getEndVertex()->tag() ) {
        ori.push_back(-1);
        ori.push_back(-1);          
        tmpId = edgesV[1]->getBeginVertex()->tag();        
      }        
      
      if ( edgesV[2]->getBeginVertex()->tag() == tmpId ) {
        ori.push_back(1);
        tmpId = edgesV[2]->getEndVertex()->tag();
      }
      else {
        ori.push_back(-1);
        tmpId = edgesV[2]->getBeginVertex()->tag();
      }
      
      if ( edgesV[3]->getBeginVertex()->tag() == tmpId ) {
        ori.push_back(1);
      }
      else {
        ori.push_back(-1);
      }      
      
      if (snap0Index.size() == 1) {
        dtGmshTransPartArea * tPA = new dtGmshTransPartArea(this, fId, edges, ori);
        tPA->setMap2dTo3d( snap0[snap0Index[0]] );
        ::GModel::add(tPA);
        tPA->setMotherSurfaceLabel( snap0[snap0Index[0]]->getLabel() );
      }
      else {
        dtGmshTransFace * tPA = new dtGmshTransFace(this, fId, edges, ori);
        ::GModel::add(tPA);
      }
      DTINFOWF(addFace(), 
        << "dtGmshFace[" << fId << "] : " 
        << getDtGmshFaceByTag(fId)->ClassName() 
      );
    }
  }
  
  /**
   * 
   * @todo: Fix usage of splineCurve3d. Maybe put a function on map1dTo3d
   * that creates a straight line?
   */
  bool blockGridModel::snapEdge(int const eID) {
    bool retBool = false;
    dtGmshEdge * dtGE = getDtGmshEdgeByTag(eID);
    dtGmshVertex * dtGV0;
    dt__MUSTDOWNCAST(dtGE->getBeginVertex(), dtGmshVertex, dtGV0);
    dtGmshVertex * dtGV1;
    dt__MUSTDOWNCAST(dtGE->getEndVertex(), dtGmshVertex, dtGV1);
    
    std::vector< map2dTo3d const *> const & aS0 = dtGV0->getRefToSnapMap2dTo3d();
    std::vector< map2dTo3d const *> const & aS1 = dtGV1->getRefToSnapMap2dTo3d();
    
    std::vector< int > aS0Snap;
    for (int ii=0; ii<aS0.size(); ii++) {
      for (int jj=0; jj<aS1.size(); jj++) {  
        if (aS0[ii] == aS1[jj]) {
          aS0Snap.push_back( ii );
        }
      }
    }

    //
    // output
    //
    DTBUFFERINIT();
    DTBUFFER(
      << "Try to snap edge " << DTLOGEVAL(eID) << "." << LOGDEL
      << DTLOGEVAL( aS0Snap.size() ) << LOGDEL 
      << "Edge is part of following faces:" << LOGDEL
    );
    dt__FORALL(aS0Snap, ii,
      DTBUFFER( << aS0[aS0Snap[ii]]->getLabel() << LOGDEL);
    );
      
    if (aS0Snap.size() == 1) {
      int ii = aS0Snap[0];            
      if ( !aS0[ii]->isClosedU() && !aS0[ii]->isClosedV() ) {
        DTBUFFER( << "Adding line in face.");      
        dtPoint2 dtGV0UV = aS0[ii]->reparamOnFace( 
                            dtPoint3(
                              dtGV0->x(), 
                              dtGV0->y(), 
                              dtGV0->z()
                            ) 
                          );
        dtPoint2 dtGV1UV = aS0[ii]->reparamOnFace( 
                            dtPoint3(
                              dtGV1->x(), 
                              dtGV1->y(), 
                              dtGV1->z()
                            ) 
                          );
        ptrHandling< map1dTo3d > dtB( 
          aS0[ii]->pickLinearUV(
            dtGV0UV.x(), 
            dtGV0UV.y(), 
            dtGV1UV.x(), 
            dtGV1UV.y()
          ) 
        );
        dtGE->setMap1dTo3d( dtB.get() );

        retBool = true;
      }
    }
    else if (aS0Snap.size() > 1) {      
      DTBUFFER( << "Adding straight line." << LOGDEL
                << "Edge is part of more than one face." << LOGDEL);            
      //
      // create straight line
      //
      ptrHandling< map1dTo3d > dtB( 
			  new splineCurve3d(			
					ptrHandling<dtCurve>(
						trimmedCurve_twoPointsConnectConstructOCC(
							dtPoint3(dtGV0->x(), dtGV0->y(), dtGV0->z()),
							dtPoint3(dtGV1->x(), dtGV1->y(), dtGV1->z())
						).result()      
					).get()
			  ) 
			);
      dtGE->setMap1dTo3d( dtB.get() );
    }
    else if (aS0Snap.size() < 1) {      
      DTBUFFER( << "Adding straight line." << LOGDEL
                << "Edge is treated as an internal edge" << LOGDEL);
      if ( (dtGV0->getRefToSnapMap3dTo3d().size() != 0) 
              && (dtGV1->getRefToSnapMap3dTo3d().size() != 0) ) {
        DTBUFFER( << "Vertices snapped to map3dTo3d" << LOGDEL);
        dtPoint3 p0UVW 
        = 
        dtGV0->getRefToSnapMap3dTo3d()[0]->reparamInVolume(
          dtGV0->cast2DtPoint3()
        );
        dtPoint3 p1UVW 
        = 
        dtGV1->getRefToSnapMap3dTo3d()[0]->reparamInVolume(
          dtGV1->cast2DtPoint3()
        );    
        
        ptrHandling< dtCurve > dtC( 
				  trimmedCurve_twoPointsConnectConstructOCC(p0UVW, p1UVW).result() 
				);
				ptrHandling< vec3dOneD > v3d1d( new vec3dCurveOneD(dtC.get()) );
				
        ptrHandling< map1dTo3d > dtB( 
          new vec3dOneDInMap3dTo3d(
            v3d1d.get(), 
            dtGV1->getRefToSnapMap3dTo3d()[0]
          ) 
        );      
        
        dtGE->setMap1dTo3d( dtB.get() );
      }
      else {
      DTBUFFER( << "Adding straight line." << LOGDEL
                << "Edge is treated as free edge" << LOGDEL);				
        //
        // create straight line
        //
      ptrHandling< map1dTo3d > dtB( 
			  new splineCurve3d(			
					ptrHandling<dtCurve>(
						trimmedCurve_twoPointsConnectConstructOCC(
							dtPoint3(dtGV0->x(), dtGV0->y(), dtGV0->z()),
							dtPoint3(dtGV1->x(), dtGV1->y(), dtGV1->z())
						).result()      
					).get()
			  ) 
			);
        
        dtGE->setMap1dTo3d( dtB.get() );
      }

      retBool = true;      
    }
    DTWARNINGWF_BUFFER(snapEdge());    
    
    return retBool;
  }

  bool blockGridModel::forceSnapEdge(int const eID) {
    bool retBool = false;
    dtGmshEdge * dtGE = getDtGmshEdgeByTag(eID);
    dtGmshVertex * dtGV0;
    dt__MUSTDOWNCAST(dtGE->getBeginVertex(), dtGmshVertex, dtGV0);
    dtGmshVertex * dtGV1;
    dt__MUSTDOWNCAST(dtGE->getEndVertex(), dtGmshVertex, dtGV1);
    
    std::vector< map2dTo3d const *> const & aS0 = dtGV0->getRefToSnapMap2dTo3d();
    std::vector< map2dTo3d const *> const & aS1 = dtGV1->getRefToSnapMap2dTo3d();
    
    std::vector< int > aS0Snap;
    for (int ii=0; ii<aS0.size(); ii++) {
      for (int jj=0; jj<aS1.size(); jj++) {  
        if (aS0[ii] == aS1[jj]) {
          aS0Snap.push_back( ii );
        }
      }
    }

    //
    // output
    //
    DTBUFFERINIT();
    DTBUFFER(
      << "Try to force snap edge " << DTLOGEVAL(eID) << "." << LOGDEL
      << DTLOGEVAL( aS0Snap.size() ) << LOGDEL 
      << "Edge is part of following faces:" << LOGDEL
    );
    dt__FORALL(aS0Snap, ii,
      DTBUFFER( << aS0[aS0Snap[ii]]->getLabel() << LOGDEL);
    );
      
//    if (aS0Snap.size() > 1) {      
    DTBUFFER( << "Check if edge is located at a boundary." << LOGDEL);
    for (int ii=0; ii<aS0Snap.size(); ii++) {
      map2dTo3d const * const aS = aS0[aS0Snap[ii]];      
      dtPoint2 dtGV0UV = aS->reparamPercentOnFace( dtGV0->cast2DtPoint3() );
      dtPoint2 dtGV1UV = aS->reparamPercentOnFace( dtGV1->cast2DtPoint3() );
//      bool atBound = true;
      bool atUBound0 = ( ( fabs( dtGV0UV.x()-1. ) < _snapTol ) 
                        || ( fabs( dtGV0UV.x()-0. ) < _snapTol )
                      );
      bool atUBound1 = ( ( fabs( dtGV1UV.x()-1. ) < _snapTol ) 
                        || ( fabs( dtGV1UV.x()-0. ) < _snapTol )
                      );
      bool atVBound0 = ( ( fabs( dtGV0UV.y()-1. ) < _snapTol ) 
                        || ( fabs( dtGV0UV.y()-0. ) < _snapTol )
                      );
      bool atVBound1 = ( ( fabs( dtGV1UV.y()-1. ) < _snapTol ) 
                        || ( fabs( dtGV1UV.y()-0. ) < _snapTol )
                      );
      bool atUSeam0 = atUBound0 && aS->isClosedU();
      bool atUSeam1 = atUBound1 && aS->isClosedU();
      bool atVSeam0 = atVBound0 && aS->isClosedV();
      bool atVSeam1 = atVBound1 && aS->isClosedV();  

      bool atSeam = false;
      bool vertexOnUSeam = false;
      bool vertexOnVSeam = false;
      bool atBound = false;

      atSeam = (
                 (atUSeam0 && atUSeam1 && aS->isClosedU()) 
                  || (atVSeam0 && atVSeam1 && aS->isClosedV()) 
               );      
      if (!atSeam) {
        vertexOnUSeam = ( (atUSeam0 || atUSeam1) && aS->isClosedU() );
        vertexOnVSeam = ( (atVSeam0 || atVSeam1) && aS->isClosedV() );
        if (!vertexOnUSeam && !vertexOnVSeam) {
          atBound = ( (atUBound0 && atUBound1 && !aS->isClosedU()) 
                      || (atVBound0 && atVBound1 && !aS->isClosedV()) 
                    );
        }
      }

      DTBUFFER(
//							<< DTLOGEVAL(dtGE->getBeginVertex()->tag()) << LOGDEL
//							<< DTLOGEVAL(dtGE->getEndVertex()->tag()) << LOGDEL
//							<< DTLOGPOI2D(dtGV0UV) << LOGDEL
//							<< DTLOGPOI2D(dtGV1UV) << LOGDEL
              << DTLOGEVAL(atUSeam0) << LOGDEL
              << DTLOGEVAL(atUSeam1) << LOGDEL
              << DTLOGEVAL(atVSeam0) << LOGDEL
              << DTLOGEVAL(atVSeam1) << LOGDEL
              << DTLOGEVAL(aS->isClosedU()) << LOGDEL
              << DTLOGEVAL(aS->isClosedV()) << LOGDEL
              << DTLOGEVAL(atBound) << LOGDEL
              << DTLOGEVAL(atSeam) << LOGDEL
              << DTLOGEVAL(vertexOnUSeam) << LOGDEL
              << DTLOGEVAL(vertexOnVSeam) << LOGDEL 
//              << DTLOGEVAL(fabs( dtGV0UV.x()-1. ) ) << LOGDEL
//							<< DTLOGEVAL(fabs( dtGV0UV.x()-0. ) ) << LOGDEL
//							<< DTLOGEVAL(fabs( dtGV1UV.x()-1. ) ) << LOGDEL 
//							<< DTLOGEVAL(fabs( dtGV1UV.x()-0. ) ) << LOGDEL
//							<< DTLOGEVAL(fabs( dtGV0UV.y()-1. ) ) << LOGDEL
//							<< DTLOGEVAL(fabs( dtGV0UV.y()-0. ) ) << LOGDEL
//							<< DTLOGEVAL(fabs( dtGV1UV.y()-1. ) ) << LOGDEL
//							<< DTLOGEVAL(fabs( dtGV1UV.y()-0. ) )<< LOGDEL	
      );
        
      //
      // do snapping
      //
      if ( atBound ) {
        DTBUFFER( << "Adding line at boundary in face.");
        dtPoint2 dtGV0UV = aS->reparamOnFace( dtGV0->cast2DtPoint3() );
        dtPoint2 dtGV1UV = aS->reparamOnFace( dtGV1->cast2DtPoint3() );      

        ptrHandling< map1dTo3d > dtB( 
                                       aS->pickLinearUV(
                                         dtGV0UV.x(), 
                                         dtGV0UV.y(), 
                                         dtGV1UV.x(), 
                                         dtGV1UV.y()
                                       ) 
                                     );
        dtGE->setMap1dTo3d( dtB.get() );

        retBool = true;

        break;
      }
      else if ( atSeam ) {
        DTBUFFER( << "Adding seam in face." << LOGDEL);      
        dtPoint2 dtGV0UV = aS->reparamPercentOnFace( dtGV0->cast2DtPoint3() );
        dtPoint2 dtGV1UV = aS->reparamPercentOnFace( dtGV1->cast2DtPoint3() );

        if ( aS->isClosedU() ) {
          dtGV0UV = dtPoint2(0., dtGV0UV.y());
          dtGV1UV = dtPoint2(0., dtGV1UV.y());
          dtGE->makeSeamU();
        }
        else if ( aS->isClosedV() ) {
          dtGV0UV = dtPoint2(dtGV0UV.x(), 0.);
          dtGV1UV = dtPoint2(dtGV1UV.x(), 0.);
          dtGE->makeSeamV();
        }
        else {
          DTBUFFER( << "aS is not closed!" << LOGDEL
                  << DTLOGEVAL(aS->isClosedU()) << LOGDEL
                  << DTLOGEVAL(aS->isClosedV()) );
          dt__THROW_BUFFER(forceSnapEdge());          
        }
        DTBUFFER( << DTLOGPOI2D(dtGV0UV) << LOGDEL
                  << DTLOGPOI2D(dtGV1UV) );
        
        ptrHandling< map1dTo3d > dtB( 
          aS->pickLinearPercent(
            dtGV0UV.x(), 
            dtGV0UV.y(), 
            dtGV1UV.x(), 
            dtGV1UV.y()
          ) 
        );
        dtGE->setMap1dTo3d( dtB.get() );

        retBool = true;        
        
        break;
      }      
      else if ( vertexOnUSeam && !vertexOnVSeam) {
        DTBUFFER( << "Adding line with vertex on seam in face." << LOGDEL);      
        dtPoint2 dtGV0UV = aS->reparamPercentOnFace( dtGV0->cast2DtPoint3() );
        dtPoint2 dtGV1UV = aS->reparamPercentOnFace( dtGV1->cast2DtPoint3() );
        if (atUSeam0) {
          if ( (dtGV1UV.x()-0.) < (1.-dtGV1UV.x()) ) {
            dtGV0UV = dtPoint2(0., dtGV0UV.y());
          }
          else {
            dtGV0UV = dtPoint2(1., dtGV0UV.y());            
          }
        }
        else {
          if ( (dtGV0UV.x()-0.) < (1.-dtGV0UV.x()) ) {
            dtGV1UV = dtPoint2(0., dtGV1UV.y());
          }
          else {
            dtGV1UV = dtPoint2(1., dtGV1UV.y());            
          }          
        }

        ptrHandling< map1dTo3d > dtB( 
          aS->pickLinearPercent(
            dtGV0UV.x(), 
            dtGV0UV.y(), 
            dtGV1UV.x(), 
            dtGV1UV.y()
          ) 
        );
        dtGE->setMap1dTo3d( dtB.get() );
        
        retBool = true;
        
        break;
      }
      else if ( !vertexOnUSeam && vertexOnVSeam) {
        DTBUFFER( << "Case not yet implemented!" << LOGDEL);
        dt__THROW_BUFFER(forceSnapEdge());
      }      
      else {
        DTBUFFER( << "Adding not critical line in face." << LOGDEL);      
        dtPoint2 dtGV0UV = aS->reparamOnFace( dtGV0->cast2DtPoint3() );
        dtPoint2 dtGV1UV = aS->reparamOnFace( dtGV1->cast2DtPoint3() );
        ptrHandling< map1dTo3d > dtB( 
                                       aS->pickLinearUV(
                                         dtGV0UV.x(), 
                                         dtGV0UV.y(), 
                                         dtGV1UV.x(), 
                                         dtGV1UV.y()
                                       ) 
                                     );
        dtGE->setMap1dTo3d( dtB.get() );

        retBool = true;        
        
        break;
      }
    }
    DTWARNINGWF_BUFFER(forceSnapEdge());    
    
    return retBool;
  }
  
  covise::coDoSet * blockGridModel::toCoDoSet(char const * const str) const {
    coDoSetHandling< analyticGeometry const * > aG;

    for (eiter e_it = __caCThis->firstEdge(); e_it != __caCThis->lastEdge(); ++e_it) {
      aG.push_back( this->cast2DtGmshEdge(*e_it)->getMap1dTo3d() );
			aG.back()->extRender(true);
			aG.back()->setRenderResolution(0, 100);
    }
    covise::coDoSet * set = aG.render3d(aG, str);
        
    return set;
    
  }
  /**
   * 
   * @todo: Fix non-supported polyhedra.
   * @todo: Add writing of hexahedra, prisms and pyramids.
   *  
   */
  covise::coDoSet * blockGridModel::toCoDoUnstructuredGrid(char const * const str) const {
//    DTBUFFERINIT();
    
    std::vector< unsigned > numElements(5,0);
    /*
     * cast away constness by macro, because getNumMeshElements 
     * is not a const routine
     */
    __caCThis->getNumMeshElements( &(numElements[0]) );
    int nElemTot = numElements[0] + numElements[1] + numElements[2] + numElements[3] + numElements[4];
//    int nElemQuad = 0;
//    for (fiter f_it = __caCThis->firstFace(); f_it != __caCThis->lastFace(); ++f_it) {
//      GFace * gf = *f_it;
//      nElemQuad = nElemQuad + gf->getNumMeshElements();
//    }
    //nElemTot = nElemQuad;
    
    DTINFOWF(toCoDoUnstructuredGrid(),
      << "tetrahedra = " << numElements[0] << LOGDEL
      << "hexahedra = " << numElements[1] << LOGDEL
      << "prisms = " << numElements[2] << LOGDEL
      << "pyramids = " << numElements[3] << LOGDEL
      << "polyhedra = " << numElements[4] << LOGDEL
    //  << "quadrangle = " << nElemQuad
    );
    
   
    //bool testBool = true;
//    int numVertis = 99;//__caCThis->indexMeshVertices(testBool);
//    DTINFOWF(toCoDoUnstructuredGrid(),
//      << "Anzahl Veritces = " << numVertis );
    
    ptrHandling< covise::coDoUnstructuredGrid > cug( 
      new covise::coDoUnstructuredGrid( 
        "grid",
        nElemTot, 
          numElements[0]*0//4 //tetrahedra
        + numElements[1]*8 //hexahedra
        + numElements[2]*0//9 //prisms
        + numElements[3]*0//5 //pyramids
        + numElements[4]*0, //polyhedra - not yet supported
//        + nElemQuad*0, //4, //surface mesh
        getNumMeshVertices(), 
        1 
      )
    );
    int * elem;
    int * conn;
    float * xx;
    float * yy;
    float * zz;
    int * tl;
        
    cug->getAddresses(&elem, &conn, &xx, &yy, &zz);
    cug->getTypeList(&tl);

    
        
    for( int ii=0; ii<getNumMeshVertices(); ii++ ) {
      ::MVertex const * const mv = __caCThis->getMeshVertexByTag(ii+1);
//      xx[ii] = static_cast< float >(mv->x());
//      yy[ii] = static_cast< float >(mv->y());
//      zz[ii] = static_cast< float >(mv->z());
      xx[mv->getIndex()-1] = static_cast< float >(mv->x());
      yy[mv->getIndex()-1] = static_cast< float >(mv->y());
      zz[mv->getIndex()-1] = static_cast< float >(mv->z());
       DTINFOWF(toCoDoUnstructuredGrid(),
                << DTLOGEVAL(ii) << LOGDEL
               << DTLOGEVAL(mv->getIndex()-1) << LOGDEL
                  << DTLOGEVAL(mv->x()) << LOGDEL
                  << DTLOGEVAL(mv->y()) << LOGDEL         
                  << DTLOGEVAL(mv->z()) );
    }

    int cellC = 0;
    int connC = 0;  
    

    std::vector< int > dimsPatchVec; 
    
    std::vector< int > allConnValues;
    

    
    
    
    for (riter r_it = __caCThis->firstRegion(); r_it != __caCThis->lastRegion(); ++r_it) {
        ::GRegion * gr = *r_it;
      
      
      if (r_it == __caCThis->firstRegion()) { //das muss dann raus und intArr in Vector schreiben oder so
          std::list<::GFace*>::iterator f_it;
          std::list<::GFace*> myFaces = (*r_it)->faces();
          for (f_it = myFaces.begin(); f_it!=myFaces.end(); ++f_it) {
//          for (fiter f_it = __caCThis->firstFace(); f_it != __caCThis->lastFace(); ++f_it) {
        ::GFace * gf = *f_it;
        
        dimsPatchVec.push_back( ( gf->getNumMeshElements() ) * 4 ); //"4" nur für quads
        
        DTINFOWF(toCoDoUnstructuredGrid(),
                  << DTLOGEVAL(gf->getNumMeshElements()) << LOGDEL
                  << DTLOGEVAL(dimsPatchVec.size() ) << LOGDEL
                  << DTLOGEVAL(gf->numRegions()) << LOGDEL
                  << DTLOGEVAL(gf->getNumMeshVertices())
                );
        
//        
        
        std::vector< int > faceConnValues ( gf->getNumMeshElements() * 4 );
        int faceCount=0;
        
        for( int ii=0; ii<gf->getNumMeshElements(); ii++ ) {
          ::MElement * faceME = gf->getMeshElement(ii);
          ::MQuadrangle * mquad = dynamic_cast< ::MQuadrangle * >(faceME);
          //
          // quadrangle
          //
          if ( mquad ) {
              std::vector< ::MVertex * > verts;
              mquad->getVertices(verts); 
              faceConnValues[faceCount+0] = verts[0]->getIndex()-1;
              faceConnValues[faceCount+1] = verts[1]->getIndex()-1;
              faceConnValues[faceCount+2] = verts[2]->getIndex()-1;
              faceConnValues[faceCount+3] = verts[3]->getIndex()-1;
              faceCount = faceCount + 4;
              
              DTINFOWF(toCoDoUnstructuredGrid(),
                  << DTLOGEVAL(verts[0]->getIndex()-1) << LOGDEL
                  << DTLOGEVAL(verts[1]->getIndex()-1) << LOGDEL
                  << DTLOGEVAL(verts[2]->getIndex()-1) << LOGDEL
                  << DTLOGEVAL(verts[3]->getIndex()-1) );
              

          }
        }
        for (int aa=0; aa<faceConnValues.size(); aa++) {
            allConnValues.push_back(faceConnValues[aa]);
        }
//        allConnValues.push_back(faceConnValues);
//        faceElemNumTot = faceElemNumTot + faceConnValues.size()
        faceConnValues.clear();

      }
      }
        
        
      
        
      for( int ii=0; ii<gr->getNumMeshElements(); ii++ ) {
        ::MElement * me = gr->getMeshElement(ii);        
        //
        // tetrahedron
        //
        ::MTetrahedron * mtet = dynamic_cast< ::MTetrahedron * >(me);
        ::MHexahedron * mhex = dynamic_cast< ::MHexahedron * >(me);
//        ::MQuadrangle * mquad = dynamic_cast< ::MQuadrangle * >(me);
        if ( mtet ) {
          elem[cellC] = connC;
          std::vector< ::MVertex * > verts;
          mtet->getVertices(verts);        
          tl[cellC] = TYPE_TETRAHEDER;      
          conn[elem[cellC]+0] = verts[0]->getIndex()-1;
          conn[elem[cellC]+1] = verts[1]->getIndex()-1;
          conn[elem[cellC]+2] = verts[2]->getIndex()-1;
          conn[elem[cellC]+3] = verts[3]->getIndex()-1;
          connC = connC + 4;
          cellC++;      
        }      
        //
        // hexahedron
        //
        else if ( mhex ) {
          elem[cellC] = connC;
          std::vector< ::MVertex * > verts;
          mhex->getVertices(verts);        
          tl[cellC] = TYPE_HEXAEDER;      
          conn[elem[cellC]+0] = verts[4]->getIndex()-1;
          conn[elem[cellC]+1] = verts[5]->getIndex()-1;
          conn[elem[cellC]+2] = verts[1]->getIndex()-1;
          conn[elem[cellC]+3] = verts[0]->getIndex()-1;
          conn[elem[cellC]+4] = verts[7]->getIndex()-1;
          conn[elem[cellC]+5] = verts[6]->getIndex()-1;
          conn[elem[cellC]+6] = verts[2]->getIndex()-1;
          conn[elem[cellC]+7] = verts[3]->getIndex()-1;          
          DTINFOWF(toCoDoUnstructuredGrid(),
                  << DTLOGEVAL(verts[4]->getIndex()-1) << LOGDEL
                  << DTLOGEVAL(verts[5]->getIndex()-1) << LOGDEL
                  << DTLOGEVAL(verts[1]->getIndex()-1) << LOGDEL
                  << DTLOGEVAL(verts[0]->getIndex()-1) << LOGDEL
                  << DTLOGEVAL(verts[7]->getIndex()-1) << LOGDEL
                  << DTLOGEVAL(verts[6]->getIndex()-1) << LOGDEL
                  << DTLOGEVAL(verts[2]->getIndex()-1) << LOGDEL
                  << DTLOGEVAL(verts[3]->getIndex()-1) );
          connC = connC + 8;
          cellC++;      
        }  
        //
        // quadrangle
        //
//        else if ( mquad ) {
//          elem[cellC] = connC;
//          std::vector< ::MVertex * > verts;
//          mquad->getVertices(verts);        
//          tl[cellC] = TYPE_QUAD;      
//          conn[elem[cellC]+0] = verts[0]->getIndex()-1;
//          conn[elem[cellC]+1] = verts[1]->getIndex()-1;
//          conn[elem[cellC]+2] = verts[2]->getIndex()-1;
//          conn[elem[cellC]+3] = verts[3]->getIndex()-1;
//          connC = connC + 4;
//          cellC++;      
//        }            
      }
    }
    
    int size[2];
    size[0] = 7;
    size[1] = allConnValues.size() / 4;
      

      covise::coDoIntArr * connPatchInt = new covise::coDoIntArr("connPatchArray", 2, size); //, connPatchValues);
      int * arrAdd;
              connPatchInt->getAddress(&arrAdd);
              for (int ii=0; ii<size[1]; ii++) {
                 arrAdd[7*ii] = allConnValues[4*ii]+1;
                 arrAdd[7*ii+1] = allConnValues[4*ii+1]+1;
                 arrAdd[7*ii+2] = allConnValues[4*ii+2]+1;
                 arrAdd[7*ii+3] = allConnValues[4*ii+3]+1;
                 arrAdd[7*ii+4] = ii+1;
                 arrAdd[7*ii+5] = 55;
                 arrAdd[7*ii+6] = 0;
              }
              
              DTINFOWF(toCoDoUnstructuredGrid(),
                  << DTLOGEVAL(connPatchInt->getNumDimensions()) << LOGDEL
                  << DTLOGEVAL(connPatchInt->getDimension(0)) << LOGDEL
                  << DTLOGEVAL(connPatchInt->getDimension(1)) << LOGDEL
                  << DTLOGEVAL(connPatchInt->getSize()) //<< LOGDEL
//                  << DTLOGEVAL(boundingFaces.size()) << LOGDEL
//                  << DTLOGEVAL(boundingFaces[0])
                      );

              
//      covise::coDistributedObject * connPatchDO = dynamic_cast<coDistributedObject *>(connPatchInt); 

//      //Testing:
//      int intiArr[2];
//      intiArr[0] = 100;
//      intiArr[1] = 999;
//      int * valadd;
//    covise::coDoIntArr * testInti = new covise::coDoIntArr("tray", 2, intiArr); //, val);
//    testInti->getAddress(&valadd);
//    for (int ii = 0; ii<1099; ii++) {
//    valadd[ii] = 23+ii;
//    }
    
//    covise::coDistributedObject * testicovDisO = dynamic_cast<coDistributedObject *>(testInti);
    ptrHandling< covise::coDistributedObject * > cdo( new covise::coDistributedObject*[2] );
    cdo.get()[0] = cug.get();
//------------------------------    
//    cdo.get()[1] = connPatchInt;//testInti;// //einkommentieren, new covise::coDistributedObject*[3], cdo.get()[2] = NULL;
//-------------------------------    
//    cdo.get()[1] = testicovDisO;
//    cdo.get()[0] = unsFaceGrid.get();
    cdo.get()[1] = NULL;
    
    

    return new covise::coDoSet(str, cdo.get());
  }
  
  
  
  covise::coDoSet * blockGridModel::toCoDoSurfUnsGrid(char const * const str, int const boundFaceNum) const {
      
      std::vector< ::GFace* > boundingFaces;
      int boundFaceNumLoc = boundFaceNum;
      
      for (riter r_it = __caCThis->firstRegion(); r_it != __caCThis->lastRegion(); ++r_it) {
          ::GRegion * gr = *r_it;
          std::list<::GFace*>::iterator f_it;
          std::list<::GFace*> myFaces = (*r_it)->faces();
          for (f_it = myFaces.begin(); f_it!=myFaces.end(); ++f_it) {
              ::GFace * gf = *f_it;
              if (gf->numRegions()==1) {
                  boundingFaces.push_back(gf);
              }
          }
      }
      
      DTINFOWF(toCoDoSurfUnsGrid(),
                  << DTLOGEVAL(boundingFaces.size()) );
      if (boundFaceNumLoc >= boundingFaces.size()) {
          DTINFOWF(toCoDoSurfUnsGrid(),
          << DTLOGEVAL("Facenumber too big, chosen to max. possible") );
          boundFaceNumLoc = (boundingFaces.size()-1);
      }
      
      ::GFace * gfb = boundingFaces[boundFaceNumLoc];
      int nElem = gfb->getNumMeshElements();
//      int nVert = boundingFaces[boundFaceNum]->getNumMeshVertices();
    
      ptrHandling< covise::coDoUnstructuredGrid > cug( 
       new covise::coDoUnstructuredGrid( 
        "boundingSurfGrid",
        nElem, 
        nElem*4, //Quads
        getNumMeshVertices(),
        1 
       )
      );
      
      int * elem;
      int * conn;
      float * xx;
      float * yy;
      float * zz;
      int * tl;
        
      cug->getAddresses(&elem, &conn, &xx, &yy, &zz);
      cug->getTypeList(&tl);
        
      for( int ii=0; ii<getNumMeshVertices(); ii++ ) {
        ::MVertex const * const mv = __caCThis->getMeshVertexByTag(ii+1);
        xx[mv->getIndex()-1] = static_cast< float >(mv->x());
        yy[mv->getIndex()-1] = static_cast< float >(mv->y());
        zz[mv->getIndex()-1] = static_cast< float >(mv->z());
         DTINFOWF(toCoDoSurfUnsGrid(),
                  << DTLOGEVAL(ii) << LOGDEL
                  << DTLOGEVAL(mv->getIndex()-1) << LOGDEL
                  << DTLOGEVAL(mv->x()) << LOGDEL
                  << DTLOGEVAL(mv->y()) << LOGDEL         
                  << DTLOGEVAL(mv->z()) );
      }
      
      
//      std::vector< int > dimsPatchVec; 
    
//      std::vector< int > allConnValues;
    
//    std::vector< GFace* > boundingFaces;    
    
//      for (riter r_it = __caCThis->firstRegion(); r_it != __caCThis->lastRegion(); ++r_it) {
//        GRegion * gr = *r_it;
        
//        if (r_it == __caCThis->firstRegion()) { //das muss dann raus und intArr in Vector schreiben oder so
//          std::list<GFace*>::iterator f_it;
//          std::list<GFace*> myFaces = (*r_it)->faces();
//          for (f_it = myFaces.begin(); f_it!=myFaces.end(); ++f_it) {
//          for (fiter f_it = __caCThis->firstFace(); f_it != __caCThis->lastFace(); ++f_it) {
//        GFace * gf = *f_it;
        
//        dimsPatchVec.push_back( ( gf->getNumMeshElements() ) * 4 ); //"4" nur für quads
//        
//        DTINFOWF(toCoDoSurfUnsGrid(),
//                  << DTLOGEVAL(gf->getNumMeshElements()) << LOGDEL
//                  << DTLOGEVAL(dimsPatchVec.size() ) << LOGDEL
//                  << DTLOGEVAL(gf->numRegions())
//                );
        
//        if(gf->numRegions()==1) {
//            boundingFaces.push_back(gf);
//        }
        
//        std::vector< int > faceConnValues ( nElem * 4 ); //4: Quads
//        int faceCount=0;
        
        int cellC = 0;
        int connC = 0;  
        
        for( int ii=0; ii<nElem; ii++ ) {
          ::MElement * faceME = gfb->getMeshElement(ii);
          ::MQuadrangle * mquad = dynamic_cast< ::MQuadrangle * >(faceME);
          
          //
        // quadrangle
        //
        if ( mquad ) {
          elem[cellC] = connC;
          std::vector< ::MVertex * > verts;
          mquad->getVertices(verts);        
          tl[cellC] = TYPE_QUAD;      
          conn[elem[cellC]+0] = verts[0]->getIndex()-1;
          conn[elem[cellC]+1] = verts[1]->getIndex()-1;
          conn[elem[cellC]+2] = verts[2]->getIndex()-1;
          conn[elem[cellC]+3] = verts[3]->getIndex()-1;
          connC = connC + 4;
          cellC++;   
          
          DTINFOWF(toCoDoSurfUnsGrid(),
                  << DTLOGEVAL(verts[0]->getIndex()-1) << LOGDEL
                  << DTLOGEVAL(verts[1]->getIndex()-1) << LOGDEL
                  << DTLOGEVAL(verts[2]->getIndex()-1) << LOGDEL
                  << DTLOGEVAL(verts[3]->getIndex()-1) );
        }            
          
          
          //
          // quadrangle
          //
//          if ( mquad ) {
//              std::vector< ::MVertex * > verts;
//              mquad->getVertices(verts); 
//              faceConnValues[faceCount+0] = verts[0]->getIndex()-1;
//              faceConnValues[faceCount+1] = verts[1]->getIndex()-1;
//              faceConnValues[faceCount+2] = verts[2]->getIndex()-1;
//              faceConnValues[faceCount+3] = verts[3]->getIndex()-1;
//              faceCount = faceCount + 4;
              
//              DTINFOWF(toCoDoSurfUnsGrid(),
//                  << DTLOGEVAL(verts[0]->getIndex()-1) << LOGDEL
//                  << DTLOGEVAL(verts[1]->getIndex()-1) << LOGDEL
//                  << DTLOGEVAL(verts[2]->getIndex()-1) << LOGDEL
//                  << DTLOGEVAL(verts[3]->getIndex()-1) );
//              

//          }
        }
//        for (int aa=0; aa<faceConnValues.size(); aa++) {
//            allConnValues.push_back(faceConnValues[aa]);
//        }
//
//        faceConnValues.clear();

//      }
//      }
      
//    }
//        int size[2];
//        size[0] = 7;
//        size[1] = allConnValues.size() / 4;
//      
//
//      covise::coDoIntArr * connPatchInt = new covise::coDoIntArr("connPatchArray", 2, size); //, connPatchValues);
//      int * arrAdd;
//              connPatchInt->getAddress(&arrAdd);
//              for (int ii=0; ii<size[1]; ii++) {
//                 arrAdd[7*ii] = allConnValues[4*ii]+1;
//                 arrAdd[7*ii+1] = allConnValues[4*ii+1]+1;
//                 arrAdd[7*ii+2] = allConnValues[4*ii+2]+1;
//                 arrAdd[7*ii+3] = allConnValues[4*ii+3]+1;
//                 arrAdd[7*ii+4] = ii+1;
//                 arrAdd[7*ii+5] = 55;
//                 arrAdd[7*ii+6] = 0;
//              }
//              
//              DTINFOWF(toCoDoSurfUnsGrid(),
//                  << DTLOGEVAL(connPatchInt->getNumDimensions()) << LOGDEL
//                  << DTLOGEVAL(connPatchInt->getDimension(0)) << LOGDEL
//                  << DTLOGEVAL(connPatchInt->getDimension(1)) << LOGDEL
//                  << DTLOGEVAL(connPatchInt->getSize()) << LOGDEL
//                  << DTLOGEVAL(boundingFaces.size()) << LOGDEL
//                  << DTLOGEVAL(boundingFaces[0])
//                      );
      
//      ptrHandling< covise::coDistributedObject * > cdo( new covise::coDistributedObject*[4] );
      
      
//      ptrHandling< covise::coDistributedObject * > bocoSet( new covise::coDistributedObject*[1] );
      covise::coDistributedObject * griddy[2] ;
      griddy[0] = cug.get();
      griddy[1] = NULL;
      char * bocoName;
      bocoName = new char[strlen(str)+5];
      sprintf(bocoName,"%s_%d", str,0);
      covise::coDoSet * bocoSet = makeBoCo(bocoName);
      covise::coDoSet * returnSet = new covise::coDoSet(str, griddy);

      
      
      int ii = 0;
      while ( bocoSet->getElement(ii) != NULL ) {
      returnSet->addElement(bocoSet->getElement(ii));
      ii++;
      }      
      
      
     // bocoSet.get()[0] = makeBoCo(str);
      
//      cdo.get()[2] = NULL;
//      cdo.get()[0] = cug.get();   
////      cdo.get()[1] = connPatchInt;
//      cdo.get()[1] = ueberSet;//makeBoCo(str);//bocoSet.get()[0];
////      cdo.get()[2] = ueberSet->getElement(1);
//      cdo.get()[3] = NULL;
      
//      return bocoSet;
      return returnSet;
      //new covise::coDoSet(str, cdo.get());
//       return makeBoCo(str);
  }
  
    covise::coDoSet * blockGridModel::makeBoCo(char const * const str) const {
  
      std::vector< ::GFace* > boundingFaces;
      std::vector< int > tmpFaces;
      std::vector< int > tmpConnValues;
      std::vector< int > inletFaces;
      std::vector< int > inletConnValues;
      std::vector< int > outletFaces;
      std::vector< int > outletConnValues;
      std::vector< int > perioLFaces;
      std::vector< int > perioLConnValues;
      std::vector< int > perioRFaces;
      std::vector< int > perioRConnValues;
      std::vector< int > wallFaces;
      std::vector< int > wallConnValues;
      int size[2];
      size[0] = 7;
    
      for (riter r_it = __caCThis->firstRegion(); r_it != __caCThis->lastRegion(); ++r_it) {
          ::GRegion * gr = *r_it;
          std::list<::GFace*>::iterator f_it;
          std::list<::GFace*> myFaces = (*r_it)->faces();
          for (f_it = myFaces.begin(); f_it!=myFaces.end(); ++f_it) {
              ::GFace * gf = *f_it;
              if (gf->numRegions()==1) {
                  boundingFaces.push_back(gf);
              }
          }
      }
      
      inletFaces.push_back(1);
      inletFaces.push_back(4);
      inletFaces.push_back(7);
      inletFaces.push_back(11);
      
      outletFaces.push_back(21);
      outletFaces.push_back(24);
      outletFaces.push_back(27);
      outletFaces.push_back(31);
      
      perioLFaces.push_back(10);
      perioLFaces.push_back(17);
      perioLFaces.push_back(30);
      
      perioRFaces.push_back(0);
      perioRFaces.push_back(14);
      perioRFaces.push_back(20);
      
      wallFaces.push_back(2);
      wallFaces.push_back(3);
      wallFaces.push_back(5);
      wallFaces.push_back(6);
      wallFaces.push_back(8);
      wallFaces.push_back(9);
      wallFaces.push_back(12);
      wallFaces.push_back(13);
      wallFaces.push_back(15);
      wallFaces.push_back(16);
      wallFaces.push_back(18);
      wallFaces.push_back(19);
      wallFaces.push_back(22);
      wallFaces.push_back(23);
      wallFaces.push_back(25);
      wallFaces.push_back(26);
      wallFaces.push_back(28);
      wallFaces.push_back(29);
      wallFaces.push_back(32);
      wallFaces.push_back(33);
      wallFaces.push_back(34);
      wallFaces.push_back(35);
      wallFaces.push_back(36);
      wallFaces.push_back(37);
      wallFaces.push_back(38);
      wallFaces.push_back(39);
      wallFaces.push_back(40);
      wallFaces.push_back(41);
      wallFaces.push_back(42);
      wallFaces.push_back(43);
      wallFaces.push_back(44);
      wallFaces.push_back(45);
      
      
      for (int tt = 0; tt<5; tt++) {
          
          if (tt == 0) {
      tmpFaces = inletFaces;
          }
          else if (tt == 1) {
              tmpFaces = outletFaces;
          }
          else if (tt == 2) {
              tmpFaces = perioLFaces;
          }
          else if (tt == 3) {
              tmpFaces = perioRFaces;
          }
          else {
              tmpFaces = wallFaces;
          }
      
      for (int ii = 0; ii < tmpFaces.size(); ii++) {
          ::GFace * gfb = boundingFaces[tmpFaces[ii]];
          
          std::vector< int > faceConnValues ( gfb->getNumMeshElements() * 4 );
          int faceCount=0;
        
          for( int ii=0; ii<gfb->getNumMeshElements(); ii++ ) {
             ::MElement * faceME = gfb->getMeshElement(ii);
             ::MQuadrangle * mquad = dynamic_cast< ::MQuadrangle * >(faceME);
             //
             // quadrangle
             //
             if ( mquad ) {
                std::vector< ::MVertex * > verts;
                mquad->getVertices(verts); 
                faceConnValues[faceCount+0] = verts[0]->getIndex()-1;
                faceConnValues[faceCount+1] = verts[1]->getIndex()-1;
                faceConnValues[faceCount+2] = verts[2]->getIndex()-1;
                faceConnValues[faceCount+3] = verts[3]->getIndex()-1;
                faceCount = faceCount + 4;
              
                DTINFOWF(makeBoCo(),
                  << DTLOGEVAL(verts[0]->getIndex()-1) << LOGDEL
                  << DTLOGEVAL(verts[1]->getIndex()-1) << LOGDEL
                  << DTLOGEVAL(verts[2]->getIndex()-1) << LOGDEL
                  << DTLOGEVAL(verts[3]->getIndex()-1) );
              
             }
          }
          for (int aa=0; aa<faceConnValues.size(); aa++) {
            
               if (tt == 0) {
                   inletConnValues.push_back(faceConnValues[aa]);
               }
      
          else if (tt == 1) {
              outletConnValues.push_back(faceConnValues[aa]);
          }
          else if (tt == 2) {
              perioLConnValues.push_back(faceConnValues[aa]);
          }
          else if (tt == 3) {
              perioRConnValues.push_back(faceConnValues[aa]);
          }
          else {
               wallConnValues.push_back(faceConnValues[aa]);
          }              
              
          }

          faceConnValues.clear();
          
      }
      
      
      }
      
      size[1] = inletConnValues.size() / 4;
      covise::coDoIntArr * inletVertices = new covise::coDoIntArr("inletPatchArray", 2, size);
      int * inArrAdd;
              inletVertices->getAddress(&inArrAdd);
              for (int ii=0; ii<size[1]; ii++) {
                 inArrAdd[7*ii] = inletConnValues[4*ii]+1;
                 inArrAdd[7*ii+1] = inletConnValues[4*ii+1]+1;
                 inArrAdd[7*ii+2] = inletConnValues[4*ii+2]+1;
                 inArrAdd[7*ii+3] = inletConnValues[4*ii+3]+1;
                 inArrAdd[7*ii+4] = ii+1;
                 inArrAdd[7*ii+5] = 100;
                 inArrAdd[7*ii+6] = 0;
              }
              
              size[1] = outletConnValues.size() / 4;
              covise::coDoIntArr * outletVertices = new covise::coDoIntArr("outletPatchArray", 2, size);
      int * outArrAdd;
              outletVertices->getAddress(&outArrAdd);
              for (int ii=0; ii<size[1]; ii++) {
                 outArrAdd[7*ii] = outletConnValues[4*ii]+1;
                 outArrAdd[7*ii+1] = outletConnValues[4*ii+1]+1;
                 outArrAdd[7*ii+2] = outletConnValues[4*ii+2]+1;
                 outArrAdd[7*ii+3] = outletConnValues[4*ii+3]+1;
                 outArrAdd[7*ii+4] = ii+1;
                 outArrAdd[7*ii+5] = 110;
                 outArrAdd[7*ii+6] = 0;
              }
              
              size[1] = perioLConnValues.size() / 4;
              covise::coDoIntArr * perioLVertices = new covise::coDoIntArr("perioLPatchArray", 2, size);
      int * pLArrAdd;
              perioLVertices->getAddress(&pLArrAdd);
              for (int ii=0; ii<size[1]; ii++) {
                 pLArrAdd[7*ii] = perioLConnValues[4*ii]+1;
                 pLArrAdd[7*ii+1] = perioLConnValues[4*ii+1]+1;
                 pLArrAdd[7*ii+2] = perioLConnValues[4*ii+2]+1;
                 pLArrAdd[7*ii+3] = perioLConnValues[4*ii+3]+1;
                 pLArrAdd[7*ii+4] = ii+1;
                 pLArrAdd[7*ii+5] = 120;
                 pLArrAdd[7*ii+6] = 0;
              }
              
              size[1] = perioRConnValues.size() / 4;
              covise::coDoIntArr * perioRVertices = new covise::coDoIntArr("perioRPatchArray", 2, size);
      int * pRArrAdd;
              perioRVertices->getAddress(&pRArrAdd);
              for (int ii=0; ii<size[1]; ii++) {
                 pRArrAdd[7*ii] = perioRConnValues[4*ii]+1;
                 pRArrAdd[7*ii+1] = perioRConnValues[4*ii+1]+1;
                 pRArrAdd[7*ii+2] = perioRConnValues[4*ii+2]+1;
                 pRArrAdd[7*ii+3] = perioRConnValues[4*ii+3]+1;
                 pRArrAdd[7*ii+4] = ii+1;
                 pRArrAdd[7*ii+5] = 130;
                 pRArrAdd[7*ii+6] = 0;
              }
              
               size[1] = wallConnValues.size() / 4;
              covise::coDoIntArr * wallVertices = new covise::coDoIntArr("wallPatchArray", 2, size);
      int * wallArrAdd;
              wallVertices->getAddress(&wallArrAdd);
              for (int ii=0; ii<size[1]; ii++) {
                 wallArrAdd[7*ii] = wallConnValues[4*ii]+1;
                 wallArrAdd[7*ii+1] = wallConnValues[4*ii+1]+1;
                 wallArrAdd[7*ii+2] = wallConnValues[4*ii+2]+1;
                 wallArrAdd[7*ii+3] = wallConnValues[4*ii+3]+1;
                 wallArrAdd[7*ii+4] = ii+1;
                 wallArrAdd[7*ii+5] = 55;
                 wallArrAdd[7*ii+6] = 0;
              }

      
//         ptrHandling< covise::coDistributedObject * > cdo( new covise::coDistributedObject*[1] );
              covise::coDistributedObject * tmpSet [6];// = new covise::coDistributedObject * [3];
//         covise::coDistributedObject * tmpRetObj [2]; // = new covise::coDistributedObject *[2];
//      cdo.get()[0] = inletVertices;
//      cdo.get()[1] = outletVertices;
//      cdo.get()[2] = perioLVertices;
//      cdo.get()[3] = perioRVertices;
//      cdo.get()[4] = NULL;

       tmpSet[0] = inletVertices;
       tmpSet[1] = outletVertices;
       tmpSet[2] = perioLVertices;
       tmpSet[3] = perioRVertices;
       tmpSet[4] = wallVertices;
       tmpSet[5] = NULL;
       
       //      return new covise::coDoSet(str, cdo.get());
             return new covise::coDoSet(str, tmpSet);
//      return tmpRetObj;
    }
  
}
