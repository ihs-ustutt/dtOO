#include "reparamInSurface.h"
#include "analyticGeometryHeaven/vec2dOneDInMap2dTo3d.h"
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <progHelper.h>
#include <logMe/logMe.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/geomCurve2d_reparamInGeomSurfaceOCC.h>

namespace dtOO {
  reparamInSurface::reparamInSurface() : dtTransformer() {
  }

  reparamInSurface::~reparamInSurface() {
  }

  dtTransformer * reparamInSurface::clone( void ) const {
	  dt__THROW(clone(), "Not yet implemented.");
	}
	
  dtTransformer * reparamInSurface::create( void ) const {
		return new reparamInSurface();
	}
	
  vectorHandling< analyticGeometry * > reparamInSurface::apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const {
    vectorHandling< analyticGeometry * > retAGeo;
    for (int ii=0; ii<aGeoVecP->size(); ii++) {
      //
      // clone and cast analyticGeometry
      //
      analyticGeometry const * aGeoP = aGeoVecP->at(ii);
      splineCurve3d const *  constSc;
      dt__ptrAss(constSc, splineCurve3d::ConstDownCast(aGeoP));

			DTINFOWF(
				apply(),
				<< "Reparam " << DTLOGEVAL(constSc->getLabel()) << " on " 
				<<  DTLOGEVAL(_aS->getLabel()) << "."
			);
			
      //
      // push translated geometry in vector
      //      
			dt__pH(dtCurve2d) dtC2d(
				geomCurve2d_reparamInGeomSurfaceOCC(
					constSc->ptrConstDtCurve(), _aS->ptrDtSurface()
				).result()
			);
			dt__pH(vec2dCurve2dOneD) v2d1d(new vec2dCurve2dOneD(dtC2d.get()) );
			retAGeo.push_back( new vec2dOneDInMap2dTo3d(v2d1d.get(), _aS.get()) );
			
			retAGeo.back()->setLabel(constSc->getLabel());
    }
    return retAGeo;
  }

  bool reparamInSurface::isNecessary( void ) const {
    return true;
  }

  void reparamInSurface::init( 
	  QDomElement const * tE, 
    baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);
    
    if (dtXmlParserBase::hasAttribute( "part_label", *tE) ) {
			_aS.reset(			
				analyticSurface::ConstDownCast(
					aG->get(dtXmlParserBase::getAttributeStr("part_label", *tE))->clone()
				)
			);
    }
    else {
      dt__THROW(
				init(), 
				<< DTLOGEVAL(dtXmlParserBase::hasAttribute( "part_label", *tE))
			);
    }
  }
}