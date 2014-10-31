#include "dtGmshVertex.h"

#include <gmsh/GPoint.h>
#include <gmsh/GModel.h>
#include "dtGmshFace.h"
#include <logMe/logMe.h>
#include <progHelper.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>

namespace dtOO { 
  dtGmshVertex::dtGmshVertex(GModel *m, int tag, double ms) : GVertex(m, tag, ms) {
    _CGALPoint = new dtPoint3(0,0,0);  
    _GPointPoint = new GPoint(0,0,0,this);
  }

  dtGmshVertex::dtGmshVertex(GModel *m, int tag) : GVertex(m, tag) {
    _CGALPoint = new dtPoint3(0,0,0);  
    _GPointPoint = new GPoint(0,0,0,this);
  }
  
  dtGmshVertex::~dtGmshVertex() {
    _m2d.clear();
    _m3d.clear();
  }
  
  GPoint dtGmshVertex::point() const {
    return *_GPointPoint;
  }
  
  double dtGmshVertex::x() const {
    return _GPointPoint->x();
  }
  
  double dtGmshVertex::y() const {
    return _GPointPoint->y();
  }
  
  double dtGmshVertex::z() const {
    return _GPointPoint->z();
  }
  
  void dtGmshVertex::setPosition(GPoint &p) {
    *_GPointPoint = p;
    *_CGALPoint = dtPoint3(p.x(), p.y(), p.z());
  }

  void dtGmshVertex::setPosition(dtPoint3 const p) {
    *_GPointPoint = GPoint( p.x(), p.y(), p.z(), this );
    *_CGALPoint = p;
  }

  void dtGmshVertex::setPosition(dtPoint3 const * const p) {
    setPosition(*p);
  }

  void dtGmshVertex::snapToMap2dTo3d( map2dTo3d const * const mm) {
    _m2d.push_back(mm);
  }

  void dtGmshVertex::snapToMap3dTo3d( map3dTo3d const * const mm) {
    _m3d.push_back(mm);
  }  

  std::vector< map2dTo3d const * > const & dtGmshVertex::getRefToSnapMap2dTo3d( void ) const {
    return _m2d;
  }
  
  std::vector< map3dTo3d const * > const & dtGmshVertex::getRefToSnapMap3dTo3d( void ) const {
    return _m3d;
  }  
  
  bool dtGmshVertex::snappedTo(map2dTo3d const * const aS) const {
    dt__FORALL(_m2d, ii,
      if ( _m2d[ii] == aS) {
        return true;
      }
    );
    return false;
  }

  dtPoint3 dtGmshVertex::cast2DtPoint3( void ) const {
    return dtPoint3(x(), y(), z());
  }
  
  dtGmshVertex * dtGmshVertex::clone( GModel * destModel ) const {
    GVertex * gv = destModel->getVertexByTag(tag());
    dtGmshVertex * clone;
    
    if ( gv ) {
      dt__MUSTDOWNCAST(gv, dtGmshVertex, clone);
    }
    else {
     clone = new dtGmshVertex( destModel, tag(), meshSize );
    
      clone->_CGALPoint =  new dtPoint3( *_CGALPoint );
      clone->_GPointPoint =  new GPoint( _GPointPoint->x(), _GPointPoint->y(), _GPointPoint->z() );
      clone->_m2d.resize( _m2d.size() );
      dt__FORALL(_m2d, ii,
        clone->_m2d[ii] = _m2d[ii];
      );
      
      destModel->add(clone);
    }
    return clone;
  }
}
