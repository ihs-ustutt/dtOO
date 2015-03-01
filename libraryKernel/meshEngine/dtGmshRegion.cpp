#include "dtGmshRegion.h"

#include <gmsh/GmshDefines.h>
#include <gmsh/GFace.h>
#include <gmsh/GModel.h>
#include "dtGmshFace.h"

namespace dtOO {
	dtGmshRegion::dtGmshRegion(::GModel *m, int tag) : GRegion(m, tag) {
		
	}
	
  dtGmshRegion::dtGmshRegion(::GModel *m, int tag, const std::list< ::GFace * > &faces, const std::vector<int> &ori )
    : GRegion(m, tag) {
    typedef std::list< ::GFace * >::const_iterator FIter;
    int ii = 0;
    for (FIter fi=faces.begin(); fi != faces.end(); ++fi) {
      ::GFace *f = *fi;
      l_faces.push_back(f);
      f->addRegion(this);
      l_dirs.push_back( ori[ii] );
      ii++;
    }
  }

  dtGmshRegion::dtGmshRegion(::GModel *m, int tag, const std::list< dtGmshFace* > &faces, const std::vector<int> &ori )
    : GRegion(m, tag) {
    typedef std::list< dtGmshFace* >::const_iterator FIter;
    int ii = 0;
    for (FIter fi=faces.begin(); fi != faces.end(); ++fi) {
      ::GFace * f = static_cast< dtGmshFace* >(*fi);
      l_faces.push_back(f);
      f->addRegion(this);
      l_dirs.push_back( ori[ii] );
      ii++;
    }
  }
   
  void dtGmshRegion::meshTransfinite( void ) {
    this->meshAttributes.method = MESH_TRANSFINITE;
    this->meshAttributes.recombine3D = 1;
		std::list< ::GFace * > fl = faces();
		for (std::list< ::GFace * >::iterator f_it = fl.begin(); f_it != fl.end(); ++f_it) {
			std::list< ::GEdge * > el = (*f_it)->edges();
			for (std::list< ::GEdge * >::iterator e_it = el.begin(); e_it != el.end(); ++e_it) {
				(*e_it)->meshAttributes.method = MESH_TRANSFINITE;
				(*e_it)->meshAttributes.nbPointsTransfinite = 2;
				(*e_it)->meshAttributes.coeffTransfinite = 1.;
				(*e_it)->meshAttributes.typeTransfinite = 1.;				
			}
			(*f_it)->meshAttributes.method = MESH_TRANSFINITE;
			(*f_it)->meshAttributes.recombine = 1;
		}
  }
	
  void dtGmshRegion::meshUnstructured( void ) {
    this->meshAttributes.method = MESH_UNSTRUCTURED;
    this->meshAttributes.recombine3D = 0;
		std::list< ::GFace * > fl = faces();
		for (std::list< ::GFace * >::iterator f_it = fl.begin(); f_it != fl.end(); ++f_it) {
			std::list< ::GEdge * > el = (*f_it)->edges();
			for (std::list< ::GEdge * >::iterator e_it = el.begin(); e_it != el.end(); ++e_it) {
				(*e_it)->meshAttributes.method = MESH_UNSTRUCTURED;
			}
			(*f_it)->meshAttributes.method = MESH_UNSTRUCTURED;
			(*f_it)->meshAttributes.recombine = 0;
		}
  }
	
  void dtGmshRegion::addFace( ::GFace * face, int const ori ) {
    l_faces.push_back( face );
    face->addRegion(this);
    l_dirs.push_back( ori );
  }  	
}
