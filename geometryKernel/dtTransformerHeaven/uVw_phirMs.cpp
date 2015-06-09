#include "uVw_phirMs.h"
#include "interfaceHeaven/systemHandling.h"
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/rotatingMap2dTo3d.h>
#include <analyticFunctionHeaven/vec2dMultiBiLinearTwoD.h>
#include <logMe/logMe.h>
#include <progHelper.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <analyticFunctionHeaven/vec3dThickedTwoD.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <analyticFunctionHeaven/analyticFunctionTransformed.h>

namespace dtOO {
  uVw_phirMs::uVw_phirMs() : dtTransformer() {
  }

  uVw_phirMs::uVw_phirMs(
	  uVw_phirMs const & orig 
	) : dtTransformer(orig) {
		_rM2d.reset( orig._rM2d->clone() );
		_ss = orig._ss;
    _ms_uSPercentVSPercent.reset( orig._ms_uSPercentVSPercent->clone() );
    _nU = orig._nU;
    _nV = orig._nV;
  }
	
  uVw_phirMs::~uVw_phirMs() {
		
  }

  dtTransformer * uVw_phirMs::clone( void ) const {
	  return new uVw_phirMs(*this);
	}
	
  dtTransformer * uVw_phirMs::create( void ) const {
		return new uVw_phirMs();
	}
	
	/**
	 * @todo ConstDownCast to current types is not a satisfying solution. Maybe
	 * "transformed cloning" is a better solution?!
   */
  vectorHandling< analyticFunction * > uVw_phirMs::apply( 
	  vectorHandling< analyticFunction * > const * const aFP 
	) const {
		vectorHandling< analyticFunction * > retV;
		
    for (int ii=0; ii<aFP->size(); ii++) {
			analyticFunction * aF = aFP->at(ii);

			vec3dCurveOneD const * const v1d = vec3dCurveOneD::ConstDownCast(aF);			
			vec3dSurfaceTwoD const * const v2d = vec3dSurfaceTwoD::ConstDownCast(aF);
			vec3dThickedTwoD const * const vT2d = vec3dThickedTwoD::ConstDownCast(aF);
			
			if (v1d) {			
				analyticFunctionTransformed<vec3dCurveOneD> * aFT
				= new analyticFunctionTransformed<vec3dCurveOneD>(*v1d);
				aFT->setTransformer(this);
				retV.push_back( aFT );
				retV.back()->setLabel(aF->getLabel());
			}			
      else if (v2d) {			
				analyticFunctionTransformed<vec3dSurfaceTwoD> * aFT
				= new analyticFunctionTransformed<vec3dSurfaceTwoD>(*v2d);
				aFT->setTransformer(this);
				retV.push_back( aFT );
				retV.back()->setLabel(aF->getLabel());				
			}
      else if (vT2d) {			
				analyticFunctionTransformed<vec3dThickedTwoD> * aFT
				= new analyticFunctionTransformed<vec3dThickedTwoD>(*vT2d);
				aFT->setTransformer(this);
				retV.push_back( aFT );
				retV.back()->setLabel(aF->getLabel());	
			}			
			else {
				dt__throw(
					apply(),
					<< dt__eval(v1d) << std::endl
					<< dt__eval(v2d) << std::endl
					<< dt__eval(vT2d) << std::endl								
					<< "Unknown type."
				);
			}
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
//      dt__warnIfWithMessageAndSolution(ss>1., ss=1., apply(), << dt__eval(ss));
//      dt__warnIfWithMessageAndSolution(ss<0., ss=0., apply(), << dt__eval(ss));
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
    QDomElement const * tE, 
    baseContainer const * const bC,
    vectorHandling< constValue * > const * const cV,
    vectorHandling< analyticFunction * > const * const aF,
    vectorHandling< analyticGeometry * > const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);
		
		if ( dtXmlParserBase::hasChild("Vector_3", *tE) ) {
			QDomElement v3El = dtXmlParserBase::getChild("Vector_3", *tE);
			handleDtVector3(
				"Vector_3", 
				dtXmlParserBase::getDtVector3(&v3El, bC, cV, aF, aG)
			);
		}
		else {
      handleDtVector3("Vector_3", dtVector3(1.,1.,1.));			
		}
		handleAnalyticGeometry(
			"part_label", 
			aG->get(dtXmlParserBase::getAttributeStr("part_label", *tE))
		);
    
		if ( dtXmlParserBase::hasAttribute("number_points_two", *tE) ) {
      handleInt(
        "number_points_two", 
        dtXmlParserBase::getAttributeIntMuParse(
          "number_points_two", *tE, cV, aF
        )
  		);
    } 
    else handleInt("number_points_two", 11);

		if ( dtXmlParserBase::hasAttribute("number_points_three", *tE) ) {
      handleInt(
        "number_points_three", 
        dtXmlParserBase::getAttributeIntMuParse(
          "number_points_three", *tE, cV, aF
        )
  		);
    } 
    else handleInt("number_points_three", 11);
    
    //
    // create piecewise bilinear mapping
    //
    twoDArrayHandling< dtPoint2 > ms(_nU, _nV);
    dt__forAllIndex(ms, ii) {
      dt__forAllIndex(ms[ii], jj) {
        float uPercent = static_cast<float>(ii)/(_nU-1);
        float vPercent = static_cast<float>(jj)/(_nV-1);
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
//    //
//    // write csv of mapping
//    //
//    std::fstream of;
//    of.open("ms.csv", std::ios::out | std::ios::trunc);
//    of.precision(8);
//    of.fixed;
//    of << "x_coord,y_coord,z_coord,u_p,v_p,m,s,m_p,s_p" << std::endl;
//    dt__forAllIndex(ms, ii) {
//      dt__forAllIndex(ms[ii], jj) {
//        float uPercent = static_cast<float>(ii)/(_nU-1);
//        float vPercent = static_cast<float>(jj)/(_nV-1);
//
//        dtPoint3 pXYZ 
//        = 
//        _rM2d->constRefMap2dTo3d().getPointPercent(uPercent, vPercent);
//        
//        of 
//        << pXYZ.x() << ", " << pXYZ.y() << ", " << pXYZ.z() << ", "
//        << uPercent << ", " << vPercent << ", "
//        << ms[ii][jj].x() << ", " << ms[ii][jj].y() << ", "
//        << ms[ii][jj].x()/ms.back()[jj].x() << ", " 
//        << ms[ii][jj].y()/ms[ii].back().y()
//        << std::endl;
//      }
//    }
//    of.close();
  }
  
  void uVw_phirMs::handleAnalyticGeometry(
    std::string const name, analyticGeometry const * value
  ) {
    if (name == "part_label") {
      dt__ptrAss(
        rotatingMap2dTo3d const * const m3d, 
        rotatingMap2dTo3d::ConstDownCast(value)
      );
			_rM2d.reset( m3d->clone() );
      return;
    }
    dtTransformer::handleAnalyticGeometry(name, value);
  }
	
  void uVw_phirMs::handleDtVector3(
    std::string const name, dtVector3 const value
  ) {
    if (name == "Vector_3") {
      _ss = value;
      return;
    }
    dtTransformer::handleDtVector3(name, value);
  }	
  
  void uVw_phirMs::handleInt(std::string const name, int const value) {
    dt__info(handleInt(),
      << dt__eval(name) << std::endl
      << dt__eval(value) 
    );
    if (name == "number_points_two") {
      _nU = value;
      return;
    }
    else if (name == "number_points_three") {
      _nV = value;
      return;
    }
    dtTransformer::handleInt(name, value);
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
		// get radius
		//
    dtVector3 vXYZ 
    = 
    dtLinearAlgebra::toDtVector3(
      _rM2d->constRefMap2dTo3d().getPoint(vv, ww)
    );
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
    dtVector3 vXYZ 
    = 
    dtLinearAlgebra::toDtVector3(
      _rM2d->constRefMap2dTo3d().getPoint(vv, ww)
    );
    dtVector3 pointOnRotAx 
    = 
    _rM2d->rotationAxis() 
    * 
    dtLinearAlgebra::dotProduct(_rM2d->rotationAxis(), vXYZ);
    dtVector3 rr = vXYZ - pointOnRotAx;
		
		
		return uu*dtLinearAlgebra::length(rr) * (2.*M_PI);
  }
  
  float uVw_phirMs::m_vVs(float const & vv, float const & ss) const {
    aFX xx 
    = 
    analyticFunction::aFXTwoD( _rM2d->constRefMap2dTo3d().u_percent(vv), ss );
    aFY yy = _ms_uSPercentVSPercent->Y(xx);
    
    return _rM2d->constRefMap2dTo3d().percent_u(yy[0]);    
  }
}