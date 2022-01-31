#include "unstructured3dMesh.h"
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
	unstructured3dMesh::unstructured3dMesh() {
    _nTet = 0;
    _nHex = 0;
		_nPyr = 0;
		_nPri = 0;
	}

	unstructured3dMesh::~unstructured3dMesh() {
	}
	
	void unstructured3dMesh::addPoints( vectorHandling< dtPoint3 > const & pp ) {
		_pp = pp;
	}
	
	void unstructured3dMesh::addPoint( dtPoint3 const & pp ) {
		_pp.push_back(pp);
	}
	
	void unstructured3dMesh::addElement( vectorHandling< dtInt > const & el ) {
		_el.push_back(el);
		if (el.size() == 4) {
			_nTet++;
		}
		else if (el.size() == 5) {
			_nPyr++;
		}		
		else if (el.size() == 6) {
			_nPri++;
		}				
		else if (el.size() == 8) {
			_nHex++;
		}		
	}
	
	vectorHandling< dtPoint3 > const & unstructured3dMesh::refP3( void ) const {
		return _pp;
	}

	vectorHandling< vectorHandling< dtInt > > const & unstructured3dMesh::refEl( void ) const {
		return _el;
	}

  dtInt unstructured3dMesh::getNHex( void ) const {
  	return _nHex;
  }

  dtInt unstructured3dMesh::getNTet( void ) const {
  	return _nTet;
  }

  dtInt unstructured3dMesh::getNPyr( void ) const {
  	return _nPyr;
  }

  dtInt unstructured3dMesh::getNPri( void ) const {
  	return _nPri;
  }	
}
