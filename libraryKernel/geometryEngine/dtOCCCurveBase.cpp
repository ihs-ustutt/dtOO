#include "dtOCCCurveBase.h"

#include <logMe/logMe.h>
#include <Geom_Curve.hxx>

namespace dtOO {
  struct dtOCCCurveBase::OCCHanWrap {
    Handle_Geom_Curve occHandle;
  };
	
	dtOCCCurveBase::dtOCCCurveBase() 
    : _hanWrap(new OCCHanWrap) {
	}

	dtOCCCurveBase::dtOCCCurveBase(Handle_Geom_Curve & orig) 
    : _hanWrap(new OCCHanWrap()) {
	  _hanWrap->occHandle = orig;
	}
  
  void dtOCCCurveBase::setOCC(Handle_Geom_Curve occCurve) {
		dt__throwIf(occCurve.IsNull(), setOCC() );
    
    _hanWrap->occHandle = occCurve;
  }
	
  Handle_Geom_Curve const dtOCCCurveBase::getOCC( void ) const {
		dt__throwIf(_hanWrap->occHandle.IsNull(), getOCC());
    
    return _hanWrap->occHandle;
  }

	std::string dtOCCCurveBase::dumpToString(void) const {
		std::stringstream ss;
    
    Handle(Geom_Curve) const & cH = getOCC();
    Handle(Standard_Type) const & stH = cH->DynamicType();

    ss 
    << dt__dumpToString(
      << dt__eval(stH->Name()) << std::endl
      << dt__eval( _hanWrap->occHandle.Access() )
    );    
    
    return ss.str();
  }

	dtOCCCurveBase::~dtOCCCurveBase() {

	}
}
