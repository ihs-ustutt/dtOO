#include "bSplineSurface_skinConstructOCCAFXmlBuilder.h"

#include <xmlHeaven/dtXmlParserBase.h>
#include "geometryEngine/geoBuilder/bSplineSurface_skinConstructOCC.h"
#include <analyticFunctionHeaven/analyticFunction.h>
#include <geometryEngine/geoBuilder/bSplineSurface_skinConstructOCC.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>

#include <boost/assign.hpp>
#include <QtXml/QDomElement>

namespace dtOO {
  bSplineSurface_skinConstructOCCAFXmlBuilder
    ::bSplineSurface_skinConstructOCCAFXmlBuilder() {
  }

  bSplineSurface_skinConstructOCCAFXmlBuilder
    ::~bSplineSurface_skinConstructOCCAFXmlBuilder() {

  }

  std::vector< std::string > bSplineSurface_skinConstructOCCAFXmlBuilder
    ::factoryAlias( void ) const {
    return ::boost::assign::list_of
      ("vec3dSurfaceTwoD")
      ("vec3dSurfaceTwoDAFXmlBuilder");
  }
  
  void bSplineSurface_skinConstructOCCAFXmlBuilder::buildPart(
		::QDomElement const & toBuildP, 
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cValP, 
		vectorHandling< analyticFunction * > const * const depSFunP,
		vectorHandling< analyticFunction * > * sFunP 
	) const {
    dt__throwIf(
      !dtXmlParserBase::hasChild("analyticFunction", toBuildP), 
      buildPart()
    );

    ::QDomElement elementP 
    = 
    dtXmlParserBase::getChild("analyticFunction", toBuildP);
    //
    // set input
    //
    vectorHandling< dtCurve const * > cL;
    while ( !elementP.isNull() ) {
      dt__vH(analyticFunction *) aF;
      dtXmlParserBase::createAdvanced(&elementP, bC, cValP, depSFunP, &aF);

      dt__forAllIndex(aF, ii) {
        dt__ptrAss(
          vec3dCurveOneD const * v3d1d, 
          vec3dCurveOneD::ConstDownCast(aF[ii])
        );
        cL.push_back( v3d1d->ptrDtCurve() );
      }

      elementP = dtXmlParserBase::getNextSibling("analyticFunction", elementP);      

    }
    ptrHandling<dtSurface> dtS( 
      bSplineSurface_skinConstructOCC(cL).result() 
    );

    //
    // create vec3dSurfaceTwoD
    //
    sFunP->push_back( new vec3dSurfaceTwoD( dtS.get() ) );			
  }
}
