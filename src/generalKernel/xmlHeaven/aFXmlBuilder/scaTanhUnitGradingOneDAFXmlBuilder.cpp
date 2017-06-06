#include "scaTanhUnitGradingOneDAFXmlBuilder.h"

#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticFunctionHeaven/scaTanhUnitGradingOneD.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/logMe.h>

#include <QtXml/QDomElement>

namespace dtOO {
  scaTanhUnitGradingOneDAFXmlBuilder::scaTanhUnitGradingOneDAFXmlBuilder() {
  }

  scaTanhUnitGradingOneDAFXmlBuilder::~scaTanhUnitGradingOneDAFXmlBuilder() {
  }

  void scaTanhUnitGradingOneDAFXmlBuilder::buildPart(
	  ::QDomElement const & toBuild, 
		baseContainer * const bC,
		cVPtrVec const * const cV, 
		aFPtrVec const * const aF,
		aFPtrVec * result
	) const {
    result->push_back(
      new scaTanhUnitGradingOneD(
        dtXmlParserBase::getAttributeFloatVectorMuParse("c", toBuild, cV, aF ),
        dtXmlParserBase::getAttributeFloatMuParse("g", toBuild, cV, aF ),
        dtXmlParserBase::getAttributeFloatMuParse("gMin", toBuild, cV, aF ),
        dtXmlParserBase::getAttributeFloatMuParse("gMax", toBuild, cV, aF )
      ) 
    );
  }

  void scaTanhUnitGradingOneDAFXmlBuilder::buildPartCompound(
	  ::QDomElement const & toBuild, 
		baseContainer * const bC,
		cVPtrVec const * const cV, 
		aFPtrVec const * const aF,
		aFPtrVec * result
	) const {
    result->push_back( 
      new scaTanhUnitGradingOneDCompound( 
        scaTanhUnitGradingOneD(
        dtXmlParserBase::getAttributeFloatVectorMuParse("c", toBuild, cV, aF ),
        dtXmlParserBase::getAttributeFloatMuParse("g", toBuild, cV, aF ),
        dtXmlParserBase::getAttributeFloatMuParse("gMin", toBuild, cV, aF ),
        dtXmlParserBase::getAttributeFloatMuParse("gMax", toBuild, cV, aF )
        ) 
      ) 
    );
  }  
}