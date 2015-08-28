#ifndef DTOCCSURFACEBASE_H
#define	DTOCCSURFACEBASE_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

class Handle_Geom_Surface;

namespace dtOO {    
  class dtOCCSurfaceBase {
  public:
    dt__classOnlyName(dtOCCSurfaceBase);  
    dtOCCSurfaceBase();
	  dtOCCSurfaceBase(Handle_Geom_Surface & orig);
    dtOCCSurfaceBase(const dtOCCSurfaceBase& orig);
    virtual ~dtOCCSurfaceBase();
    void setOCC(Handle_Geom_Surface);    
    Handle_Geom_Surface const getOCC( void ) const;
    std::string dumpToString(void) const;
  private:
    class OCCHanWrap;
    dt__pH(OCCHanWrap) _hanWrap;
  };
}
#endif	/* DTOCCSURFACEBASE_H */

