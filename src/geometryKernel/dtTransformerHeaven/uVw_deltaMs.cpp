#include "uVw_deltaMs.h"
#include "interfaceHeaven/systemHandling.h"
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticFunctionHeaven/vec2dMultiBiLinearTwoD.h>
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <analyticFunctionHeaven/vec3dThickedTwoD.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <analyticFunctionHeaven/vec3dTransVolThreeD.h>
#include <analyticFunctionHeaven/analyticFunctionTransformed.h>
#include <analyticGeometryHeaven/translatingMap2dTo3d.h>
#include "dtTransformerFactory.h"

namespace dtOO {
  bool uVw_deltaMs::_registrated 
  =
  dtTransformerFactory::registrate(
    dt__tmpPtr(uVw_deltaMs, new uVw_deltaMs())
  );
  
  uVw_deltaMs::uVw_deltaMs() : dtTransformer() {
    _ss = dtVector3(1.,1.,1.);
    _nV = 11;
    _nW = 11;        
  }

  uVw_deltaMs::uVw_deltaMs( uVw_deltaMs const & orig ) : dtTransformer(orig) {
		_tM2dTo3d.reset( orig._tM2dTo3d->clone() );
    _msCut.reset( orig._msCut->clone() );
		_ss = orig._ss;
    _ms_uSPercentVSPercent.reset( orig._ms_uSPercentVSPercent->clone() );
    _nV = orig._nV;
    _nW = orig._nW;
  }
	
  uVw_deltaMs::~uVw_deltaMs() {
		
  }

  dtTransformer * uVw_deltaMs::clone( void ) const {
	  return new uVw_deltaMs(*this);
	}
	
  dtTransformer * uVw_deltaMs::create( void ) const {
		return new uVw_deltaMs();
	}
	
  aFPtrVec uVw_deltaMs::apply( aFPtrVec const * const aFP ) const {
		aFPtrVec retV;
		
    for (int ii=0; ii<aFP->size(); ii++) {
			analyticFunction * aF = aFP->at(ii);

      retV.push_back( aF->cloneTransformed( this ) );
      retV.back()->setLabel( aF->getLabel() );
		}
		
		return retV;
  }

	std::vector< dtPoint3 > uVw_deltaMs::apply( 
    std::vector< dtPoint3 > const * const toTrans 
  ) const {
		std::vector< dtPoint3 > retVec;
		dt__forAllIndex(*toTrans, ii) {
      float delta = toTrans->at(ii).x() * _ss.x();
		  float mm = std::max( toTrans->at(ii).y() * _ss.y(), 0.);
		  float ss = toTrans->at(ii).z() * _ss.z();
      dt__solution(ss>1., ss=1.);
      dt__solution(ss<0., ss=0.);
      
      float wV = wV_ms(mm, ss);						
			float vV = vV_ms(mm, ss);
			float uV = uV_deltaVVWV(delta, vV, wV);
			
			retVec.push_back( dtPoint3(uV, vV, wV) );
		}
		
		return retVec;
	}
	
	std::vector< dtPoint3 > uVw_deltaMs::retract( 
    std::vector< dtPoint3 > const * const toRetract 
  ) const {
		std::vector< dtPoint3 > retVec;
		dt__forAllIndex(*toRetract, ii) {
      float uu = toRetract->at(ii).x() / _ss.x();
		  float vv = toRetract->at(ii).y() / _ss.y();
		  float ww = toRetract->at(ii).z() / _ss.z();
		
			float delta = delta_uVvVwV(uu, vv, ww);
      aFY ms
      =
      _ms_uSPercentVSPercent->Y(
        analyticFunction::aFXTwoD(
          _msCut->percent_u(vv), _msCut->percent_v(ww)
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
             "to point (delta, mm, ss) = (%E, %E, %E)"
           ) % delta % mm % ss
      );
      
			retVec.push_back( dtPoint3(delta, mm, ss) );
		}
		
		return retVec;
	}
  
  bool uVw_deltaMs::isNecessary( void ) const {
    return true;
  }

  void uVw_deltaMs::init( 
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
      translatingMap2dTo3d const * const tM2dTo3d, 
      translatingMap2dTo3d::ConstDownCast(
        aG->get(dtXmlParserBase::getAttributeStr("part_label", *tE))
      )
    );
    _tM2dTo3d.reset( tM2dTo3d->clone() );

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
    _msCut.reset( _tM2dTo3d->constRefMap2dTo3d().clone() );
    dt__forAllIndex(ms, ii) {
      dt__forAllIndex(ms[ii], jj) {
        float uPercent = static_cast<float>(ii)/(_nW-1);
        float vPercent = static_cast<float>(jj)/(_nW-1);
        ms[ii][jj] 
        = 
        dtPoint2(
          m_uSVS(
            _msCut->u_percent(uPercent), 
            _msCut->v_percent(vPercent)
          ), 
          s_uSVS( 
            _msCut->u_percent(uPercent), 
            _msCut->v_percent(vPercent)          
          )
        );
      }
    }
    _ms_uSPercentVSPercent.reset( new vec2dMultiBiLinearTwoD(ms) );
    
    dt__info(
      init(),
      << dt__eval(_msCut->getUMin()) << std::endl
      << dt__eval(_msCut->getUMax()) << std::endl
      << dt__eval(_msCut->getVMin()) << std::endl
      << dt__eval(_msCut->getVMax()) << std::endl
      << dt__point2d( _ms_uSPercentVSPercent->YdtPoint2Percent(0.,0.) ) 
      << std::endl
      << dt__point2d( _ms_uSPercentVSPercent->YdtPoint2Percent(1.,0.) ) 
      << std::endl
      << dt__point2d( _ms_uSPercentVSPercent->YdtPoint2Percent(1.,1.) ) 
      << std::endl
      << dt__point2d( _ms_uSPercentVSPercent->YdtPoint2Percent(0.,1.) )
    );
  }
  
	float uVw_deltaMs::m_uSVS(float const & uu, float const & vv) const {
		ptrHandling< map1dTo3d > m1d( 
      _msCut->segmentConstV(vv) 
    );
		return m1d->l_u(uu);		
	}
  
	float uVw_deltaMs::s_uSVS(float const & uu, float const & vv) const {
		ptrHandling< map1dTo3d > m1d( 
      _msCut->segmentConstU(uu) 
    );
		
		return m1d->lPercent_u(vv);		
	} 
  
	float uVw_deltaMs::uV_deltaVVWV(
    float const & delta, float const & vv, float const & ww
  ) const {
		return _tM2dTo3d->u_percent( 
      delta/dtLinearAlgebra::length( _tM2dTo3d->translationAxis() ) 
    );
	}
	
	float uVw_deltaMs::vV_ms(float const & mm, float const & ss) const {
    aFY ms = analyticFunction::aFYTwoD(mm, ss);
    aFX uvPercent = _ms_uSPercentVSPercent->invY(ms);
    
    return _msCut->u_percent(uvPercent[0]);
	}	
	
	float uVw_deltaMs::wV_ms(float const & mm, float const & ss) const {
    aFY ms = analyticFunction::aFYTwoD(mm, ss);
    aFX uvPercent = _ms_uSPercentVSPercent->invY(ms);
    
    return _msCut->v_percent(uvPercent[1]);    
	}    

  float uVw_deltaMs::delta_uVvVwV(
    float const & uu, float const & vv, float const & ww
  ) const {
		return 
      _tM2dTo3d->percent_u(uu) 
      * 
      dtLinearAlgebra::length( _tM2dTo3d->translationAxis() );
  }
}