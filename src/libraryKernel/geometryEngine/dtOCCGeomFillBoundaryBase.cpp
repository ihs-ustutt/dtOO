#include "dtOCCGeomFillBoundaryBase.h"

#include <logMe/logMe.h>
#include <GeomFill_Boundary.hxx>

namespace dtOO {
  struct dtOCCGeomFillBoundaryBase::OCCHanWrap {
    Handle_GeomFill_Boundary occHandle;
  };
	
	dtOCCGeomFillBoundaryBase::dtOCCGeomFillBoundaryBase() 
    : _hanWrap(new OCCHanWrap) {
	}

	dtOCCGeomFillBoundaryBase::dtOCCGeomFillBoundaryBase(Handle_GeomFill_Boundary & orig) 
    : _hanWrap(new OCCHanWrap()) {
	  _hanWrap->occHandle = orig;
	}
  
  void dtOCCGeomFillBoundaryBase::setOCC(Handle_GeomFill_Boundary occCurve) {
		dt__throwIf(occCurve.IsNull(), setOCC() );
    
    _hanWrap->occHandle = occCurve;
  }
	
  Handle_GeomFill_Boundary const dtOCCGeomFillBoundaryBase::getOCC( void ) const {
		dt__throwIf(_hanWrap->occHandle.IsNull(), getOCC());
    
    return _hanWrap->occHandle;
  }

	std::string dtOCCGeomFillBoundaryBase::dumpToString(void) const {
		std::stringstream ss;
    
    Handle(GeomFill_Boundary) const & bH = getOCC();
    Handle(Standard_Type) const & stH = bH->DynamicType();

    ss 
    << dt__dumpToString(
      << dt__eval(stH->Name()) << std::endl
      << dt__eval( _hanWrap->occHandle.Access() )
    );    
    
    return ss.str();
  }

	dtOCCGeomFillBoundaryBase::~dtOCCGeomFillBoundaryBase() {

	}
}
