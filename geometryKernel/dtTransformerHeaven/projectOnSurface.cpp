#include "projectOnSurface.h"
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <progHelper.h>
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
	  dt__THROW(clone(), "Not yet implemented.");
	}
	
  dtTransformer * projectOnSurface::create( void ) const {
		return new projectOnSurface();
	}
	
  vectorHandling< analyticGeometry * > projectOnSurface::apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const {
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
				<< "Projecting " << DTLOGEVAL(constSc->getLabel()) << " on " 
				<<  DTLOGEVAL(_aS->getLabel()) << "."
			);
			
      //
      // push translated geometry in vector
      //      
      retAGeo.push_back( 
				new splineCurve3d(
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