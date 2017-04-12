#include "compositeSurface_geomSurfaceConnectConstructOCCAGXmlBuilder.h"
#include "geometryEngine/geoBuilder/compositeSurface_surfaceConnectConstructOCC.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <geometryEngine/dtCurve.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/geoBuilder/bSplineSurface_skinConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve_curveConnectConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  compositeSurface_geomSurfaceConnectConstructOCCAGXmlBuilder
    ::compositeSurface_geomSurfaceConnectConstructOCCAGXmlBuilder() {
  }

  compositeSurface_geomSurfaceConnectConstructOCCAGXmlBuilder
    ::~compositeSurface_geomSurfaceConnectConstructOCCAGXmlBuilder() {
  }

  void compositeSurface_geomSurfaceConnectConstructOCCAGXmlBuilder::buildPart(
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    cVPtrVec const * const cV,           
    aFPtrVec const * const aF,    
    aGPtrVec const * const aG,
    aGPtrVec * result 
	) const {  
    dt__throwIf(
      !dtXmlParserBase::hasChild("matrix", toBuild), 
      buildPart()
    );

    twoDArrayHandling< ::QDomElement > mat 
    = 
    dtXmlParserBase::getChildMatrix("analyticGeometry", toBuild);
    twoDArrayHandling< dtSurface const * > ssV(mat.size(0), mat.size(1));
    
    dt__forAllIndex( mat, i ) {
      dt__forAllIndex( mat.at(i), j ) {
        dt__pH(analyticGeometry) aG_t(
          dtXmlParserBase::createAnalyticGeometry(mat[i][j], bC, cV, aF, aG)
        );
        ssV[i][j] 
        =  
        analyticSurface::MustConstDownCast(aG_t.get())->ptrConstDtSurface()->clone();
      }
    }

    result->push_back( 
      new analyticSurface(
        dt__tmpPtr(
          dtSurface, 
          compositeSurface_surfaceConnectConstructOCC(ssV).result()
        )
      )
    );
    ssV.clear();
  }
}