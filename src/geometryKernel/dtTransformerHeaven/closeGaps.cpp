#include "closeGaps.h"
#include <analyticGeometryHeaven/analyticSurface.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <geometryEngine/dtSurface.h>

namespace dtOO {
  closeGaps::closeGaps() : dtTransformer() {
  }

  closeGaps::~closeGaps() {
  }

  dtTransformer * closeGaps::clone( void ) const {
	  dt__throw(clone(), << "Not yet implemented.");
	}
	
  dtTransformer * closeGaps::create( void ) const {
		return new closeGaps();
	}
	
  vectorHandling< analyticGeometry * > closeGaps::apply( 
    vectorHandling< analyticGeometry * > const * const toTrans 
  ) const {
    vectorHandling< analyticGeometry * > retAGeo;

    dt__forAllRefAuto( *toTrans, aTrans ) {
      //
      // clone and cast
      //
      dt__ptrAss(
        analyticSurface * aS, analyticSurface::DownCast( aTrans->clone() )
      );

      if ( !_vvStartAGeo.isNull() ) {
        for (int ii = 0;ii<aS->ptrDtSurface()->nControlPointsU();ii++) {
          dtPoint3 cP = aS->ptrDtSurface()->controlPoint(ii, 0);
          dtPoint2 nearest = _vvStartAGeo->ptrDtSurface()->reparam( cP );
          dtPoint3 cPNearest = _vvStartAGeo->getPoint(
                                nearest.x(), 
                                nearest.y()
                              );
          aS->ptrDtSurface()->setControlPoint(ii, 0, cPNearest);
        }
      }
      if ( !_vvEndAGeo.isNull() ) {
        int nV = aS->ptrDtSurface()->nControlPointsV();
        for (int ii = 0;ii<aS->ptrDtSurface()->nControlPointsU();ii++) {        
          dtPoint3 cP = aS->ptrDtSurface()->controlPoint(ii, nV-1);
          dtPoint2 nearest = _vvEndAGeo->ptrDtSurface()->reparam( cP );
          dtPoint3 cPNearest = _vvEndAGeo->getPoint(
                                nearest.x(), 
                                nearest.y()
                              );
          aS->ptrDtSurface()->setControlPoint(ii, nV-1, cPNearest); 
        }
      }

      //
      // push translated geometry in vector
      //      
      retAGeo.push_back( aS );
    }
    return retAGeo;
  }  

  bool closeGaps::isNecessary( void ) const {
    return true;
  }

  void closeGaps::init( 
	  ::QDomElement const * tE, 
    baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);

    dt__throwIf(
      !dtXmlParserBase::hasAttribute("parameter_two_start_part_label", *tE) 
      || 
      !dtXmlParserBase::hasAttribute("parameter_two_end_part_label", *tE),
      init()
    );
      
    _vvStartAGeo.reset(
      analyticSurface::DownCast(
        aG->get(
          dtXmlParserBase::getAttributeStr(
            "parameter_two_start_part_label", *tE
          )
        )->clone()
      )
    );
    _vvEndAGeo.reset(
      analyticSurface::DownCast(
        aG->get(
          dtXmlParserBase::getAttributeStr(
            "parameter_two_end_part_label", *tE
          )
        )->clone()
      )
    );  
    dt__throwIf( _vvStartAGeo.isNull() || _vvEndAGeo.isNull(), init() );
  }
}