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

#include "geomShape_readOCCAGXmlBuilder.h"

#include <xmlHeaven/aGXmlBuilderFactory.h>
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/geomShape_readOCC.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

#include <boost/assign.hpp>

namespace dtOO {
  bool geomShape_readOCCAGXmlBuilder::_registrated 
  =
  aGXmlBuilderFactory::registrate(
    dt__tmpPtr(
      geomShape_readOCCAGXmlBuilder, 
      new geomShape_readOCCAGXmlBuilder()
    )
  );
  
  geomShape_readOCCAGXmlBuilder
    ::geomShape_readOCCAGXmlBuilder() {
  }

  geomShape_readOCCAGXmlBuilder
    ::~geomShape_readOCCAGXmlBuilder() {
  }

  std::vector< std::string > 
    geomShape_readOCCAGXmlBuilder::factoryAlias( 
      void 
  ) const {
    return 
      ::boost::assign::list_of
        ("geomShape_readIgesOCCAGXmlBuilder")
        ("geomSurface_readStepOCCAGXmlBuilder");
  }
  
  void geomShape_readOCCAGXmlBuilder::buildPart(
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    lvH_constValue const * const cV,           
    lvH_analyticFunction const * const aF,    
    lvH_analyticGeometry const * const aG,
    lvH_analyticGeometry * result 
	) const {  
    dt__throwIf(
      !dtXmlParserBase::hasAttribute("filename", toBuild), 
      buildPart()
    );

    geomShape_readOCC readIges(
      dtXmlParserBase::getAttributeStr("filename", toBuild)
    );

    dt__forAllRefAuto(readIges.resultSurfaceRef(), aSurf) {
      result->push_back( new analyticSurface( &aSurf ) );
    }
    dt__forAllRefAuto(readIges.resultCurveRef(), aCurv) {
      result->push_back( new analyticCurve( &aCurv ) );
    }
  }
}
