#ifndef DTXMLPARSERDECORATORINTERFACE_H
#define	DTXMLPARSERDECORATORINTERFACE_H

#include <vector>

#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class pointContainer;
  class vectorContainer;

  class dtXmlParserDecoratorInterface {
  public:  
    dt__classOnlyName(dtXmlParserDecoratorInterface);
    dtXmlParserDecoratorInterface();
    virtual ~dtXmlParserDecoratorInterface();
    /**
     * decorator for creating analyticGeometries
     * @param toBuildP pointer to xml element
     * @param sFunP vector of all scalar functions
     * @param geoP vector of geometries for storing all construction objects
     * @param aGeoP result vector to store analyticGeometry
     */
    virtual void buildPart(QDomElement ** toBuildP,
                                  pointContainer * const pointContainerP,
                                  vectorContainer * const vectorContainerP,      
                                  vectorHandling< constValue * > const * const cValP,
                                  vectorHandling< analyticFunction * > const * const sFunP,
                                  vectorHandling< analyticGeometry * > const * const depAGeoP,
                                  vectorHandling< analyticGeometry * > * aGeoP ) const = 0;
    virtual void buildPartCompound(
      QDomElement ** toBuildP,
      pointContainer * const pointContainerP,
      vectorContainer * const vectorContainerP,      
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP,
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      vectorHandling< analyticGeometry * > * aGeoP 
    ) const;    
  private:

  };
}
#endif	/* DTXMLPARSERDECORATORINTERFACE_H */

