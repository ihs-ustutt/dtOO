#include "conformalMappingScale.h"

#include <logMe/logMe.h>
#include <progHelper.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <functionHeaven/analyticFunction.h>

namespace dtOO {
  conformalMappingScale::conformalMappingScale() : dtTransformer() {
  }

  conformalMappingScale::conformalMappingScale(
	  conformalMappingScale const & orig 
	) : dtTransformer(orig) {
		_m3d.reset( orig._m3d->clone() );
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
			analyticFunction * aF = aFP->at(ii)->clone();
			retV.push_back( aF );
		}
		
		return retV;
//    std::vector< dtPoint2 * > pointVec2d(pointVecP->size());
//    
//    //
//    //calculate paraValue - coordinates
//    //
//		std::vector<float> itVal;       
//		std::vector< std::string > header;
//		header.push_back("P_phiR");
//		header.push_back("P_m");
//		header.push_back("P_u");
//		header.push_back("P_v");
//		header.push_back("P_x");
//		header.push_back("P_y");
//		header.push_back("P_z");
//		
//    for (int ii = 0; ii<pointVecP->size(); ii++) {
//      float const & phiR = pointVecP->at(ii)->x();
//      float const & mm = pointVecP->at(ii)->y();
//      
//      //
//      //debug
//      //
//      dtPoint2 point = _rotSplineP->uv_phiRadiusM(phiR, mm);
//      dtPoint3 point3d = _rotSplineP->getPoint(point.x(), point.y());
//      pointVec2d[ii] = new dtPoint2(point);
//
////      DTBUFFER(
////              << "phiR = " << phiR << " mm = " << mm
////              << " >> <cMap> >> "
////              << "uu = " << point.x() << " vv = " << point.y() << " ::> "
////              << DTLOGPOI3D(point3d) << LOGDEL );
//			itVal.push_back(phiR); itVal.push_back(mm);
//			itVal.push_back(point.x()); itVal.push_back(point.y());
//			itVal.push_back(point3d.x()); itVal.push_back(point3d.y()); itVal.push_back(point3d.z());
//
//    }  
//		
//	  DTDEBUGWF( apply(), << logMe::floatVecToTable(header, itVal) );
//       
//    return pointVec2d;
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
      dt__PTRASS(map3dTo3d const * m3d, map3dTo3d::ConstDownCast(value));
			_m3d.reset( m3d->clone() );
      return;
    }
    dtTransformer::handleAnalyticGeometry(name, value);
  }
}