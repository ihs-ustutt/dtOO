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

			
//			std::vector< dtPoint3 > ppXYZ
//			=
//			dtPoint3_map1dTo3dEquidistantPoint(m1d, _nPoints).result();
//			
//			std::vector< dtPoint2 > ppUV(ppXYZ.size());
//			for (int ii=0; ii<ppUV.size(); ii++) {
//				ppUV[ii] = _m2d->reparamOnFace( ppXYZ[ii] );
//			}
//		
//			DTINFOWF(
//				apply(),
//				<< "Approx " << DTLOGEVAL(m1d->getLabel()) << " on " 
//				<<  DTLOGEVAL(_m2d->getLabel()) << "."
//			);
//			
//      //
//      // push approx geometry in vector
//      //      
//			dt__pH(dtCurve2d) dtC2d(
//				bSplineCurve2d_pointConstructOCC(ppUV, 1).result()
//			);
//			dt__pH(vec2dCurve2dOneD) v2d1d(new vec2dCurve2dOneD(dtC2d.get()) );
//			
//			dt__pH(map1dTo3d) tmpM1d( new vec2dOneDInMap2dTo3d(v2d1d.get(), _m2d.get()) );
			retAGeo.push_back(
			  map1dTo3d_normalOffsetMap1dTo2dInMap2dTo3d(m1d, _thickness).result()
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
  }
}