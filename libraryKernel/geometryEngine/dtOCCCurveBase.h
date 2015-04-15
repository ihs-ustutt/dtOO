#ifndef DTOCCCURVEBASE_H
#define	DTOCCCURVEBASE_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <boost/shared_ptr.hpp>

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
  private:
    class OCCHanWrap;
    typedef boost::shared_ptr<OCCHanWrap> OCCHanWrapPtr;    
    OCCHanWrapPtr _hanWrap;
  };
}
#endif	/* DTOCCCURVEBASE_H */

