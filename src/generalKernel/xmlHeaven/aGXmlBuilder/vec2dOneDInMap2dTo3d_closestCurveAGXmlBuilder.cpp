#include "vec2dOneDInMap2dTo3d_closestCurveAGXmlBuilder.h"

#include <xmlHeaven/aGXmlBuilderFactory.h>
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h>
#include <geometryEngine/dtCurve2d.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/vec2dOneDInMap2dTo3d.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

#include <dtAnalysis.h>
#include <Math/Functor.h>

#include <QtXml/QDomElement>

namespace dtOO {
  bool vec2dOneDInMap2dTo3d_closestCurveAGXmlBuilder::_registrated 
  =
  aGXmlBuilderFactory::registrate(
    dt__tmpPtr(
      vec2dOneDInMap2dTo3d_closestCurveAGXmlBuilder, 
      new vec2dOneDInMap2dTo3d_closestCurveAGXmlBuilder()
    )
  );
    
  vec2dOneDInMap2dTo3d_closestCurveAGXmlBuilder
    ::vec2dOneDInMap2dTo3d_closestCurveAGXmlBuilder() {
  }

  vec2dOneDInMap2dTo3d_closestCurveAGXmlBuilder
    ::~vec2dOneDInMap2dTo3d_closestCurveAGXmlBuilder() {
  }

  void vec2dOneDInMap2dTo3d_closestCurveAGXmlBuilder::buildPart( 
    ::QDomElement const & toBuild,
    baseContainer * const bC,
    cVPtrVec const * const cV,  
    aFPtrVec const * const aF,  
    aGPtrVec const * const aG,
    aGPtrVec * result 
  ) const {
    dt__throwIf(
      !dtXmlParserBase::hasChild("analyticGeometry", toBuild), buildPart()
    );
		dt__throwIf(
      !dtXmlParserBase::hasAttribute("numPoints", toBuild), buildPart()
    );
    
    //
    // get analyticGeometries
    //
    dt__forAllRefAuto(
      dtXmlParserBase::getChildVector("analyticGeometry", toBuild), aChild
    ) {
      ptrHandling< analyticGeometry > aG_t(
        dtXmlParserBase::createAnalyticGeometry(aChild, bC, cV, aF, aG)
      );
      
      if ( map2dTo3d::ConstDownCast(aG_t.get()) ) {
        _m2d.reset( map2dTo3d::ConstSecureCast(aG_t->clone()) );
      }
      else if ( map1dTo3d::ConstDownCast(aG_t.get()) ) {
        _m1d.reset( map1dTo3d::ConstSecureCast(aG_t->clone()) );
      }
      else dt__throwUnexpected(buildPart());
    }
    dt__throwIf(_m1d.isNull() || _m2d.isNull(), buildPart());    
    
    dt__forAllRefAuto(
      dtLinearAlgebra::unitGrid(
        dtXmlParserBase::getAttributeIntMuParse("numPoints", toBuild, cV, aF)
      ),
      aSample
    ) {
      _sampleXYZ.push_back( _m1d->getPointPercent(aSample));
    }
   
    
    // 
    // multidimensional minimization
    //
    dt__pH(dtMinimizer) min(
      dtAnalysis::createMinimizer(":Minuit2::kMigrad:")
    );
    ::ROOT::Math::Functor toMin(
      this, 
      &vec2dOneDInMap2dTo3d_closestCurveAGXmlBuilder::F, 
      _sampleXYZ.size() * 2 
    );			
    min->SetFunction(toMin);          
    //
    // set bounds
    //
    int cc = 0;
    dt__forAllRefAuto(_sampleXYZ, aSampleXYZ) {
      min->SetVariable( cc, "U_"+stringPrimitive::intToString(cc), 0.5, 0.01 );
      min->SetVariableLimits(cc, 0., 1.);	
      cc++;
      min->SetVariable( cc, "V_"+stringPrimitive::intToString(cc), 0.5, 0.01 );
      min->SetVariableLimits(cc, 0., 1.);	
      cc++;
    }
    
    //
    // minimizer options
    //        
    min->SetMaxFunctionCalls(1000000);
    min->SetMaxIterations(1000);
    min->SetTolerance( 1.e-8 );			
    min->SetPrintLevel(
      staticPropertiesHandler::getInstance()->getOptionInt("root_printLevel") 
    );

    //
    // minimize
    //
    bool converged = min->Minimize();
    dt__quickinfo( << dt__eval(converged) );
    double const * const theRoot = min->X();

    cc = 0;
    vectorHandling< dtPoint2 > uv;
    dt__forAllRefAuto(_sampleXYZ, aSampleXYZ) {
      dt__quickinfo(
        << logMe::dtFormat("theRoot[ %3i %3i ] = %5.2f %5.2f")
          % cc % (cc+1) % theRoot[cc] % theRoot[cc+1]
      );
      uv.push_back( (*_m2d) % dtPoint2(theRoot[cc], theRoot[cc+1]) );
      cc = cc + 2;
    }

    result->push_back( 
      new vec2dOneDInMap2dTo3d(
        dt__tmpPtr(
          vec2dCurve2dOneD,
          new vec2dCurve2dOneD(
            dt__tmpPtr(
              dtCurve2d, bSplineCurve2d_pointConstructOCC(uv, 1).result()
            )
          )
        )
        , 
        _m2d.get(), 
        false
      ) 
    );                    
  }
  
	double vec2dOneDInMap2dTo3d_closestCurveAGXmlBuilder::F(
    double const * xx
  ) const {	
    double objective = 0.;
    int cc = 0;
    dt__forAllRefAuto(_sampleXYZ, aSampleXYZ) {
      objective
      =
      objective
      +
      dtLinearAlgebra::length(
        aSampleXYZ - _m2d->getPointPercent( dtPoint2(xx[cc], xx[cc+1]) )
      );
      
      cc = cc + 2;
    }
    return objective;
	}  
}
