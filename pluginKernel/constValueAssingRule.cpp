#include "constValueAssingRule.h"
#include "xmlHeaven/dtXmlParser.h"

#include <logMe/logMe.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>

namespace dtOO {  
  constValueAssingRule::constValueAssingRule() { 
  }

  constValueAssingRule::~constValueAssingRule() {
  }

	void constValueAssingRule::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG,
		vectorHandling< boundedVolume * > const * const bV,
		vectorHandling< dtPlugin * > const * const pL
	) {
	  dtPlugin::init(element, bC, cV, aF, aG, bV, pL);

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
        
    std::vector< std::string > assignRule 
    = 
    dtXmlParser::getAttributeStrVector("assignRule", element);
    
    dt__forAllConstIter(std::vector< std::string >, assignRule, it) {
      std::string theString = *it;

      std::vector< float > val 
      = 
      dtXmlParser::muParseCSString(
        dtXmlParser::replaceDependencies(
          stringPrimitive::getStringBetweenAndRemove(
            ":", ":", &theString
          ), 
          cV, 
          aF, 
          aG
        )
      ); 

      std::vector< std::string > cVLabel
      =
      dtXmlParser::convertToStringVector(":", ":", theString);

      dt__throwIf(cVLabel.size()>val.size(), init());

      dt__forAllIndex(cVLabel, ii) {
        _cV.push_back( cV->get(cVLabel[ii]) );
        _val.push_back( val[ii] );
      }
    }
    
    dt__info(
      init(), 
      << "_cV = " << logMe::stringPtrVec(_cV, &constValue::getLabel, 1 ) 
      << std::endl
      << "_val = " << logMe::vecToString(_val, 1)
    );
    
	}
		
  void constValueAssingRule::apply(void) {  
    dt__forAllIndex(_cV, ii) _cV[ii]->setValue( _val[ii] );
  }
}


