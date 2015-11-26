#include "dtGmshEdge.h"

#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/geoBuilder/trimmedCurve_twoPointsConnectConstructOCC.h>
#include "dtGmshFace.h"
#include "dtGmshVertex.h"

#include <gmsh/GmshDefines.h>
#include <gmsh/GVertex.h>
#include <gmsh/GModel.h>
#include <gmsh/GFace.h>
#include <gmsh/MLine.h>

#include <logMe/logMe.h>
#include <progHelper.h>

namespace dtOO {  
  dtGmshEdge::dtGmshEdge(::GModel *m, int tag)
    : GEdge(m, tag, NULL, NULL) {
    _isSeamU = false;
    _isSeamV = false;		
  }
	
  dtGmshEdge::dtGmshEdge(::GModel *m, int tag, ::GVertex *v1, ::GVertex *v2)
    : GEdge(m, tag, v1, v2) {
    _isSeamU = false;
    _isSeamV = false;
		
		trimmedCurve_twoPointsConnectConstructOCC builder(
			dtPoint3(v1->x(), v1->y(), v1->z()),
			dtPoint3(v2->x(), v2->y(), v2->z())
		);
		ptrHandling< dtCurve > tmpEdge(builder.result());
				
		_mm.reset( new splineCurve3d(tmpEdge.get()) );
  }

  dtGmshEdge::~dtGmshEdge(void) {
  }
  
  Range<double> dtGmshEdge::parBounds(int i) const { 
    if (i == 0) {
      return Range<double>(
        static_cast< double >(_mm->getUMin()), 
        static_cast< double >(_mm->getUMax()) 
      );
    }
    else {
      dt__throw(parBounds(),
              << dt__eval(i) << std::endl
              << "i should be 0.");
    }
  }

  GPoint dtGmshEdge::point(double par) const {
    dtPoint3 pp = _mm->getPoint( static_cast<float>(par) );
   
    return GPoint(pp.x(), pp.y(), pp.z(), this, par);
  }

  SVector3 dtGmshEdge::firstDer(double par) const {
    dtVector3 vv = _mm->firstDerU( static_cast<float>(par) );

    return SVector3(vv.x(), vv.y(), vv.z());
  }
  
  void dtGmshEdge::setMap1dTo3d( map1dTo3d const * const base ) {   
    _mm.reset(base->clone());
  }

  void dtGmshEdge::setMap1dTo3dNoClone( map1dTo3d * base ) {   
    _mm.reset(base);
  }

  map1dTo3d const * dtGmshEdge::getMap1dTo3d( void ) const {
    return _mm.get();
  }
  
  void dtGmshEdge::setNElements( int const nE ) {
    this->meshAttributes.nbPointsTransfinite = nE+1;
  }

  void dtGmshEdge::meshTransfinite( int const type, float const coeff ) {
    this->meshAttributes.method = MESH_TRANSFINITE;
    this->meshAttributes.typeTransfinite = type;
    this->meshAttributes.coeffTransfinite = coeff;
  }

  void dtGmshEdge::meshTransfiniteWNElements( 
    int const type, float const coeff, int const nElements 
  ) {
    if (nElements != 0) {
      setNElements(nElements);
      meshTransfinite(type, coeff);
    }
  }
	
	void dtGmshEdge::addGEntity( ::GEntity * const gEnt ) {
		//
		// cast
		//		
		dtGmshVertex * gv = dtGmshVertex::DownCast(gEnt);
		dtGmshFace * gf = dtGmshFace::DownCast(gEnt);
		dt__throwIf( (gv==NULL)&&(gf==NULL), addGEntity() );
		
		if (gv) {
			// add this edge to vertex
			gv->addEdge(this);
      // add vertex to this edge
			addVertex(gv);
		}
		else {
			// add this edge to face
			gf->addEdge(this, 1);
      // add face to this edge
			addFace(gf);
		}
	}
	
  void dtGmshEdge::addElement( ::MElement * me ) {
    dt__ptrAss(
      ::MLine * aLine,
      dynamic_cast< ::MLine * >(me)
    );
    GEdge::addLine(aLine);
  }
  
	void dtGmshEdge::addVertex( ::GVertex * gv) {
	  if (
      std::find(l_vertices.begin(), l_vertices.end(), gv) == l_vertices.end()
    ) { 
		  l_vertices.push_back(gv);
    }
	}
	
  bool dtGmshEdge::isEqual( 
    ::GEdge const * const ge0, ::GEdge const * const ge1 
  ) {	
		std::list< ::GVertex * > VL0 = ge0->vertices();
		std::list< ::GVertex * > VL1 = ge1->vertices();
		
		if (VL0.size() != VL1.size()) {
			return false;
		}
		
		float xyzRes 
		= 
		staticPropertiesHandler::getInstance()->getOptionFloat(
      "xyz_resolution"
    );
		int counter = 0;
		std::list< ::GVertex * >::iterator V0_it;
		std::list< ::GVertex * >::iterator V1_it;
		for (V0_it = VL0.begin(); V0_it != VL0.end(); ++V0_it) {
			dtPoint3 v0((*V0_it)->x(), (*V0_it)->y(), (*V0_it)->z());
      for (V1_it = VL1.begin(); V1_it != VL1.end(); ++V1_it) {
				dtPoint3 v1((*V1_it)->x(), (*V1_it)->y(), (*V1_it)->z());
				if ( dtLinearAlgebra::distance(v0, v1) < xyzRes ) {
					counter++;
				}
			}
		}
		
		if (VL0.size() == counter) {
			return true;
		}
		else {
			return false;
		}		
	}	
}