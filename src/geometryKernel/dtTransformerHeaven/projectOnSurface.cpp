#include "projectOnSurface.h"
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/geomCurve_projectOnGeomSurfaceOCC.h>

namespace dtOO {
  projectOnSurface::projectOnSurface() : dtTransformer() {
  }

  projectOnSurface::~projectOnSurface() {
  }

  dtTransformer * projectOnSurface::clone( void ) const {
	  dt__throw(clone(), << "Not yet implemented.");
	}
	
  dtTransformer * projectOnSurface::create( void ) const {
		return new projectOnSurface();
	}
	
  aGPtrVec projectOnSurface::apply( 
    aGPtrVec const * const toTrans 
  ) const {
    aGPtrVec retAGeo;
    for (int ii=0; ii<toTrans->size(); ii++) {
      //
      // clone and cast analyticGeometry
      //
      analyticGeometry const * aGeoP = toTrans->at(ii);
      analyticCurve const *  constSc;
      dt__ptrAss(constSc, analyticCurve::ConstDownCast(aGeoP));

			dt__info(
				apply(),
				<< "Projecting " << dt__eval(constSc->getLabel()) << " on " 
				<<  dt__eval(_aS->getLabel()) << "."
			);
			
      //
      // push translated geometry in vector
      //      
      retAGeo.push_back( 
				new analyticCurve(
					dt__tmpPtr(
			      dtCurve,
						geomCurve_projectOnGeomSurfaceOCC(
							constSc->ptrConstDtCurve(), _aS->ptrDtSurface()
						).result()
					)
				)
			);
			retAGeo.back()->setLabel(constSc->getLabel());
    }
    return retAGeo;
  }

  bool projectOnSurface::isNecessary( void ) const {
    return true;
  }

  void projectOnSurface::init( 
	  ::QDomElement const * tE, 
    baseContainer const * const bC,
		cVPtrVec const * const cV,
		aFPtrVec const * const aF,
		aGPtrVec const * const aG 
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
      dt__throw(
				init(), 
				<< dt__eval(dtXmlParserBase::hasAttribute( "part_label", *tE))
			);
    }
  }
}