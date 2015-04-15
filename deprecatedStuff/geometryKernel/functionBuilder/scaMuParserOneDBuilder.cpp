#include "scaMuParserOneDBuilder.h"

#include <analyticFunctionHeaven/scaMuParserOneD.h>
#include <progHelper.h>
#include <logMe/logMe.h>

namespace dtOO {
  scaMuParserOneDBuilder::scaMuParserOneDBuilder() {
    _scaMuParserOneD = NULL;
  }

  scaMuParserOneDBuilder::~scaMuParserOneDBuilder() {
    delete _scaMuParserOneD;
  }

  int scaMuParserOneDBuilder::buildPart(void) {
    /* ------------------------------------------------------------------------ */
    /* check input */
    /* ------------------------------------------------------------------------ */
    bool hasString = false;
    bool hasFloat = false;
    bool hasScaFun = false;
    
    if (_string.getNAttributes() >= 2) {
//      dt__THROW( buildPart(),
//              << dt__eval( _string.getNAttributes() ) << std::endl
//              << "Should be: "<< std::endl
//              << "(1) argument name" << std::endl
//              << "(2) expression");
      hasString = true;
    }
//    else {
//      
//    }
    if (_float.getNAttributes() == 2) {
//      dt__THROW( buildPart(),
//              << dt__eval( _float.getNAttributes() ) << std::endl
//              << "Should be: "<< std::endl
//              << "(1) min value of argument" << std::endl
//              << "(2) max value of argument");
      hasFloat = true;
    }
//    else {
//      hasFloat = true;
//    }
    if (_scaFunctionP.getNAttributes() == 1) {
      hasScaFun = true;
    }

    if (hasFloat && hasString) {
      /* ------------------------------------------------------------------------ */
      /* create parser */
      /* ------------------------------------------------------------------------ */
      std::string argument = *( _string.getAttribute(1) );
      std::string expression = *( _string.getAttribute(2) );
      _scaMuParserOneD =  new scaMuParserOneD( expression, argument );

      /* ------------------------------------------------------------------------ */
      /* set argument range */
      /* ------------------------------------------------------------------------ */
      _scaMuParserOneD->setArgumentRange( *(_float.getAttribute(1)), *(_float.getAttribute(2)) );

      /* ------------------------------------------------------------------------ */
      /* create derivative */
      /* ------------------------------------------------------------------------ */
      if (_string.getNAttributes() == 3) { 
        argument = *( _string.getAttribute(1) );
        expression = *( _string.getAttribute(3) );
        _scaMuParserOneD->setDerivative( expression, argument );
      }
    }
    else if (hasScaFun && !hasString && !hasFloat) {
      //
      // copy existing
      //
      analyticFunction * const * const sFunP = _scaFunctionP.getAttribute(1);
      //
      // check cast
      //
      scaMuParserOneD * sMPOD;
      dt__mustCastWM(
        *sFunP, 
        scaMuParserOneD, 
        sMPOD,
        << "Object to copy is not a scaSISLSplineTwoD."
      );
      //
      // copy
      //
      _scaMuParserOneD = new scaMuParserOneD( *sMPOD );
    }
    else {
      dt__THROW(buildPart(),
              << dt__eval(hasScaFun) << std::endl
              << dt__eval(hasString) << std::endl
              << dt__eval(hasFloat) );
    }
  }

  int scaMuParserOneDBuilder::getResult(vectorHandling<analyticFunction*>* sFunP) {
    sFunP->push_back( _scaMuParserOneD->clone() );
  }
}