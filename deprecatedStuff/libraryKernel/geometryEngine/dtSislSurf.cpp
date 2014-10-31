#include "dtSislSurf.h"

#include <math.h>
#include <logMe/logMe.h>
#include <progHelper.h>
#include "dtSislCurve2d.h"
#include "dtSislCurve.h"
#include "dtLinearAlgebra.h"
#include <algorithm>
#include <stdio.h>
#include <sisl.h>
#include <sisl-copyright.h>
#include <sislP.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/floatHandling.h>

namespace dtOO {
  dtSislSurf::dtSislSurf() : dtSislBase(), dtSurface() {
    _leftknot1 = 0;
    _leftknot2 = 0;
    _cLengthMin = 1.;
    _cLengthMax = 1.;
  }

  dtSislSurf::dtSislSurf(const dtSislSurf& orig) : dtSislBase(), dtSurface() {
    _SISLSurfP = copySurface( orig._SISLSurfP );
    _leftknot1 = 0;
    _leftknot2 = 0;
    _cLengthMin = orig._cLengthMin;
    _cLengthMax = orig._cLengthMax;
  }

  dtSislSurf::dtSislSurf(SISLSurf const & surf) : dtSislBase(), dtSurface() {
    _SISLSurfP = copySurface( const_cast< SISLSurf * > (&surf) );
    _leftknot1 = 0;
    _leftknot2 = 0;
    updateCLength();
  }

	dtSurface * dtSislSurf::clone( void ) const {
		return new dtSislSurf(*this);
	}
  
  dtSislSurf::~dtSislSurf() {
  }

  float dtSislSurf::getMin( int const dim ) const {
    double startParaFirst;
    double startParaSecond;
    double endParaFirst;
    double endParaSecond;
    int errFlag;
    s1603(_SISLSurfP, //SISLSurf *surf;
          &startParaFirst,//double *min1;
          &startParaSecond, //double *min2;
          &endParaFirst, //double *max1;
          &endParaSecond, //double *max2;
          &errFlag);//int *stat;

    allRight(errFlag, __FILE__, __LINE__);

    switch (dim) {
      case 0:		
		    return static_cast<float>(startParaFirst);
      case 1:		
		    return static_cast<float>(startParaSecond);
      default:
        dt__THROW(getMin(),
              << DTLOGEVAL(dim) << LOGDEL
              << "dim should be 0 or 1.");				
		}
	}
  
	float dtSislSurf::getMax( int const dim ) const {
    double startParaFirst;
    double startParaSecond;
    double endParaFirst;
    double endParaSecond;
    int errFlag;
    s1603(_SISLSurfP, //SISLSurf *surf;
          &startParaFirst,//double *min1;
          &startParaSecond, //double *min2;
          &endParaFirst, //double *max1;
          &endParaSecond, //double *max2;
          &errFlag);//int *stat;

    allRight(errFlag, __FILE__, __LINE__);

    switch (dim) {
      case 0:		
		    return static_cast<float>(endParaFirst);
      case 1:		
		    return static_cast<float>(endParaSecond);
      default:
        dt__THROW(getMax(),
              << DTLOGEVAL(dim) << LOGDEL
              << "dim should be 0 or 1.");				
		}		
	}
	
	int dtSislSurf::getNControlPoints( int const dim ) const {
    switch (dim) {
      case 0:		
		    return _SISLSurfP->in1;
      case 1:		
		    return _SISLSurfP->in2;
      default:
        dt__THROW(getMax(),
              << DTLOGEVAL(dim) << LOGDEL
              << "dim should be 0 or 1.");				
		}
  }
	
	bool dtSislSurf::isClosed( int const dim ) const {
    switch (dim) {
      case 0:		
				if (_SISLSurfP->cuopen_1 == 0) {
					return true;
				}
				return false;
      case 1:		
				if (_SISLSurfP->cuopen_2 == 0) {
					return true;
				}
				return false;				
      default:
        dt__THROW(getMax(),
              << DTLOGEVAL(dim) << LOGDEL
              << "dim should be 0 or 1.");				
		}
	}
		
  double dtSislSurf::getUMinD( void ) const {
    double startParaFirst;
    double startParaSecond;
    double endParaFirst;
    double endParaSecond;
    int errFlag;
    s1603(_SISLSurfP, //SISLSurf *surf;
          &startParaFirst,//double *min1;
          &startParaSecond, //double *min2;
          &endParaFirst, //double *max1;
          &endParaSecond, //double *max2;
          &errFlag);//int *stat;

    if ( !allRight(errFlag, __FILE__, __LINE__) ) {
      return 0;
    }
    else {
      return startParaFirst;
    }
  }

  double dtSislSurf::getUMaxD( void ) const {
    double startParaFirst;
    double startParaSecond;
    double endParaFirst;
    double endParaSecond;
    int errFlag;
    s1603(_SISLSurfP, //SISLSurf *surf;
          &startParaFirst,//double *min1;
          &startParaSecond, //double *min2;
          &endParaFirst, //double *max1;
          &endParaSecond, //double *max2;
          &errFlag);//int *stat;

    if ( !allRight(errFlag, __FILE__, __LINE__) ) {
      return 0;
    }
    else {
      return endParaFirst;
    }
  }

//  float dtSislSurf::getUMin( void ) const {
//    double tmp = getUMinD();
//    return ( (float) tmp );
//  }
//
//  float dtSislSurf::getUMax( void ) const {
//    double tmp = getUMaxD();
//    return ( (float) tmp );
//  }

  double dtSislSurf::getVMinD( void ) const {
    double startParaFirst;
    double startParaSecond;
    double endParaFirst;
    double endParaSecond;
    int errFlag;
    s1603(_SISLSurfP, //SISLSurf *surf;
          &startParaFirst,//double *min1;
          &startParaSecond, //double *min2;
          &endParaFirst, //double *max1;
          &endParaSecond, //double *max2;
          &errFlag);//int *stat;

    if ( !allRight(errFlag, __FILE__, __LINE__) ) {
      return 0;
    }
    else {
      return startParaSecond;
    }
  }

  double dtSislSurf::getVMaxD( void ) const {
    double startParaFirst;
    double startParaSecond;
    double endParaFirst;
    double endParaSecond;
    int errFlag;
    s1603(_SISLSurfP, //SISLSurf *surf;
          &startParaFirst,//double *min1;
          &startParaSecond, //double *min2;
          &endParaFirst, //double *max1;
          &endParaSecond, //double *max2;
          &errFlag);//int *stat;

    if ( !allRight(errFlag, __FILE__, __LINE__) ) {
      return 0;
    }
    else {
      return endParaSecond;
    }
  }

  int dtSislSurf::getKind( void ) const {
    return _SISLSurfP->ikind;
  }
   
  dtPoint3 dtSislSurf::getPoint3d(float const uu, float const vv) const {
    double dbl_para[2];
    int errFlag;
    double dbl_point[3];

    dbl_para[0] =  static_cast<double>(uu);
    dbl_para[1] =  static_cast<double>(vv);

    s1424(_SISLSurfP, //SISLSurf *surf;
          0, //int der;
          0, //int der;
          dbl_para, //double parvalue[ ];
          &_leftknot1, //int *leftknot1;
          &_leftknot2, //int *leftknot2;
          dbl_point, //double derive[ ];
          &errFlag); //int *stat; 
    allRight(errFlag, __FILE__, __LINE__);
    
    return ( dtPoint3( dbl_point[0] , dbl_point[1], dbl_point[2] ) ); 
  }  

  void dtSislSurf::dump( void ) const {
    DTBUFFERINIT();
    DTBUFFER( 
            << "uu = [ " <<  getUMin() << ", " << getUMax() << "]" << LOGDEL
            << "vv = [ " <<  getVMin() << ", " << getVMax() << "]" << LOGDEL
            << "kind = " << _SISLSurfP->ikind << LOGDEL
            << "dim = " << _SISLSurfP->idim << LOGDEL
            << DTLOGEVAL(_SISLSurfP->ik1) << LOGDEL
            << DTLOGEVAL(_SISLSurfP->ik2) << LOGDEL
            << DTLOGEVAL(_SISLSurfP->in1) << LOGDEL
            << DTLOGEVAL(_SISLSurfP->in2) << LOGDEL
            << DTLOGEVAL(_SISLSurfP->ikind) << LOGDEL
            << DTLOGEVAL(_SISLSurfP->idim) << LOGDEL
            << DTLOGEVAL(_SISLSurfP->icopy) << LOGDEL
            << DTLOGEVAL(_SISLSurfP->use_count) << LOGDEL
            << DTLOGEVAL(_SISLSurfP->cuopen_1) << LOGDEL
            << DTLOGEVAL(_SISLSurfP->cuopen_2) << LOGDEL
            << DTLOGEVAL(_cLengthMin) << LOGDEL
            << DTLOGEVAL(_cLengthMax) );
    
//    int nPoints;
//    //
//    //et1
//    //
//    nPoints = _SISLSurfP->in1 * _SISLSurfP->idim;
//    for (int ii=0;ii<nPoints;ii++) {
//      DTBUFFER( << "_SISLSurfP->et1[" << ii << "] = " << _SISLSurfP->et1[ii] << LOGDEL);
//    }
//    //
//    //et2
//    //
//    nPoints = _SISLSurfP->in2 * _SISLSurfP->idim;
//    for (int ii=0;ii<nPoints;ii++) {
//      DTBUFFER( << "_SISLSurfP->et2[" << ii << "] = " << _SISLSurfP->et2[ii] << LOGDEL);
//    }
//    //
//    //ecoef
//    //
//    nPoints = _SISLSurfP->in1 * _SISLSurfP->in2 * _SISLSurfP->idim;
//    for (int ii=0;ii<nPoints;ii++) {
//      DTBUFFER( << "_SISLSurfP->ecoef[" << ii << "] = " << _SISLSurfP->ecoef[ii] << LOGDEL);
//    }
//    if ( (_SISLSurfP->ikind == 2) || (_SISLSurfP->ikind == 4) ) {
//      //
//      //rcoef
//      //
//      nPoints = _SISLSurfP->in1 * _SISLSurfP->in2 * _SISLSurfP->idim;
//      for (int ii=0;ii<nPoints;ii++) {
//        DTBUFFER( << "_SISLSurfP->rcoef[" << ii << "] = " << _SISLSurfP->rcoef[ii] << LOGDEL);
//      }
//    }
    DTDEBUGWF_BUFFER(dump());
  }

  dtVector3 dtSislSurf::normal(float const uu, float const vv) const {
    int der = 1;
    double dbl_para[2];
    int errFlag;
    double dbl_point[3];
    double normalVector[3];

    dbl_para[0] =  (double) uu;
    dbl_para[1] =  (double) vv;

    s1421(_SISLSurfP, //SISLSurf *surf;
          der, //int der;
          dbl_para, //double parvalue[ ];
          &_leftknot1, //int *leftknot1;
          &_leftknot2, //int *leftknot2;
          dbl_point, //double derive[ ];
          normalVector, //double normal[ ];
          &errFlag); //int *stat; 
 
    if (errFlag == 1) {
      DTWARNINGWF(normal(),
              << "Surface is degenerate at the point, normal has zero length.");
    }
    else if (errFlag == 2) {
      DTWARNINGWF(normal(),
              << "Angle between tangents is less than the angular tolerance.");
    }
    
    allRight(errFlag, __FILE__, __LINE__);
    
            //create and return vector
    double length = sqrt( normalVector[0]*normalVector[0] 
                          + normalVector[1]*normalVector[1] 
                          + normalVector[2] * normalVector[2] );
    normalVector[0] = normalVector[0] / length;
    normalVector[1] = normalVector[1] / length;
    normalVector[2] = normalVector[2] / length;
    
    return dtVector3( normalVector[0], normalVector[1], normalVector[2] );
  }

  std::vector<dtVector3> dtSislSurf::firstDer(float const uu, float const vv) const {
    int der = 1;
    double dbl_para[2];
    int leftknot1 = 0;
    int leftknot2 = 0;
    int errFlag;
    double dbl_point[9];
    double normalVector[3];

    dbl_para[0] =  (double) uu;
    dbl_para[1] =  (double) vv;

    s1421(_SISLSurfP, //SISLSurf *surf;
          der, //int der;
          dbl_para, //double parvalue[ ];
          &leftknot1, //int *leftknot1;
          &leftknot2, //int *leftknot2;
          dbl_point, //double derive[ ];
          normalVector, //double normal[ ];
          &errFlag); //int *stat; 
    
    allRight(errFlag, __FILE__, __LINE__);
    
		std::vector<dtVector3> retVec(2);
    retVec[0] = dtVector3( dbl_point[3], dbl_point[4], dbl_point[5] );	
		retVec[1] = dtVector3( dbl_point[6], dbl_point[7], dbl_point[8] );	
		
		return retVec;
  }	
	
	std::vector<dtVector3> dtSislSurf::secondDer(float const uu, float const vv) const {
    int der = 2;
    double dbl_para[2];
    int leftknot1 = 0;
    int leftknot2 = 0;
    int errFlag;
    double dbl_point[18];
    double normalVector[3];

    dbl_para[0] =  static_cast<double>(uu);
    dbl_para[1] =  static_cast<double>(vv);

    s1421(_SISLSurfP, //SISLSurf *surf;
          der, //int der;
          dbl_para, //double parvalue[ ];
          &leftknot1, //int *leftknot1;
          &leftknot2, //int *leftknot2;
          dbl_point, //double derive[ ];
          normalVector, //double normal[ ];
          &errFlag); //int *stat; 
    
    allRight(errFlag, __FILE__, __LINE__);
            
	  std::vector<dtVector3> retVec(3);
    retVec[0] = dtVector3( dbl_point[ 9], dbl_point[10], dbl_point[11] );  
		retVec[1] = dtVector3( dbl_point[12], dbl_point[13], dbl_point[14] );  
		retVec[2] = dtVector3( dbl_point[15], dbl_point[16], dbl_point[17] );  
		
		return retVec;
  }

  void dtSislSurf::rotate( dtPoint3 const origin, dtVector3 const vector, const float angle ) {
    if (_SISLSurfP->idim != DIMTHREED ) {
      dt__THROW(rotate(),
              << DTLOGEVAL(_SISLSurfP->idim) << LOGDEL
              << " should be 3");
      return;
    }
    dtAffTransformation3 rotation = dtLinearAlgebra::getRotation(vector, angle);
    for (int ii=0;ii<dtSurface::getNControlPoints();ii++) {
      int pIndex = ii * _SISLSurfP->idim;
      dtVector3 toRot = dtSurface::controlPoint(ii) - origin;     
      toRot = rotation.transform(toRot);
      dtPoint3 rotPoint = origin + toRot;
      _SISLSurfP->ecoef[pIndex] = rotPoint.x();
      _SISLSurfP->ecoef[pIndex+1] = rotPoint.y();
      _SISLSurfP->ecoef[pIndex+2] = rotPoint.z();
    }
    updateCLength();
  }

  dtPoint3 dtSislSurf::getControlPoint3d(int const uI, int const vI) const {  
		int pIndex = (uI + vI * _SISLSurfP->in1)*3;
    return dtPoint3( _SISLSurfP->ecoef[pIndex], 
										 _SISLSurfP->ecoef[pIndex+1], 
										 _SISLSurfP->ecoef[pIndex+2] );
  }

  void dtSislSurf::setControlPoint3d(int const uI, int const vI, dtPoint3 const point) {
    int pIndex = (uI + vI * _SISLSurfP->in1)*3;
    _SISLSurfP->ecoef[pIndex] = point.x();
    _SISLSurfP->ecoef[pIndex+1]  = point.y();
    _SISLSurfP->ecoef[pIndex+2] = point.z();
    
    updateCLength();
  }  

 dtCurve * dtSislSurf::getCurveConstU( float const uu, 
                                            float const vvMin, 
                                            float const vvMax) const {
    double uuD = (double) uu;
    
    SISLCurve * newCurve; 
    int errFlag;
    s1439(_SISLSurfP, uuD, 1, &newCurve, &errFlag);
    allRight(errFlag, __FILE__, __LINE__);
    
    dtSislCurve * rCurve = new dtSislCurve(*newCurve);
    freeCurve(newCurve);
    rCurve->trim(vvMin, vvMax);
    
    return rCurve;
  }

  dtCurve * dtSislSurf::getCurveConstV( float const vv, 
                                            float const uuMin, 
                                            float const uuMax) const {
    double vvD = (double) vv;
    
    SISLCurve * newCurve; 
    int errFlag;
    s1439(_SISLSurfP, vvD, 2, &newCurve, &errFlag);
    allRight(errFlag, __FILE__, __LINE__);
    
    dtSislCurve * rCurve = new dtSislCurve(*newCurve);
    rCurve->trim(uuMin, uuMax);
    
    freeCurve(newCurve);
    
    return rCurve;
  }  
  
  dtCurve * dtSislSurf::getCurveSpline(dtCurve2d const * const parameterSpline) const {
    SISLCurve * approxCurve;
    int errFlag;
    double epsgeD = dtSislBase::refToMinPointDistance();
    double maxstepD = 0.;
    s1383(
      _SISLSurfP, 
      const_cast< SISLCurve * >( 
        cast2Sisl(parameterSpline)->getSISLCurve() 
      ), 
      epsgeD, 
      maxstepD, 
      0, 
      &approxCurve, 
      NULL,
      NULL, 
      &errFlag
    );
   
    allRight(errFlag, __FILE__, __LINE__);
    
    dtCurve * retCurve = new dtSislCurve(*approxCurve);
    
    //
    // force closing after approximation
    //
    if ( parameterSpline->closed() ) {
      retCurve->closeArithmetic();
    }
    
    freeCurve(approxCurve);
    return retCurve;
  }
  
  dtCurve * dtSislSurf::getCurveLinear( float const uuStart, float const vvStart,
                                        float const uuEnd, float const vvEnd ) const {
    //
    // create linear spline
    //
    std::vector< dtPoint2 > point;
    point.push_back( dtPoint2(uuStart, vvStart) );
    point.push_back( dtPoint2(uuEnd, vvEnd) );
    dtSislCurve2d parameterSpline = dtSislCurve2d( &point, 2);
    
    return getCurveSpline(&parameterSpline);
  }

  void dtSislSurf::revert(void) {
    SISLSurf * twin = copySurface( _SISLSurfP );
    freeSurf(_SISLSurfP);
    
    int errFlag;
    s1440(twin, &_SISLSurfP, &errFlag);
    allRight(errFlag, __FILE__, __LINE__);
  }
  
	/**
   * @todo: Check this routine. Seems to move only one point?
   */
  void dtSislSurf::offsetNormal(float const nn) {
    if ( getKind() == dtSislBase::POLYB ) {
      typedef std::vector< dtPoint3 > newCPEl;
      std::vector< newCPEl > newCP;
      
      //
      // allocate
      //
      newCP.reserve(getNControlPointsU());
      for (int ii=0;ii<getNControlPointsU(); ii++) {
        newCP[ii].reserve( getNControlPointsV() );
      }
      
      DTBUFFERINIT();
      int ii = getNControlPointsU() / 2;
      int jj = getNControlPointsV() / 2;
//      for (int ii=0;ii<getNControlPointsU();ii++) {
//        for (int jj=0;jj<getNControlPointsV();jj++) {
          dtPoint3 cPXYZ = getControlPoint3d(ii, jj);
          dtPoint2 cPUV = reparam( cPXYZ );
          dtVector3 vv = normal( cPUV.x(), cPUV.y());
          newCP[ii][jj] = cPXYZ + nn * vv;
          DTBUFFER( << DTLOGPOI3D(cPXYZ) << " | " << DTLOGPOI2D(cPUV) << " | " << DTLOGVEC3D(vv) << LOGDEL );
//        }
//      }
      DTDEBUGWF_BUFFER(offsetNormal());
      
//      for (int ii=0;ii<getNControlPointsU();ii++) {
//        for (int jj=0;jj<getNControlPointsV();jj++) {
          setControlPoint3d( ii, jj, newCP[ii][jj] );
//        }
//      }

      
      for (int ii=0;ii<getNControlPointsU(); ii++) {
        newCP[ii].clear();
      }
      newCP.clear(); 
    }
    else {
      dt__THROW(offsetNormal(), << "Not yet supported for rational surfaces.");
    }
    
    updateCLength();
  }

  dtPoint2 dtSislSurf::reparam(dtPoint3 const point) const {
    int errFlag;
    double epoint[DIMTHREED];
    double gpar[2];
    double dist;
    epoint[0] = point.x(); epoint[1] = point.y(); epoint[2] = point.z();
    
    double geoRes = dtSislBase::refToGeometricResolution() * _cLengthMin;
    s1958(
      _SISLSurfP, 
      epoint, 
      DIMTHREED, 
      0., //not used
      geoRes,
      gpar, 
      &dist, 
      &errFlag
    );

//    DTINFOWF(getClosestPointParameter(),
//            << DTLOGEVAL( _cLengthMin ) << LOGDEL
//            << DTLOGEVAL( _cLengthMax ) << LOGDEL
//            << DTLOGEVAL( geoRes ) << LOGDEL
//            << DTLOGEVAL( dist ) << LOGDEL
//            << DTLOGPOI3D( point ) << LOGDEL
//            << DTLOGPOI3D( getPoint3d(gpar[0], gpar[1]) ) << LOGDEL
//            << DTLOGEVAL(gpar[0]) << LOGDEL
//            << DTLOGEVAL(gpar[1]) );
    
    //
    // check error
    //
    allRight(errFlag, __FILE__, __LINE__);
    
    //
    // check precision
    //
    bool inPrecision = false;
    if (dist <= (dtSislBase::refToMinPointDistance()*_cLengthMin) ) {
      inPrecision = true;
    }
    
    //
    // check if cheap routine is enough
    //
    if (inPrecision) {
      return dtPoint2(gpar[0], gpar[1]);
    }
    else {
      //
      // no point found, try extend routine
      //
      return getClosestPointParameterExtend(point);
    }
  }

  dtPoint2 dtSislSurf::getClosestPointParameterExtend( dtPoint3 const point ) const {
    int errFlag;
    double epoint[DIMTHREED];
    epoint[0] = point.x();
    epoint[1] = point.y();
    epoint[2] = point.z();

    double retU;
    double retV;
    
    int nClosestPoints;
    double * closestPoints;
    int nClosestCurves;    
    SISLIntcurve ** closestCurves;
    double geoRes = dtSislBase::refToGeometricResolution() * _cLengthMin;
    s1954(
      _SISLSurfP, 
      epoint, 
      DIMTHREED, 
      0., // not used
      geoRes,
      &nClosestPoints, 
      &closestPoints,
      &nClosestCurves,
      &closestCurves,
      &errFlag
    );
    
    allRight(errFlag, __FILE__, __LINE__);  
    
    if ( nClosestPoints > 0 ) {      
      retU = closestPoints[0];
      retV = closestPoints[1];
      
    }
    else if (nClosestCurves > 0) {
      retU = closestCurves[0]->epar1[0];
      retV = closestCurves[0]->epar1[1];
    }
    else {
      dt__THROW(getClosestPointParameterExtend(),
              << DTLOGEVAL(nClosestPoints) << LOGDEL
              << DTLOGEVAL(nClosestCurves) << LOGDEL
              << DTLOGPOI3D(point) << LOGDEL
              << "No point found.");      
    }
    
    //
    // check distance
    //
    dtPoint3 retPoint = point((float) retU, (float) retV);
    dtVector3 diffPoint = retPoint - point;   
    double dist  = sqrt(diffPoint.squared_length());

    if (dist <= (dtSislBase::refToMinPointDistance()*_cLengthMin) ) {
      DTWARNINGWF(
			  getClosestPointParameterExtend(),
				<< "Distance of point to reparameterized point is bigger than expected." << LOGDEL
				<< "Should be smaller than " << dtSislBase::refToMinPointDistance()*_cLengthMin << LOGDEL
				<< "Is " << DTLOGEVAL(dist) << LOGDEL
				<< DTLOGEVAL( _cLengthMin ) << LOGDEL
				<< DTLOGEVAL( _cLengthMax ) << LOGDEL
				<< DTLOGEVAL( geoRes ) << LOGDEL              
				<< DTLOGPOI3D(point) << LOGDEL
				<< DTLOGPOI3D(retPoint) << LOGDEL
				<< DTLOGPOI3D(diffPoint) << LOGDEL
				<< DTLOGEVAL(dist) << LOGDEL
				<< DTLOGEVAL(retU) << LOGDEL
				<< DTLOGEVAL(retV) );
    }
    

    //
    // free memory
    //
    if ( nClosestPoints > 0) {   
      free(closestPoints);
    }

    if (nClosestCurves > 0) {
      freeIntcrvlist(closestCurves, nClosestCurves); // causes a segmentation fault
    }
    
    return dtPoint2(retU, retV);
  }  
 
 
  void dtSislSurf::sisl_newSurf(
         int in1, int in2, int ik1, int ik2, double *et1, double *et2, 
         double *ecoef, int ikind, int idim, int icopy,
         int * et1Size, int * et2Size, int * rcoefSize, int * ecoefSize) const {
    int i, j, J, jj, k;		/* loop variables                      */
    int k1, k2;                   /* Superfluous knots at the ends.      */
    int kdim;			/* Dimension indicator.                */
    /* Allocate space for surface.  */
    if (ikind == 2 || ikind == 4)
      kdim = idim + 1;
    else
      kdim = idim;

    /* Count superfluous knots at ends, first in u parameter directions. */

    if (ik1 == 0)
    {
      k1 = k2 = 0;
    }
    else
    {
      /* Count superfluous knots in the start. */

      for (k1 = 0; k1 < in1; k1++)
        if (et1[ik1 - 1] < et1[ik1 + k1]) break;

      /* Count superfluous knots in the end. */

      for (k2 = 0; k2 < in1; k2++)
        if (et1[in1] > et1[in1 - 1 - k2]) break;
    }

    /* Reduce knots and vertices according to k1 and k2. */
    in1 -= (k1 + k2);

    /* Count superfluous knots at ends in v parameter directions. */

    if (ik2 == 0)
    {
      k1 = k2 = 0;
    }
    in2 -= (k1 + k2);
    *et1Size = in1+ik1;
    *et2Size = in2+ik2;
    if (ikind == 2 || ikind == 4)
      {
        *ecoefSize = in1 * in2 * idim;
        *rcoefSize = in1 * in2 * kdim;        
      }
    else
      {
        *ecoefSize = in1 * in2 * idim;
        *rcoefSize = 0;
      }
  }

  int dtSislSurf::getDimension( void ) const {
    return _SISLSurfP->idim;
  }  

  dtSislSurf * dtSislSurf::getSurface(
									float const uuMin, float const uuMax,
									float const vvMin, float const vvMax
								) const {

		int errFlag;
		SISLSurf * rsnew;
		double uuMinD = floatHandling::boundToRange(uuMin, getUMinD(), getUMaxD());//(double) uuMin;
		double uuMaxD = floatHandling::boundToRange(uuMax, getUMinD(), getUMaxD());//(double) uuMax;
		double vvMinD = floatHandling::boundToRange(vvMin, getVMinD(), getVMaxD());//(double) vvMin;
		double vvMaxD = floatHandling::boundToRange(vvMax, getVMinD(), getVMaxD());//(double) vvMax;
		s1001(
			const_cast< SISLSurf * >(_SISLSurfP), 
			uuMinD, 
			vvMinD, 
			uuMaxD, 
			vvMaxD, 
			&rsnew, 
			&errFlag
		);

		allRight(errFlag, __FILE__, __LINE__);

		dtSislSurf * retSurf = new dtSislSurf( *rsnew );

		freeSurf(rsnew);

		return retSurf;
	}

  std::vector< dtPoint3 > dtSislSurf::getBoundingBox3d( void ) const {
    if (getDimension() != DIMTHREED ) {
      dt__THROW(getBoundingBox3d(),
              << DTLOGEVAL( getDimension() ) << LOGDEL
              << " should be 3");      
    }
    int errFlag;
    double * emax;
    double * emin;
    s1989(
      _SISLSurfP, 
      &emax, 
      &emin, 
      &errFlag
    );
    
    allRight(errFlag, __FILE__, __LINE__);
    
    std::vector< dtPoint3 > ret;
    ret.push_back( dtPoint3(emin[0], emin[1], emin[2]) );
    ret.push_back( dtPoint3(emax[0], emax[1], emax[2]) );
    
		free(emin); free(emax);
		
    return ret;
  }
    
  void dtSislSurf::updateCLength( void ) {
		std::vector< dtPoint3 > pp = getBoundingBox3d();
		dtVector3 vv = pp[0] - pp[1];
		vv = dtVector3(fabs(vv.x()), fabs(vv.y()), fabs(vv.z()));
		_cLengthMin = std::min<float>( std::min<float>(vv.x(), vv.y()), vv.z() );
		_cLengthMax = std::max<float>( std::min<float>(vv.x(), vv.y()), vv.z() );
		return;
  }
	
	dtSislCurve const * dtSislSurf::cast2Sisl( dtCurve const * curve ) const {
		dtSislCurve const * sislC;
		dt__MUSTDOWNCAST(curve, dtSislCurve const, sislC);
		return sislC;
	}
	
  dtSislCurve * dtSislSurf::cast2Sisl( dtCurve * curve ) const {
		dtSislCurve * sislC;
		dt__MUSTDOWNCAST(curve, dtSislCurve, sislC);
		return sislC;		
	}

	dtSislCurve2d const * dtSislSurf::cast2Sisl( dtCurve2d const * curve ) const {
		dtSislCurve2d const * sislC;
		dt__MUSTDOWNCAST(curve, dtSislCurve2d const, sislC);
		return sislC;
	}
	
  dtSislCurve2d * dtSislSurf::cast2Sisl( dtCurve2d * curve ) const {
		dtSislCurve2d * sislC;
		dt__MUSTDOWNCAST(curve, dtSislCurve2d, sislC);
		return sislC;		
	}
	
	SISLSurf const * dtSislSurf::SISLPtr( void ) const {
		return _SISLSurfP;
	}
	
	SISLSurf const & dtSislSurf::SISLRef( void ) const {
		return *_SISLSurfP;
	}
}