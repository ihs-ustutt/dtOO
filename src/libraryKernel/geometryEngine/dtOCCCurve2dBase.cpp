#include "dtOCCCurve2dBase.h"

#include <logMe/logMe.h>
#include <Geom2d_Curve.hxx>

namespace dtOO {
  struct dtOCCCurve2dBase::OCCHanWrap {
    Handle_Geom2d_Curve occHandle;
  };
	
	dtOCCCurve2dBase::dtOCCCurve2dBase() 
    : _hanWrap(new OCCHanWrap) {
	}

	dtOCCCurve2dBase::dtOCCCurve2dBase(Handle_Geom2d_Curve & orig) 
    : _hanWrap(new OCCHanWrap()) {
	  _hanWrap->occHandle = orig;
	}
  
  void dtOCCCurve2dBase::setOCC(Handle_Geom2d_Curve occCurve) {
		dt__throwIf(occCurve.IsNull(), setOCC() );    

    _hanWrap->occHandle = occCurve;
  }
	
  Handle_Geom2d_Curve const dtOCCCurve2dBase::getOCC( void ) const {
		dt__throwIf(_hanWrap->occHandle.IsNull(), getOCC());
    
    return _hanWrap->occHandle;
  }
  
	std::string dtOCCCurve2dBase::dumpToString(void) const {
		std::stringstream ss;
    
    Handle(Geom2d_Curve) const & cH = getOCC();
    Handle(Standard_Type) const & stH = cH->DynamicType();
    
    ss 
    << dt__dumpToString(
      << dt__eval(stH->Name()) << std::endl
      << dt__eval( _hanWrap->occHandle.Access() )
    );
    
    return ss.str();
  }  

	dtOCCCurve2dBase::~dtOCCCurve2dBase() {

	}
}
