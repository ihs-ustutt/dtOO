#ifndef DTOCCCURVE2DBASE_H
#define	DTOCCCURVE2DBASE_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <boost/shared_ptr.hpp>

class Handle_Geom2d_Curve;

namespace dtOO {    
  class dtOCCCurve2dBase {
  public:
    dt__classOnlyName(dtOCCCurve2dBase);
    dtOCCCurve2dBase();
    virtual ~dtOCCCurve2dBase();
    void setOCC(Handle_Geom2d_Curve occCurve);    
    Handle_Geom2d_Curve const getOCC( void ) const;    
	  std::string dumpToString(void) const;    
  private:
    class OCCHanWrap;
    dt__pH(OCCHanWrap) _hanWrap;
  };
}
#endif	/* DTOCCCURVE2DBASE_H */

