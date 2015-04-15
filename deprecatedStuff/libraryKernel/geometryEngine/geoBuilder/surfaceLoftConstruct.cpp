#include "surfaceLoftConstruct.h"

#include <logMe/logMe.h>
#include <progHelper.h>
#include <geometryEngine/dtSislCurve.h>
#include <geometryEngine/dtSislSurf.h>
#include <sisl.h>
#include <sisl-copyright.h>
#include <sislP.h>

namespace dtOO {
	surfaceLoftConstruct::surfaceLoftConstruct( vectorHandling< dtCurve const * > const &  curveList, int const order ) {
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
      int errFlag; 

      std::vector< int > curveType;
      
      SISLCurve * SISLCurveListPP[curveList.size()];
      for (int ii=0;ii<curveList.size();ii++) {
        curveType.push_back(1);
				dtSislCurve const * tmpCurve;
        dt__mustCast(curveList.at(ii), dtSislCurve const, tmpCurve);
				SISLCurve const * tmpSislCurve;
				dt__mustCast(tmpCurve->getSISLCurve(), SISLCurve const, tmpSislCurve); 
				SISLCurveListPP[ii] = const_cast< SISLCurve * >(tmpSislCurve);
      }  

      //
      // create surface
      //
      int openFlag = 1;
      double * gpar;
      int const adjustDerivatives = 1;
      double const startPara = 0.;      
			SISLSurf * surf;
      s1538( 
			  curveList.size(), //int inbrcv 
				SISLCurveListPP, // SISLCurve *vpcrv[]
				&(curveType[0]), //int nctyp[ ];
				startPara, //double astpar
				openFlag,  //int iopen
				order, //int iord2 
				adjustDerivatives, //int iflag
				&surf,  //SISLSurf **rsurf
				&gpar,  // double **gpar
				&errFlag 
			); //int * jstat
      dtSislBase().allRight(errFlag, __FILE__, __LINE__);
			
			_dtS.reset( new dtSislSurf(*surf) );
			freeSurf(surf);			
    }
    else if (!isPolynomial && isRational) {     
      SISLCurve * SISLCurveListPP[curveList.size()];
      double parArr[curveList.size()];
      double parArrInt = 1. / ( ((double) curveList.size()) - 1. );
      for (int ii=0;ii<curveList.size();ii++) {
        //SISLCurveListPP[ii] = const_cast< SISLCurve * >( cast2Sisl(curveList.at(ii))->getSISLCurve() );
				dtSislCurve const * tmpCurve;
        dt__mustCast(curveList.at(ii), dtSislCurve const, tmpCurve);
				SISLCurve const * tmpSislCurve;
				dt__mustCast(tmpCurve->getSISLCurve(), SISLCurve const, tmpSislCurve); 
				SISLCurveListPP[ii] = const_cast< SISLCurve * >(tmpSislCurve);
        double iiD = (double) ii;
        parArr[ii] = iiD * parArrInt;
      }
      //
      // adjust start and end of array
      //
      parArr[0] = 0.;
      parArr[curveList.size()] = 1.;
      
      int errFlag;
			SISLSurf * surf;
      s1508( curveList.size(), 
             SISLCurveListPP,
             parArr, 
             &surf, 
             &errFlag);
      dtSislBase().allRight(errFlag, __FILE__, __LINE__);
			
			_dtS.reset( new dtSislSurf(*surf) );
			freeSurf(surf);
    }
    else if (isPolynomial && isRational) {     
      dt__THROW(surfaceLoftConstruct(),
              << "Surface creation out of rational and polyomial " << std::endl
              << "B-Splines is not yet possible.");
    }		
	}

	surfaceLoftConstruct::~surfaceLoftConstruct() {
	}
	
	dtSurface * surfaceLoftConstruct::result( void ) {
		return _dtS->clone();
	}
}
