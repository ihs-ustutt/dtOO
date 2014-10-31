#include "dtGmshRegion.h"

#include <gmsh/GmshDefines.h>
#include <gmsh/GFace.h>
#include <gmsh/GModel.h>

namespace dtOO {
  dtGmshRegion::dtGmshRegion(GModel *m, int tag, const std::list<GFace*> &faces, const std::vector<int> &ori )
    : GRegion(m, tag) {
    typedef std::list<GFace*>::const_iterator FIter;
    int ii = 0;
    for (FIter fi=faces.begin(); fi != faces.end(); ++fi) {
      GFace *f = *fi;
      l_faces.push_back(f);
      f->addRegion(this);
      l_dirs.push_back( ori[ii] );
      ii++;
    }
  }
  
  dtGmshRegion::dtGmshRegion( GModel *m, int tag ) : GRegion(m, tag) {
    
  }
  
  bool dtGmshRegion::containsEdge( GEdge const * const edge ) const {
    std::list< GFace* > const gfs = faces();
    std::list<GFace*>::const_iterator gf_it = gfs.begin();
    for (gf_it; gf_it != gfs.end(); ++gf_it) {
      if ( (*gf_it)->containsEdge( edge->tag() ) ) return true;
    }
    return false;
  }

  bool dtGmshRegion::containsFace( GFace const * const face ) const {
    std::list< GFace* > const gfs = faces();
    std::list<GFace*>::const_iterator gf_it = gfs.begin();
    for (gf_it; gf_it != gfs.end(); ++gf_it) {
      if ( (*gf_it)->tag() == face->tag() ) return true;
    }
    return false;
  }  
  
  void dtGmshRegion::meshTransfinite( void ) {
    this->meshAttributes.method = MESH_TRANSFINITE;
    this->meshAttributes.recombine3D = 1;
		std::list<GFace*> fl = faces();
		for (std::list<GFace*>::iterator f_it = fl.begin(); f_it != fl.end(); ++f_it) {
			std::list<GEdge*> el = (*f_it)->edges();
			for (std::list<GEdge*>::iterator e_it = el.begin(); e_it != el.end(); ++e_it) {
				(*e_it)->meshAttributes.method = MESH_TRANSFINITE;
				(*e_it)->meshAttributes.nbPointsTransfinite = 10;
				(*e_it)->meshAttributes.coeffTransfinite = 1.;
			}
			(*f_it)->meshAttributes.method = MESH_TRANSFINITE;
			(*f_it)->meshAttributes.recombine = 1;
		}
  }
}
