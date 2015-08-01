#include "scaMuParserOneDAFXmlBuilder.h"

#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticFunctionHeaven/scaMuParserOneD.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/logMe.h>

#include <QtXml/QDomElement>

namespace dtOO {
  scaMuParserOneDAFXmlBuilder::scaMuParserOneDAFXmlBuilder() {
  }

  scaMuParserOneDAFXmlBuilder::~scaMuParserOneDAFXmlBuilder() {
  }

  void scaMuParserOneDAFXmlBuilder::buildPart(
	  ::QDomElement const & toBuildP, 
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cValP, 
		vectorHandling< analyticFunction * > const * const depSFunP,
		vectorHandling< analyticFunction * > * sFunP
	) const {
    //
    //check input
    //
    bool hasString = dtXmlParserBase::hasChild("string", toBuildP);
    bool hasRange = dtXmlParserBase::hasChild("range", toBuildP);
    bool hasScaFun = dtXmlParserBase::hasChild("function", toBuildP);
    
    if (hasString && hasRange && !hasScaFun) {
      /* ---------------------------------------------------------------------- */
      /* put argument and expression to builder */
      /* ---------------------------------------------------------------------- */
      std::string argument;
      std::string expression;
      float xMin;
      float xMax;

      ::QDomElement wElement = toBuildP.firstChildElement();
      while ( !wElement.isNull() ) {
        //
        // string
        //
        if ( dtXmlParserBase::is( "string", wElement) ) {
          //
          // expression
          //
          if ( dtXmlParserBase::getAttributeStr("type", wElement) == "expression") {
            expression = dtXmlParserBase::createString( &wElement, cValP, depSFunP );
          }
          //
          // argument
          //
          if ( dtXmlParserBase::getAttributeStr("type", wElement) == "argument") {
            argument = dtXmlParserBase::createString( &wElement, cValP, depSFunP );
          }  
        }
        //
        // range
        //
        else if ( dtXmlParserBase::is( "range", wElement) ) {
          xMin = dtXmlParserBase::muParseString(
                   dtXmlParserBase::replaceDependencies(
                     dtXmlParserBase::getAttributeStr("min", wElement),
                     cValP,
                     depSFunP
                   )
                 );
          xMax = dtXmlParserBase::muParseString(
                   dtXmlParserBase::replaceDependencies(
                     dtXmlParserBase::getAttributeStr("max", wElement),
                     cValP,
                     depSFunP
                   )
                 );          
        }    
        //
        // goto next sibling
        //
        wElement = ::QDomElement( wElement.nextSiblingElement() );
      }
			//
			// create muParser function
			//
			sFunP->push_back( new scaMuParserOneD(expression, argument) );
			scaMuParserOneD * sMP1d = scaMuParserOneD::SecureCast(sFunP->back());			
			//
			// set derivative if present
			//
			//
			// set range
			//
			sMP1d->setMinMax(xMin, xMax);
    }
    else {
      dt__throw(buildPart(), 
              << dt__eval(hasString) << std::endl
              << dt__eval(hasRange) << std::endl
              << dt__eval(hasScaFun) );
    }
  }
}