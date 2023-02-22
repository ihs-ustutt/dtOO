#ifndef mainConceptFwd_H
#define	mainConceptFwd_H

#include <dtOOTypeDef.h>

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  class dtCase;
  class dtPlugin;
  class dtCurve;
  class dtSurface;
  class dtMeshOperator;

  template< typename T > class vectorHandling;
  typedef vectorHandling< constValue * >       vH_constValue;
  typedef vectorHandling< analyticFunction * > vH_analyticFunction;
  typedef vectorHandling< analyticGeometry * > vH_analyticGeometry;
  typedef vectorHandling< boundedVolume * >    vH_boundedVolume;
  typedef vectorHandling< dtCase * >           vH_dtCase;
  typedef vectorHandling< dtPlugin * >         vH_dtPlugin;  
  typedef vectorHandling< dtCurve * >          vH_dtCurve;
  typedef vectorHandling< dtSurface * >        vH_dtSurface;
   
  template< typename T > class labeledVectorHandling;
  typedef labeledVectorHandling< dtMeshOperator * >   lvH_dtMeshOperator;  
  typedef labeledVectorHandling< constValue * >       lvH_constValue;
  typedef labeledVectorHandling< analyticFunction * > lvH_analyticFunction;
  typedef labeledVectorHandling< analyticGeometry * > lvH_analyticGeometry;
  typedef labeledVectorHandling< boundedVolume * >    lvH_boundedVolume;
  typedef labeledVectorHandling< dtCase * >           lvH_dtCase;
  typedef labeledVectorHandling< dtPlugin * >         lvH_dtPlugin;  
}
#endif	/* mainConceptFwd_H */

