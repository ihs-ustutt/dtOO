#include "qtXmlBase.h"
#include "analyticFunctionHeaven/scaOneD.h"

#include <muParser.h>
//using namespace mu;
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
    //check if there is a function in expression
    //
    found = returnExpression.find("$");
    while ( found < returnExpression.size() ) {
      //
      //find start and end of function
      //
      unsigned int foundEnd = returnExpression.find("$", found+1);
      int replaceStart = found;
      int replaceEnd = foundEnd-found+1;
      std::string funString = returnExpression.substr(replaceStart+1, replaceEnd-2);

      //
      // check for options
      //
      std::string option = getStringBetweenAndRemove("[", "]", &funString);
      if (option != "") {
        dt__debug(replaceUsedFunctions(),
                << "Option found" << std::endl
                << dt__eval(option) );
      }

      
      //
      //find name and argument value of function
      //
      std::string funName = getStringBetween("", "(", funString);//funString.substr( 0, funString.find("(") );

      std::string funArgumentValue = replaceUsedFunctions(
                                       getStringBetween("(", ")", funString),
                                       cValP,
                                       sFunP
                                     );      
//      bool getInv = false;
//      if (getInv) {
//        funName = funName.substr(0, getInv-1);
//      }
      
      //
      //evaluate function
      //
      float insertMeFloat;
      dt__ptrAss( 
			  scaOneD const * const sF, 
				scaOneD::ConstDownCast( sFunP->get(funName) ) 
			);        
      if (option == "") {
        insertMeFloat = sF->YFloat( stringToFloat(funArgumentValue) );
      }
      else {
        dt__throw(replaceUsedFunctions(),
                << "Function name " << dt__eval(funName) << " has no "
								<< dt__eval(option) );				
      }      
      //
      //replace function string with value
      //
      returnExpression.replace(replaceStart, replaceEnd, floatToString(insertMeFloat));
      
      //
      //go to next parser function
      //
      found = returnExpression.find("$");//, foundEnd+1);
    }
    //
    //check if there is a constValue in expression
    //
    found = returnExpression.find("#");
    while ( found < returnExpression.size() ) {
      //
      //find start and end of function
      //
      unsigned int foundEnd = returnExpression.find("#", found+1);
      int replaceStart = found;
      int replaceEnd = foundEnd-found+1;
      std::string funString = returnExpression.substr(replaceStart+1, replaceEnd-2);
      //
      //find name of scaValue
      //
      std::string funName = funString;
      //
      //evaluate function
      //
      float insertMeFloat;
      //convert to float
      //argumentFloat;
      bool isFound = false;
      for (int ii=0; ii<cValP->size(); ii++) {
        if ( funName == ((*cValP)[ii])->getLabel() ) {
          insertMeFloat = ((*cValP)[ii])->getValue();
          isFound = true;
          break;
        }
      }
      //
      //replace function string with value
      //
      if (isFound) {
        returnExpression.replace(replaceStart, replaceEnd, floatToString(insertMeFloat));
      }
      else {
        dt__throw(replaceUsedFunctions(),
                << "ConstValue name " << dt__eval(funName) << " not found.");
        returnExpression.replace(replaceStart, replaceEnd, floatToString(0.));
      }
      //
      //go to next parser function
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
      dt__throw( muParseString(),
              << e.GetMsg() << std::endl
              << "Error in muParser. Return 0");
    }
  }

  int qtXmlBase::muParseStringInt( std::string const expression ) {
    int ret = static_cast< int >(muParseString(expression));
    return ret;
  }  

  float qtXmlBase::getAttributeFloatMuParse( 
    std::string const attName, 
    QDomElement const element, 
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
    QDomElement const element, 
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
		QDomElement const element, 
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
		QDomElement const element, 
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
	
	/**
	 * 
   * @todo This function should be removed. Please use createDtVector3.
   */
  dtVector3 qtXmlBase::getDtVector3(
    QDomElement const element,
    vectorHandling< constValue * > const * const cValP,
    vectorHandling< analyticFunction * > const * const sFunP 
  ) {
    //
    // check tagName
    //
    if ( !is("dtVector3", element) && !is("Vector_3", element) ) {
      dt__throw(getDtVector3(), 
        << dt__eval( getTagName(element) ) << std::endl
        << "Not a dtVector3 element.");
    }
    
    //
    // create vector from u-v-w-coordinates
    //
    if (    hasAttribute("u", element) 
         && hasAttribute("v", element) 
         && hasAttribute("w", element) 
       ) {
      return dtVector3(
        getAttributeFloatMuParse("u", element, cValP, sFunP), 
        getAttributeFloatMuParse("v", element, cValP, sFunP),
        getAttributeFloatMuParse("w", element, cValP, sFunP) 
      );
    }
    //
    // create vector from x-y-z-coordinates
    //
    if (    hasAttribute("x", element) 
         && hasAttribute("y", element) 
         && hasAttribute("z", element) 
       ) {
      return dtVector3(
        getAttributeFloatMuParse("x", element, cValP, sFunP), 
        getAttributeFloatMuParse("y", element, cValP, sFunP),
        getAttributeFloatMuParse("z", element, cValP, sFunP) 
      );
    }    
  }
}