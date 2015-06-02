#include "dtOCCCurveBase.h"

#include <logMe/logMe.h>
#include <Geom_Curve.hxx>

namespace dtOO {
  struct dtOCCCurveBase::OCCHanWrap {
    Handle_Geom_Curve occHandle;
  };
	
	dtOCCCurveBase::dtOCCCurveBase() {	
		_hanWrap = OCCHanWrapPtr(new OCCHanWrap);
	}

  void dtOCCCurveBase::setOCC(Handle_Geom_Curve occCurve) {
		if ( occCurve.IsNull() ) {
			dt__throw(setOCC(), << "Try to set NULL handle in setOCC().");
		}		
    _hanWrap->occHandle = occCurve;
  }
	
  Handle_Geom_Curve const dtOCCCurveBase::getOCC( void ) const {
		if ( _hanWrap->occHandle.IsNull() ) {
			dt__throw(getOCC(), << "getOCC() return NULL Handle.");
		}
    return _hanWrap->occHandle;
  }

	std::string dtOCCCurveBase::dumpToString(void) const {
		std::stringstream ss;
    
    Handle(Geom_Curve) const & cH = getOCC();
    Handle(Standard_Type) const & stH = cH->DynamicType();
    ss << stH->Name();
    
    return ss.str();
  }

	dtOCCCurveBase::dtOCCCurveBase(const dtOCCCurveBase& orig) {
	}

	dtOCCCurveBase::~dtOCCCurveBase() {
	}
}
