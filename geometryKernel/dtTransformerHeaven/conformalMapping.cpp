#include "conformalMapping.h"

#include <logMe/logMe.h>
#include <progHelper.h>
#include <analyticGeometryHeaven/rotatingSpline.h>
//#include <geometryEngine/dtSislBase.h>

namespace dtOO {
  conformalMapping::conformalMapping() : dtTransformer() {
  }

  conformalMapping::conformalMapping(conformalMapping const & orig ) : dtTransformer(orig) {
  }
	
  conformalMapping::~conformalMapping() {
  }

  dtTransformer * conformalMapping::clone( void ) const {
	  dt__THROW(clone(), "Not yet implemented.");
	}
	
  dtTransformer * conformalMapping::create( void ) const {
		return new conformalMapping();
	}
	
  std::vector< dtPoint2 * > conformalMapping::apply( std::vector< dtPoint2 * > const * const pointVecP ) const {
    
    std::vector< dtPoint2 * > pointVec2d(pointVecP->size());
    
    //
    //calculate paraValue - coordinates
    //
		std::vector<float> itVal;       
		std::vector< std::string > header;
		header.push_back("P_phiR");
		header.push_back("P_m");
		header.push_back("P_u");
		header.push_back("P_v");
		header.push_back("P_x");
		header.push_back("P_y");
		header.push_back("P_z");
		
    for (int ii = 0; ii<pointVecP->size(); ii++) {
      float const & phiR = pointVecP->at(ii)->x();
      float const & mm = pointVecP->at(ii)->y();
      
      //
      //debug
      //
      dtPoint2 point = _rotSplineP->uv_phiRadiusM(phiR, mm);
      dtPoint3 point3d = _rotSplineP->getPoint(point.x(), point.y());
      pointVec2d[ii] = new dtPoint2(point);

//      DTBUFFER(
//              << "phiR = " << phiR << " mm = " << mm
//              << " >> <cMap> >> "
//              << "uu = " << point.x() << " vv = " << point.y() << " ::> "
//              << DTLOGPOI3D(point3d) << LOGDEL );
			itVal.push_back(phiR); itVal.push_back(mm);
			itVal.push_back(point.x()); itVal.push_back(point.y());
			itVal.push_back(point3d.x()); itVal.push_back(point3d.y()); itVal.push_back(point3d.z());

    }  
		
	  DTDEBUGWF( apply(), << logMe::floatVecToTable(header, itVal) );
       
    return pointVec2d;
  }

  bool conformalMapping::isNecessary( void ) const {
    return true;
  }

  void conformalMapping::init( 
	  QDomElement const * transformerElementP, 
    baseContainer * const bC,  
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const sFunP,
		vectorHandling< analyticGeometry * > const * const depAGeoP 
	) {
    dtTransformer::init(transformerElementP, bC, cValP, sFunP, depAGeoP);
		
    //initialize with default values
    _rotSplineP = NULL;
    _tolerance = 0.01;
    if ( hasAttribute("tolerance", *transformerElementP) ) {
      handleFloat(
        "tolerance", 
        muParseString( 
          replaceUsedFunctions(
            getAttributeStr("tolerance", *transformerElementP), 
            cValP, 
            sFunP
          ) 
        )
      );
    }
    else {
      DTINFOWF(init(),
              << "Using default tolerance of " << _tolerance );
    }
    //
    // search part
    //
    dt__FORALL(*depAGeoP, ii,
      std::string geoName = depAGeoP->at(ii)->getLabel();
      if ( geoName == getAttributeStr("part_label", *transformerElementP ) ) {
        handleAnalyticGeometry("part_label", depAGeoP->at(ii));
        break;
      }
    );
  }
  
  void conformalMapping::handleFloat(std::string const name, float const value) {
    if (name == "tolerance") {
      _tolerance = value;
      return;
    }
    dtTransformer::handleFloat(name, value);
  }
  
  void conformalMapping::handleAnalyticGeometry(std::string const name, analyticGeometry const * value) {
    if (name == "part_label") {
      dt__MUSTDOWNCAST(value, rotatingSpline const, _rotSplineP);
      return;
    }
    dtTransformer::handleAnalyticGeometry(name, value);
  }
}