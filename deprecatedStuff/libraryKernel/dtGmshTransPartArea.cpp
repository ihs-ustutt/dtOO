#include "dtGmshTransPartArea.h"

#include <gmsh/GEdge.h>
#include <gmsh/GModel.h>

#include <logMe/logMe.h>
#include <progHelper.h>
#include "dtGmshEdge.h"
#include "dtGmshFace.h"
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO { 
  dtGmshTransPartArea::dtGmshTransPartArea(::GModel *m, int tag, const std::list< ::GEdge * > &edges, const std::vector< int > & ori) 
  : dtGmshFace(m, tag, edges, ori) {
    if (edges.size() != 4) {
      dt__THROW(dtGmshTransPartArea(),
              << "Currently only supported with 4 edges.");
    }
    typedef std::list< ::GEdge * >::const_iterator EIter;    
    for (EIter ei=edges.begin(); ei != edges.end(); ++ei) {
      ::GEdge * ge = *ei;
      dtGmshEdge const * dge;
      dt__mustDownCast(ge, dtGmshEdge const, dge);
     _edge.push_back(dge);
     ge->addFace(this);
    }
  }
  
  dtGmshTransPartArea::dtGmshTransPartArea(::GModel *m, int tag, const std::list< ::GEdge * > &edges) 
  : dtGmshFace(m, tag, edges) {
    if (edges.size() != 4) {
      dt__THROW(dtGmshTransPartArea(),
              << "Currently only supported with 4 edges.");
    } 

    typedef std::list< ::GEdge * >::const_iterator EIter;    
    for (EIter ei=edges.begin(); ei != edges.end(); ++ei) {
      ::GEdge * ge = *ei;
      dtGmshEdge const * dge;
      dt__mustDownCast(ge, dtGmshEdge const, dge);
      _edge.push_back(dge);
      ge->addFace(this);
    }    
  }
  
  dtGmshTransPartArea::~dtGmshTransPartArea() {
    _edge.clear();
  }

//  void dtGmshTransPartArea::setMap2dTo3d( map2dTo3d const * const base ) {
//    dtGmshFace::setMap2dTo3d(base);        
//    
//    if ( _edge[0]->isSeamU() || _edge[3]->isSeamU() ) {
//      GVertex * gv0 = _edge[1]->getBeginVertex();
//      GVertex * gv1 = _edge[1]->getEndVertex();
//      SPoint2 gv0UV = dtGmshFace::reparamOnFace(gv0);
//      SPoint2 gv1UV = dtGmshFace::reparamOnFace(gv1);
//      float uBound = gv1UV.x();
//      if ( gv0UV.x() > gv1UV.x() ) {
//        uBound = gv0UV.x();
//      }
//      
//      float uBoundPercent = base->percent_u(uBound);
//      if ( (1.-uBoundPercent) < (uBoundPercent-0.) ) {
//        ptrHandling< map2dTo3d > tmp(base->pickPercent(0.1, 1.0, 0.0, 1.0));
//        dtGmshFace::setMap2dTo3d( tmp.get() );        
//      }
//      else {
//        ptrHandling< map2dTo3d > tmp(base->pickPercent(0.0, 0.9, 0.0, 1.0));
//        dtGmshFace::setMap2dTo3d( tmp.get() );             
//      }   
//    }
//    if ( _edge[1]->isSeamU() || _edge[2]->isSeamU() ) {
//      GVertex * gv0 = _edge[0]->getBeginVertex();
//      GVertex * gv1 = _edge[0]->getEndVertex();
//      SPoint2 gv0UV = dtGmshFace::reparamOnFace(gv0);
//      SPoint2 gv1UV = dtGmshFace::reparamOnFace(gv1);
//      float uBound = gv1UV.x();
//      if ( gv0UV.x() > gv1UV.x() ) {
//        uBound = gv0UV.x();
//      }
//      
//      float uBoundPercent = base->percent_u(uBound);
//      if ( (1.-uBoundPercent) < (uBoundPercent-0.) ) {
//        ptrHandling< map2dTo3d > tmp(base->pickPercent(0.1, 1.0, 0.0, 1.0));
//        dtGmshFace::setMap2dTo3d( tmp.get() );             
//      }
//      else {
//        ptrHandling< map2dTo3d > tmp(base->pickPercent(0.0, 0.9, 0.0, 1.0));
//        dtGmshFace::setMap2dTo3d( tmp.get() );             
//      }   
//    }
//    if ( _edge[1]->isSeamV() || _edge[1]->isSeamV() || _edge[2]->isSeamV() || _edge[3]->isSeamV() ) {
//      dt__THROW(setMap2dTo3d(),
//              << DTLOGEVAL(_edge[0]->isSeamV()) << LOGDEL
//              << DTLOGEVAL(_edge[1]->isSeamV()) << LOGDEL
//              << DTLOGEVAL(_edge[2]->isSeamV()) << LOGDEL
//              << DTLOGEVAL(_edge[3]->isSeamV()) << LOGDEL              
//              << "Not yet supported.");
//    }    
//  }
   
  std::string dtGmshTransPartArea::getMotherSurfaceLabel( void ) const {
    return _motherSurfaceName;
  }
  
  void dtGmshTransPartArea::setMotherSurfaceLabel( std::string const label ) {
    _motherSurfaceName = label;
  }
  
  void dtGmshTransPartArea::updateFace(void) {
    DTINFOWF(updateFace(), << "No update necessary.");
  }
}
