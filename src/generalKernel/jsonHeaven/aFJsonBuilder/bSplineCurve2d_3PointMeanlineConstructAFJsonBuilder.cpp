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

#include "bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder.h"

#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <logMe/logMe.h>
#include <baseContainerHeaven/baseContainer.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <geometryEngine/dtOCCCurve2d.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_angleRatioDeltaYConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_angleDeltaXDeltaYConstructOCC.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>

#include <boost/assign.hpp>

namespace dtOO { 
  bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder
    ::bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder() { 
  }

  bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder
    ::~bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder() {

  }
 
  lvH_analyticFunction 
  bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder::buildPart(
		jsonPrimitive const & config, 
		baseContainer const * const bC,
		lvH_constValue const * const cV, 
		lvH_analyticFunction const * const aF
	) const {
		dt__pH(dtCurve2d) dtC2d;
    if ( 
      config.contains("ratio") 
      && 
      !config.contains("deltaX") 
      && 
      config.contains("deltaY") 
    ) {
			dtC2d.reset(
				bSplineCurve2d_angleRatioDeltaYConstructOCC(
					config.lookup< dtReal >("alphaOne"), 
          config.lookup< dtReal >("alphaTwo"), 
          config.lookup< dtReal >("ratio"), 
          config.lookup< dtReal >("deltaY")
				).result()
			);
    }
    else if (
      !config.contains("ratio") 
      && 
      config.contains("deltaX") 
      && 
      config.contains("deltaY")             
    ) {
      //
      // get necessary values
      //
			dtC2d.reset(
				bSplineCurve2d_angleDeltaXDeltaYConstructOCC(
					config.lookup< dtReal >("alphaOne"), 
          config.lookup< dtReal >("alphaTwo"), 
          config.lookup< dtReal >("deltaX"), 
          config.lookup< dtReal >("deltaY")      
				).result()
			);
    }		
    else {
			dt__throw(
			  buildPart(), 
				<< "config = " << config.toStdString()
			);
		}

    if ( 
      config.contains("targetLength") 
    ) {
      logContainer<bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder> logC(
        logINFO, "buildPart()"
      );      
      logC() << "Detect targetLength" << std::endl;
      dt__forFromToIndex(0, 99, ii) {
        dtReal scale 
        = 
        config.lookup< dtReal >("targetLength") / dtC2d->length();
      
        dtPoint2 p0 = dtC2d->controlPoint(0);
        dtVector2 v0 = dtC2d->controlPoint(1) - dtC2d->controlPoint(0);
        dtVector2 v1 = dtC2d->controlPoint(2) - dtC2d->controlPoint(1);

        dtC2d->setControlPoint( 1, p0 + ( 1. + (scale-1.)/2. ) * v0 );
        dtC2d->setControlPoint( 2, p0 + ( 1. + (scale-1.)/2. ) * ( v0 + v1 ) );
        
        logC() 
          << logMe::dtFormat("( %3d ) : %5.2f -> %5.2f") 
            % ii 
            % scale 
            % (config.lookup< dtReal >("targetLength") / dtC2d->length())
          << std::endl;
        if ( 
          fabs(config.lookup< dtReal >("targetLength") / dtC2d->length() - 1.) 
          <= 
          config.lookup< dtReal >("targetLengthTolerance") 
        ) break;
      }
    } 
    
    if ( config.contains("originOnLengthPercent") ) {
      dtPoint2 adjustP 
      = 
      dtC2d->point( 
        dtC2d->u_lPercent( config.lookup< dtReal >("originOnLengthPercent") ) 
      );
      dt__forFromToIndex(0, dtC2d->nControlPoints(), ii) {
        dtC2d->setControlPoint(
          ii,
          dtLinearAlgebra::toDtPoint2( dtC2d->controlPoint(ii) - adjustP )
        );
      }
    } 
		if ( config.lookupDef< bool >("revert", false) ) {
		  dtOCCCurve2d::SecureCast(dtC2d.get())->revert();
		}		
    lvH_analyticFunction ret;
		ret.push_back( new vec2dCurve2dOneD( dtC2d.get() ) );
    return ret; 
  }
}
