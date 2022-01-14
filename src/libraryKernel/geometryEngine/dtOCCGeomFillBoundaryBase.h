#ifndef dtOCCGeomFillBoundaryBase_H
#define	dtOCCGeomFillBoundaryBase_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

class Handle_GeomFill_Boundary;

namespace dtOO {    
  class dtOCCGeomFillBoundaryBase {
  public:
    dt__classOnlyName(dtOCCGeomFillBoundaryBase);
    dtOCCGeomFillBoundaryBase();
	  dtOCCGeomFillBoundaryBase(Handle_GeomFill_Boundary & orig);    
    virtual ~dtOCCGeomFillBoundaryBase();
    void setOCC(Handle_GeomFill_Boundary occCurve);    
    Handle_GeomFill_Boundary const getOCC( void ) const;
	  std::string dumpToString(void) const;
  private:
    class OCCHanWrap;
    dt__pH(OCCHanWrap) _hanWrap;
  };
}
#endif	/* dtOCCGeomFillBoundaryBase_H */

