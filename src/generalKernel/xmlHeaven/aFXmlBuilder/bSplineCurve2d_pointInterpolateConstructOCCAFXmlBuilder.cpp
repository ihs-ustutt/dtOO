/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "bSplineCurve2d_pointInterpolateConstructOCCAFXmlBuilder.h"

#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointInterpolateConstructOCC.h>
#include <geometryEngine/dtCurve2d.h>
#include <analyticFunctionHeaven/scaCurve2dOneD.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bool bSplineCurve2d_pointInterpolateConstructOCCAFXmlBuilder::_registrated 
  =
  aFXmlBuilderFactory::registrate(
    dt__tmpPtr(
      bSplineCurve2d_pointInterpolateConstructOCCAFXmlBuilder, 
      new bSplineCurve2d_pointInterpolateConstructOCCAFXmlBuilder()
    )
  );
  
  bSplineCurve2d_pointInterpolateConstructOCCAFXmlBuilder
    ::bSplineCurve2d_pointInterpolateConstructOCCAFXmlBuilder() {
  }

  bSplineCurve2d_pointInterpolateConstructOCCAFXmlBuilder
    ::~bSplineCurve2d_pointInterpolateConstructOCCAFXmlBuilder() {

  }

  void bSplineCurve2d_pointInterpolateConstructOCCAFXmlBuilder::buildPart(
		::QDomElement const & toBuildP, 
		baseContainer * const bC,
		lvH_constValue const * const cValP, 
		lvH_analyticFunction const * const depSFunP,
		lvH_analyticFunction * sFunP 
	) const {
    dt__throwIf(!dtXmlParserBase::hasChild("Point_2", toBuildP), buildPart());

    std::vector< dtPoint2 > pointsArray;
    std::vector< ::QDomElement > wElement 
    = 
    dtXmlParserBase::getChildVector("Point_2", toBuildP);

    dt__forAllRefAuto(wElement, anEl) {
      std::vector< dtPoint2 > wP;
      dtXmlParserBase::dtXmlParserBase::createBasic( 
        &anEl, bC, cValP, depSFunP, &wP 
      );
      for (int ii=0; ii<wP.size();ii++) pointsArray.push_back( wP[ii] );
    }

    //
    // create scaCurve2dOneD
    //
    sFunP->push_back( 
      new scaCurve2dOneD( 
        dt__tmpPtr(
          dtCurve2d, 
          bSplineCurve2d_pointInterpolateConstructOCC(pointsArray).result()
        )
      ) 
    );			
  }
}
