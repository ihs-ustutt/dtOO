#include "uVw_phiMs.h"
#include "interfaceHeaven/systemHandling.h"
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/rotatingMap2dTo3d.h>
#include <analyticFunctionHeaven/vec2dMultiBiLinearTwoD.h>
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <analyticFunctionHeaven/vec3dThickedTwoD.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <analyticFunctionHeaven/vec3dTransVolThreeD.h>
#include <analyticFunctionHeaven/analyticFunctionTransformed.h>
#include "dtTransformerFactory.h"

namespace dtOO {
  bool uVw_phiMs::_registrated 
  =
  dtTransformerFactory::registrate(
    dt__tmpPtr(uVw_phiMs, new uVw_phiMs())
  );
  
  uVw_phiMs::uVw_phiMs() : dtTransformer() {
    _ss = dtVector3(1.,1.,1.);
    _nV = 11;
    _nW = 11;        
  }

  uVw_phiMs::uVw_phiMs(
	  uVw_phiMs const & orig 
	) : dtTransformer(orig) {
		_rM2d.reset( orig._rM2d->clone() );
		_ss = orig._ss;
    _ms_uSPercentVSPercent.reset( orig._ms_uSPercentVSPercent->clone() );
    _nV = orig._nV;
    _nW = orig._nW;
  }
	
  uVw_phiMs::~uVw_phiMs() {
		
  }

  dtTransformer * uVw_phiMs::clone( void ) const {
	  return new uVw_phiMs(*this);
	}
	
  dtTransformer * uVw_phiMs::create( void ) const {
		return new uVw_phiMs();
	}
	
  aFPtrVec uVw_phiMs::apply( aFPtrVec const * const aFP ) const {
		aFPtrVec retV;
		
    for (int ii=0; ii<aFP->size(); ii++) {
			analyticFunction * aF = aFP->at(ii);

      retV.push_back( aF->cloneTransformed( this ) );
      retV.back()->setLabel( aF->getLabel() );
		}
		
		return retV;
  }

	std::vector< dtPoint3 > uVw_phiMs::apply( 
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
	
	std::vector< dtPoint3 > uVw_phiMs::retract( 
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
  
  bool uVw_phiMs::isNecessary( void ) const {
    return true;
  }

  void uVw_phiMs::init( 
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
      _nW 
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
    twoDArrayHandling< dtPoint2 > ms(_nW, _nW);
    dt__forAllIndex(ms, ii) {
      dt__forAllIndex(ms[ii], jj) {
        float uPercent = static_cast<float>(ii)/(_nW-1);
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
  
//  void uVw_phiMs::handleAnalyticGeometry(
//    std::string const name, analyticGeometry const * value
//  ) {
//    if (name == "part_label") {
//      dt__ptrAss(
//        rotatingMap2dTo3d const * const m3d, 
//        rotatingMap2dTo3d::ConstDownCast(value)
//      );
//			_rM2d.reset( m3d->clone() );
//      return;
//    }
//    dtTransformer::handleAnalyticGeometry(name, value);
//  }
//	
//  void uVw_phiMs::handleDtVector3(
//    std::string const name, dtVector3 const value
//  ) {
//    if (name == "Vector_3") {
//      _ss = value;
//      return;
//    }
//    dtTransformer::handleDtVector3(name, value);
//  }	
//  
//  void uVw_phiMs::handleInt(std::string const name, int const value) {
//    dt__info(handleInt(),
//      << dt__eval(name) << std::endl
//      << dt__eval(value) 
//    );
//    if (name == "number_points_two") {
//      _nV = value;
//      return;
//    }
//    else if (name == "number_points_three") {
//      _nW = value;
//      return;
//    }
//    dtTransformer::handleInt(name, value);
//  }	  
  
	float uVw_phiMs::m_uSVS(float const & uu, float const & vv) const {
		ptrHandling< map1dTo3d > m1d( 
      _rM2d->constRefMap2dTo3d().segmentConstV(vv) 
    );
		return m1d->l_u(uu);		
	}
  
	float uVw_phiMs::s_uSVS(float const & uu, float const & vv) const {
		ptrHandling< map1dTo3d > m1d( 
      _rM2d->constRefMap2dTo3d().segmentConstU(uu) 
    );
		
		return m1d->lPercent_u(vv);		
	} 
  
	float uVw_phiMs::uV_phirVVWV(
    float const & phir, float const & vv, float const & ww
  ) const {
//    //
//		// get radius
//		//
//    dtVector3 vXYZ 
//    = 
//    _rM2d->origin()
//    -
////    dtLinearAlgebra::toDtVector3(
//      _rM2d->constRefMap2dTo3d().getPoint(vv, ww)
////    );
//      ;
//    dtVector3 pointOnRotAx 
//    = 
//    _rM2d->rotationAxis() 
//    * 
//    dtLinearAlgebra::dotProduct(_rM2d->rotationAxis(), vXYZ);
//    
//    dtVector3 rr = vXYZ - pointOnRotAx;
    
		return phir / (2.*M_PI);
	}
	
	float uVw_phiMs::vV_ms(float const & mm, float const & ss) const {
    aFY ms = analyticFunction::aFYTwoD(mm, ss);
    aFX uvPercent = _ms_uSPercentVSPercent->invY(ms);
    
    return _rM2d->constRefMap2dTo3d().u_percent(uvPercent[0]);
	}	
	
	float uVw_phiMs::wV_ms(float const & mm, float const & ss) const {
    aFY ms = analyticFunction::aFYTwoD(mm, ss);
    aFX uvPercent = _ms_uSPercentVSPercent->invY(ms);
    
    return _rM2d->constRefMap2dTo3d().v_percent(uvPercent[1]);    
	}    

  float uVw_phiMs::phir_uVvVwV(
    float const & uu, float const & vv, float const & ww
  ) const {
//    dtVector3 vXYZ 
//    = 
//    _rM2d->origin() -
////    dtLinearAlgebra::toDtVector3(
//      _rM2d->constRefMap2dTo3d().getPoint(vv, ww)
////    );
//      ;
//    dtVector3 pointOnRotAx 
//    = 
//    _rM2d->rotationAxis() 
//    * 
//    dtLinearAlgebra::dotProduct(_rM2d->rotationAxis(), vXYZ);
//    dtVector3 rr = vXYZ - pointOnRotAx;
		
		return uu * (2.*M_PI);
  }
  
//  float uVw_phiMs::m_vVs(float const & vv, float const & ss) const {
//    aFX xx 
//    = 
//    analyticFunction::aFXTwoD( _rM2d->constRefMap2dTo3d().u_percent(vv), ss );
//    aFY yy = _ms_uSPercentVSPercent->Y(xx);
//    
//    return _rM2d->constRefMap2dTo3d().percent_u(yy[0]);    
//  }
}