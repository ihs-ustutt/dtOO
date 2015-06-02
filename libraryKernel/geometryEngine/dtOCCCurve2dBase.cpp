#include "dtOCCCurve2dBase.h"

#include <logMe/logMe.h>
#include <Geom2d_Curve.hxx>

namespace dtOO {
  struct dtOCCCurve2dBase::OCCHanWrap {
    Handle_Geom2d_Curve occHandle;
  };
	
	dtOCCCurve2dBase::dtOCCCurve2dBase() {	
		_hanWrap = OCCHanWrapPtr(new OCCHanWrap);
	}

  void dtOCCCurve2dBase::setOCC(Handle_Geom2d_Curve occCurve) {
    _hanWrap->occHandle = occCurve;
  }
	
  Handle_Geom2d_Curve const dtOCCCurve2dBase::getOCC( void ) const {
    return _hanWrap->occHandle;
  }
  
	std::string dtOCCCurve2dBase::dumpToString(void) const {
		std::stringstream ss;
    
    Handle(Geom2d_Curve) const & cH = getOCC();
    Handle(Standard_Type) const & stH = cH->DynamicType();
    ss << stH->Name();
    
    return ss.str();
  }  

	dtOCCCurve2dBase::dtOCCCurve2dBase(const dtOCCCurve2dBase& orig) {
	}

	dtOCCCurve2dBase::~dtOCCCurve2dBase() {
	}
}
