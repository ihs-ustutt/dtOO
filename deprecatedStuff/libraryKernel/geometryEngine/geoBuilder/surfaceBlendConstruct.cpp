#include "surfaceBlendConstruct.h"

#include <logMe/logMe.h>
#include <progHelper.h>
#include <geometryEngine/dtSislCurve.h>
#include <geometryEngine/dtSislSurf.h>
#include <sisl.h>
#include <sisl-copyright.h>
#include <sislP.h>

namespace dtOO {	
	surfaceBlendConstruct::surfaceBlendConstruct( vectorHandling< dtCurve const * > const &  curveList ) {
    bool isPolynomial = false;    
    bool isRational = false;

    //
    // check curve types
    //
    for (int ii=0;ii<curveList.size();ii++) {
      int kind = curveList.at(ii)->getKind();
      if ( (kind == 1) && (!isPolynomial) ) {
        isPolynomial = true;
      }
      else if ( (kind == 2) && (!isRational) ) {
        isRational = true;
      }
    }

    //
    // polynomial splines
    //
    if (isPolynomial && !isRational) {
			vectorHandling< dtCurve const * > reoCurve = reorderForBlending(&curveList);
			
      int errFlag;
      int numder[4];
      
      SISLCurve * SISLCurveListPP[reoCurve.size()];
      for (int ii=0;ii<reoCurve.size();ii++) {
				dtSislCurve const * tmpCurve;
        dt__mustDownCast(reoCurve.at(ii), dtSislCurve const, tmpCurve);
				SISLCurve const * tmpSislCurve;
				dt__mustDownCast(tmpCurve->getSISLCurve(), SISLCurve const, tmpSislCurve); 
				SISLCurveListPP[ii] = const_cast< SISLCurve * >(tmpSislCurve);
      }  
      if (reoCurve.size() != 4) {
        dt__THROW(blendMe(),
                << DTLOGEVAL(reoCurve.size()) << LOGDEL
                << "Creation of a blended surface out of more" << LOGDEL
                << " than 4 curves is not yet supported.");
      }
      numder[0] = 1;
      numder[1] = 1;
      numder[2] = 1;
      numder[3] = 1;
      
      //
      // create surface
      //
			SISLSurf * surf;
      s1390(
        SISLCurveListPP, 
        &surf,
        numder,
        &errFlag
      );
      dtSislBase().allRight(errFlag, __FILE__, __LINE__);
			
			_dtS.reset( new dtSislSurf(*surf) );
			freeSurf(surf);
    }
    else if (!isPolynomial && isRational) {     
      dt__THROW( surfaceBlendConstruct(),
              << "Surface creation out of rational and polyomial " << LOGDEL
              << "B-Splines is not yet possible.");
    }
    else if (isPolynomial && isRational) {     
      dt__THROW( surfaceBlendConstruct(),
              << "Surface creation out of rational and polyomial " << LOGDEL
              << "B-Splines is not yet possible.");
    }
	}

	surfaceBlendConstruct::~surfaceBlendConstruct() {
	}
	
	dtSurface * surfaceBlendConstruct::result( void ) {
		return _dtS->clone();
	}

  vectorHandling< dtCurve const * > surfaceBlendConstruct::reorderForBlending( 
      vectorHandling< dtCurve const * > const * const curveV 
  ) const {
		vectorHandling< dtCurve * > retVec;    
		vectorHandling< dtCurve const * > constRetVec;
		for (int ii=0;ii<curveV->size();ii++) {
			retVec.push_back( curveV->at(ii)->clone() );
		}  
		dtPoint3 p0 = retVec[0]->getPointPercent3d(0.);
		dtPoint3 p1 = retVec[0]->getPointPercent3d(1.);
		dtPoint3 q0 = retVec[1]->getPointPercent3d(0.);
		dtPoint3 q1 = retVec[1]->getPointPercent3d(1.);
		dtPoint3 r0 = retVec[2]->getPointPercent3d(0.);
		dtPoint3 r1 = retVec[2]->getPointPercent3d(1.);
		dtPoint3 s0 = retVec[3]->getPointPercent3d(0.);
		dtPoint3 s1 = retVec[3]->getPointPercent3d(1.);

		if ( dtVector3(p1-q0).squared_length() > dtVector3(p0-q0).squared_length() ) {
			retVec[0]->revert();
			p0 = retVec[0]->getPointPercent3d(0.);
			p1 = retVec[0]->getPointPercent3d(1.);

		}

		if ( dtVector3(p1-q0).squared_length() > dtVector3(p1-q1).squared_length() ) {
			retVec[1]->revert();
			dtPoint3 q0 = retVec[1]->getPointPercent3d(0.);
			dtPoint3 q1 = retVec[1]->getPointPercent3d(1.);
		}

		if ( dtVector3(q1-r1).squared_length() > dtVector3(q1-r0).squared_length() ) {
			retVec[2]->revert();
			dtPoint3 r0 = retVec[2]->getPointPercent3d(0.);
			dtPoint3 r1 = retVec[2]->getPointPercent3d(1.);

		}      
		if ( dtVector3(p0-s0).squared_length() > dtVector3(p0-s1).squared_length() ) {
			retVec[3]->revert();
//        dtPoint3 s0 = retVec[3]->getPointPercent3d(0.);
//        dtPoint3 s1 = retVec[3]->getPointPercent3d(1.);
		}         

		constRetVec.push_back( retVec[0] );
		constRetVec.push_back( retVec[1] );
		constRetVec.push_back( retVec[2] );
		constRetVec.push_back( retVec[3] );

		return constRetVec;
  }    	
}
