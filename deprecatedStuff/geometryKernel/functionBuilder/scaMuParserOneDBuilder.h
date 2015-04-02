#ifndef SCAMUPARSERONEDBUILDER_H
#define	SCAMUPARSERONEDBUILDER_H

#include "scaFunctionBuilder.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class scaMuParserOneD;

  class scaMuParserOneDBuilder : public scaFunctionBuilder {
    public:
      dt__classOnlyName(scaMuParserOneDBuilder);    
      scaMuParserOneDBuilder();
      virtual ~scaMuParserOneDBuilder();
      virtual int buildPart(void);
      virtual int getResult(vectorHandling< analyticFunction* >* sFunP);
    private:
      scaMuParserOneD * _scaMuParserOneD;
  };
}
#endif	/* SCAMUPARSERONEDBUILDER_H */

