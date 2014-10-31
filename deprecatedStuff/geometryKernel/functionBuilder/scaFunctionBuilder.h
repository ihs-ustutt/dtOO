#ifndef SCAFUNCTIONBUILDER_H
#define	SCAFUNCTIONBUILDER_H

#include <dtLinearAlgebra.h>
#include <vector>
#include <string>
#include <templateClass/vectorTemplate.h>
#include <functionHeaven/analyticFunction.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class scaFunctionBuilder {
  public:
    dt__CLASSNAME(scaFunctionBuilder);    
    /**
     * constructor
     */
    scaFunctionBuilder();
    /**
     * destructor
     */
    virtual ~scaFunctionBuilder();
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
    virtual int getResult(vectorHandling< analyticFunction* >* sFunP) = 0;
    /**
     * optional final routine for a builder that inherits from this class
     */
    virtual void final(void);
    /**
     * set a point attribute
     * @param _attributeToSet attribute
     */
    void setAttribute(dtPoint2 const * const _attributeToSet);
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
     * set a string attribute
     * @param _attributeToSet attribute
     */  
    void setAttribute(string const * const _attributeToSet);  
    /**
     * set a scaFunction attribute
     * @param _attributeToSet attribute
     */  
    void setAttribute(analyticFunction* const * const _attributeToSet);
  protected:
    vectorTemplate< dtPoint2 > _pointTwo;
    vectorTemplate< int > _integer;
    vectorTemplate< float > _float;
    vectorTemplate< string > _string;
    vectorTemplate< analyticFunction * > _scaFunctionP;
  };
}
#endif	/* SCAFUNCTIONBUILDER_H */

