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

#include "translate.h"
#include <progHelper.h>
#include <logMe/logMe.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/scaCurve2dOneD.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>

#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/map1dTo3dTransformed.h>
#include <analyticGeometryHeaven/map2dTo3dTransformed.h>
#include <analyticGeometryHeaven/map3dTo3dTransformed.h>

#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/geomCurve_curveTranslateConstructOCC.h>
#include <geometryEngine/geoBuilder/geomCurve2d_curve2dTranslateConstructOCC.h>
#include <geometryEngine/geoBuilder/geomSurface_surfaceTranslateConstructOCC.h>
#include <baseContainerHeaven/baseContainer.h>
#include <baseContainerHeaven/pointContainer.h>
#include <baseContainerHeaven/vectorContainer.h>
#include "dtTransformerFactory.h"

namespace dtOO {
  bool translate::_registrated 
  =
  dtTransformerFactory::registrate(
    dt__tmpPtr(translate, new translate())
  );
 

  translate::translate( 
    jsonPrimitive const & jE 
  ) : dtTransformer(jE) {
    this->jInit(jE, NULL, NULL, NULL, NULL);
  }

  translate::translate() : dtTransformer() {
  }

	translate::translate( translate const & orig ) : dtTransformer(orig) {
	}
	
  translate::~translate() {
  }

  dtTransformer * translate::clone( void ) const {
	  return new translate(*this);
	}
	
  dtTransformer * translate::create( void ) const {
		return new translate();
	}

	std::vector< dtPoint3 > translate::apply( 
    std::vector< dtPoint3 > const * const toTrans 
  ) const {
		std::vector< dtPoint3 > ret(toTrans->size());
    
		dt__forAllIndex(*toTrans, ii) {
		  ret[ii] = toTrans->at(ii) + config().lookup<dtVector3>("_v3");
		}
		
		return ret;
	}
  
  lvH_analyticFunction translate::apply( 
	  lvH_analyticFunction const * const aFunVecP 
	) const {
    lvH_analyticFunction retSFun;

		
    dt__forAllRefAuto(*aFunVecP, aF) {
      //
      // clone and cast scaFunction
      //
			scaCurve2dOneD  const * const sC2d1d 
      = 
      scaCurve2dOneD::ConstDownCast( aF );
		  vec2dCurve2dOneD  const * const v2dC1d 
      = 
      vec2dCurve2dOneD::ConstDownCast( aF );
			vec3dCurveOneD  const * const v3dC1d 
      = 
      vec3dCurveOneD::ConstDownCast( aF );
			
			if ( sC2d1d ) {
				retSFun.push_back( 
          new scaCurve2dOneD( 
            dt__tmpPtr(
              dtCurve2d, 
              geomCurve2d_curve2dTranslateConstructOCC(
                sC2d1d->constPtrDtCurve2d(), 
								config().lookup<dtVector2>("_v2")
              ).result()
            )
          ) 
        );
				retSFun.back()->setLabel(sC2d1d->getLabel());
			}
			else if ( v2dC1d ) {
				retSFun.push_back( 
          new vec2dCurve2dOneD(
            dt__tmpPtr(
              dtCurve2d, 
              geomCurve2d_curve2dTranslateConstructOCC(
                v2dC1d->ptrDtCurve2d(), 
								config().lookup<dtVector2>("_v2")
              ).result()
            )
          ) 
        );
				retSFun.back()->setLabel(v2dC1d->getLabel());
			}			
			else if ( v3dC1d ) {
				retSFun.push_back( 
          new vec3dCurveOneD(
            dt__tmpPtr(
              dtCurve, 
              geomCurve_curveTranslateConstructOCC(
                v3dC1d->ptrDtCurve(), 
								config().lookup<dtVector3>("_v3")
              ).result()
            )
          ) 
        );
				retSFun.back()->setLabel(v3dC1d->getLabel());
			}
			else {
        dt__throw(
					apply(),
          << dt__eval(sC2d1d) << std::endl
          << dt__eval(v2dC1d) << std::endl
          << dt__eval(v3dC1d) << std::endl								
          << "Unknown type."
				);
      }					
    }
    return retSFun;    
  }

  lvH_analyticGeometry translate::apply( 
	  lvH_analyticGeometry const * const aGeoVecP 
	) const {
    lvH_analyticGeometry retAGeo;

    dt__forAllConstIter(lvH_analyticGeometry, *aGeoVecP, it) {
			//
			// clone and cast analyticGeometry
			//
      map1dTo3d const * const m1d = map1dTo3d::ConstDownCast(*it);      
      map2dTo3d const * const m2d = map2dTo3d::ConstDownCast(*it);
      map3dTo3d const * const m3d = map3dTo3d::ConstDownCast(*it);
      
      if (m1d) {
        analyticCurve const * const s3 = analyticCurve::ConstDownCast(*it);
        if ( s3 &&  !s3->isTransformed() ) {
          retAGeo.push_back( 
            new analyticCurve(
              dt__tmpPtr(
                dtCurve,
                geomCurve_curveTranslateConstructOCC( 
                  s3->ptrConstDtCurve(),
								  config().lookup<dtVector3>("_v3")
                ).result()
              )
            ) 
          );
        }
        else {
          retAGeo.push_back( m1d->cloneTransformed(this) );
        }
      }
			else if (m2d) {
        analyticSurface const * const aS = analyticSurface::ConstDownCast(*it);
        if ( aS &&  !aS->isTransformed() ) {
          retAGeo.push_back( 
            new analyticSurface(
              dt__tmpPtr(
                dtSurface,
                geomSurface_surfaceTranslateConstructOCC( 
                  aS->ptrDtSurface(), 
								  config().lookup<dtVector3>("_v3")
							  ).result()
              )
            ) 
          );
        }
        else {        
          retAGeo.push_back( m2d->cloneTransformed(this) );
        }
      }
			else if (m3d) {
        retAGeo.push_back( m3d->cloneTransformed(this) );
      }
      else dt__throwUnexpected(apply());
    }
    return retAGeo;
  }
  
	std::vector< dtPoint3 > translate::retract( 
    std::vector< dtPoint3 > const * const toRetract 
  ) const {
		std::vector< dtPoint3 > ret(toRetract->size());
    
		dt__forAllIndex(*toRetract, ii) {
			ret[ii] = toRetract->at(ii) - config().lookup<dtVector3>("_v3");
		}
		
		return ret;
	}
  
  bool translate::isNecessary( void ) const {
    return true;
  }

	void translate::jInit( 
    jsonPrimitive const & jE,
    baseContainer * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG 
	) {
    dtTransformer::jInit(jE, bC, cV, aF, aG);
  }



  void translate::init( 
	  ::QDomElement const * tE, 
    baseContainer * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);
		
		jsonPrimitive config;
    if ( dtXmlParserBase::hasChild("Vector_2", *tE) ) {
   		::QDomElement	v2El = dtXmlParserBase::getChild("Vector_2", *tE);
      config.append< dtVector2 >(
			  "_v2", 
			  dtXmlParserBase::getDtVector2(&v2El, bC, cV, aF, aG)
			);

    }
    if ( dtXmlParserBase::hasChild("Vector_3", *tE) ) {
			::QDomElement v3El = dtXmlParserBase::getChild("Vector_3", *tE);
      config.append< dtVector3 >(
			  "_v3", 
			  dtXmlParserBase::getDtVector3(&v3El, bC, cV, aF, aG)
			);
    }
		jInit(config, bC, cV, aF, aG);
  }
  
  dtVector3 translate::translationAxis3d( void ) const {
    return config().lookup<dtVector3>("_v3");
  }
  
  dtVector2 translate::translationAxis2d( void ) const {
    return config().lookup<dtVector2>("_v2");
  }
}
