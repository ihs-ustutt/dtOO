#include "conformalMappingStartFromPoint.h"
#include <analyticGeometryHeaven/rotatingSpline.h>
#include <logMe/logMe.h>
#include <progHelper.h>

namespace dtOO {
  conformalMappingStartFromPoint::conformalMappingStartFromPoint(
  ) : conformalMapping() {
    
  }

  conformalMappingStartFromPoint::conformalMappingStartFromPoint(
    const conformalMappingStartFromPoint& orig
  ) : conformalMapping(orig) {
    
  }

  conformalMappingStartFromPoint::~conformalMappingStartFromPoint() {
  
  }

  dtTransformer * conformalMappingStartFromPoint::clone( void ) const {
	  dt__throw(clone(), << "Not yet implemented.");
	}
	
  dtTransformer * conformalMappingStartFromPoint::create( void ) const {
		return new conformalMappingStartFromPoint();
	}
	
  void conformalMappingStartFromPoint::init( 
	  QDomElement const * tE, 
    baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG  
	) {
    //
    // call on base class
    //
    conformalMapping::init(tE, bC, cV, aF, aG);
            
    //
    // init me
    //
    bool hasOffsetZ = dtXmlParserBase::hasAttribute("offset_z", *tE);
    bool hasOffsetM = dtXmlParserBase::hasAttribute("offset_m", *tE);
    bool hasOffsetPhi = dtXmlParserBase::hasAttribute("offset_phi", *tE);
    
    if (hasOffsetZ && hasOffsetM) {
        dt__throw(init(), 
                << "Please set offset_z OR offset_m");
    }
    
    float offsetZ = 0.;    
    /** @xmlAttribute{offset_z, float, Offset in \f$z\f$-Coordinate} */
    if ( hasOffsetZ ) {
       offsetZ = dtXmlParserBase::muParseString( 
                   dtXmlParserBase::replaceUsedFunctions(
                     dtXmlParserBase::getAttributeStr("offset_z", *tE), 
                     cV, 
							       aF
                   ) 
                 );
    }  
    float offsetPhi = 0.;
    /** @xmlAttribute{offset_phi, float, Offset in \f$\phi\f$-Coordinate} */
    if ( hasOffsetPhi ) {
      offsetPhi = dtXmlParserBase::muParseString( 
                    dtXmlParserBase::replaceUsedFunctions(
                      dtXmlParserBase::getAttributeStr("offset_phi", *tE), 
                      cV, 
							        aF
                    ) 
                  );
    }  
    float offsetM = 0.;
    /** @xmlAttribute{offset_m, float, Offset in \f$m\f$-Coordinate} */
    if ( hasOffsetM ) {
      offsetM = dtXmlParserBase::muParseString( 
                    dtXmlParserBase::replaceUsedFunctions(
                      dtXmlParserBase::getAttributeStr("offset_m", *tE), 
                      cV, 
							        aF
                    ) 
                  );
    }  
    
		_basePoint = dtPoint2(0.,0.);
    if ( hasOffsetM ) {
      handleDtPoint2("base_point", _rotSplineP->uv_phiM(offsetPhi, offsetM));
    }
    else if ( hasOffsetZ ) {
      handleDtPoint2("base_point", _rotSplineP->uv_phiZ(offsetPhi, offsetZ));
    }

    dt__info(init(),
//            << dt__eval(_offsetPhiR) << std::endl
//            << dt__eval(_offsetM) << std::endl
            << dt__point2d(_basePoint) );
  }
  
  std::vector< dtPoint2 * > conformalMappingStartFromPoint::apply( 
                             std::vector< dtPoint2 * > const * const pointVecP 
                           ) const {
//    //
//    // modify points
//    //
//    std::vector< dtPoint2 * > pointDummy;
//    dt__FORALL(*pointVecP,ii,
//      pointDummy.push_back( 
//                   new dtPoint2(
//                         pointVecP->at(ii)->x() + _offsetPhiR, 
//                         pointVecP->at(ii)->y() + _offsetM
//                   ) 
//                 );
//    );
//    
//    return conformalMapping::apply( &pointDummy );
    
    std::vector< dtPoint2 * > pointVec2d(pointVecP->size(), NULL);
    
    //
    //calculate paraValue - coordinates
    //
		std::vector<float> itVal;       
		std::vector< std::string > header;
		header.push_back("P_deltaPhiR");
		header.push_back("P_deltaM");
		header.push_back("P_u");
		header.push_back("P_v");
		header.push_back("P_x");
		header.push_back("P_y");
		header.push_back("P_z");		
    for (int ii = 0; ii<pointVecP->size(); ii++) {
      float const & deltaPhiR = pointVecP->at(ii)->x();
      float const & deltaMm = pointVecP->at(ii)->y();
      
      //
      // debug
      //
      rotatingSpline const * rS = _rotSplineP;
      dtPoint2 point = rS->uv_deltaPhiRadiusDeltaM(
                        _basePoint.x(), 
                        _basePoint.y(),
                        deltaPhiR, 
                        deltaMm
                      );
      dtPoint3 point3d = rS->getPoint(point.x(), point.y());
      pointVec2d[ii] = new dtPoint2(point);      
			itVal.push_back(deltaPhiR); 
      itVal.push_back(deltaMm);
			itVal.push_back(point.x()); 
      itVal.push_back(point.y());
			itVal.push_back(point3d.x()); 
      itVal.push_back(point3d.y()); 
      itVal.push_back(point3d.z());			
    }  
	  dt__debug( apply(), << logMe::vecToTable(header, itVal) );
       
    return pointVec2d;    
  }
  
  void conformalMappingStartFromPoint::handleDtPoint2(
    std::string const name, dtPoint2 const value
  ) {
    if (name == "base_point") {
      _basePoint = value;
      return;
    } 
    conformalMapping::handleDtPoint2(name, value);
  }  
}
