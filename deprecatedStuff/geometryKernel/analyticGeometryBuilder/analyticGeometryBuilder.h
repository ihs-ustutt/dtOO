#ifndef ANALYTICGEOMETRYBUILDER_H
#define	ANALYTICGEOMETRYBUILDER_H

#include <dtLinearAlgebra.h>
//#include <vector>
#include <templateClass/vectorTemplate.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <functionHeaven/scaFunction.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  /**
   * Abstract analytic geometry builder class.
   * @author A. Tismer
   * @date 27.12.2012
   */
  class analyticGeometryBuilder {
    DTCLASSNAMEMETHOD(analyticGeometryBuilder);
    DTCLASSLOGMETHOD(analyticGeometryBuilder);
  public:
    /**
     * constructor
     */
    analyticGeometryBuilder();
    /**
     * destructor
     */
    virtual ~analyticGeometryBuilder();
    /**
     * build the part that this builder is responsible for
     * @return error flag
     */
    virtual int buildPart(void) = 0;
    /**
     * get result of this building process
     * @param aGeo vector of analyticGeometries
     * @return error flag
     */
    virtual void getResult(vectorHandling< analyticGeometry* >* aGeo) = 0;
    /**
     * optional final routine for a builder that inherits from this class
     */
    virtual void final(void);
    /**
     * set a point attribute
     * @param _attributeToSet attribute
     */
    void setAttribute(dtPoint3 const * const _attributeToSet);
    /**
     * set a point attribute
     * @param _attributeToSet attribute
     */
    void setAttribute(dtPoint2 const * const _attributeToSet);
    /**
     * set a vector attribute
     * @param _attributeToSet attribute
     */  
    void setAttribute(dtVector3 const * const _attributeToSet);
    /**
     * set an integer attribute
     * @param _attributeToSet attribute
     */  
    void setAttribute(int const * const _attributeToSet);
    /**
     * set a float attribute
     * @param _attributeToSet attribute
     */  
    void setAttribute(float const * const _attributeToSet);
    /**
     * set an analyticGeometry attribute
     * @param _attributeToSet attribute
     */  
    void setAttribute(analyticGeometry* const * const _attributeToSet);
    /**
     * set a scaFunction attribute
     * @param _attributeToSet attribute
     */  
    void setAttribute(scaFunction* const * const _attributeToSet);
    /**
     * set an option attribute
     * @param _attributeToSet attribute
     */  
    void setAttribute(string const * const _attributeToSet);
  protected:
    vectorTemplate< dtPoint3 > _point;
    vectorTemplate< dtPoint2 > _pointTwo;
    vectorTemplate< dtVector3 > _vector;
    vectorTemplate< int > _integer;
    vectorTemplate< float > _float;
    vectorTemplate< analyticGeometry * > _analyticGeometryP;
    vectorTemplate< scaFunction * > _scaFunctionP;
    vectorTemplate< string > _option;
  };
}
#endif	/* ANALYTICGEOMETRYBUILDER_H */

