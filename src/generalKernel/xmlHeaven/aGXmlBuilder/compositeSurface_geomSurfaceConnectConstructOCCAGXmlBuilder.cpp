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

#include "compositeSurface_geomSurfaceConnectConstructOCCAGXmlBuilder.h"

#include <xmlHeaven/aGXmlBuilderFactory.h>

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <geometryEngine/dtCurve.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/compositeSurface_surfaceConnectConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bool compositeSurface_geomSurfaceConnectConstructOCCAGXmlBuilder::_registrated 
  =
  aGXmlBuilderFactory::registrate(
    dt__tmpPtr(
      compositeSurface_geomSurfaceConnectConstructOCCAGXmlBuilder, 
      new compositeSurface_geomSurfaceConnectConstructOCCAGXmlBuilder()
    )
  );
  
  compositeSurface_geomSurfaceConnectConstructOCCAGXmlBuilder
    ::compositeSurface_geomSurfaceConnectConstructOCCAGXmlBuilder() {
  }

  compositeSurface_geomSurfaceConnectConstructOCCAGXmlBuilder
    ::~compositeSurface_geomSurfaceConnectConstructOCCAGXmlBuilder() {
  }

  void compositeSurface_geomSurfaceConnectConstructOCCAGXmlBuilder::buildPart(
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    lvH_constValue const * const cV,           
    lvH_analyticFunction const * const aF,    
    lvH_analyticGeometry const * const aG,
    lvH_analyticGeometry * result 
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
