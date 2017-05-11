#include "scaSingleTanhGradingOneDAFXmlBuilder.h"

#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticFunctionHeaven/scaSingleTanhGradingOneD.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/logMe.h>

#include <QtXml/QDomElement>

namespace dtOO {
  scaSingleTanhGradingOneDAFXmlBuilder::scaSingleTanhGradingOneDAFXmlBuilder() {
  }

  scaSingleTanhGradingOneDAFXmlBuilder::~scaSingleTanhGradingOneDAFXmlBuilder() {
  }

  void scaSingleTanhGradingOneDAFXmlBuilder::buildPart(
	  ::QDomElement const & toBuild, 
		baseContainer * const bC,
		cVPtrVec const * const cV, 
		aFPtrVec const * const aF,
		aFPtrVec * result
	) const {
    result->push_back( 
      new scaSingleTanhGradingOneD(
        dtXmlParserBase::getAttributeFloatMuParse("g", toBuild, cV, aF ),
        dtXmlParserBase::getAttributeFloatMuParse("gMin", toBuild, cV, aF ),
        dtXmlParserBase::getAttributeFloatMuParse("gMax", toBuild, cV, aF )
      ) 
    );
  }

  void scaSingleTanhGradingOneDAFXmlBuilder::buildPartCompound(
	  ::QDomElement const & toBuild, 
		baseContainer * const bC,
		cVPtrVec const * const cV, 
		aFPtrVec const * const aF,
		aFPtrVec * result
	) const {
    result->push_back( 
      new scaSingleTanhGradingOneDCompound( 
        scaSingleTanhGradingOneD(
        dtXmlParserBase::getAttributeFloatMuParse("g", toBuild, cV, aF ),
        dtXmlParserBase::getAttributeFloatMuParse("gMin", toBuild, cV, aF ),
        dtXmlParserBase::getAttributeFloatMuParse("gMax", toBuild, cV, aF )
        ) 
      ) 
    );
  }  
}