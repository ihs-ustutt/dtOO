#include "uVw_skewPhirMs.h"

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
#include <analyticFunctionHeaven/scaOneD.h>
#include <analyticFunctionHeaven/aFBuilder/float_scaOneDPoint.h>
#include <analyticFunctionHeaven/vec3dMultiThreeD.h>
#include <analyticFunctionHeaven/vec3dTriLinearThreeD.h>

namespace dtOO {
  uVw_skewPhirMs::uVw_skewPhirMs() : dtTransformer() {
    _ss = dtVector3(1.,1.,1.);
    _nU = 11;
    _nV = 11;
    _nW = 11;    
    _relaxM = 0.50;    
    _relaxPhir = 0.50;     
    _nSteps = 10;    
    _angle = 2.*M_PI;
  }

  uVw_skewPhirMs::uVw_skewPhirMs( uVw_skewPhirMs const & orig ) 
    : dtTransformer(orig) {
		_rM2d.reset( orig._rM2d->clone() );
		_ss = orig._ss;
    _phirMS_uvw.reset( orig._phirMS_uvw->clone() );
    _nU = orig._nU;
    _nV = orig._nV;
    _nW = orig._nW;
    if( !orig._fU.isNull() ) _fU.reset( orig._fU->clone() );
    if( !orig._fV.isNull() ) _fV.reset( orig._fV->clone() );
    _relaxM = orig._relaxM;
    _relaxPhir = orig._relaxPhir;
    _nSteps = orig._nSteps;
    _angle = orig._angle;
  }
	
  uVw_skewPhirMs::~uVw_skewPhirMs() {
		
  }

  dtTransformer * uVw_skewPhirMs::clone( void ) const {
	  return new uVw_skewPhirMs(*this);
	}
	
  dtTransformer * uVw_skewPhirMs::create( void ) const {
		return new uVw_skewPhirMs();
	}
	
  aFPtrVec uVw_skewPhirMs::apply( 
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

	std::vector< dtPoint3 > uVw_skewPhirMs::apply( 
    std::vector< dtPoint3 > const * const toTrans 
  ) const {
		std::vector< dtPoint3 > retVec;
		dt__forAllIndex(*toTrans, ii) {
      float phir = toTrans->at(ii).x() * _ss.x();
      float correctPhir = 1.;
      if ( phir < 0. ) {
        phir = -1. * phir;
        correctPhir = -1.;
      }
		  float mm = std::max( toTrans->at(ii).y() * _ss.y(), 0.);
		  float ss = toTrans->at(ii).z() * _ss.z();
      dt__solution(ss>1., ss=1.);
      dt__solution(ss<0., ss=0.);
      
      aFX uvw = _phirMS_uvw->invY( analyticFunction::aFXThreeD(phir, mm, ss) );
			
			retVec.push_back( dtPoint3(correctPhir * uvw[0], uvw[1], uvw[2]) );
      
		}
		
		return retVec;
	}
	
	std::vector< dtPoint3 > uVw_skewPhirMs::retract( 
    std::vector< dtPoint3 > const * const toRetract 
  ) const {
		std::vector< dtPoint3 > retVec;
		dt__forAllIndex(*toRetract, ii) {
      float uu = toRetract->at(ii).x() / _ss.x();
      float correctPhir = 1.;
      if ( uu > 0.5 ) {
        uu = 1.-uu;
        correctPhir = -1.;
      }      
		  float vv = toRetract->at(ii).y() / _ss.y();
		  float ww = toRetract->at(ii).z() / _ss.z();
		
      aFY phirms = _phirMS_uvw->Y(uu, vv, ww);
      float phir = correctPhir * phirms[0];        
      float mm = phirms[1];        
      float ss = phirms[2];
						
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
  
  bool uVw_skewPhirMs::isNecessary( void ) const {
    return true;
  }

  void uVw_skewPhirMs::init( 
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

		if ( dtXmlParserBase::hasAttribute("number_points_one", *tE) ) {
      _nU
      = 
      dtXmlParserBase::getAttributeIntMuParse(
        "number_points_one", *tE, cV, aF
      );
    } 
		if ( dtXmlParserBase::hasAttribute("function_percent_one", *tE) ) {
      _fU.reset(
        scaOneD::MustDownCast(
          aF->get(
            dtXmlParserBase::getAttributeStr(
              "function_percent_one", *tE
            )
          )
        )->clone()
      );
    }     
		if ( dtXmlParserBase::hasAttribute("function_percent_two", *tE) ) {
      _fV.reset(
        scaOneD::MustDownCast(
          aF->get(
            dtXmlParserBase::getAttributeStr(
              "function_percent_two", *tE
            )
          )
        )->clone()
      );
    }     
        
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
    if ( dtXmlParserBase::hasAttribute("relaxM", *tE) ) {
      _relaxM
      = 
      dtXmlParserBase::getAttributeFloatMuParse(
        "relaxM", *tE, cV, aF
      );
    }  
    if ( dtXmlParserBase::hasAttribute("relaxPhir", *tE) ) {
      _relaxPhir
      = 
      dtXmlParserBase::getAttributeFloatMuParse(
        "relaxPhir", *tE, cV, aF
      );
    } 
    if ( dtXmlParserBase::hasAttribute("nSteps", *tE) ) {
      _nSteps
      = 
      dtXmlParserBase::getAttributeIntMuParse(
        "nSteps", *tE, cV, aF
      );
    }     
    if ( dtXmlParserBase::hasAttribute("angle", *tE) ) {
      _angle
      = 
      dtXmlParserBase::getAttributeFloatMuParse("angle", *tE, cV, aF);
    } 
    
    std::vector< std::vector< std::vector< dtPoint3 > > > phirMS(
      _nU, 
      std::vector< std::vector< dtPoint3 > >(
        _nV, std::vector< dtPoint3 >(_nW)
      )
    );    

    std::vector< float > uPercent;
    std::vector< float > vPercent;
    if ( !_fU.isNull() ) {
      uPercent = float_scaOneDPoint(_fU.get(), _nU).result();
    }
    else {
      uPercent = dtLinearAlgebra::unitGrid(_nU);        
    }
    uPercent = dtLinearAlgebra::multiply( _angle/(2.*M_PI), uPercent );
    
    if ( !_fV.isNull() ) {
      vPercent = float_scaOneDPoint(_fV.get(), _nV).result();
    }
    else {
      vPercent = dtLinearAlgebra::unitGrid(_nV);        
    }          
    std::vector< float > wPercent = dtLinearAlgebra::unitGrid(_nW);            
    dt__forFromToIndex(0, _nW, kk) {
      //
      // log container
      //
      logContainer<uVw_skewPhirMs> dt__infoContainer(logC);
      logC() 
        << logMe::dtFormat("%12s | %12s | %12s") 
          % "" % "|| isM ||" % "|| isPhir ||" << std::endl
        << logMe::dtFormat("%12s | %12s | %12s") 
          % "step" % "------------" % "------------" << std::endl        
        << logMe::dtFormat("%12s | %12s | %12s") 
          % "" % "size(isM)" % "size(isPhir)" << std::endl;

//      //
//      // calculate w coordinate
//      //
//      float wPercent = static_cast<float>(kk)/(_nW-1);
      
      //
      // create twoDArraHandlings
      //
      twoDArrayHandling< dtVector2 > phirM_0(_nU, _nV);
      twoDArrayHandling< dtVector2 > phirM_1(_nU, _nV);
      std::vector< float > matchM(_nU, 0);
      std::vector< float > matchPhir(_nV, 0);
      twoDArrayHandling< dtVector2 > sumPhirM( _nU, _nV, dtVector2(0.,0.) );      

      //
      // init twoDArray
      //
      dt__forAllIndex(phirM_1, ii) {
        dt__forAllIndex(phirM_1[ii], jj) {
          dtVector3 vXYZ 
          = 
          _rM2d->origin() 
          - 
          _rM2d->constRefMap2dTo3d().getPointPercent(vPercent[jj], wPercent[kk]);
          dtVector3 pointOnRotAx 
          = 
          _rM2d->rotationAxis() 
          * 
          dtLinearAlgebra::dotProduct(_rM2d->rotationAxis(), vXYZ);
          dtVector3 rr = vXYZ - pointOnRotAx;
          float mm 
          = 
          m_uSVS(
            _rM2d->constRefMap2dTo3d().u_percent(vPercent[jj]), 
            _rM2d->constRefMap2dTo3d().v_percent(wPercent[kk])
          );
          phirM_0[ii][jj] 
          = 
          dtVector2( 
            uPercent[ii]*2.*M_PI*dtLinearAlgebra::length(rr),
            mm
          );
          phirM_1[ii][jj] = phirM_0[ii][jj];
        }
      }
      
      //
      // calculate values to match
      //
      dt__forFromToIndex(0, phirM_1.size(), ii) {
        dt__forFromToIndex(1, phirM_1[ii].size(), jj) {
          matchM[ii] 
          = 
          matchM[ii] + (phirM_0[ii][jj].y() - phirM_0[ii][jj-1].y());
        }
      }
      dt__forFromToIndex(1, phirM_1.size(), ii) {
        dt__forFromToIndex(0, phirM_1[ii].size(), jj) {
          matchPhir[jj] 
          = 
          matchPhir[jj] + (phirM_0[ii][jj].x() - phirM_0[ii-1][jj].x());
        }
      }


      dt__forFromToIndex(0, _nSteps, thisStep) {
//        //
//        // write step to file
//        //
//        std::fstream of;
//        of.open( 
//          getLabel()
//            +"_"
//            +stringPrimitive::intToString(thisStep)
//            +"_phirM_"
//            +stringPrimitive::floatToString(wPercent)
//            +".csv",
//          std::ios::out | std::ios::trunc 
//        );
//        of.precision(8);
//        of.fixed;
//        of << "phirM, m, w" << std::endl;
//        dt__forAllIndex(phirM_1, ii) {
//          dt__forAllIndex(phirM_1[ii], jj) {
//            of 
//              << phirM_1[ii][jj].x() << ", " 
//              << phirM_1[ii][jj].y() << ", " 
//              << wPercent 
//              << std::endl;
//          }
//          of << std::endl << std::endl;
//        }
//        dt__forAllIndex(phirM_1[0], jj) {
//          dt__forAllIndex(phirM_1, ii) {
//            of 
//              << phirM_1[ii][jj].x() << ", " 
//              << phirM_1[ii][jj].y() << ", " 
//              << wPercent 
//              << std::endl;
//          }
//          of << std::endl << std::endl;
//        }      
//        of.close();

        //
        // perform iteration
        //
        dt__forFromToIndex(1, phirM_1.size(), ii) {
          dt__forFromToIndex(1, phirM_1[ii].size(), jj) {
            float matchDeltaPhir = phirM_0[ii][jj].x() - phirM_0[ii-1][jj].x();
            float matchDeltaM = phirM_0[ii][jj].y() - phirM_0[ii][jj-1].y();

            //
            // first order
            //
            dtVector2 deltaPhir 
            = 
            dtVector2(
              phirM_1[ii][jj][0] - phirM_1[ii-1][jj][0],
              phirM_1[ii][jj][1] - phirM_1[ii-1][jj][1]
            );
            dtVector2 deltaM 
            = 
            dtVector2(
              phirM_1[ii][jj][0] - phirM_1[ii][jj-1][0], 
              phirM_1[ii][jj][1] - phirM_1[ii][jj-1][1]
            );

            float isDeltaPhir = dtLinearAlgebra::length( deltaPhir );
            float isDeltaM = dtLinearAlgebra::length( deltaM );
            
            //
            // calculate new values
            //
            dtVector2 phirM_phir_n
            =
            phirM_1[ii-1][jj] + (matchDeltaPhir/isDeltaPhir) * deltaPhir;
            dtVector2 phirM_m_n
            =
            phirM_1[ii][jj-1] + (matchDeltaM/isDeltaM) * deltaM;

            //
            // relax between new and old values
            //
            phirM_1[ii][jj] 
            = 
            dtVector2(
              (
                (1.-_relaxM-_relaxPhir) * phirM_1[ii][jj].x() 
                + 
                _relaxPhir * phirM_phir_n.x() + _relaxM * phirM_m_n.x()
              ),
              (
                (1.-_relaxM-_relaxPhir) * phirM_1[ii][jj].y() 
                + 
                _relaxPhir * phirM_phir_n.y() + _relaxM * phirM_m_n.y()
              )
            );

          }
        }
        
        //
        // calculate sum
        //
        sumPhirM = twoDArrayHandling< dtVector2 >( _nU, _nV, dtVector2(0.,0.) );
        dt__forAllIndex(phirM_1, ii) {
          dt__forAllIndex(phirM_1[ii], jj) {
            if (ii == 0) {
              sumPhirM[ii][jj] 
              =
              dtVector2(
                0.,
                sumPhirM[ii][jj-1].y() 
                + 
                dtLinearAlgebra::length( phirM_1[ii][jj]  - phirM_1[ii][jj-1] )
              );
            }
            else if (jj == 0) {
              sumPhirM[ii][jj] 
              =
              dtVector2(
                sumPhirM[ii-1][jj].x() 
                + 
                dtLinearAlgebra::length( phirM_1[ii][jj]  - phirM_1[ii-1][jj] ),
                0.
              );
            }
            else {
              sumPhirM[ii][jj] 
              =
              dtVector2(
                sumPhirM[ii-1][jj].x() 
                +               
                dtLinearAlgebra::length( phirM_1[ii][jj]  - phirM_1[ii-1][jj] ),
                sumPhirM[ii][jj-1].y() 
                + 
                dtLinearAlgebra::length( phirM_1[ii][jj]  - phirM_1[ii][jj-1] )
              );
            }
          }
        }

        //
        // calculate current values
        //
        std::vector< float > isM(_nU, 0);
        std::vector< float > isPhir(_nV, 0);      
        dt__forFromToIndex(0, phirM_1.size(), ii) {
          dt__forFromToIndex(1, phirM_1[ii].size(), jj) {
            isM[ii] 
            = 
            isM[ii] + dtLinearAlgebra::length(phirM_1[ii][jj] - phirM_1[ii][jj-1]);
          }
        }
        dt__forFromToIndex(1, phirM_1.size(), ii) {
          dt__forFromToIndex(0, phirM_1[ii].size(), jj) {
            isPhir[jj] 
            = 
            isPhir[jj] + dtLinearAlgebra::length(phirM_1[ii][jj] - phirM_1[ii-1][jj]);
          }
        }
        
        //
        // make relative
        //
        dt__forAllIndex(isM, ii) isM[ii] = isM[ii] / matchM[ii];
        dt__forAllIndex(isPhir, ii) isPhir[ii] = isPhir[ii] / matchPhir[ii];
        
        //
        // output to container
        //
        logC() 
          << logMe::dtFormat("%12i | %12.5f | %12.5f") 
            % thisStep 
            % ( dtLinearAlgebra::euclidianNorm(isM)/isM.size() )
            % ( dtLinearAlgebra::euclidianNorm(isPhir)/isPhir.size() )
          << std::endl;        
      }

      //
      // write to file
      //
      std::fstream of;
      of.open( 
        getLabel()
          +"_phirM_"
          +stringPrimitive::floatToString(wPercent[kk])
          +".csv",
        std::ios::out | std::ios::trunc 
      );
      of.precision(8);
      of.fixed;
      of << "phirM, m, w" << std::endl;
      dt__forAllIndex(phirM_1, ii) {
        dt__forAllIndex(phirM_1[ii], jj) {
          of 
            << phirM_1[ii][jj].x() << ", " 
            << phirM_1[ii][jj].y() << ", " 
            << wPercent[kk]
            << std::endl;
        }
        of << std::endl << std::endl;
      }
      dt__forAllIndex(phirM_1[0], jj) {
        dt__forAllIndex(phirM_1, ii) {
          of 
            << phirM_1[ii][jj].x() << ", " 
            << phirM_1[ii][jj].y() << ", " 
            << wPercent[kk] 
            << std::endl;
        }
        of << std::endl << std::endl;
      }      
      of.close();

      //
      // enforce length
      //
      dt__forAllIndex(sumPhirM, ii) {
        dt__forAllIndex(sumPhirM[ii], jj) {
          sumPhirM[ii][jj] 
          = 
          dtVector2(
            (matchPhir[jj] / sumPhirM.back()[jj].x()) * sumPhirM[ii][jj].x(),
            (matchM[ii] / sumPhirM[ii].back().y()) * sumPhirM[ii][jj].y() 
          );
          phirMS[ii][jj][kk] 
          = 
          dtPoint3(
            sumPhirM[ii][jj].x(), sumPhirM[ii][jj].y(), wPercent[kk]
          );
        }
      }            
      
      //
      // write mapping
      //
      dt__forAllIndex(sumPhirM, ii) {      
        std::fstream of;
        of.open( 
          getLabel()
          +"_const_phir_"+stringPrimitive::intToString(ii)+"_"
          +stringPrimitive::intToString(kk)
          +".csv",
          std::ios::out | std::ios::trunc 
        );
        of.precision(8);
        of.fixed;
        dt__forAllRefAuto(sumPhirM.fixI(ii), anEl) {
          of << anEl.x() << ", " << anEl.y() << ", " << wPercent[kk] << std::endl;
        }
        of.close();
      }
      dt__forAllIndex(sumPhirM[0], jj) {
        std::fstream of;
        of.open( 
          getLabel()
          +"_const_m_"+stringPrimitive::intToString(jj)+"_"
          +stringPrimitive::intToString(kk)
          +".csv",
          std::ios::out | std::ios::trunc 
        );
        of.precision(8);
        of.fixed;
        dt__forAllRefAuto(sumPhirM.fixJ(jj), anEl) {
          of << anEl.x() << ", " << anEl.y() << ", " << wPercent[kk] << std::endl;
        }
        of.close();
      }    
    } 
    
    _phirMS_uvw.reset(  new vec3dMultiThreeD< vec3dTriLinearThreeD >() );
    vec3dMultiThreeD< vec3dTriLinearThreeD > & theMap 
    = 
    *vec3dMultiThreeD< vec3dTriLinearThreeD >::SecureCast( _phirMS_uvw.get() );
    
    dt__forFromToIndex(1, phirMS.size(), ii) {
      dt__forFromToIndex(1, phirMS[ii].size(), jj) {
        dt__forFromToIndex(1, phirMS[ii][jj].size(), kk) {
          theMap.add(
            new vec3dTriLinearThreeD(
              phirMS[ii-1][jj-1][kk-1], phirMS[ii-0][jj-1][kk-1],
              phirMS[ii-1][jj-0][kk-1], phirMS[ii-0][jj-0][kk-1],
              phirMS[ii-1][jj-1][kk-0], phirMS[ii-0][jj-1][kk-0],
              phirMS[ii-1][jj-0][kk-0], phirMS[ii-0][jj-0][kk-0],
              dtPoint3(uPercent[ii-1], vPercent[jj-1], wPercent[kk-1]),
              dtPoint3(uPercent[ii], vPercent[jj], wPercent[kk])
            )
          );
        }
      }
    }
    
//    //
//    // dump function
//    //
//    theMap.dump();   
  }
  
	float uVw_skewPhirMs::m_uSVS(float const & uu, float const & vv) const {
		ptrHandling< map1dTo3d > m1d( 
      _rM2d->constRefMap2dTo3d().segmentConstV(vv) 
    );
		return m1d->l_u(uu);
	}
}