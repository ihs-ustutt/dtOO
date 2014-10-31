#include "curvePointConstruct.h"

#include <logMe/logMe.h>

#include <geometryEngine/dtSislBase.h>
#include <geometryEngine/dtSislCurve.h>
#include <sisl.h>
#include <sisl-copyright.h>
#include <sislP.h>


namespace dtOO {
	curvePointConstruct::curvePointConstruct( std::vector < dtPoint3 > const & pp, int const & order ) {
    //
    // open/close check
    //
    int openFlag;
    int nPoints = pp.size();

    if ( CGAL::compare_xyz(pp.at(0), pp.at(pp.size()-1)) == CGAL::EQUAL ) {
      if ( nPoints == 2 ) {
        dt__THROW(curvePointConstruct(),
                << "Try to create a spline out of two equal points." << LOGDEL
                << DTLOGEVAL(nPoints) );
      }
      openFlag = 0;
    }
    else {
      openFlag = 1;
    }
    
    //
    // fill array and count points that are different
    //
    int numPoints = pp.size();
    double * pointsArray = new double[ nPoints * 3 ];    
    int counter  = 0;
    pointsArray[counter] = pp.at(0).x();
    pointsArray[counter+1] = pp.at(0).y();
    pointsArray[counter+2] = pp.at(0).z();
    counter =  counter+3;
    for (int ii=1;ii<nPoints;ii++ ) {
      pointsArray[counter] = pp.at(ii).x();
      pointsArray[counter+1] = pp.at(ii).y();
      pointsArray[counter+2] = pp.at(ii).z();
      if (     (pointsArray[counter+0] == pointsArray[counter+0-3])
            && (pointsArray[counter+1] == pointsArray[counter+1-3])
            && (pointsArray[counter+2] == pointsArray[counter+2-3])
         ) {
        numPoints--;
        DTINFOWF(curvePointConstruct(),
                << "Duplicate point detected at " << DTLOGEVAL(ii) );
      }
      else {
        counter =  counter+3;
      }
    }

    //
    // create curve
    //
    int errFlag;    
    double startPara = 0.;
		SISLCurve * curve;
    s1630( pointsArray, //epoint[]
           numPoints, //inbpnt;
           startPara, //astpar;
           openFlag, //iopen;
           3, //idim;
           order, //ik;
           &curve, //**rc;
           &errFlag);//*jstat;
    dtSislBase().allRight(errFlag, __FILE__, __LINE__);
    
		//
		// store object
		//
		_dtC.reset( new dtSislCurve(*curve) );
		
		//
		// free memory
		//
		delete [] pointsArray;		
		freeCurve(curve);
	}
	
	curvePointConstruct::~curvePointConstruct() {
	}
	
	dtCurve * curvePointConstruct::result( void ) {
		return _dtC->clone();
	}
}
