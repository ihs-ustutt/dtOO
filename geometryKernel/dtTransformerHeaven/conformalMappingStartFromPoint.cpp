#include "conformalMappingStartFromPoint.h"
#include <analyticGeometryHeaven/rotatingSpline.h>
#include <logMe/logMe.h>
#include <progHelper.h>

namespace dtOO {
  conformalMappingStartFromPoint::conformalMappingStartFromPoint() {
  }

  conformalMappingStartFromPoint::conformalMappingStartFromPoint(const conformalMappingStartFromPoint& orig) {
  }

  conformalMappingStartFromPoint::~conformalMappingStartFromPoint() {
  }

  void conformalMappingStartFromPoint::init( 
         QDomElement * transformerElementP, 
         pointContainer * const pointContainerP,
         vectorContainer * const vectorContainerP,    
         vectorHandling< constValue * > const * const cValP,
         vectorHandling< analyticFunction * > const * const sFunP,
         vectorHandling< analyticGeometry * > const * const depAGeoP 
       ) {
    //
    // call on base class
    //
    conformalMapping::init(
      transformerElementP,
      pointContainerP,
      vectorContainerP,
      cValP,
      sFunP,
      depAGeoP
    );
            
    //
    // init me
    //
    bool hasOffsetZ = hasAttribute("offset_z", *transformerElementP);
    bool hasOffsetM = hasAttribute("offset_m", *transformerElementP);
    bool hasOffsetPhi = hasAttribute("offset_phi", *transformerElementP);
    
    if (hasOffsetZ && hasOffsetM) {
        dt__THROW(init(), 
                << "Please set offset_z OR offset_m");
    }
    
    float offsetZ = 0.;    
    /** @xmlAttribute{offset_z, float, Offset in \f$z\f$-Coordinate} */
    if ( hasOffsetZ ) {
       offsetZ = muParseString( 
                   replaceUsedFunctions(
                     getAttributeStr("offset_z", *transformerElementP), 
                     cValP, 
                     sFunP
                   ) 
                 );
    }  
    float offsetPhi = 0.;
    /** @xmlAttribute{offset_phi, float, Offset in \f$\phi\f$-Coordinate} */
    if ( hasOffsetPhi ) {
      offsetPhi = muParseString( 
                    replaceUsedFunctions(
                      getAttributeStr("offset_phi", *transformerElementP), 
                      cValP, 
                      sFunP
                    ) 
                  );
    }  
    float offsetM = 0.;
    /** @xmlAttribute{offset_m, float, Offset in \f$m\f$-Coordinate} */
    if ( hasOffsetM ) {
      offsetM = muParseString( 
                    replaceUsedFunctions(
                      getAttributeStr("offset_m", *transformerElementP), 
                      cValP, 
                      sFunP
                    ) 
                  );
    }  
    
		_basePoint = dtPoint2(0.,0.);
    if ( hasOffsetM ) {
      handlePoint2d("base_point", _rotSplineP->uv_phiM(offsetPhi, offsetM));
    }
    else if ( hasOffsetZ ) {
      handlePoint2d("base_point", _rotSplineP->uv_phiZ(offsetPhi, offsetZ));
    }

    DTINFOWF(init(),
//            << DTLOGEVAL(_offsetPhiR) << LOGDEL
//            << DTLOGEVAL(_offsetM) << LOGDEL
            << DTLOGPOI2D(_basePoint) );
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
//    DTBUFFERINIT();
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
//      DTBUFFER(
//              << DTLOGEVAL(deltaPhiR) << " " << DTLOGEVAL(deltaMm)
//              << " >> <cMap> >> uu = " << point.x() << " vv = " << point.y() << " ::> "
//              << DTLOGPOI3D(point3d) << LOGDEL );
			itVal.push_back(deltaPhiR); itVal.push_back(deltaMm);
			itVal.push_back(point.x()); itVal.push_back(point.y());
			itVal.push_back(point3d.x()); itVal.push_back(point3d.y()); itVal.push_back(point3d.z());			
    }  
	  DTDEBUGWF( apply(), << logMe::floatVecToTable(header, itVal) );
       
    return pointVec2d;    
  }
  
  void conformalMappingStartFromPoint::handlePoint2d(std::string const name, dtPoint2 const value) {
    if (name == "base_point") {
      _basePoint = value;
      return;
    }
    
    conformalMapping::handlePoint2d(name, value);
  }
    
}
