#include "geomVolume_multipleBoundedVolumeAGXmlBuilder.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/multipleBoundedVolume.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  geomVolume_multipleBoundedVolumeAGXmlBuilder
    ::geomVolume_multipleBoundedVolumeAGXmlBuilder() {
  }

  geomVolume_multipleBoundedVolumeAGXmlBuilder
    ::~geomVolume_multipleBoundedVolumeAGXmlBuilder() {
  }

  void geomVolume_multipleBoundedVolumeAGXmlBuilder::buildPart(
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    cVPtrVec const * const cV,           
    aFPtrVec const * const aF,    
    aGPtrVec const * const aG,
    aGPtrVec * result 
	) const {  
    dt__throwIf(
      !dtXmlParserBase::hasChild("analyticGeometry", toBuild), 
      buildPart()
    );

    dt__pH(analyticGeometry) m3d;
    dt__pVH(analyticGeometry) m2d;
    dt__forAllRefAuto(
      dtXmlParserBase::getChildVector("analyticGeometry", toBuild), anEl
    ) {
      dt__pH(analyticGeometry) aG_t(
        dtXmlParserBase::createAnalyticGeometry(anEl, bC, cV, aF, aG)
      );
      if ( aG_t->dim() == 3 ) {
        m3d.reset( aG_t->clone() );
      }
      else if ( aG_t->dim() == 2 ) {
        m2d.push_back( aG_t->clone() );
      }
      else dt__throwUnexpected(buildPart());
    }

    result->push_back( new multipleBoundedVolume( m3d.get(), m2d ) );
  }
}