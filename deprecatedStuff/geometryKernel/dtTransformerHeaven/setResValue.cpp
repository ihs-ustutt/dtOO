#include "setResValue.h"
#include <analyticFunctionHeaven/scaFunction.h>

#include <progHelper.h>
#include <logMe/logMe.h>

namespace dtOO {
  setResValue::setResValue() {
  }

  setResValue::~setResValue() {
  }

  vectorHandling< scaFunction * > setResValue::apply( vectorHandling< scaFunction * > const * const sFunP ) const {
    vectorHandling< scaFunction * > transSFun;
    for (int ii=0;ii<sFunP->size();ii++) {
      scaFunction * sF = sFunP->get(ii)->clone();
      sF->setResValue(_resValue);
      transSFun.push_back( sF );
    }
    return transSFun;
  }

  void setResValue::init( QDomElement * transformerElementP, 
                                  vectorHandling< constValue * > const * const cValP,
                                  vectorHandling< scaFunction * > const * const sFunP) {
    handleFloat(
      "resValue", 
      getAttributeFloatMuParse(
        "resValue", 
        *transformerElementP, 
        cValP, 
        sFunP
      )
    );     
  }

  bool setResValue::isNecessary( void ) const {
    return true;
  }

  void setResValue::handleFloat(std::string const name, float const value) {
    if (name == "resValue" ) {
      _resValue = value;
      return;
    }
    dtTransformer::handleFloat(name, value);
  }
}

