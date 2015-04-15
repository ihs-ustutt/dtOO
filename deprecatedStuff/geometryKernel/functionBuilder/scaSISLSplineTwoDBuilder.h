#ifndef SCASISLSPLINETWODBUILDER_H
#define	SCASISLSPLINETWODBUILDER_H

#include <dtLinearAlgebra.h>
#include "scaFunctionBuilder.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class scaSISLSplineTwoD;

  class scaSISLSplineTwoDBuilder : public scaFunctionBuilder {
    public:
      dt__classOnlyName(scaSISLSplineTwoDBuilder);
      scaSISLSplineTwoDBuilder();
      virtual ~scaSISLSplineTwoDBuilder();
      virtual int buildPart(void);
      virtual int getResult(vectorHandling< analyticFunction* >* sFunP);
    private:
      scaSISLSplineTwoD * _scaSISLSplineTwoDP;
  };
}
#endif	/* SCASISLSPLINETWODBUILDER_H */

