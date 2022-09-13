#include "scaTanhGradingOneDAFXmlBuilder.h"

#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticFunctionHeaven/scaTanhGradingOneD.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/logMe.h>

#include <QtXml/QDomElement>

namespace dtOO {
  bool scaTanhGradingOneDAFXmlBuilder::_registrated 
  =
  aFXmlBuilderFactory::registrate(
    dt__tmpPtr(
      scaTanhGradingOneDAFXmlBuilder, new scaTanhGradingOneDAFXmlBuilder()
    )
  );
  
  scaTanhGradingOneDAFXmlBuilder::scaTanhGradingOneDAFXmlBuilder() {
  }

  scaTanhGradingOneDAFXmlBuilder::~scaTanhGradingOneDAFXmlBuilder() {
  }

  void scaTanhGradingOneDAFXmlBuilder::buildPart(
	  ::QDomElement const & toBuild, 
		baseContainer * const bC,
		lvH_constValue const * const cV, 
		lvH_analyticFunction const * const aF,
		lvH_analyticFunction * result
	) const {
    result->push_back( 
      new scaTanhGradingOneD(
        dtXmlParserBase::getAttributeFloatVectorMuParse("c", toBuild, cV, aF ),
        dtXmlParserBase::getAttributeFloatMuParse("g", toBuild, cV, aF ),
        dtXmlParserBase::getAttributeFloatMuParse("gMin", toBuild, cV, aF ),
        dtXmlParserBase::getAttributeFloatMuParse("gMax", toBuild, cV, aF )
      ) 
    );
  }

  void scaTanhGradingOneDAFXmlBuilder::buildPartCompound(
	  ::QDomElement const & toBuild, 
		baseContainer * const bC,
		lvH_constValue const * const cV, 
		lvH_analyticFunction const * const aF,
		lvH_analyticFunction * result
	) const {
    result->push_back( 
      new scaTanhGradingOneDCompound( 
        scaTanhGradingOneD(
        dtXmlParserBase::getAttributeFloatVectorMuParse("c", toBuild, cV, aF ),
        dtXmlParserBase::getAttributeFloatMuParse("g", toBuild, cV, aF ),
        dtXmlParserBase::getAttributeFloatMuParse("gMin", toBuild, cV, aF ),
        dtXmlParserBase::getAttributeFloatMuParse("gMax", toBuild, cV, aF )
        ) 
      ) 
    );
  }  
}