#include "unstructured3dSurfaceMesh.h"
#include <progHelper.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
	unstructured3dSurfaceMesh::unstructured3dSurfaceMesh() {
    _nQuads = 0;
		_nTris = 0;
	}

	unstructured3dSurfaceMesh::~unstructured3dSurfaceMesh() {
	}
	
	void unstructured3dSurfaceMesh::addPoints( vectorHandling< dtPoint3 > const & pp ) {
		_pp = pp;
	}
	
	void unstructured3dSurfaceMesh::addPoint( dtPoint3 const & pp ) {
		_pp.push_back(pp);
	}
	
	void unstructured3dSurfaceMesh::addElement( vectorHandling< int > const & el ) {
		_el.push_back(el);
		if (el.size() == 4) {
			_nQuads++;
		}
		else if (el.size() == 3) {
			_nTris++;
		}
		else {
			dt__throw(addElement, << dt__eval(el.size()) );
		}
	}
	
	vectorHandling< dtPoint3 > const & unstructured3dSurfaceMesh::refP3( void ) const {
		return _pp;
	}

	vectorHandling< vectorHandling< int > > const & unstructured3dSurfaceMesh::refEl( void ) const {
		return _el;
	}

  int unstructured3dSurfaceMesh::getNQuads( void ) const {
  	return _nQuads;
  }
	
  int unstructured3dSurfaceMesh::getNTris( void ) const {
  	return _nTris;
  }	
}
