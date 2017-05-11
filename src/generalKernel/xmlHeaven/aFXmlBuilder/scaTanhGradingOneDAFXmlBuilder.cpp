#include "scaTanhGradingOneDAFXmlBuilder.h"

#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticFunctionHeaven/scaTanhGradingOneD.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/logMe.h>

#include <QtXml/QDomElement>

namespace dtOO {
  scaTanhGradingOneDAFXmlBuilder::scaTanhGradingOneDAFXmlBuilder() {
  }

  scaTanhGradingOneDAFXmlBuilder::~scaTanhGradingOneDAFXmlBuilder() {
  }

  void scaTanhGradingOneDAFXmlBuilder::buildPart(
	  ::QDomElement const & toBuild, 
		baseContainer * const bC,
		cVPtrVec const * const cV, 
		aFPtrVec const * const aF,
		aFPtrVec * result
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
		cVPtrVec const * const cV, 
		aFPtrVec const * const aF,
		aFPtrVec * result
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