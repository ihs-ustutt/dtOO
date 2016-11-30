#include "constValueAssingRule.h"
#include "xmlHeaven/dtXmlParser.h"

#include <logMe/logMe.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <dtCase.h>

namespace dtOO {  
  constValueAssingRule::constValueAssingRule() { 
  }

  constValueAssingRule::~constValueAssingRule() {
  }

	void constValueAssingRule::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		cVPtrVec const * const cV,
		aFPtrVec const * const aF,
		aGPtrVec const * const aG,
		bVPtrVec const * const bV,
    dCPtrVec const * const dC,
		dPPtrVec const * const pL
	) {
	  dtPlugin::init(element, bC, cV, aF, aG, bV, dC, pL);

//	<plugin 
//		name="constValueAssingRule" 
//		label="constValueAssingRule"
//		assignRule="
//      {
//				:
//		    @rM2dTo3d_channel[-1](@aS_meanplane_rot[%](0.00, 0.00)@)@
//		    :
//		    :
//		    cV_gridChannel_le_u_w0.00
//		    :
//		    :
//		    cV_gridChannel_le_v_w0.00
//		    :
//		  }
//		"
//	>
//		<option name="pickDimension" value="2"/>
//		<option name="percent" value="true"/>
//	</plugin>    
        
    _assignRule 
    = 
    dtXmlParser::getAttributeStrVector("assignRule", element);
  
    _bC = bC;
    _cV = cV;
    _aF = aF;
    _aG = aG;
	}
		
  void constValueAssingRule::apply(void) {
    std::vector< constValue * > cV;
    std::vector< float > cVVal;    
    
    dt__forAllConstIter(std::vector< std::string >, _assignRule, it) {
      std::string theString = *it;

      std::vector< float > val 
      = 
      dtXmlParser::muParseCSString(
        dtXmlParser::replaceDependencies(
          stringPrimitive::getStringBetweenAndRemove(
            ":", ":", &theString
          ),
          _bC,
          _cV, 
          _aF, 
          _aG
        )
      ); 

      std::vector< std::string > cVLabel
      =
      dtXmlParser::convertToStringVector(":", ":", theString);

      dt__throwIf(cVLabel.size()>val.size(), apply());

      dt__forAllIndex(cVLabel, ii) {
        if ( cVLabel[ii] == "" ) continue;
        
        cV.push_back( _cV->get(cVLabel[ii]) );
        cVVal.push_back( val[ii] );
      }
    }
    
    dt__info(
      init(), 
      << "cV = " << logMe::stringPtrVec(cV, &constValue::getLabel, 1 ) 
      << std::endl
      << "cVVal = " << logMe::vecToString(cVVal, 1)
    );
    
    dt__forAllIndex(cV, ii) cV[ii]->setValue( cVVal[ii] );
  }
}


