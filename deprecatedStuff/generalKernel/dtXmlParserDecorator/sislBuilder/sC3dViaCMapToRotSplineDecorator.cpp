#include "sC3dViaCMapToRotSplineDecorator.h"

#include <dtLinearAlgebra.h>
#include <progHelper.h>
#include <logMe/logMe.h>
#include <dtTransformerHeaven/conformalMappingStartFromPoint.h>
#include <dtXmlParserDecorator/dtXmlParserDecoratorInterface.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <analyticFunctionHeaven/scaCurve2dOneD.h>
#include <analyticGeometryHeaven/rotatingSpline.h>
#include <dtTransformerHeaven/conformalMappingStartFromPoint.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>
#include <QtXml/QDomElement>

namespace dtOO { 
  sC3dViaCMapToRotSplineDecorator::sC3dViaCMapToRotSplineDecorator() {
  }

  sC3dViaCMapToRotSplineDecorator::~sC3dViaCMapToRotSplineDecorator() {
  }
  
  void sC3dViaCMapToRotSplineDecorator::buildPart(QDomElement ** toBuildP,
    pointContainer * const pointContainerP,
    vectorContainer * const vectorContainerP,    
    vectorHandling< constValue * > const * const cValP,  
    vectorHandling< analyticFunction * > const * const sFunP,
    vectorHandling< analyticGeometry * > const * const depAGeoP,
    vectorHandling< analyticGeometry * > * aGeoP ) const {
    
    bool hasAGeo = hasChild("Point_2", **toBuildP);
    bool hasPartLabel = hasAttribute("part_label", **toBuildP);
    bool hasOrder = hasAttribute("order", **toBuildP);
    
    if ( hasAGeo && hasOrder && hasPartLabel ) {
      //
      // get rotatingSpline
      //
      analyticGeometry * tmpAG 
      = 
      depAGeoP->get(
        getAttributeStr(
          "part_label", 
          **toBuildP
        )
      )->clone();
      
      ptrHandling< analyticGeometry > aG(tmpAG);
      rotatingSpline * rS;
      dt__mustCast(aG.get(), rotatingSpline, rS);

      
      //
      // prepare conformal mapping
      //
      conformalMappingStartFromPoint cMap;
      if ( hasAttribute("tolerance", **toBuildP) ) {
        cMap.handleFloat(
          "tolerance", 
          getAttributeFloatMuParse("tolerance", **toBuildP, cValP,  sFunP)
        );
      }
      
      float offsetPhi = 0.;
      float offsetZ = 0.;
      if ( hasAttribute("offset_z", **toBuildP) ) {
          offsetZ = getAttributeFloatMuParse("offset_z", **toBuildP, cValP,  sFunP);        
      }
      if ( hasAttribute("offset_phi", **toBuildP) ) {
          offsetPhi = getAttributeFloatMuParse("offset_phi", **toBuildP, cValP, sFunP);
      }
      dtPoint2 basePointUV( rS->u_phi(offsetPhi), rS->v_z(offsetZ) );
      dtPoint2 basePointPhiM( offsetPhi, rS->m_v( rS->v_z(offsetZ) ) );
      cMap.handlePoint2d("base_point", basePointUV);
      
      cMap.handleAnalyticGeometry("part_label", rS);
      
      

      //
      // analyticGeometries
      //
      QDomElement wElement = getChild("Point_2", **toBuildP);
      vectorHandling< dtPoint2 * > vH;
      std::vector< dtPoint3 > p3;
      std::vector< dtPoint2 > p2uv;
      std::vector< dtPoint2 > p2phiM;
      while ( !wElement.isNull() ) {
        std::vector< dtPoint2 * > tP;
        std::vector< dtPoint2 * > tPuv;        
        
        this->createBasic( &wElement, 
                           pointContainerP, 
                           vectorContainerP, 
                           cValP, 
                           sFunP,
                           depAGeoP,
                           &tP );
        tPuv = cMap.apply( &tP );
        for (int ii=0;ii<tPuv.size();ii++) {  
          p3.push_back( rS->getPoint( tPuv[ii]->x(), tPuv[ii]->y() ) );
        }
        vH.destroy(tP);
        vH.destroy(tPuv);        
        wElement = getNextSibling("Point_2", wElement);
      }
      
      //
      // call builder
      //
      int order = getAttributeInt("order", **toBuildP);         
      splineCurve3d * sC3d = new splineCurve3d(&p3, order);
      
      aGeoP->push_back( sC3d );
    }
    else {
      dt__THROW(buildPart(),
              << dt__eval(hasAGeo) << std::endl
              << dt__eval(hasOrder) << std::endl
              << dt__eval(hasPartLabel) );      
    }
  }
}
