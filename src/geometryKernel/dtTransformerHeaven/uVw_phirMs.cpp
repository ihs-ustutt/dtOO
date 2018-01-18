#include "uVw_phirMs.h"

#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/rotatingMap2dTo3d.h>
#include <analyticFunctionHeaven/vec2dMultiBiLinearTwoD.h>
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <logMe/logContainer.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <analyticFunctionHeaven/vec3dThickedTwoD.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <analyticFunctionHeaven/vec3dTransVolThreeD.h>
#include <analyticFunctionHeaven/analyticFunctionTransformed.h>

namespace dtOO {
  uVw_phirMs::uVw_phirMs() : dtTransformer() {
    _ss = dtVector3(1.,1.,1.);
    _nV = 11;
    _nW = 11;    
  }

  uVw_phirMs::uVw_phirMs( uVw_phirMs const & orig ) 
    : dtTransformer(orig) {
		_rM2d.reset( orig._rM2d->clone() );
		_ss = orig._ss;
    _ms_uSPercentVSPercent.reset( orig._ms_uSPercentVSPercent->clone() );
    _nV = orig._nV;
    _nW = orig._nW;
  }
	
  uVw_phirMs::~uVw_phirMs() {
		
  }

  dtTransformer * uVw_phirMs::clone( void ) const {
	  return new uVw_phirMs(*this);
	}
	
  dtTransformer * uVw_phirMs::create( void ) const {
		return new uVw_phirMs();
	}
	
  aFPtrVec uVw_phirMs::apply( 
	  aFPtrVec const * const aFP 
	) const {
		aFPtrVec retV;
		
    for (int ii=0; ii<aFP->size(); ii++) {
			analyticFunction * aF = aFP->at(ii);

      retV.push_back( aF->cloneTransformed( this ) );
      retV.back()->setLabel( aF->getLabel() );
		}
		
		return retV;
  }

	std::vector< dtPoint3 > uVw_phirMs::apply( 
    std::vector< dtPoint3 > const * const toTrans 
  ) const {
		std::vector< dtPoint3 > retVec;
		dt__forAllIndex(*toTrans, ii) {
      float phir = toTrans->at(ii).x() * _ss.x();
		  float mm = std::max( toTrans->at(ii).y() * _ss.y(), 0.);
		  float ss = toTrans->at(ii).z() * _ss.z();
      dt__solution(ss>1., ss=1.);
      dt__solution(ss<0., ss=0.);
      
      float wV = wV_ms(mm, ss);						
			float vV = vV_ms(mm, ss);
			float uV = uV_phirVVWV(phir, vV, wV);
			
			retVec.push_back( dtPoint3(uV, vV, wV) );
		}
		
		return retVec;
	}
	
	std::vector< dtPoint3 > uVw_phirMs::retract( 
    std::vector< dtPoint3 > const * const toRetract 
  ) const {
		std::vector< dtPoint3 > retVec;
		dt__forAllIndex(*toRetract, ii) {
      float uu = toRetract->at(ii).x() / _ss.x();
		  float vv = toRetract->at(ii).y() / _ss.y();
		  float ww = toRetract->at(ii).z() / _ss.z();
		
			float phir = phir_uVvVwV(uu, vv, ww);
      aFY ms
      =
      _ms_uSPercentVSPercent->Y(
        analyticFunction::aFXTwoD(
          _rM2d->constRefMap2dTo3d().percent_u(vv), 
          _rM2d->constRefMap2dTo3d().percent_v(ww)
        )
      );
      float mm = ms[0];        
      float ss = ms[1];
						
      dt__debug(
        retract(),
        << logMe::dtFormat(
             "Retracting point (u, v, w) = (%E, %E, %E)"
           ) % uu % vv % ww << std::endl
        << logMe::dtFormat(
             "to point (phir, mm, ss) = (%E, %E, %E)"
           ) % phir % mm % ss
      );
      
			retVec.push_back( dtPoint3(phir, mm, ss) );
		}
		
		return retVec;
	}
  
  bool uVw_phirMs::isNecessary( void ) const {
    return true;
  }

  void uVw_phirMs::init( 
    ::QDomElement const * tE, 
    baseContainer * const bC,
    cVPtrVec const * const cV,
    aFPtrVec const * const aF,
    aGPtrVec const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);
		    
		if ( dtXmlParserBase::hasChild("Vector_3", *tE) ) {
			::QDomElement v3El = dtXmlParserBase::getChild("Vector_3", *tE);
      _ss = dtXmlParserBase::getDtVector3(&v3El, bC, cV, aF, aG);
		}
    
    dt__ptrAss(
      rotatingMap2dTo3d const * const rM2d, 
      rotatingMap2dTo3d::ConstDownCast(
        aG->get(dtXmlParserBase::getAttributeStr("part_label", *tE))
      )
    );
    _rM2d.reset( rM2d->clone() );

		if ( dtXmlParserBase::hasAttribute("number_points_two", *tE) ) {
      _nV 
      = 
      dtXmlParserBase::getAttributeIntMuParse(
        "number_points_two", *tE, cV, aF
      );
    } 

		if ( dtXmlParserBase::hasAttribute("number_points_three", *tE) ) {
      _nW
      = 
      dtXmlParserBase::getAttributeIntMuParse(
        "number_points_three", *tE, cV, aF
      );
    } 
    
    //
    // create piecewise bilinear mapping
    //
    twoDArrayHandling< dtPoint2 > ms(_nV, _nW);
    dt__forAllIndex(ms, ii) {
      dt__forAllIndex(ms[ii], jj) {
        float uPercent = static_cast<float>(ii)/(_nV-1);
        float vPercent = static_cast<float>(jj)/(_nW-1);
        ms[ii][jj] 
        = 
        dtPoint2(
          m_uSVS(
            _rM2d->constRefMap2dTo3d().u_percent(uPercent), 
            _rM2d->constRefMap2dTo3d().v_percent(vPercent)
          ), 
          s_uSVS( 
            _rM2d->constRefMap2dTo3d().u_percent(uPercent), 
            _rM2d->constRefMap2dTo3d().v_percent(vPercent)          
          )
        );
      }
    }
    _ms_uSPercentVSPercent.reset( new vec2dMultiBiLinearTwoD(ms) );
    
    dt__info(
      init(),
      << dt__eval(_rM2d->constRefMap2dTo3d().getUMin()) << std::endl
      << dt__eval(_rM2d->constRefMap2dTo3d().getUMax()) << std::endl
      << dt__eval(_rM2d->constRefMap2dTo3d().getVMin()) << std::endl
      << dt__eval(_rM2d->constRefMap2dTo3d().getVMax()) << std::endl
      << dt__point2d( _ms_uSPercentVSPercent->YdtPoint2Percent(0.,0.) ) << std::endl
      << dt__point2d( _ms_uSPercentVSPercent->YdtPoint2Percent(1.,0.) ) << std::endl
      << dt__point2d( _ms_uSPercentVSPercent->YdtPoint2Percent(1.,1.) ) << std::endl
      << dt__point2d( _ms_uSPercentVSPercent->YdtPoint2Percent(0.,1.) ) << std::endl
    );
  }
  
	float uVw_phirMs::m_uSVS(float const & uu, float const & vv) const {
		ptrHandling< map1dTo3d > m1d( 
      _rM2d->constRefMap2dTo3d().segmentConstV(vv) 
    );
		return m1d->l_u(uu);
	}
  
	float uVw_phirMs::s_uSVS(float const & uu, float const & vv) const {
		ptrHandling< map1dTo3d > m1d( 
      _rM2d->constRefMap2dTo3d().segmentConstU(uu) 
    );
		
		return m1d->lPercent_u(vv);		
	} 
  
	float uVw_phirMs::uV_phirVVWV(
    float const & phir, float const & vv, float const & ww
  ) const {
    //
		// radius
		//
    dtVector3 vXYZ 
    = 
    _rM2d->origin() - _rM2d->constRefMap2dTo3d().getPoint(vv, ww);
    dtVector3 pointOnRotAx 
    = 
    _rM2d->rotationAxis() 
    * 
    dtLinearAlgebra::dotProduct(_rM2d->rotationAxis(), vXYZ);
    dtVector3 rr = vXYZ - pointOnRotAx;
    
		return (phir/dtLinearAlgebra::length(rr)) / (2.*M_PI);
	}
	
	float uVw_phirMs::vV_ms(float const & mm, float const & ss) const {
    aFY ms = analyticFunction::aFYTwoD(mm, ss);
    aFX uvPercent = _ms_uSPercentVSPercent->invY(ms);
    
    return _rM2d->constRefMap2dTo3d().u_percent(uvPercent[0]);
	}	
	
	float uVw_phirMs::wV_ms(float const & mm, float const & ss) const {
    aFY ms = analyticFunction::aFYTwoD(mm, ss);
    aFX uvPercent = _ms_uSPercentVSPercent->invY(ms);
    
    return _rM2d->constRefMap2dTo3d().v_percent(uvPercent[1]);    
	}    

  float uVw_phirMs::phir_uVvVwV(
    float const & uu, float const & vv, float const & ww
  ) const {
    //
		// radius
		//    
    dtVector3 vXYZ 
    = 
    _rM2d->origin() - _rM2d->constRefMap2dTo3d().getPoint(vv, ww);
    dtVector3 pointOnRotAx 
    = 
    _rM2d->rotationAxis() 
    * 
    dtLinearAlgebra::dotProduct(_rM2d->rotationAxis(), vXYZ);
    dtVector3 rr = vXYZ - pointOnRotAx;
		
		return uu*dtLinearAlgebra::length(rr) * (2.*M_PI);
  }
}