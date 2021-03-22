#include "geomSurface_multipleBoundedSurfaceAGXmlBuilder.h"

#include <xmlHeaven/aGXmlBuilderFactory.h>
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/multipleBoundedSurface.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bool geomSurface_multipleBoundedSurfaceAGXmlBuilder::_registrated 
  =
  aGXmlBuilderFactory::registrate(
    dt__tmpPtr(
      geomSurface_multipleBoundedSurfaceAGXmlBuilder, 
      new geomSurface_multipleBoundedSurfaceAGXmlBuilder()
    )
  );
  
  geomSurface_multipleBoundedSurfaceAGXmlBuilder
    ::geomSurface_multipleBoundedSurfaceAGXmlBuilder() {
  }

  geomSurface_multipleBoundedSurfaceAGXmlBuilder
    ::~geomSurface_multipleBoundedSurfaceAGXmlBuilder() {
  }

  void geomSurface_multipleBoundedSurfaceAGXmlBuilder::buildPart(
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

    dt__pH(analyticGeometry) m2d;
    dt__pVH(analyticGeometry) m1d;
    dt__forAllRefAuto(
      dtXmlParserBase::getChildVector("analyticGeometry", toBuild), anEl
    ) {
      dt__pH(analyticGeometry) aG_t(
        dtXmlParserBase::createAnalyticGeometry(anEl, bC, cV, aF, aG)
      );
      if ( aG_t->dim() == 2 ) {
        m2d.reset( aG_t->clone() );
      }
      else if ( aG_t->dim() == 1 ) {
        m1d.push_back( aG_t->clone() );
      }
      else dt__throwUnexpected(buildPart());
    }

    result->push_back( new multipleBoundedSurface( m2d.get(), m1d ) );
  }
}