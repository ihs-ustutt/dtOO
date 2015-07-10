#include "closeGaps.h"
#include <analyticGeometryHeaven/analyticSurface.h>
#include <progHelper.h>
#include <logMe/logMe.h>
#include <geometryEngine/dtSurface.h>

namespace dtOO {
  closeGaps::closeGaps() : dtTransformer() {
    _vvStartAGeo = NULL;
    _vvEndAGeo = NULL;
  }

  closeGaps::~closeGaps() {
  }

  dtTransformer * closeGaps::clone( void ) const {
	  dt__throw(clone(), << "Not yet implemented.");
	}
	
  dtTransformer * closeGaps::create( void ) const {
		return new closeGaps();
	}
	
	/**
	 * 
   * @todo: Rational check was removed.
   */
  vectorHandling< analyticGeometry * > closeGaps::apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const {
    vectorHandling< analyticGeometry * > retAGeo;

    //each analyticGeometry
    for (int ii=0;ii< aGeoVecP->size();ii++) {
      //
      // clone and cast analyticGeometry
      //
      analyticGeometry * aGeoP = aGeoVecP->at(ii)->clone();
      dt__ptrAss(analyticSurface * aS, analyticSurface::DownCast(aGeoP));

//      //
//      // check if it is rotational
//      //
//      if (aS->isRational()) {
//        dt__THROW(apply(),
//                << "Closing gaps of a rotational surface is not yet supported.");
//      }

      if (_vvStartAGeo) {
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
      if (_vvEndAGeo) {
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
      retAGeo.push_back( aGeoP );
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
		
    //
    // check input
    //
    bool hasVStart = dtXmlParserBase::hasAttribute("parameter_two_start_part_label", *tE);
    bool hasVEnd = dtXmlParserBase::hasAttribute("parameter_two_end_part_label", *tE);
    
    //
    // get pointers to parts
    //
    if (hasVStart && hasVEnd) {
      std::string vStartLabel = dtXmlParserBase::getAttributeStr(
                                  "parameter_two_start_part_label", 
                                  *tE
                                );
      std::string vEndLabel = dtXmlParserBase::getAttributeStr(
                                  "parameter_two_end_part_label", 
                                  *tE
                                );      
      for (int ii = 0; ii<aG->size();ii++) {
         if ( aG->at(ii)->getLabel() == vStartLabel ) {
           dt__ptrAss(_vvStartAGeo, analyticSurface::ConstDownCast(aG->at(ii)) );
         }
         if ( aG->at(ii)->getLabel() == vEndLabel ) {
           dt__ptrAss(_vvEndAGeo, analyticSurface::ConstDownCast(aG->at(ii)) );
         }
         if (_vvStartAGeo && _vvEndAGeo) {
           break;
         }
      }
    }
    else {
      dt__throw(init(),
              << dt__eval(hasVStart) << std::endl
              << dt__eval(hasVEnd) );
    }
  }
}