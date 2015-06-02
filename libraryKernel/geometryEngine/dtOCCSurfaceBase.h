#ifndef DTOCCSURFACEBASE_H
#define	DTOCCSURFACEBASE_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <boost/shared_ptr.hpp>

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
    typedef boost::shared_ptr<OCCHanWrap> OCCHanWrapPtr;    
    OCCHanWrapPtr _hanWrap;
  };
}
#endif	/* DTOCCSURFACEBASE_H */

