#ifndef DTOCCCURVE2DBASE_H
#define	DTOCCCURVE2DBASE_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <boost/shared_ptr.hpp>

class Handle_Geom2d_Curve;

namespace dtOO {    
  class dtOCCCurve2dBase {
  public:
    dt__CLASSNAME(dtOCCCurve2dBase);
    dtOCCCurve2dBase();
    dtOCCCurve2dBase(const dtOCCCurve2dBase& orig);
    virtual ~dtOCCCurve2dBase();
    void setOCC(Handle_Geom2d_Curve occCurve);    
    Handle_Geom2d_Curve const getOCC( void ) const;    
  private:
    class OCCHanWrap;
    typedef boost::shared_ptr<OCCHanWrap> OCCHanWrapPtr;    
    OCCHanWrapPtr _hanWrap;
  };
}
#endif	/* DTOCCCURVE2DBASE_H */

