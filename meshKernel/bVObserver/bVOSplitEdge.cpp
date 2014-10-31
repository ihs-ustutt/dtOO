#include "bVOSplitEdge.h"

#include <boundedVolume.h>
//#include <c14Volume.h>
//#include <blockGridModel.h>
#include <dtGmshEdge.h>
#include <dtGmshFace.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
//#include <analyticGeometryHeaven/map1dTo3dInMap3dTo3d.h>
#include <analyticGeometryHeaven/splineCurve3d.h>

#include <logMe/logMe.h>
#include <progHelper.h>
#include <interfaceHeaven/ptrHandling.h>
#include <vector>
#include <math.h>

namespace dtOO {  
  bVOSplitEdge::bVOSplitEdge() {
    _scale = dtVector3(.5,.5,.5);
  }

  bVOSplitEdge::~bVOSplitEdge() {
    
  }
  
  void bVOSplitEdge::bVOSplitEdge::init( 
    QDomElement const & element,
    vectorHandling< constValue * > const * const cValP,
    vectorHandling< analyticFunction * > const * const sFunP,
    vectorHandling< analyticGeometry * > const * const depAGeoP,
    vectorHandling< boundedVolume * > const * const depBVolP,
    boundedVolume * bV
  ) {
    //
    // init bVOInterface
    //
    bVOInterface::init(element, cValP, sFunP, depAGeoP, depBVolP, bV);
    
    DTINFOWF(init(), << convertToString(element) );
    QDomElement edge = getChild("dtGmshEdge", element);
    
    _edge.resize(2);
    _edge[0] = getAttributeInt("from", edge);
    _edge[1] = getAttributeInt("to", edge);
           
    _scale = getDtVector3( getChild("dtVector3", element), cValP, sFunP );
  }
  
  /**
   * @todo: What happens if edge is already split? Currently update function 
   *        expects a linear edge and does not check for that.
   */
  void bVOSplitEdge::update( void ) {
		dt__THROW(update(), << "This Observer is currently not useable.");
//    //
//    // check for correct model type
//    //
//    c14Volume * c14v = NULL;
//    dt__CANDOWNCAST(_bV, c14Volume, c14v);
//
//    //
//    // get desired edge
//    //
//    dtGmshEdge * ge
//    =
//    c14v->refToBlockGridModel()->getDtGmshEdgeByFromTo(_edge[0]+1, _edge[1]+1);
//
//    //
//    // get mapping, has to be a map1dTo3dInMap3dTo3d
//    //
//    map1dTo3dInMap3dTo3d const * m1i3;
//    dt__CANDOWNCAST(ge->getMap1dTo3d(), map1dTo3dInMap3dTo3d const, m1i3);
//      
//    if (c14v && ge && m1i3) {
//      //
//      // get end points of edge and insert one point
//      //
//      std::vector< dtPoint3 > pp;
//      pp.push_back(m1i3->getConstPtrToMap1dTo3d()->getPointPercent(0.));
//      pp.push_back( dtPoint3(0,0,0) );
//      pp.push_back(m1i3->getConstPtrToMap1dTo3d()->getPointPercent(1.));
//      //
//      // calculate new point with given scale vector
//      //
//      dtAffTransformation3 dd = dtLinearAlgebra::getDiagTrans(_scale);
//      pp[1] 
//      = 
//      pp[0] + dd.transform(pp[2]-pp[0]); 
//
//      //
//      // respline and recreate edge
//      //
//      ptrHandling< splineCurve3d > sc3(new splineCurve3d(&pp, 3));
//      ge->setMap1dTo3dNoClone(
//        new map1dTo3dInMap3dTo3d( sc3.get(), m1i3->getConstPtrToMap3dTo3d() )
//      );
//
//      //
//      // update faces to enable again reparamOnFace of edge
//      //
//      std::list< GFace * > faces = ge->faces();
//      for(std::list<GFace*>::iterator it = faces.begin(); it != faces.end(); it++) {
//        dtGmshFace * dgf;
//        dt__CANDOWNCAST(*it, dtGmshFace, dgf);
//        if (dgf) dgf->updateFace();
//      }
//    }
//    else {
//      DTWARNINGWF(
//        update(),
//        << "Edge from=" << _edge[0] << " to=" << _edge[1] << " cannot be split."
//        << DTLOGEVAL(c14v) << LOGDEL
//        << DTLOGEVAL(ge) << LOGDEL
//        << DTLOGEVAL(m1i3) << LOGDEL );
//    }
  }
}
