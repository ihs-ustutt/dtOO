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

#include "geomCurve_map2dTo3dIntersectAGXmlBuilder.h"

#include <xmlHeaven/aGXmlBuilderFactory.h>
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/pairUUV_map1dTo3dClosestPointToMap2dTo3d.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h>
#include <geometryEngine/dtCurve2d.h>
#include <analyticGeometryHeaven/vec2dOneDInMap2dTo3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bool geomCurve_map2dTo3dIntersectAGXmlBuilder::_registrated 
  =
  aGXmlBuilderFactory::registrate(
    dt__tmpPtr(
      geomCurve_map2dTo3dIntersectAGXmlBuilder, 
      new geomCurve_map2dTo3dIntersectAGXmlBuilder()
    )
  );
  
	geomCurve_map2dTo3dIntersectAGXmlBuilder
		::geomCurve_map2dTo3dIntersectAGXmlBuilder() {

	}

	geomCurve_map2dTo3dIntersectAGXmlBuilder
		::~geomCurve_map2dTo3dIntersectAGXmlBuilder() {

	}

	void geomCurve_map2dTo3dIntersectAGXmlBuilder::buildPart(
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    lvH_constValue const * const cV,           
    lvH_analyticFunction const * const aF,    
    lvH_analyticGeometry const * const aG,
    lvH_analyticGeometry * result 
	) const {
    dt__throwIf(
      !dtXmlParserBase::hasChild("analyticGeometry", toBuild)
      || 
      !dtXmlParserBase::hasAttribute("order", toBuild)
      ||
      !dtXmlParserBase::hasAttribute("wire", toBuild), 
      buildPart()
    );
		
    dt__ptrAss(
      map2dTo3d const * wire, 
      map2dTo3d::MustConstDownCast(
        aG->get( dtXmlParserBase::getAttributeStr("wire", toBuild) )
      )
    );
    
    dt__ptrAss(
      map2dTo3d const * m2d, 
      map2dTo3d::MustConstDownCast(
        dtXmlParserBase::createAnalyticGeometry(      
          dtXmlParserBase::getChild("analyticGeometry", toBuild), 
          bC, 
          cV, 
          aF, 
          aG
        )
      )
    );
    
    dt__pVH(map1dTo3d) m1dV;
    if ( dtXmlParserBase::hasAttribute("number_points_one", toBuild) ) {
      dt__forAllRefAuto(
        dtLinearAlgebra::unitGrid(
          dtXmlParserBase::getAttributeIntMuParse(
            "number_points_one", toBuild, cV
          )
        ),
        segPer
      ) {
        m1dV.push_back( wire->segmentConstUPercent(segPer) );
      }
    }
    else if ( dtXmlParserBase::hasAttribute("number_points_two", toBuild) ) {
      dt__forAllRefAuto(
        dtLinearAlgebra::unitGrid(
          dtXmlParserBase::getAttributeIntMuParse(
            "number_points_two", toBuild, cV
          )
        ),
        segPer
      ) {
        m1dV.push_back( wire->segmentConstVPercent(segPer) );
      }
    }
    else dt__throwUnexpected(buildPart());
    
    std::vector< std::pair< dtReal, dtPoint2 > > interPoints;
    dt__forAllRefAuto(m1dV, aMap) {
      interPoints.push_back(
        pairUUV_map1dTo3dClosestPointToMap2dTo3d(&aMap, m2d).result()
      );
    }
    
    std::vector< dtPoint2 > pUV;
    dt__forAllRefAuto(interPoints, aPoint) pUV.push_back( aPoint.second );
    
		result->push_back(
      new vec2dOneDInMap2dTo3d(
        dt__tmpPtr(
          vec2dCurve2dOneD,
          new vec2dCurve2dOneD(      
            dt__tmpPtr(
              dtCurve2d, 
              bSplineCurve2d_pointConstructOCC(
                pUV, 
                dtXmlParserBase::getAttributeIntMuParse("order", toBuild, cV)
              ).result()
            )
          )
        ),
        m2d
      )
    );
	}
}
