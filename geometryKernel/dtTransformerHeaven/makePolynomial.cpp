#include "makePolynomial.h"
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h>
#include <geometryEngine/dtCurve.h>
#include <interfaceHeaven/ptrHandling.h>
#include <progHelper.h>
#include <logMe/logMe.h>
#include <analyticGeometryHeaven/analyticGeometry.h>

namespace dtOO { 
  makePolynomial::makePolynomial() : dtTransformer() {
    _nPointsOne = 5;
    _nPointsTwo = 5;
    _orderOne = 2;
    _orderTwo = 2;    
  }

  makePolynomial::makePolynomial(const makePolynomial& orig) : dtTransformer(orig) {
  }

  makePolynomial::~makePolynomial() {
  }

  dtTransformer * makePolynomial::clone( void ) const {
	  dt__THROW(clone(), << "Not yet implemented.");
	}
	
  dtTransformer * makePolynomial::create( void ) const {
		return new makePolynomial();
	}	

  vectorHandling< analyticGeometry * > makePolynomial::apply( 
              vectorHandling< analyticGeometry * > const * const aGeoVecP 
  ) const {
    vectorHandling< analyticGeometry * > retAGeo;
    
    for (int ii=0;ii<aGeoVecP->size();ii++) {
      //
      // clone geometry
      //
      retAGeo.push_back( aGeoVecP->at(ii)->clone() );

      //
      // check kind of mapping
      //
//      map2dTo3d const * map2dTo3dP = dynamic_cast< map2dTo3d const * >( retAGeo[ii] );
      map1dTo3d const * map1dTo3dP = dynamic_cast< map1dTo3d const * >( retAGeo[ii] );
      
//      //
//      // surface mapping
//      //
//      if ( map2dTo3dP && !map1dTo3dP ) {
//        //
//        // ask original surface for some points
//        //
//        std::vector< std::vector< dtPoint3 > > point;
//        for(int kk=0;kk<=_nPointsOne;kk++) { // second direction
//          point.push_back(std::vector< dtPoint3 >() );
//          for(int ll=0;ll<=_nPointsTwo;ll++) { // first direction
//            float kkF = (float) kk;
//            float llF = (float) ll;
//            float nPointsTwoF = (float) _nPointsTwo;
//            float nPointsOneF = (float) _nPointsOne;
//            point[kk].push_back( 
//              map2dTo3dP->getPointPercent(
//                llF/nPointsOneF, 
//                kkF/nPointsTwoF
//              ) 
//            );
//          }
//        }
//        analyticSurface * aS = new analyticSurface(&point, _orderOne, _orderTwo);
//        aS->setLabel( retAGeo[ii]->getLabel() );
//        delete retAGeo[ii];
//        retAGeo[ii] = aS;
//      }
      //
      // curve mapping
      //
      if ( map1dTo3dP ) {
        //
        // ask original splineCurve for some points
        //
        std::vector< dtPoint3 > point;
        point.push_back( map1dTo3dP->getPointPercent(0.) );
        for(int ll=1; ll<=_nPointsOne-1; ll++) { // first direction
          float llF = (float) ll;
          float nPointsOneF = (float) _nPointsOne;
          point.push_back( map1dTo3dP->getPointPercent(llF/nPointsOneF) );
        }
        point.push_back( map1dTo3dP->getPointPercent(1.) );
        
        //
        // reconstruct spline
        //
				ptrHandling<dtCurve> dtC(
				  bSplineCurve_pointConstructOCC(point, _orderOne).result()
				);
        splineCurve3d * s3 = new splineCurve3d(dtC.get());
        s3->setLabel( retAGeo[ii]->getLabel() );
        delete retAGeo[ii];
        retAGeo[ii] = s3;        
      }
      else {
        dt__THROW(apply(),
                << DTLOGEVAL(map1dTo3dP) << LOGDEL
                << "Unexpected error.");
      }
  }

    return retAGeo;    
  }

  bool makePolynomial::isNecessary( void ) const {
    return true;
  }  

  void makePolynomial::init( 
	  QDomElement const * tE, 
    baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG 
	) {     
    dtTransformer::init(tE, bC, cV, aF, aG);		
		
    if (dtXmlParserBase::hasAttribute("number_points_one", *tE)) {
      _nPointsOne = dtXmlParserBase::muParseStringInt( dtXmlParserBase::replaceUsedFunctions(
                      dtXmlParserBase::getAttributeStr("number_points_one", *tE),
                      cV, 
                      aF) 
                    );
    }    
    if (dtXmlParserBase::hasAttribute("number_points_two", *tE)) {
      _nPointsTwo = dtXmlParserBase::muParseStringInt( dtXmlParserBase::replaceUsedFunctions(
                      dtXmlParserBase::getAttributeStr("number_points_two", *tE),
                      cV, 
                      aF) 
                    );
    }    
    if (dtXmlParserBase::hasAttribute("order_one", *tE)) {
      _orderOne = dtXmlParserBase::muParseStringInt( dtXmlParserBase::replaceUsedFunctions(
                    dtXmlParserBase::getAttributeStr("order_one", *tE),
                    cV, 
                    aF) 
                  );
    }        
    if (dtXmlParserBase::hasAttribute("order_two", *tE)) {
      _orderTwo = dtXmlParserBase::muParseStringInt( dtXmlParserBase::replaceUsedFunctions(
                    dtXmlParserBase::getAttributeStr("order_two", *tE),
                    cV, 
                    aF) 
                  );
    } 
  }
}
