#include "scaMuParserOneDFunctionDecorator.h"
#include <functionHeaven/scaMuParserOneD.h>
#include <functionHeaven/analyticFunction.h>
#include <interfaceHeaven/ptrHandling.h>
#include <dtTransformerHeaven/dtTransformer.h>
#include <logMe/logMe.h>

#include <QtXml/QDomElement>

namespace dtOO {
  scaMuParserOneDFunctionDecorator::scaMuParserOneDFunctionDecorator() {
  }

  scaMuParserOneDFunctionDecorator::~scaMuParserOneDFunctionDecorator() {
  }

  void scaMuParserOneDFunctionDecorator::buildPart(QDomElement const & toBuildP, 
                                                                  vectorHandling< constValue * > const * const cValP, 
                                                                  vectorHandling< analyticFunction * > const * const depSFunP,
                                                                  vectorHandling< analyticFunction * > * sFunP) const {
    //
    //check input
    //
    bool hasString = hasChild("string", toBuildP);
    bool hasRange = hasChild("range", toBuildP);
    bool hasScaFun = hasChild("function", toBuildP);
    
    if (hasString && hasRange && !hasScaFun) {
      /* ---------------------------------------------------------------------- */
      /* put argument and expression to builder */
      /* ---------------------------------------------------------------------- */
      std::string argument;
      std::string expression;
      float xMin;
      float xMax;

      QDomElement wElement = toBuildP.firstChildElement();
      while ( !wElement.isNull() ) {
        //
        // string
        //
        if ( is( "string", wElement) ) {
          //
          // expression
          //
          if ( getAttributeStr("type", wElement) == "expression") {
            expression = this->createString( &wElement, cValP, depSFunP );
          }
          //
          // argument
          //
          if ( getAttributeStr("type", wElement) == "argument") {
            argument = this->createString( &wElement, cValP, depSFunP );
          }  
        }
        //
        // range
        //
        else if ( is( "range", wElement) ) {
          xMin = muParseString(
                   replaceUsedFunctions(
                     getAttributeStr("min", wElement),
                     cValP,
                     depSFunP
                   )
                 );
          xMax = muParseString(
                   replaceUsedFunctions(
                     getAttributeStr("max", wElement),
                     cValP,
                     depSFunP
                   )
                 );          
        }    
        //
        // goto next sibling
        //
        wElement = QDomElement( wElement.nextSiblingElement() );
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
      dt__THROW(buildPart(), 
              << DTLOGEVAL(hasString) << LOGDEL
              << DTLOGEVAL(hasRange) << LOGDEL
              << DTLOGEVAL(hasScaFun) );
    }

		//
		// transform
		//
    ptrHandling< dtTransformer > cTransP(  
		  createTransformer(&toBuildP, cValP, depSFunP)
		);	
    if ( cTransP->isNecessary() ) {
      *sFunP = cTransP->apply(sFunP);
    }
  }
}