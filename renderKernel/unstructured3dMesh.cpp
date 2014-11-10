#include "unstructured3dMesh.h"
#include <progHelper.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
	unstructured3dMesh::unstructured3dMesh() {
    _nTet = 0;
    _nHex = 0;
	}

	unstructured3dMesh::~unstructured3dMesh() {
	}
	
	void unstructured3dMesh::addPoints( vectorHandling< dtPoint3 > const & pp ) {
		_pp = pp;
	}
	
	void unstructured3dMesh::addPoint( dtPoint3 const & pp ) {
		_pp.push_back(pp);
	}
	
	void unstructured3dMesh::addElement( vectorHandling< int > const & el ) {
		_el.push_back(el);
		if (el.size() == 4) {
			_nTet++;
		}
		else if (el.size() == 8) {
			_nHex++;
		}		
	}
	
	vectorHandling< dtPoint3 > const & unstructured3dMesh::refP3( void ) const {
		return _pp;
	}

	vectorHandling< vectorHandling< int > > const & unstructured3dMesh::refEl( void ) const {
		return _el;
	}

  int unstructured3dMesh::getNHex( void ) const {
  	return _nHex;
  }

  int unstructured3dMesh::getNTet( void ) const {
  	return _nTet;
  }
}
