#include "qtXmlBase.h"

#include <geometryEngine/dtMuParser.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <QtXml/QDomNode>
#include <logMe/logMe.h>
#include <analyticFunctionHeaven/analyticFunction.h>

#include <analyticFunctionHeaven/scaOneD.h>
#include <analyticFunctionHeaven/vec2dFunction.h>
#include <analyticFunctionHeaven/vec3dFunction.h>

namespace dtOO {
  std::string qtXmlBase::_CALCSIGN = "`";
  std::string qtXmlBase::_POINTSIGN = "!";
  std::string qtXmlBase::_DTTSIGN = "~";
  std::string qtXmlBase::_CVSIGN = "#";
  std::string qtXmlBase::_AFSIGN = "$";
  std::string qtXmlBase::_AGSIGN = "@";
  
  
  qtXmlBase::qtXmlBase() {
  }

  qtXmlBase::~qtXmlBase() {
  }

  std::string qtXmlBase::replaceDependencies( 
	  std::string const expression, 
		cVPtrVec const * const cV
	) {
    std::string returnExpression;
    returnExpression = expression;
    unsigned int found;
    //
    // check if there is a constValue in expression
    // 1. * #cVLabel#
    //
    found = returnExpression.find(_CVSIGN);
    while ( found < returnExpression.size() ) {
      //
      // find start and end of function
      //
      unsigned int foundEnd = returnExpression.find(_CVSIGN, found+1);
      int replaceStart = found;
      int replaceEnd = foundEnd-found+1;
      
      //
      // replace constValue string by value
      //
      returnExpression.replace(
        replaceStart, 
        replaceEnd, 
        floatToString(
          cV->get(
            returnExpression.substr(replaceStart+1, replaceEnd-2)
          )->getValue()
        )
      );

      //
      // go to next constValue
      //
      found = returnExpression.find(_CVSIGN);
    }
    //
    // check if there is an instruction to calculate in expression
    // `#cVLabel# + #cVLabel#`
    //
    found = returnExpression.find(_CALCSIGN);
    while ( found < returnExpression.size() ) {
      //
      // find start and end of function
      //
      unsigned int foundEnd = returnExpression.find(_CALCSIGN, found+1);
      int replaceStart = found;
      int replaceEnd = foundEnd-found+1;
      
      //
      // muparse string
      //
      returnExpression.replace(
        replaceStart, 
        replaceEnd, 
        floatToString(
          muParseString(        
            returnExpression.substr(replaceStart+1, replaceEnd-2)
          )
        )
      );

      //
      // go to next constValue
      //
      found = returnExpression.find(_CALCSIGN);
    }    
    return returnExpression;
  }
  
  std::string qtXmlBase::replaceDependencies( 
	  std::string const expression, 
		cVPtrVec const * const cV,
		aFPtrVec const * const aF
	) {
    dt__debug( replaceDependencies(), << "expression = " << expression );    
    
    std::string returnExpression = expression;
          
    //
    // crumble string down, respect brackets
    //
    std::vector< std::string > crumbles = crumbleDown("(", ")", expression);
    
    dt__debug( replaceDependencies(), << "crumbles = " << crumbles );
    dt__forAllRefAuto(crumbles, aCrumble) {
      if ( !stringContains(_AFSIGN, aCrumble) ) continue;
      returnExpression 
      = 
      replaceStringInString(
        aCrumble, 
        replaceDependencies(aCrumble, cV, aF),
        returnExpression
      );
    }
    
    //
    // check if there is a function in expression
    // $functionName(value * #constValue#)$
    //
    unsigned int found = returnExpression.find(_AFSIGN);
    while ( found < returnExpression.size() ) {
      //
      // find start and end of function
      //
      //unsigned int foundEnd = returnExpression.find_last_of(_AFSIGN);
      unsigned int foundEnd = returnExpression.find_first_of(_AFSIGN, found+1);
      int replaceStart = found;
      int replaceEnd = foundEnd-found+1;
      std::string replaceString 
      = 
      returnExpression.substr(replaceStart+1, replaceEnd-2);
     
      //
      // replace in argument
      //
      std::string arg 
      = 
      replaceDependencies(
        getStringBetweenFirstLast("(", ")", replaceString), cV, aF
      );

      //
      // get and cast analyticFunction
      //
      std::string aFLabel = getStringBetween("", "(", replaceString);
      std::string aFOption = "";
      if ( stringPrimitive::stringContains("[", aFLabel) ) {
        aFOption 
        = 
        stringPrimitive::getStringBetweenAndRemove("[", "]", &aFLabel);
      }
      analyticFunction const * const theAF = aF->get(aFLabel); 
      
      scaFunction const * const sF = scaFunction::ConstDownCast(theAF);
      vec2dFunction const * const v2dF = vec2dFunction::ConstDownCast(theAF);
      vec3dFunction const * const v3dF = vec3dFunction::ConstDownCast(theAF);
      
      std::vector< float > pp; 
      std::vector< float > argCS 
      = 
      muParseCSString( replaceDependencies(arg, cV, aF) );

        
      if (sF) {
        if (aFOption == "") {
          pp = sF->Y( argCS ).stdVector();
        }        
        else dt__throwUnexpected(replaceDependencies());        
      }
      else if (v2dF) {
        if (aFOption == "") {
          pp = v2dF->Y( argCS ).stdVector();
        }        
        else dt__throwUnexpected(replaceDependencies()); 
      }
      else if (v3dF) {
        if (aFOption == "") {
          pp = v3dF->Y( argCS ).stdVector();
        }        
        else dt__throwUnexpected(replaceDependencies()); 
      }      
      else dt__throwUnexpected(replaceDependencies());
      
      if (pp.size() == 3) {
        returnExpression.replace(
          replaceStart, 
          replaceEnd, 
          stringPrimitive::floatToString(pp[0])
          +
          ","
          +
          stringPrimitive::floatToString(pp[1])
          +
          ","
          +
          stringPrimitive::floatToString(pp[2])
        );
      }
      else if (pp.size() == 2) {
        returnExpression.replace(
          replaceStart, 
          replaceEnd, 
          stringPrimitive::floatToString(pp[0])
          +
          ","
          +
          stringPrimitive::floatToString(pp[1])
        );
      }        
      else if (pp.size() == 1) {
        returnExpression.replace(
          replaceStart, 
          replaceEnd, 
          stringPrimitive::floatToString(pp[0])
        );
      }            
      else dt__throwUnexpected(replaceDependencies());
        
      //
      // go to next analyticGeometry
      //
      found = returnExpression.find(_AFSIGN);  
    }

    return replaceDependencies(returnExpression, cV);
  }

  float qtXmlBase::muParseString( std::string const expression ) {
    dtMuParser parser;
    try {
      parser.SetExpr( expression );
      return ( (float) parser.Eval() );
    }
    catch (mu::Parser::exception_type &e) {
      dt__throw( 
        muParseString(), 
        << dt__eval(expression) << std::endl
        << "muParser returns: " << dt__eval(e.GetMsg())
      );
    }
  }

  std::vector< float > qtXmlBase::muParseCSString( 
    std::string const expression 
  ) {
    dtMuParser parser;
    try {
      parser.SetExpr( expression );
      
      int nDim;
      double * yyD = parser.Eval(nDim);
      std::vector< float > yy(nDim, 0.);
			dt__forFromToIndex(0, nDim, ii) yy[ii] = static_cast<float>(yyD[ii]);
      
      return yy;
    }
    catch (mu::Parser::exception_type &e) {
      dt__throw( 
        muParseCSString(), << "muParser returns: " << dt__eval(e.GetMsg())
      );
    }
  }  

  int qtXmlBase::muParseStringInt( std::string const expression ) {
    int ret = static_cast< int >(muParseString(expression));
    return ret;
  }  

  std::vector< int > qtXmlBase::muParseCSStringInt( 
    std::string const expression 
  ) {
    std::vector< float > vec = muParseCSString(expression);
    std::vector< int > retVec(vec.size());
    dt__forAllIndex(vec, ii) retVec[ii] = static_cast< int >(vec[ii]);

    return retVec;
  }  
  
  float qtXmlBase::getAttributeFloatMuParse( 
    std::string const attName, 
    ::QDomElement const element, 
    cVPtrVec const * const cV
  ) {
    return muParseString( 
      replaceDependencies( getAttributeStr(attName, element), cV )
    );
  }
  
  float qtXmlBase::getAttributeFloatMuParse( 
    std::string const attName, 
    ::QDomElement const element, 
    cVPtrVec const * const cV,
    aFPtrVec const * const aF
  ) {
    return muParseString( 
      replaceDependencies( getAttributeStr(attName, element), cV, aF )
    );
  }

  int qtXmlBase::getAttributeIntMuParse(
    std::string const attName, 
    ::QDomElement const element, 
    cVPtrVec const * const cV
  ) {
    return muParseStringInt( 
      replaceDependencies( getAttributeStr(attName, element), cV )
    );
  }
  
  int qtXmlBase::getAttributeIntMuParse(
    std::string const attName, 
    ::QDomElement const element, 
    cVPtrVec const * const cV,
    aFPtrVec const * const aF 
  ) {
    return muParseStringInt( 
      replaceDependencies( getAttributeStr(attName, element), cV, aF )
    );
  }
  
	std::vector< float > qtXmlBase::getAttributeFloatVectorMuParse( 
		std::string const attName, 
		::QDomElement const element, 
		cVPtrVec const * const cV,
		aFPtrVec const * const aF 
	) {
		std::string att = getAttributeStr(attName, element);
		std::vector< std::string > attVec = convertToStringVector("{", "}", att);
		std::vector< float > floatVec(attVec.size(), 0.);
		int counter = 0;
		for ( auto &el : attVec ) {		
			floatVec[counter] = muParseString( replaceDependencies(el, cV, aF) );
			counter++;
		}		
		return floatVec;
	}

	std::vector< double > qtXmlBase::getAttributeDoubleVectorMuParse( 
		std::string const attName, 
		::QDomElement const element, 
		cVPtrVec const * const cV,
		aFPtrVec const * const aF 
	) {
		std::vector< float > floatVec
    =
    getAttributeFloatVectorMuParse(attName, element, cV, aF);
    
    return std::vector< double >( floatVec.begin(), floatVec.end() );
	}  

	std::vector< int > qtXmlBase::getAttributeIntVectorMuParse( 
		std::string const attName, 
		::QDomElement const element, 
		cVPtrVec const * const cV,
		aFPtrVec const * const aF 
	) {
		std::string att = getAttributeStr(attName, element);
		std::vector< std::string > attVec = convertToStringVector("{", "}", att);
		std::vector< int > intVec(attVec.size(), 0.);
		int counter = 0;
		for ( auto &el : attVec ) {		
			intVec[counter] = muParseStringInt( replaceDependencies(el, cV, aF) );
			counter++;
		}		
		return intVec;
	}
}