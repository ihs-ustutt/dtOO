#ifndef TYPECAST_H
#define	TYPECAST_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>

namespace dtOO {
  class analyticGeometry;
  
  /**
   * @todo: This is a senseless transformer. Does not work on any kind of 
   *        analyticGeometry. Remove?
   */
  class typeCast : public dtTransformer {
  public:
    DTCLASSNAMEMETHOD(typeCast);
    DTCLASSLOGMETHOD(typeCast);    
    typeCast();
    typeCast(const typeCast& orig);
    virtual ~typeCast();
    virtual bool isNecessary( void ) const;
    void init( ::QDomElement * transformerElementP, 
               pointContainer * const pointContainerP,
               vectorContainer * const vectorContainerP,    
               vectorHandling< constValue * > const * const cValP,
               vectorHandling< scaFunction * > const * const sFunP,
               vectorHandling< analyticGeometry * > const * const depAGeoP );
    virtual vectorHandling< analyticGeometry * > apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const;
  private:
    float _tolerance;
    float _maxStep;
  };
}
#endif	/* TYPECAST_H */

