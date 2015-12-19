#include "dtGmshVertex.h"

#include <gmsh/GPoint.h>
#include <gmsh/GEdge.h>
#include <gmsh/GModel.h>
#include "dtGmshFace.h"
#include "dtGmshEdge.h"
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

namespace dtOO { 
  dtGmshVertex::dtGmshVertex(::GModel *m, int tag, double ms) : GVertex(m, tag, ms) {
    _dtP = dtPoint3(0., 0., 0.);  
  }

  dtGmshVertex::dtGmshVertex(::GModel *m, int tag) : GVertex(m, tag) {
    _dtP = dtPoint3(0., 0., 0.);  
  }
  
  dtGmshVertex::~dtGmshVertex() {
  }
  
  GPoint dtGmshVertex::point() const {
    return GPoint(_dtP.x(), _dtP.y(), _dtP.z(), this);
  }
  
  double dtGmshVertex::x() const {
    return _dtP.x();
  }
  
  double dtGmshVertex::y() const {
    return _dtP.y();
  }
  
  double dtGmshVertex::z() const {
    return _dtP.z();
  }
  
  void dtGmshVertex::setPosition(GPoint &p) {
    _dtP = dtPoint3(p.x(), p.y(), p.z());
  }

  void dtGmshVertex::setPosition(dtPoint3 const p) {
    _dtP = p;
  }

  void dtGmshVertex::setPosition(dtPoint3 const * const p) {
    _dtP = *p;
  }

  dtPoint3 dtGmshVertex::cast2DtPoint3( void ) const {
    return dtPoint3(x(), y(), z());
  }
	
	void dtGmshVertex::addGEntity( ::GEntity * const gEnt ) {
		//
		// cast
		//		
		dt__ptrAss(dtGmshEdge * ge, dtGmshEdge::DownCast(gEnt));
		
		// add this vertex to edge
		ge->addVertex(this);
		// add edge to this vertex
		addEdge(ge);
	}
	
  bool dtGmshVertex::isEqual( 
    ::GVertex const * const gv0, ::GVertex const * const gv1 
  ) {	   
		float xyzRes 
    = 
		staticPropertiesHandler::getInstance()->getOptionFloat(
      "XYZ_resolution"
    );
  	dtPoint3 v0(gv0->x(), gv0->y(), gv0->z());
		dtPoint3 v1(gv1->x(), gv1->y(), gv1->z());
		if ( dtLinearAlgebra::distance(v0, v1) < xyzRes ) {
			return true;
		}
		else {
			return false;
		}
	}	
}
