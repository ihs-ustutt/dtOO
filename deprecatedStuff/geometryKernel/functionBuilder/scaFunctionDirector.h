#ifndef SCAFUNCTIONDIRECTOR_H
#define	SCAFUNCTIONDIRECTOR_H

#include <functionHeaven/scaFunction.h>
#include "scaFunctionBuilder.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class scaFunctionDirector {
  public:
    dt__CLASSNAME(scaFunctionDirector);    
    /**
     * constructor
     */
    scaFunctionDirector();
    /**
     * destructor
     */
    virtual ~scaFunctionDirector();
    /**
     * set the pointer to the desired builder
     * @param _builderToSet pointer to builder
     */
    void setBuilder(scaFunctionBuilder* _builderToSet);
    /**
     * construct the object
     * @param aFunP result of builder process
     * @return error flag
     */
    int construct(vectorHandling< analyticFunction* >* sFunP);
  private:
    scaFunctionBuilder* _builder;
  };
}
#endif	/* SCAFUNCTIONDIRECTOR_H */

