#include "scaFunctionBuilder.h"

namespace dtOO {
  scaFunctionBuilder::scaFunctionBuilder() {

  }

  scaFunctionBuilder::~scaFunctionBuilder() {

  }

  void scaFunctionBuilder::final(void) {
    _pointTwo.~vectorTemplate();
    _integer.~vectorTemplate();
    _float.~vectorTemplate();
  }

  void scaFunctionBuilder::setAttribute(dtPoint2 const * const _attributeToSet) {
    _pointTwo.setAttribute(_attributeToSet);
  }

  void scaFunctionBuilder::setAttribute(int const * const _attributeToSet) {
    _integer.setAttribute(_attributeToSet);
  }

  void scaFunctionBuilder::setAttribute(float const * const _attributeToSet) {
    _float.setAttribute(_attributeToSet);
  }

  void scaFunctionBuilder::setAttribute(string const * const _attributeToSet) {
    _string.setAttribute(_attributeToSet);
  }

  void scaFunctionBuilder::setAttribute(analyticFunction* const * const _attributeToSet) {
    _scaFunctionP.setAttribute(_attributeToSet);
  }
}