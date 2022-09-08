#ifndef mainConceptFwd_H
#define	mainConceptFwd_H

#include <dtOOTypeDef.h>

#include <interfaceHeaven/vectorHandling.h>
#include <interfaceHeaven/labeledVectorHandling.h>

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  class dtCase;
  class dtPlugin;
  class dtCurve;
  class dtSurface;
  
  typedef vectorHandling< constValue * >       vH_constValue;
  typedef vectorHandling< analyticFunction * > vH_analyticFunction;
  typedef vectorHandling< analyticGeometry * > vH_analyticGeometry;
  typedef vectorHandling< boundedVolume * >    vH_boundedVolume;
  typedef vectorHandling< dtCase * >           vH_dtCase;
  typedef vectorHandling< dtPlugin * >         vH_dtPlugin;  
  typedef vectorHandling< dtCurve * >          vH_dtCurve;
  typedef vectorHandling< dtSurface * >        vH_dtSurface;
   
  typedef labeledVectorHandling< constValue * >       lvH_constValue;
  typedef labeledVectorHandling< analyticFunction * > lvH_analyticFunction;
  typedef labeledVectorHandling< analyticGeometry * > lvH_analyticGeometry;
  typedef labeledVectorHandling< boundedVolume * >    lvH_boundedVolume;
  typedef labeledVectorHandling< dtCase * >           lvH_dtCase;
  typedef labeledVectorHandling< dtPlugin * >         lvH_dtPlugin;  
  
  [[deprecated("Please use lvH_constValue")]]
    typedef lvH_constValue cVPtrVec;
  [[deprecated("Please use lvH_analyticFunction")]]
    typedef lvH_analyticFunction aFPtrVec;
  [[deprecated("Please use lvH_analyticGeometry")]]
    typedef lvH_analyticGeometry aGPtrVec;
  [[deprecated("Please use lvH_boundedVolume")]]
    typedef lvH_boundedVolume bVPtrVec;
  [[deprecated("Please use lvH_dtCase")]]
    typedef lvH_dtCase dCPtrVec;  
  [[deprecated("Please use lvH_dtPlugin")]]
    typedef lvH_dtPlugin dPPtrVec;  
}
#endif	/* mainConceptFwd_H */

