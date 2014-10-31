#include "scaFunctionDirector.h"
#include <logMe/logMe.h>

namespace dtOO {
  scaFunctionDirector::scaFunctionDirector() {
  }

  scaFunctionDirector::~scaFunctionDirector() {
  }

  void scaFunctionDirector::setBuilder(scaFunctionBuilder* _builderToSet) {
    _builder = _builderToSet;
  }

  int scaFunctionDirector::construct(vectorHandling< analyticFunction* >* sFunP) {
    int errFlag;

    errFlag = _builder->buildPart();
    errFlag = _builder->getResult(sFunP);
    _builder->final();
  }
}