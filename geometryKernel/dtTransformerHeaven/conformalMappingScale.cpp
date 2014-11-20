#include "conformalMappingScale.h"
#include <analyticGeometryHeaven/rotatingMap2dTo3d.h>
#include <logMe/logMe.h>
#include <progHelper.h>
#include <functionHeaven/analyticFunction.h>
#include <functionHeaven/vec3dSurfaceTwoD.h>
#include <functionHeaven/vec3dCurveOneD.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/dtCurve.h>

namespace dtOO {
  conformalMappingScale::conformalMappingScale() : dtTransformer() {
  }

  conformalMappingScale::conformalMappingScale(
	  conformalMappingScale const & orig 
	) : dtTransformer(orig) {
		_rM2d.reset( orig._rM2d->clone() );
  }
	
  conformalMappingScale::~conformalMappingScale() {
		
  }

  dtTransformer * conformalMappingScale::clone( void ) const {
	  return new conformalMappingScale(*this);
	}
	
  dtTransformer * conformalMappingScale::create( void ) const {
		return new conformalMappingScale();
	}
	
  vectorHandling< analyticFunction * > conformalMappingScale::apply( 
	  vectorHandling< analyticFunction * > const * const aFP 
	) const {
		vectorHandling< analyticFunction * > retV;
		
    for (int ii=0; ii<aFP->size(); ii++) {
			analyticFunction * aF = aFP->at(ii);

			vec3dCurveOneD const * const vec3d1d = vec3dCurveOneD::ConstDownCast(aF);			
			vec3dSurfaceTwoD const * const vec3d2d = vec3dSurfaceTwoD::ConstDownCast(aF);
			
			std::vector<float> itVal;
			std::vector<std::string> header;
			header.push_back("cP_x"); header.push_back("cP_y"); header.push_back("cP_z");
			header.push_back("uu"); header.push_back("vv"); header.push_back("ww");
			
			if (vec3d1d) {
				dtCurve * dtC = vec3d1d->ptrDtCurve()->clone();
				for (int jj=0; jj<dtC->getNControlPoints(); jj++) {
					dtPoint3 cP = dtC->getControlPoint3d(jj);
          itVal.push_back(cP.x());
					itVal.push_back(cP.y());
					itVal.push_back(cP.z());
					float ww = _rM2d->w_s( _rM2d->w_percent(cP.z()) );
					float vv = _rM2d->v_mw(cP.y(), ww);
					float uu = _rM2d->u_phirvw(cP.x(), vv, ww);
          itVal.push_back(uu);
					itVal.push_back(vv);
					itVal.push_back(ww);
					dtC->setControlPoint3d(jj, dtPoint3(uu, vv, ww));
				}
				retV.push_back( new vec3dCurveOneD(dtC) );
				retV.back()->setLabel(aF->getLabel());				
			}			
      else if (vec3d2d) {			
				dtSurface * dtS = vec3d2d->ptrDtSurface()->clone();
				for (int jj=0; jj<dtS->nControlPoints(0); jj++) {
					for (int kk=0; kk<dtS->nControlPoints(1); kk++) {
						dtPoint3 cP = dtS->controlPoint(jj, kk);
						itVal.push_back(cP.x());
						itVal.push_back(cP.y());
						itVal.push_back(cP.z());

						float ww = _rM2d->w_s( _rM2d->w_percent(cP.z()) );
						float vv = _rM2d->v_mw(cP.y(), ww);
						float uu = _rM2d->u_phirvw(cP.x(), vv, ww);
						itVal.push_back(uu);
						itVal.push_back(vv);
						itVal.push_back(ww);
						
						dtS->setControlPoint(jj, kk, dtPoint3(uu, vv, ww));
					}
				}
				retV.push_back( new vec3dSurfaceTwoD(dtS) );
				retV.back()->setLabel(aF->getLabel());
			}
			else {
				dt__THROW(
					apply(),
					<< DTLOGEVAL(vec3d1d) << LOGDEL
					<< DTLOGEVAL(vec3d2d) << LOGDEL
					<< "Unknown type."
				);
			}
			
			DTINFOWF(
				apply(),
				<< logMe::floatVecToTable(header, itVal)
			);
		}
		
		return retV;
  }

  bool conformalMappingScale::isNecessary( void ) const {
    return true;
  }

  void conformalMappingScale::init( 
	  QDomElement const * transformerElementP, 
    baseContainer * const bC,  
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const sFunP,
		vectorHandling< analyticGeometry * > const * const depAGeoP 
	) {
    dtTransformer::init(transformerElementP, bC, cValP, sFunP, depAGeoP);
		
		handleAnalyticGeometry(
			"part_label", 
			depAGeoP->get(getAttributeStr("part_label", *transformerElementP))
		);
  }
  
  void conformalMappingScale::handleAnalyticGeometry(std::string const name, analyticGeometry const * value) {
    if (name == "part_label") {
      dt__PTRASS(rotatingMap2dTo3d const * m3d, rotatingMap2dTo3d::ConstDownCast(value));
			_rM2d.reset( m3d->clone() );
      return;
    }
    dtTransformer::handleAnalyticGeometry(name, value);
  }
}