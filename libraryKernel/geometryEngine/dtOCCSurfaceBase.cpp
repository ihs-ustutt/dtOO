#include "dtOCCSurfaceBase.h"

#include <logMe/logMe.h>
#include <Geom_Surface.hxx>

namespace dtOO {
  struct dtOCCSurfaceBase::OCCHanWrap {
    Handle_Geom_Surface occHandle;
  };
		
	dtOCCSurfaceBase::dtOCCSurfaceBase() 
    : _hanWrap(new OCCHanWrap()) {	
	}
	
	dtOCCSurfaceBase::dtOCCSurfaceBase(Handle_Geom_Surface & orig) 
    : _hanWrap(new OCCHanWrap()) {
	  _hanWrap->occHandle = orig;
	}

  void dtOCCSurfaceBase::setOCC(Handle_Geom_Surface occSurface) {
    _hanWrap->occHandle = occSurface;
  }

  Handle_Geom_Surface const dtOCCSurfaceBase::getOCC( void ) const {
    return _hanWrap->occHandle;
  }
  
 std::string dtOCCSurfaceBase::dumpToString(void) const {
		std::stringstream ss;
    
    Handle(Geom_Surface) const & sH = getOCC();
    Handle(Standard_Type) const & stH = sH->DynamicType();
    ss << stH->Name();
    
    return ss.str();
  }

	dtOCCSurfaceBase::~dtOCCSurfaceBase() {

	}
}
