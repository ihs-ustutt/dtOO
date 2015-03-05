#include "makeCompoundDecorator.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticGeometryCompound.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/vec3dThreeDInMap3dTo3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  makeCompoundDecorator::makeCompoundDecorator() {
  }

  makeCompoundDecorator::~makeCompoundDecorator() {
  }

  void makeCompoundDecorator::buildPart( 
    QDomElement ** toBuildP,
    baseContainer * const bC,            
    vectorHandling< constValue * > const * const cValP,           
    vectorHandling< analyticFunction * > const * const sFunP,    
    vectorHandling< analyticGeometry * > const * const depAGeoP,
    vectorHandling< analyticGeometry * > * aGeoP 
  ) const {
		
    bool hasAG = hasChild("analyticGeometry", **toBuildP);

    if ( hasAG ) {
      //
      // get analyticGeometry
      //
			std::vector<QDomElement> aGElVec = getChildVector("analyticGeometry", **toBuildP);
			
			
			int ii = 0;
      dt__pH(analyticGeometry const) aG(
        createAnalyticGeometry(&aGElVec[ii], bC, cValP, sFunP, depAGeoP)
			);
        
      //
      // check if it is a map3dTo3d
      //
      dt__PTRASS( 
				vec3dThreeDInMap3dTo3d const * v3dIn3d, 
				vec3dThreeDInMap3dTo3d::ConstDownCast(aG.get()) 
			);
			
			analyticGeometryCompound<vec3dThreeDInMap3dTo3d> * v3dIn3dC 
			= 
			new analyticGeometryCompound<vec3dThreeDInMap3dTo3d>();
			
			dt__FORALL(aGElVec, ii,
				dt__pH(analyticGeometry const) aG(
					createAnalyticGeometry(&aGElVec[ii], bC, cValP, sFunP, depAGeoP)
				);

				//
				// check if it is a map3dTo3d
				//
				dt__PTRASS( 
					vec3dThreeDInMap3dTo3d const * v3dIn3d, 
					vec3dThreeDInMap3dTo3d::ConstDownCast(aG.get()) 
				);
				v3dIn3dC->addComponent(*v3dIn3d);
			);
			
			aGeoP->push_back(v3dIn3dC);
    }
  }
}
