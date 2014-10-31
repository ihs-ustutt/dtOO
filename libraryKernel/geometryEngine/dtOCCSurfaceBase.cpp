#include "dtOCCSurfaceBase.h"

#include <logMe/logMe.h>
#include <Geom_Surface.hxx>

namespace dtOO {
  struct dtOCCSurfaceBase::OCCHanWrap {
    Handle_Geom_Surface occHandle;
  };
		
	dtOCCSurfaceBase::dtOCCSurfaceBase() {	
		_hanWrap = OCCHanWrapPtr(new OCCHanWrap);
	}
	
	dtOCCSurfaceBase::dtOCCSurfaceBase(Handle_Geom_Surface & orig) {
		_hanWrap = OCCHanWrapPtr(new OCCHanWrap);		
	  _hanWrap->occHandle = orig;
	}

  void dtOCCSurfaceBase::setOCC(Handle_Geom_Surface occSurface) {
    _hanWrap->occHandle = occSurface;
  }

  Handle_Geom_Surface const dtOCCSurfaceBase::getOCC( void ) const {
    return _hanWrap->occHandle;
  }

	dtOCCSurfaceBase::dtOCCSurfaceBase(const dtOCCSurfaceBase& orig) {
	}

	dtOCCSurfaceBase::~dtOCCSurfaceBase() {
	}
}
