#include "dtGmshFace.h"

#include <analyticGeometryHeaven/map2dTo3d.h>
#include "dtGmshEdge.h"
#include <gmsh/GmshDefines.h>
#include <gmsh/GEdge.h>
#include <gmsh/GModel.h>
#include <list>
#include "logMe/logMe.h"
#include "progHelper.h"

namespace dtOO {    
  dtGmshFace::dtGmshFace(GModel *m, int tag, const std::list<GEdge*> &edges, const std::vector< int > &ori )
    : GFace(m, tag) {
    edgeLoops.push_back(GEdgeLoop(edges));
    typedef std::list<GEdge*>::const_iterator EIter;
    int ii = 0;
    for (EIter ei=edges.begin(); ei != edges.end(); ++ei) {
      GEdge *e = *ei;
      l_edges.push_back(e);
      e->addFace(this);
      l_dirs.push_back(ori[ii]);
      ii++;
    }
    _mm = NULL;
  }

  dtGmshFace::dtGmshFace(GModel *m, int tag, const std::list<GEdge*> &edges)
    : GFace(m, tag) {
    edgeLoops.push_back(GEdgeLoop(edges));
    typedef std::list<GEdge*>::const_iterator EIter;
    for (EIter ei=edges.begin(); ei != edges.end(); ++ei) {
      GEdge *e = *ei;
      l_edges.push_back(e);
      e->addFace(this);
      l_dirs.push_back(1);
    }
    _mm = NULL;
  }  

  dtGmshFace::~dtGmshFace() {
    delete _mm;
  }
  
  Range<double> dtGmshFace::parBounds(int i) const {
    if (i == 0) {
      return Range<double>(_mm->getUMin(), _mm->getUMax());
    }    
    if (i == 1) {
      return Range<double>(_mm->getVMin(), _mm->getVMax());
    }
    else {
      dt__THROW(parBounds(),
              << DTLOGEVAL(i) << LOGDEL
              << "i should be 0 or 1.");
    }
  }

  Pair<SVector3, SVector3> dtGmshFace::firstDer(const SPoint2 &param) const {
    dtVector3 ddU = _mm->firstDerU( (float) param.x(), (float) param.y() );
    dtVector3 ddV = _mm->firstDerV( (float) param.x(), (float) param.y() );
    
    return Pair<SVector3, SVector3>( SVector3( (double) ddU.x(), (double) ddU.y(), (double) ddU.z()),
                                     SVector3( (double) ddV.x(), (double) ddV.y(), (double) ddV.z()) );
  }

  void dtGmshFace::secondDer(const SPoint2 &param, 
                           SVector3 *dudu, SVector3 *dvdv, SVector3 *dudv) const {
    dtVector3 ddUddU = _mm->secondDerUU( (float) param.x(), (float) param.y() );
    dtVector3 ddVddV = _mm->secondDerVV( (float) param.x(), (float) param.y() );    
    dtVector3 ddUddV = _mm->secondDerUV( (float) param.x(), (float) param.y() );    
      *dudu = SVector3(ddUddU.x(), ddUddU.y(), ddUddU.z());
      *dvdv = SVector3(ddVddV.x(), ddVddV.y(), ddVddV.z());
      *dudv = SVector3(ddUddV.x(), ddUddV.y(), ddUddV.z()); 
  }

  GPoint dtGmshFace::point(double par1, double par2) const {
    double pp[2] = {par1, par2};
    
    dtPoint3 retPoint = _mm->getPoint(par1, par2);
    
    return GPoint(retPoint.x(), retPoint.y(), retPoint.z(), this, pp);
  }

  SPoint2 dtGmshFace::reparamOnFace(dtPoint3 const ppXYZ) const {
    dtPoint2 ppUV = _mm->reparamOnFace( ppXYZ );
    
    return SPoint2(ppUV.x(), ppUV.y());
  }

  SPoint2 dtGmshFace::reparamOnFace(GVertex const * gv) const {
    dtPoint3 pp(gv->x(), gv->y(), gv->z());
    
    return reparamOnFace(pp);
  }  
  
  dtPoint2 dtGmshFace::reparamOnFace2d( dtPoint3 const ppXYZ ) const {
    SPoint2 ppUV = reparamOnFace(ppXYZ);
    
    return dtPoint2(ppUV.x(), ppUV.y());
  }    
    
  void dtGmshFace::setMap2dTo3d( map2dTo3d const * const base ) {
    if (_mm) {
      delete _mm;
    }
    _mm = base->clone();
  }

  map2dTo3d const * dtGmshFace::getMap2dTo3d( void ) const {
    return _mm;
  }
  
  void dtGmshFace::addEdge( GEdge * edge, int const ori ) {
    l_edges.push_back( edge );
    edge->addFace(this);
    l_dirs.push_back( ori );
    edgeLoops.clear();
    edgeLoops.push_back( GEdgeLoop(l_edges) );
  }  

  void dtGmshFace::addEdgeLoop( std::list< GEdge * > edgeL ) {
    addEdge( edgeL.front(), 1);
    GVertex * gv = edgeL.front()->getEndVertex();
    edgeL.erase( edgeL.begin() );
    for (int ii=0;edgeL.size();ii++) {
      for(std::list<GEdge*>::iterator it = edgeL.begin(); it != edgeL.end(); ++it ) {
        if ( (*it)->getBeginVertex() == gv ) {
          addEdge(*it, 1);
          gv = (*it)->getEndVertex();
          edgeL.erase(it);
          break;
        }
        if ( (*it)->getEndVertex() == gv ) {
          addEdge(*it, -1);
          gv = (*it)->getBeginVertex();
          edgeL.erase(it);
          break;
        }
      }
    }
    if (edgeL.size() != 0) {
      dt__THROW(addEdgeLoop(),
              << DTLOGEVAL( edgeL.size() ) );
    }
  }  
  
  SPoint2 dtGmshFace::parFromPoint(const SPoint3 &p, bool onSurface) const {
    dtPoint3 pp( p.x(), p.y(), p.z() );
    
    return reparamOnFace( pp );
  }
  
  std::list<GEdge*> dtGmshFace::edges( void ) const { 
    return l_edges;
  }
  
  bool dtGmshFace::isClosed( int const dim ) const {
    if (dim == 0) {
      return _mm->isClosedU();
    }
    else if (dim == 1) {
      return _mm->isClosedV();
    }
    else {
      dt__THROW(isClosed(),
              << DTLOGEVAL(dim) << LOGDEL
              << "dim should be 0 or 1.");
    }    
  }
  
  void dtGmshFace::meshTransfinite( void ) {
      this->meshAttributes.method = MESH_TRANSFINITE;
      this->meshAttributes.recombine = 1;
  }
  
  void dtGmshFace::updateFace( void ) {
    dt__THROW(updateFace(), << "Not yet implemented. This could produce errors.");
  }
  
  void dtGmshFace::makeSuitable( void ) {
    DTINFOWF(updateFace(), << "Base class calling. Nothing to do.");
  }
}