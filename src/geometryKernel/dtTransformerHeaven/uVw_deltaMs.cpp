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

#include "uVw_deltaMs.h"
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
    _ss = dtVector3(1,1,1);
    _nV = 11;
    _nW = 11;        
  }

  uVw_deltaMs::uVw_deltaMs( uVw_deltaMs const & orig ) : dtTransformer(orig) {
		_tM2d.reset( orig._tM2d->clone() );
    _msCut.reset( orig._msCut->clone() );
		_ss = orig._ss;
    _ms_uSPercentVSPercent.reset( orig._ms_uSPercentVSPercent->clone() );
    _nV = orig._nV;
    _nW = orig._nW;
  }

  uVw_deltaMs::uVw_deltaMs( 
    jsonPrimitive const & jE 
  ) : dtTransformer(jE){
    this->jInit(jE, NULL, NULL, NULL, NULL);
  }

  uVw_deltaMs::~uVw_deltaMs() {
		
  }

  dtTransformer * uVw_deltaMs::clone( void ) const {
	  return new uVw_deltaMs(*this);
	}
	
  dtTransformer * uVw_deltaMs::create( void ) const {
		return new uVw_deltaMs();
	}
	
  lvH_analyticFunction uVw_deltaMs::apply( 
    lvH_analyticFunction const * const aFP 
  ) const {
		lvH_analyticFunction retV;
		
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
      dtReal delta = toTrans->at(ii).x() * _ss.x();
		  dtReal mm = std::max( toTrans->at(ii).y() * _ss.y(), 0.);
		  dtReal ss = toTrans->at(ii).z() * _ss.z();
      dt__solution(ss>1., ss=1.);
      dt__solution(ss<0., ss=0.);
      
      dtReal wV = wV_ms(mm, ss);						
			dtReal vV = vV_ms(mm, ss);
			dtReal uV = uV_deltaVVWV(delta, vV, wV);
			
			retVec.push_back( dtPoint3(uV, vV, wV) );
		}
		
		return retVec;
	}
	
	std::vector< dtPoint3 > uVw_deltaMs::retract( 
    std::vector< dtPoint3 > const * const toRetract 
  ) const {
		std::vector< dtPoint3 > retVec;
		dt__forAllIndex(*toRetract, ii) {
      dtReal uu = toRetract->at(ii).x() / _ss.x();
		  dtReal vv = toRetract->at(ii).y() / _ss.y();
		  dtReal ww = toRetract->at(ii).z() / _ss.z();
		
			dtReal delta = delta_uVvVwV(uu, vv, ww);
      aFY ms
      =
      _ms_uSPercentVSPercent->Y(
        analyticFunction::aFXTwoD(
          _msCut->percent_u(vv), _msCut->percent_v(ww)
        )
      );
      dtReal mm = ms[0];        
      dtReal ss = ms[1];
						
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

  void uVw_deltaMs::jInit( 
    jsonPrimitive const & jE, 
    baseContainer * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG 
	) {
    dtTransformer::jInit(jE, bC, cV, aF, aG);
		    
    _ss = config().lookupDef< dtVector3 >("_ss", dtVector3(1,1,1));
    _tM2d
    =
    translatingMap2dTo3d::PointerDownCast(
      config().lookupClone< analyticGeometry >("_tM2d", aG)
    );

    int nV = config().lookupDef< int >("_nV", 11);
    int nW = config().lookupDef< int >("_nW", 11);
    
    //
    // create piecewise bilinear mapping
    //
    twoDArrayHandling< dtPoint2 > ms(_nW, _nW);
    _msCut.reset( _tM2d->constRefMap2dTo3d().clone() );
    dt__forAllIndex(ms, ii) {
      dt__forAllIndex(ms[ii], jj) {
        dtReal uPercent = static_cast<dtReal>(ii)/(_nW-1);
        dtReal vPercent = static_cast<dtReal>(jj)/(_nW-1);
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

  void uVw_deltaMs::init( 
    ::QDomElement const * tE, 
    baseContainer * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);
		    
    jsonPrimitive config;

		if ( dtXmlParserBase::hasChild("Vector_3", *tE) ) {
			::QDomElement v3El = dtXmlParserBase::getChild("Vector_3", *tE);
      config.append< dtVector3 >(
        "_ss",
        dtXmlParserBase::getDtVector3(&v3El, bC, cV, aF, aG)
      );
		}
    
    config.append<analyticGeometry const *>(
      "_tM2d",
      aG->get(dtXmlParserBase::getAttributeStr("part_label", *tE))
    );

		if ( dtXmlParserBase::hasAttribute("number_points_two", *tE) ) {
      config.append< int >(
        "_nV",
        dtXmlParserBase::getAttributeIntMuParse(
          "number_points_two", *tE, cV, aF
        )
      );
    } 

		if ( dtXmlParserBase::hasAttribute("number_points_three", *tE) ) {
       config.append< int >(
        "_nW",
        dtXmlParserBase::getAttributeIntMuParse(
          "number_points_three", *tE, cV, aF
        )
      );
    } 
    jInit(config, bC, cV, aF, aG);
  }
  
	dtReal uVw_deltaMs::m_uSVS(dtReal const & uu, dtReal const & vv) const {
		ptrHandling< map1dTo3d > m1d( 
      _msCut->segmentConstV(vv) 
    );
		return m1d->l_u(uu);		
	}
  
	dtReal uVw_deltaMs::s_uSVS(dtReal const & uu, dtReal const & vv) const {
		ptrHandling< map1dTo3d > m1d( 
      _msCut->segmentConstU(uu) 
    );
		
		return m1d->lPercent_u(vv);		
	} 
  
	dtReal uVw_deltaMs::uV_deltaVVWV(
    dtReal const & delta, dtReal const & vv, dtReal const & ww
  ) const {
		return _tM2d->u_percent( 
      delta/dtLinearAlgebra::length( _tM2d->translationAxis() ) 
    );
	}
	
	dtReal uVw_deltaMs::vV_ms(dtReal const & mm, dtReal const & ss) const {
    aFY ms = analyticFunction::aFYTwoD(mm, ss);
    aFX uvPercent = _ms_uSPercentVSPercent->invY(ms);
    
    return _msCut->u_percent(uvPercent[0]);
	}	
	
	dtReal uVw_deltaMs::wV_ms(dtReal const & mm, dtReal const & ss) const {
    aFY ms = analyticFunction::aFYTwoD(mm, ss);
    aFX uvPercent = _ms_uSPercentVSPercent->invY(ms);
    
    return _msCut->v_percent(uvPercent[1]);    
	}    

  dtReal uVw_deltaMs::delta_uVvVwV(
    dtReal const & uu, dtReal const & vv, dtReal const & ww
  ) const {
		return 
      _tM2d->percent_u(uu) 
      * 
      dtLinearAlgebra::length( _tM2d->translationAxis() );
  }
}
