#include "normalOffsetInSurface.h"
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/map1dTo3d_normalOffsetMap1dTo2dInMap2dTo3d.h>
#include <progHelper.h>
#include <logMe/logMe.h>


namespace dtOO {
  normalOffsetInSurface::normalOffsetInSurface() : dtTransformer() {
  }

	normalOffsetInSurface::normalOffsetInSurface( normalOffsetInSurface const & orig ) {
	  _m2d.reset( orig._m2d->clone() );
		_thickness = orig._thickness;
		_nPoints = orig._nPoints;
		_nIntPoints = orig._nIntPoints;
	}
	
  normalOffsetInSurface::~normalOffsetInSurface() {
  }

  dtTransformer * normalOffsetInSurface::clone( void ) const {
	  return new normalOffsetInSurface(*this);
	}
	
  dtTransformer * normalOffsetInSurface::create( void ) const {
		return new normalOffsetInSurface();
	}
	
  vectorHandling< analyticGeometry * > normalOffsetInSurface::apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const {
    vectorHandling< analyticGeometry * > retAGeo;
    for (int ii=0; ii<aGeoVecP->size(); ii++) {
      //
      // cast analyticGeometry
      //
      dt__PTRASS(
			  map1dTo3d const * const m1d, 
				map1dTo3d::ConstDownCast(aGeoVecP->at(ii))
			);

			
			retAGeo.push_back(
			  map1dTo3d_normalOffsetMap1dTo2dInMap2dTo3d(
			    m1d, _thickness, _nPoints, _nIntPoints
			  ).result()
			);
			
			retAGeo.back()->setLabel(m1d->getLabel());
    }
    return retAGeo;
  }

  bool normalOffsetInSurface::isNecessary( void ) const {
    return true;
  }

  void normalOffsetInSurface::init( 
	  QDomElement const * tE, 
    baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);
    
    if (dtXmlParserBase::hasAttribute( "part_label", *tE) ) {
			_m2d.reset(			
				map2dTo3d::ConstDownCast(
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
		
    if (dtXmlParserBase::hasAttribute( "thickness", *tE) ) {
			_thickness 
			= 
			dtXmlParserBase::getAttributeFloatMuParse("thickness", *tE, cV, aF);
    }
    else {
      dt__THROW(
				init(), 
				<< DTLOGEVAL(dtXmlParserBase::hasAttribute( "thickness", *tE))
			);
    }		

    if (dtXmlParserBase::hasAttribute( "number_points", *tE) ) {
			_nPoints 
			= 
			dtXmlParserBase::getAttributeIntMuParse("number_points", *tE, cV, aF);
    }
    else {
      dt__THROW(
				init(), 
				<< DTLOGEVAL(dtXmlParserBase::hasAttribute( "number_points", *tE))
			);
    }		

    if (dtXmlParserBase::hasAttribute( "number_integration_points", *tE) ) {
			_nIntPoints 
			= 
			dtXmlParserBase::getAttributeIntMuParse("number_integration_points", *tE, cV, aF);
    }
    else {
      dt__THROW(
				init(), 
				<< DTLOGEVAL(dtXmlParserBase::hasAttribute( "number_integration_points", *tE))
			);
    }				
  }
}