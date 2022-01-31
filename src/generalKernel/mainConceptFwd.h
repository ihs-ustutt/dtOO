#ifndef mainConceptFwd_H
#define	mainConceptFwd_H

#include <dtOOTypeDef.h>

#include <interfaceHeaven/labeledVectorHandling.h>

namespace dtOO {
//#ifndef constValue_H
  class constValue;
//#endif  
  typedef labeledVectorHandling< constValue * > cVPtrVec;
//#ifndef analyticFunction_H
  class analyticFunction;
//#endif    
  typedef labeledVectorHandling< analyticFunction * > aFPtrVec;
//#ifndef analyticGeometry_H
  class analyticGeometry;
//#endif    
  typedef labeledVectorHandling< analyticGeometry * > aGPtrVec;
//#ifndef boundedVolume_H
  class boundedVolume;
//#endif  
  typedef labeledVectorHandling< boundedVolume * > bVPtrVec;
//#ifndef dtCase_H
  class dtCase;
//#endif    
  typedef labeledVectorHandling< dtCase * > dCPtrVec;  
//#ifndef dtPlugin_H
  class dtPlugin;
//#endif    
  typedef labeledVectorHandling< dtPlugin * > dPPtrVec;    
}
#endif	/* mainConceptFwd_H */

