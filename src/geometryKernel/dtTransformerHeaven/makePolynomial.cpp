/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "makePolynomial.h"

#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h>
#include <geometryEngine/dtCurve.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include "dtTransformerFactory.h"

namespace dtOO { 
  bool makePolynomial::_registrated 
  =
  dtTransformerFactory::registrate(
    dt__tmpPtr(makePolynomial, new makePolynomial())
  );
  
  makePolynomial::makePolynomial() : dtTransformer() {
    _nPointsOne = 5;
    _nPointsTwo = 5;
    _orderOne = 2;
    _orderTwo = 2;    
  }

  makePolynomial::makePolynomial(
    const makePolynomial& orig
  ) : dtTransformer(orig) {
    
  }

  makePolynomial::~makePolynomial() {
  }

  dtTransformer * makePolynomial::clone( void ) const {
	  dt__throw(clone(), << "Not yet implemented.");
	}
	
  dtTransformer * makePolynomial::create( void ) const {
		return new makePolynomial();
	}	

  lvH_analyticGeometry makePolynomial::apply( 
              lvH_analyticGeometry const * const aGeoVecP 
  ) const {
    lvH_analyticGeometry retAGeo;
    
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
//            dtReal kkF = (float) kk;
//            dtReal llF = (float) ll;
//            dtReal nPointsTwoF = (float) _nPointsTwo;
//            dtReal nPointsOneF = (float) _nPointsOne;
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
          dtReal llF = (dtReal) ll;
          dtReal nPointsOneF = (dtReal) _nPointsOne;
          point.push_back( map1dTo3dP->getPointPercent(llF/nPointsOneF) );
        }
        point.push_back( map1dTo3dP->getPointPercent(1.) );
        
        //
        // reconstruct spline
        //
				ptrHandling<dtCurve> dtC(
				  bSplineCurve_pointConstructOCC(point, _orderOne).result()
				);
        analyticCurve * s3 = new analyticCurve(dtC.get());
        s3->setLabel( retAGeo[ii]->getLabel() );
        delete retAGeo[ii];
        retAGeo[ii] = s3;        
      }
      else {
        dt__throw(apply(),
                << dt__eval(map1dTo3dP) << std::endl
                << "Unexpected error.");
      }
  }

    return retAGeo;    
  }

  bool makePolynomial::isNecessary( void ) const {
    return true;
  }  

  void makePolynomial::init( 
	  ::QDomElement const * tE, 
    baseContainer * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG 
	) {     
    dtTransformer::init(tE, bC, cV, aF, aG);		
		
    if (dtXmlParserBase::hasAttribute("number_points_one", *tE)) {
      _nPointsOne = dtXmlParserBase::muParseStringInt( dtXmlParserBase::replaceDependencies(
                      dtXmlParserBase::getAttributeStr("number_points_one", *tE),
                      cV, 
                      aF) 
                    );
    }    
    if (dtXmlParserBase::hasAttribute("number_points_two", *tE)) {
      _nPointsTwo = dtXmlParserBase::muParseStringInt( dtXmlParserBase::replaceDependencies(
                      dtXmlParserBase::getAttributeStr("number_points_two", *tE),
                      cV, 
                      aF) 
                    );
    }    
    if (dtXmlParserBase::hasAttribute("order_one", *tE)) {
      _orderOne = dtXmlParserBase::muParseStringInt( dtXmlParserBase::replaceDependencies(
                    dtXmlParserBase::getAttributeStr("order_one", *tE),
                    cV, 
                    aF) 
                  );
    }        
    if (dtXmlParserBase::hasAttribute("order_two", *tE)) {
      _orderTwo = dtXmlParserBase::muParseStringInt( dtXmlParserBase::replaceDependencies(
                    dtXmlParserBase::getAttributeStr("order_two", *tE),
                    cV, 
                    aF) 
                  );
    } 
  }
}
