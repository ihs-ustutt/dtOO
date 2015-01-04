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
	  dt__THROW(clone(), "Not yet implemented.");
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
      analyticSurface *  aS;
      dt__MUSTDOWNCAST(aGeoP, analyticSurface, aS);

//      //
//      // check if it is rotational
//      //
//      if (aS->isRational()) {
//        dt__THROW(apply(),
//                << "Closing gaps of a rotational surface is not yet supported.");
//      }

        DTBUFFERINIT();
      if (_vvStartAGeo) {
        for (int ii = 0;ii<aS->ptrDtSurface()->nControlPointsU();ii++) {
          dtPoint3 cP = aS->ptrDtSurface()->controlPoint(ii, 0);
          dtPoint2 nearest = _vvStartAGeo->ptrDtSurface()->reparam( cP );
          dtPoint3 cPNearest = _vvStartAGeo->getPoint(
                                nearest.x(), 
                                nearest.y()
                              );
          aS->ptrDtSurface()->setControlPoint(ii, 0, cPNearest);
            DTBUFFER( << " ii::> " << ii << " " << DTLOGPOI3D(cP) << LOGDEL 
                      << DTLOGPOI2D(nearest) << LOGDEL
                      << DTLOGPOI3D(cPNearest) << LOGDEL);
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
            DTBUFFER( << " ii::> " << ii << DTLOGPOI3D(cP) << LOGDEL 
                      << DTLOGPOI2D(nearest) << LOGDEL
                      << DTLOGPOI3D(cPNearest) << LOGDEL);        
        }
      }
        DTDEBUGWF_BUFFER(apply());

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
	  QDomElement const * transformerElementP, 
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const sFunP,
		vectorHandling< analyticGeometry * > const * const depAGeoP 
	) {
    dtTransformer::init(transformerElementP, bC, cValP, sFunP, depAGeoP);
		
    //
    // check input
    //
    bool hasVStart = hasAttribute("parameter_two_start_part_label", *transformerElementP);
    bool hasVEnd = hasAttribute("parameter_two_end_part_label", *transformerElementP);
    
    //
    // get pointers to parts
    //
    if (hasVStart && hasVEnd) {
      std::string vStartLabel = getAttributeStr(
                                  "parameter_two_start_part_label", 
                                  *transformerElementP
                                );
      std::string vEndLabel = getAttributeStr(
                                  "parameter_two_end_part_label", 
                                  *transformerElementP
                                );      
      for (int ii = 0; ii<depAGeoP->size();ii++) {
         if ( depAGeoP->at(ii)->getLabel() == vStartLabel ) {
           dt__MUSTDOWNCAST(depAGeoP->at(ii), analyticSurface const, _vvStartAGeo);
         }
         if ( depAGeoP->at(ii)->getLabel() == vEndLabel ) {
           dt__MUSTDOWNCAST(depAGeoP->at(ii), analyticSurface const, _vvEndAGeo);
         }
         if (_vvStartAGeo && _vvEndAGeo) {
           break;
         }
      }
    }
    else {
      dt__THROW(init(),
              << DTLOGEVAL(hasVStart) << LOGDEL
              << DTLOGEVAL(hasVEnd) );
    }
  }
}