#include "splitTrans6SidedCubeAGXmlBuilder.h"

#include <xmlHeaven/aGXmlBuilderFactory.h>
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/trans6SidedCube.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>
#include <geometryEngine/geoBuilder/bSplineSurfaces_bSplineSurfaceSkinConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurface_exchangeSurfaceConstructOCC.h>
#include <geometryEngine/dtSurface.h>
#include <analyticGeometryHeaven/aGBuilder/trans6SidedCube_splitTrans6SidedCube.h>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bool splitTrans6SidedCubeAGXmlBuilder::_registrated 
  =
  aGXmlBuilderFactory::registrate(
    dt__tmpPtr(
      splitTrans6SidedCubeAGXmlBuilder, 
      new splitTrans6SidedCubeAGXmlBuilder()
    )
  );
  
  splitTrans6SidedCubeAGXmlBuilder::splitTrans6SidedCubeAGXmlBuilder() {
  }

  splitTrans6SidedCubeAGXmlBuilder::~splitTrans6SidedCubeAGXmlBuilder() {
  }

  void splitTrans6SidedCubeAGXmlBuilder::buildPart( 
    ::QDomElement const & toBuild,
    baseContainer * const bC,
    lvH_constValue const * const cV,  
    lvH_analyticFunction const * const aF,  
    lvH_analyticGeometry const * const aG,
    lvH_analyticGeometry * result
  ) const {
    //
		// check input
		//    
    dt__throwIf(
      !dtXmlParserBase::hasChild("analyticGeometry", toBuild)
      &&
      !dtXmlParserBase::hasChild("Point_3", toBuild),
      buildPart()
    );
    
    //
    // create and cast analyticGeometry
    //
    lvH_analyticGeometry aGV;
    dtXmlParserBase::createAdvanced( 
      dtXmlParserBase::getChild("analyticGeometry", toBuild), 
      bC, cV, aF, aG, &aGV 
    );
      
    //
    // modify
    //
    dt__forAllRefAuto( aGV, anAG ) {
      trans6SidedCube const * const t6 
      = 
      trans6SidedCube::MustDownCast( anAG );

      dt__forAllRefAuto(        
        trans6SidedCube_splitTrans6SidedCube(
          t6, 
          dtXmlParserBase::createDtPoint3(
            dtXmlParserBase::getChild("Point_3", toBuild), bC, cV, aF, aG
          ) 
        ).result(), 
        aRes
      ) {
        result->push_back( aRes );  
      }
    }
    aGV.destroy();
  }
}
