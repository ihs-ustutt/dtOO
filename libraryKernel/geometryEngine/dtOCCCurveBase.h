#ifndef DTOCCCURVEBASE_H
#define	DTOCCCURVEBASE_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

class Handle_Geom_Curve;

namespace dtOO {    
  class dtOCCCurveBase {
  public:
    dt__classOnlyName(dtOCCCurveBase);
    dtOCCCurveBase();
    dtOCCCurveBase(const dtOCCCurveBase& orig);
    virtual ~dtOCCCurveBase();
    void setOCC(Handle_Geom_Curve occCurve);    
    Handle_Geom_Curve const getOCC( void ) const;
	  std::string dumpToString(void) const;
  private:
    class OCCHanWrap;
    dt__pH(OCCHanWrap) _hanWrap;
  };
}
#endif	/* DTOCCCURVEBASE_H */

