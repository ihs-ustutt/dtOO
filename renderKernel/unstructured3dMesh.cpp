#include "unstructured3dMesh.h"
#include <progHelper.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
	unstructured3dMesh::unstructured3dMesh() {
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
	}
	
	vectorHandling< dtPoint3 > const & unstructured3dMesh::refP3( void ) const {
		return _pp;
	}	
}
