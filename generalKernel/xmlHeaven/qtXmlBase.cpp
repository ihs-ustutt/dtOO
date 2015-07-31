#include "qtXmlBase.h"
#include "analyticFunctionHeaven/scaOneD.h"

#include <muParser.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <QtXml/QDomNode>
#include <logMe/logMe.h>
#include <analyticFunctionHeaven/analyticFunction.h>


namespace dtOO {
  qtXmlBase::qtXmlBase() {
  }

  qtXmlBase::~qtXmlBase() {
  }

  std::string qtXmlBase::replaceUsedFunctions( 
	  std::string const expression, 
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const sFunP
	) {
    std::string returnExpression;
    returnExpression = expression;
    unsigned int found;
    //
    // check if there is a function in expression
    // $functionName(value * #constValue#)$
    //
    found = returnExpression.find("$");
    while ( found < returnExpression.size() ) {
      //
      // find start and end of function
      //
      unsigned int foundEnd = returnExpression.find("$", found+1);
      int replaceStart = found;
      int replaceEnd = foundEnd-found+1;
      std::string funString = returnExpression.substr(replaceStart+1, replaceEnd-2);

      //
      // get and cast function
      //
      dt__ptrAss( 
			  scaOneD const * const sF, 
				scaOneD::ConstDownCast( 
          sFunP->get(getStringBetween("", "(", funString)) 
        )
			);        
      
      //
      // replace function string by value
      //
      returnExpression.replace(
        replaceStart, 
        replaceEnd, 
        floatToString( 
          sF->YFloat( 
            stringToFloat(
              replaceUsedFunctions(
                getStringBetween("(", ")", funString), cValP, sFunP
              )
            ) 
          ) 
        )
      );
      
      //
      // go to next function
      //
      found = returnExpression.find("$");//, foundEnd+1);
    }
    
    //
    // check if there is a constValue in expression
    //
    found = returnExpression.find("#");
    while ( found < returnExpression.size() ) {
      //
      // find start and end of function
      //
      unsigned int foundEnd = returnExpression.find("#", found+1);
      int replaceStart = found;
      int replaceEnd = foundEnd-found+1;
      
      //
      // replace constValue string by value
      //
      returnExpression.replace(
        replaceStart, 
        replaceEnd, 
        floatToString(
          cValP->get(
            returnExpression.substr(replaceStart+1, replaceEnd-2)
          )->getValue()
        )
      );

      //
      // go to next constValue
      //
      found = returnExpression.find("#");//, foundEnd+1);
    }
    return returnExpression;
  }

  float qtXmlBase::muParseString( std::string const expression ) {
    mu::Parser parser;
    try {
      parser.SetExpr( expression );
      return ( (float) parser.Eval() );
    }
    catch (mu::Parser::exception_type &e) {
      dt__throw( 
        muParseString(), << "muParser returns: " << dt__eval(e.GetMsg())
      );
    }
  }

  int qtXmlBase::muParseStringInt( std::string const expression ) {
    int ret = static_cast< int >(muParseString(expression));
    return ret;
  }  

  float qtXmlBase::getAttributeFloatMuParse( 
    std::string const attName, 
    ::QDomElement const element, 
    vectorHandling< constValue * > const * const cValP,
    vectorHandling< analyticFunction * > const * const sFunP 
  ) {
    return muParseString( 
      replaceUsedFunctions(
        getAttributeStr(attName, element), 
        cValP, 
        sFunP
      )
    );
  }

  int qtXmlBase::getAttributeIntMuParse(
    std::string const attName, 
    ::QDomElement const element, 
    vectorHandling< constValue * > const * const cValP,
    vectorHandling< analyticFunction * > const * const sFunP 
  ) {
    return muParseStringInt( 
      replaceUsedFunctions(
        getAttributeStr(attName, element), 
        cValP, 
        sFunP
      )
    );
  }  
  
	std::vector< float > qtXmlBase::getAttributeFloatVectorMuParse( 
		std::string const attName, 
		::QDomElement const element, 
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF 
	) {
		std::string att = getAttributeStr(attName, element);
		std::vector< std::string > attVec = convertToStringVector("{", "}", att);
		std::vector< float > floatVec(attVec.size(), 0.);
		int counter = 0;
		for ( auto &el : attVec ) {		
			floatVec[counter] = muParseString( replaceUsedFunctions(el, cV, aF) );
			counter++;
		}		
		return floatVec;
	}

	std::vector< int > qtXmlBase::getAttributeIntVectorMuParse( 
		std::string const attName, 
		::QDomElement const element, 
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF 
	) {
		std::string att = getAttributeStr(attName, element);
		std::vector< std::string > attVec = convertToStringVector("{", "}", att);
		std::vector< int > intVec(attVec.size(), 0.);
		int counter = 0;
		for ( auto &el : attVec ) {		
			intVec[counter] = muParseStringInt( replaceUsedFunctions(el, cV, aF) );
			counter++;
		}		
		return intVec;
	}
}