#include "dtSislCurve2d.h"
#include "progHelper.h"

#include <logMe/logMe.h>
#include <stdio.h>
#include <sisl.h>
#include <sisl-copyright.h>
#include <sislP.h>

#include <interfaceHeaven/floatHandling.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {  
  dtSislCurve2d::dtSislCurve2d() : dtSislBase() {
    _SISLCurveP = NULL;
    _leftknot = 0;
    
  }

//  dtSislCurve2d::dtSislCurve2d( dtSislBase const * base ) : dtSislBase() {
//    _SISLCurveP = NULL;
//    dtSislCurve2d const * dC;
//    dt__MUSTDOWNCAST(base, dtSislCurve2d const, dC);
//    
//    _SISLCurveP = copyCurve( const_cast< SISLCurve * >(dC->_SISLCurveP) );
//    _leftknot = 0;
//  }
  
//  dtSislCurve2d::dtSislCurve2d( SISLCurve const & curve) : dtSislBase() {
//    _SISLCurveP = NULL;
//    _SISLCurveP = copyCurve( const_cast< SISLCurve * > (&curve) );
//    _leftknot = 0;
//  }

  dtSislCurve2d::dtSislCurve2d( std::vector < dtPoint2 > const * const pointV, int const order ) : dtSislBase() {
    _SISLCurveP = NULL;
    pointConstruct2d(pointV, order);
    _leftknot = 0;
  }
  
  dtSislCurve2d::dtSislCurve2d(const dtSislCurve2d& orig) : dtSislBase() {
    _SISLCurveP = NULL;
    _SISLCurveP = copyCurve( orig._SISLCurveP );
    _leftknot = 0;
  }

  dtSislCurve2d::dtSislCurve2d( dtSislCurve2d const * const orig) : dtSislBase() {
    _SISLCurveP = NULL;
    _SISLCurveP = copyCurve( orig->_SISLCurveP );
    _leftknot = 0;
  }
  
  dtSislCurve2d::dtSislCurve2d( std::string const attribute, std::vector < dtPoint2 > const * const pointV, float const factor ) : dtSislBase() {
    _SISLCurveP = NULL;
    double * pointsArray = new double[ pointV->size() * DIMTWOD];
    int counter = 0;
    for (int ii = 0; ii< pointV->size();ii=ii++ ) {
      pointsArray[counter] = pointV->at(ii).x();
      pointsArray[counter+1] = pointV->at(ii).y();
      counter = counter + 2;
    }
    int errFlag;
    double factorD = (double) factor;
    if (attribute == "conicArc") {
      s1011(
        pointsArray, 
        pointsArray+2, 
        pointsArray+4, 
        factorD, 
        DIMTWOD, 
        &_SISLCurveP, 
        &errFlag);     
    }
    else {
      dt__THROW(dtSislCurve2d(),
              << "Unknown " << DTLOGEVAL(attribute) );
    }
    //
    // destructor
    //
    delete [] pointsArray;
    
    //
    //error check
    //
    allRight(errFlag, __FILE__, __LINE__);
    _leftknot = 0;
  }
	
	dtSislCurve2d * dtSislCurve2d::clone( void ) const {
		return new dtSislCurve2d( *this );
	}
  
  dtSislCurve2d::~dtSislCurve2d() {
    if (_SISLCurveP) freeCurve(_SISLCurveP);
  //  errStr.clear();
  }
  
  void dtSislCurve2d::pointConstruct2d( std::vector < dtPoint2 > const * const pointV, int const order ) {
    //
    // open/close check
    //
    int openFlag;
    int nPoints;
    if ( CGAL::compare_xy(pointV->at(0), pointV->at(pointV->size()-1)) == CGAL::EQUAL ) {
      openFlag = 0;
      nPoints = pointV->size();
    }
    else {
      openFlag = 1;
      nPoints = pointV->size();
    }
    
    //
    // fill array
    //
    double * pointsArray = new double[ nPoints * DIMTWOD ];    
    int counter  = 0;
    for (int ii=0;ii<nPoints;ii++ ) {
      pointsArray[counter] = pointV->at(ii).x();
      pointsArray[counter+1] = pointV->at(ii).y();
      counter =  counter+2;
    }

    //
    // create curve
    //
    int errFlag;    
    double startPara = 0.;
    s1630( pointsArray, //epoint[]
           pointV->size(), //inbpnt;
           startPara, //astpar;
           openFlag, //iopen;
           DIMTWOD, //idim;
           order, //ik;
           &_SISLCurveP, //**rc;
           &errFlag);//*jstat;
    allRight(errFlag, __FILE__, __LINE__);
    
    delete[] pointsArray;
  }  

  float dtSislCurve2d::minPara( int const & dir) const {
    switch (dir) {
      case 0:
				double startPara;
				double endPara;
				int errFlag;
				s1363(_SISLCurveP, //*curve
							&startPara, //*startpar
							&endPara, //*endpar
							&errFlag);//*stat
				
				allRight(errFlag, __FILE__, __LINE__);
				
				return static_cast<float>(startPara);
        break;
      default:
        dt__THROW(minPara(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0.");
    }    
  }
	
  float dtSislCurve2d::maxPara( int const & dir) const {
    switch (dir) {
      case 0:
				double startPara;
				double endPara;
				int errFlag;
				s1363(_SISLCurveP, //*curve
							&startPara, //*startpar
							&endPara, //*endpar
							&errFlag);//*stat
				
				allRight(errFlag, __FILE__, __LINE__);
				
				return static_cast<float>(endPara);
        break;
      default:
        dt__THROW(maxPara(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0.");
    }    
  }

  int dtSislCurve2d::kind( void ) const {
    return _SISLCurveP->ikind;
  }

  int dtSislCurve2d::order( void ) const {
    return _SISLCurveP->ik;
  }  

  bool dtSislCurve2d::closed( void ) const {
    if (_SISLCurveP->cuopen == 0) {
      return true;
    }
    return false;
  }  

  dtPoint2 dtSislCurve2d::point(float const uu) const {
    int der = 0;
    double dbl_para = (double) uu;
    int leftknot;
    int errFlag;
    double dbl_point[2];

    s1227(_SISLCurveP, //SISLCurve*
          der,
          dbl_para, //parvalue
          &_leftknot, //leftknot*
          dbl_point, //derive[]
          &errFlag); //*stat;
    
    allRight(errFlag, __FILE__, __LINE__);
    
    return dtPoint2( dbl_point[0] , dbl_point[1] );
  }

//  dtVector2 dtSislCurve2d::normal(float const uu) const {
//    int der = 1;
//    double dbl_para = (double) uu;
//    int _leftknot;
//    int errFlag;
//    double dbl_pointLeft[4];
//    std::vector< double > yyV(2,0);
//    
//    s1227(_SISLCurveP, //SISLCurve*
//          der,
//          dbl_para, //parvalue
//          &_leftknot, //leftknot*
//          dbl_pointLeft, //derive[]
//          &errFlag); //*stat;
//    allRight(errFlag, __FILE__, __LINE__);
//    
//    double xP = (float) dbl_pointLeft[2];
//    double yP = (float) dbl_pointLeft[3];
//    if ( (xP == 0.) && (yP == 0.)) {
//      dt__THROW(normal(),
//              << DTLOGEVAL(xP) << LOGDEL
//              << DTLOGEVAL(yP) );      
//    } 
//    else if ( xP == 0. ) {
//      DTWARNINGWF(normal(),
//              << DTLOGEVAL(xP) << LOGDEL
//              << DTLOGEVAL(yP) << LOGDEL
//              << "Adjusting to (1., 0.) ");
//      yyV[0] = 1.;
//      yyV[1] = 0.;
//    }
//    else {
//      yyV[1] = sqrt( 
//                 1.
//                 /
//                 ( (yP/xP)*(yP/xP) + 1 ) 
//               );
//      yyV[0] = - yP*yyV[1] / xP;
//    }
//    
//    //
//    // check orientation
//    //
//    double orientation = xP * yyV[1] - yP * yyV[0];
//    if (orientation < 0) {
//      DTDEBUGWF(normal(),
//              << DTLOGEVAL(xP) << LOGDEL
//              << DTLOGEVAL(yP) << LOGDEL
//              << DTLOGEVAL(yyV[0]) << LOGDEL
//              << DTLOGEVAL(yyV[1]) << LOGDEL
//              << DTLOGEVAL(orientation) << LOGDEL
//              << "Changing direction of normal vector.");
//      yyV[0] = -yyV[0];
//      yyV[1] = -yyV[1];
//    }
//    
//    return dtVector2( yyV[0], yyV[1] );
//  }

  dtVector2 dtSislCurve2d::firstDer(float const uu) const {
    int der = 1;
    double dbl_para = (double) uu;
    int _leftknot;
    int errFlag;
    double dbl_pointLeft[6];
		
    s1227(_SISLCurveP, //SISLCurve*
          der,
          dbl_para, //parvalue
          &_leftknot, //leftknot*
          dbl_pointLeft, //derive[]
          &errFlag); //*stat;
    allRight(errFlag, __FILE__, __LINE__);
    
    return dtVector2( dbl_pointLeft[2], dbl_pointLeft[3]);    
  }
 
  /**
   * @todo check for min bound 
   */
	float dtSislCurve2d::l_u( float const uu ) const {
    SISLCurve * trimmedCurve;
    int errFlag;
		double uuMin = minPara(0);
		double uuMax = maxPara(0);
		
    if (uu == uuMax) {
      trimmedCurve = _SISLCurveP;
    }
    else {
      double uuD = static_cast<double>(uu);
      if (uuD == uuMin) {
        return 0.;
      }
      s1712(_SISLCurveP, uuMin, uuD, &trimmedCurve, &errFlag);
      allRight(errFlag, __FILE__, __LINE__);
    }
            
    double length;
    s1240(trimmedCurve, dtSislBase::refToGeometricResolution(), &length, &errFlag);
    allRight(errFlag, __FILE__, __LINE__);
    return ( static_cast<float>(length) ); 
  }
 
  float dtSislCurve2d::u_l( float const length ) const {
    int const nIter = 100;
    std::vector< float > uu(nIter,0.);
    std::vector< float > lengthUU(nIter,0.);
    std::vector< float > delta(nIter,0.);
    std::vector< float > res(nIter,0.);

    float uuMin = minPara(0);
    float uuMax = maxPara(0);		
    float maxLength = l_u( uuMax );

      
    //
    // length is close to zero
    //
    if (length < 0.) {
      DTWARNINGWF(u_l(),
              << DTLOGEVAL(length) << LOGDEL
              << "Length is below zero." << LOGDEL
              << "Return " << DTLOGEVAL(uuMin) );
      return uuMin;      
    }
    else if (length > maxLength) {
      DTWARNINGWF(u_l(),
              << DTLOGEVAL(length) << LOGDEL
              << "Length is bigger than " << DTLOGEVAL(maxLength) << LOGDEL
              << "Return " << DTLOGEVAL(uuMax) );
      return uuMax;      
    }    
    else if (length <= (maxLength * dtSislBase::refToGeometricResolution()) ) {
      DTWARNINGWF(u_l(),
              << DTLOGEVAL(length) << LOGDEL
              << "Length is smaller than "
              << DTLOGEVAL(dtSislBase::refToGeometricResolution()) << " times " << maxLength );

      return uuMin;
    }
    else if (length >= (maxLength * (1. - dtSislBase::refToGeometricResolution()) ) ) {
      DTWARNINGWF(u_l(),
              << DTLOGEVAL(length) << LOGDEL
              << "Length is bigger than "
              << DTLOGEVAL(1. - dtSislBase::refToGeometricResolution()) << " times " << maxLength );

      return uuMax;
    }
    

    
    //
    // first guess
    //
//    uu[0] = uuMax;
    uu[0] = minU() + (length / maxLength) * maxU();
    lengthUU[0] = l_u(uu[0]);
    delta[0] = lengthUU[0] / length;
    res[0] = (lengthUU[0] - 0.) / lengthUU[0];
     
    //
    // iteration
    //
    float uuIterMin = uuMin;
    float uuIterMax = uuMax;
    int counter = 0;
    for (int ii=1;ii<nIter;ii++) {
      counter++;
      //
      // new guess
      //
      if ( delta[ii-1] > 1.) {
        uuIterMax = uu[ii-1];
      }
      else {
        uuIterMin = uu[ii-1];
      }
      uu[ii] = uuIterMin + .5 * (uuIterMax - uuIterMin);
      
      //
      // new length
      //
      lengthUU[ii] = l_u(uu[ii]);
      
      delta[ii] = lengthUU[ii] / length;
      res[ii] = (lengthUU[ii] - lengthUU[ii-1]);

      //
      // convergence check
      //
      if ( fabs(res[ii]) <= dtSislBase::refToGeometricResolution() ) {
        DTDEBUGWF(u_l(),
                << "Convergence after " << counter << " iterations.");
        break;
      }
    }
    
    //
    // output
    //
    DTDEBUGWF(u_l(),
            << "Result after " << counter << " iterations." << LOGDEL
            << DTLOGEVAL(length) << LOGDEL
            << DTLOGEVAL(maxLength) << LOGDEL
            << DTLOGEVAL(lengthUU[counter]) << " => " << delta[counter] << " * length" << LOGDEL
            << "uu = " << uu[counter]);
            
    
    return uu[counter];
  }
    
  float dtSislCurve2d::reparam(dtPoint2 const point) const {
		DTFUNCTIONNOTI(reparam);
	}
	
  SISLCurve const * dtSislCurve2d::getSISLCurve( void ) const{
    return _SISLCurveP;
  }

  int dtSislCurve2d::nControlPoints( void ) const {
    return _SISLCurveP->in;
  }

  dtPoint2 dtSislCurve2d::controlPoint(int const nPoint) const {
    if (_SISLCurveP->idim != DIMTWOD) {
      dt__THROW(controlPoint(),
              << DTLOGEVAL(_SISLCurveP->idim) << LOGDEL
              << "Should be 2");
    }
    int pIndex = nPoint * _SISLCurveP->idim;
    return dtPoint2( 
		  _SISLCurveP->ecoef[pIndex], 
      _SISLCurveP->ecoef[pIndex+1] 
		);
  }

  void dtSislCurve2d::setControlPoint(int const nPoint, dtPoint2 const point) {
    if (_SISLCurveP->idim != DIMTWOD) {
      dt__THROW(setControlPoint(),
              << DTLOGEVAL(_SISLCurveP->idim) << LOGDEL
              << "Should be 2");
    }        
    int pIndex = nPoint * _SISLCurveP->idim;
    _SISLCurveP->ecoef[pIndex] = point.x();
    _SISLCurveP->ecoef[pIndex+1] = point.y();
  }

  void dtSislCurve2d::translate( dtVector2 const transVec ) {
    for (int ii=0;ii<nControlPoints();ii++) {
      int pIndex = ii * _SISLCurveP->idim;
      _SISLCurveP->ecoef[pIndex] = _SISLCurveP->ecoef[pIndex] + transVec.x(); 
      _SISLCurveP->ecoef[pIndex+1] = _SISLCurveP->ecoef[pIndex+1] + transVec.y(); 
    }
  }

  void dtSislCurve2d::dump( void ) {
    DTDEBUGWF(dump(),
            << "uu = [ " <<  minPara(0) << ", " << maxPara(0) << "]" << LOGDEL
            << "kind = " << _SISLCurveP->ikind << LOGDEL
            << "dim = " << _SISLCurveP->idim << LOGDEL
            << DTLOGEVAL(_SISLCurveP->ik) << LOGDEL
            << DTLOGEVAL(_SISLCurveP->in) << LOGDEL
            << DTLOGEVAL(_SISLCurveP->ikind) << LOGDEL
            << DTLOGEVAL(_SISLCurveP->idim) << LOGDEL
            << DTLOGEVAL(_SISLCurveP->icopy) << LOGDEL
            << DTLOGEVAL(_SISLCurveP->cuopen) );
    DTBUFFERINIT();
    for (int ii=0; ii<nControlPoints(); ii++) {
      DTBUFFER(<< DTLOGPOI2D( controlPoint(ii) ) << LOGDEL);
    }
    DTINFOWF_BUFFER(dump());
  }

//  void dtSislCurve2d::closeStraight( void ) {
//		dtPoint2 endPoint = pointPercent(0.);
//		dtPoint2 startPoint = pointPercent(1.);
//
//		if ( CGAL::compare_xy(endPoint, startPoint) != CGAL::EQUAL ) {
//			DTWARNINGWF(closeStraight(),
//							<< DTLOGPOI2D(endPoint) << LOGDEL
//							<< DTLOGPOI2D(startPoint) << ")" << LOGDEL
//							<< "gap between endPoint and startPoint is filled with a linear spline!");      
//			std::vector< dtPoint2 > pointArray(2,dtPoint2(0,0));
//			pointArray[0] = endPoint;
//			pointArray[1] = startPoint;    
//			int order = 2;
//			ptrHandling<dtSislCurve2d> endExt( new dtSislCurve2d(&pointArray, order) );
//			SISLCurve * tmpCurve;
//			int errFlag;
//			s1715(
//				const_cast< SISLCurve * >(
//					getSISLCurve()
//				), 
//				const_cast< SISLCurve * >(
//					endExt->getSISLCurve()
//				),
//				1,
//				0,
//				&tmpCurve, 
//				&errFlag
//			);
//			allRight(errFlag, __FILE__, __LINE__);
//			freeCurve(_SISLCurveP);        
//			_SISLCurveP = tmpCurve;
//		}
//  }
  
  void dtSislCurve2d::connectArithmetic( dtCurve2d const * const toConnect ) {
    dtSislCurve2d * endExt = NULL;

		if ( !cast2Sisl(toConnect) ) {
			dt__THROW(connectArithmetic(),
							<< "Curve to connect with is not a dtSislCurve2d.");
		}
		dtPoint2 endPoint = pointPercent(1.);
		dtPoint2 startPoint = toConnect->pointPercent(0.);

		DTWARNINGWF(connectArithmetic(),
						<< DTLOGPOI2D(endPoint) << LOGDEL
						<< DTLOGPOI2D(startPoint) << ")" );

		//
		// create startExt
		//
		std::vector< dtPoint2 > pointArray;
		for (int ii = 0; ii<(nControlPoints()-1); ii++) {
			pointArray.push_back( controlPoint(ii) );
		}

		//
		// create endExt
		//
		for (int ii = 0; ii < toConnect->nControlPoints(); ii++) {
			pointArray.push_back( toConnect->controlPoint(ii) );
		}
		endExt = new dtSislCurve2d( &pointArray, toConnect->order() );
		pointArray.clear();

    freeCurve( _SISLCurveP );
    _SISLCurveP = copyCurve(
                    const_cast< SISLCurve * >(
                      endExt->getSISLCurve()
                    ) 
                 );

    delete endExt;
  }  
  
//  void dtSislCurve2d::closeArithmetic( void ) {
//    //
//    // check if curve is already closed
//    //
//    if (closed()) {
//      return;
//    }
//    
//		dtPoint2 endPoint = pointPercent(1.);
//		dtPoint2 startPoint = pointPercent(0.);
//
//		if ( CGAL::compare_xy(endPoint, startPoint) != CGAL::EQUAL ) {
//			DTWARNINGWF(connectArithmetic(),
//							<< DTLOGPOI2D(endPoint) << LOGDEL
//							<< DTLOGPOI2D(startPoint) << ")" );
//			dtPoint2 newPoint = dtPoint2(
//													 .5 * (startPoint.x() + endPoint.x()), 
//													 .5 * (startPoint.y() + endPoint.y())
//												 );
//
//			//
//			// create new curve
//			//
//			std::vector< dtPoint2 > pointArray;
//			pointArray.push_back( newPoint );        
//			for (int ii = 1; ii<nControlPoints(); ii++) {
//				pointArray.push_back( controlPoint(ii) );
//			}
//			pointArray[nControlPoints()-1] = newPoint;
//			int order = this->order();
//			freeCurve( _SISLCurveP );
//			pointConstruct2d( &pointArray, order );
//		}
//		//
//		// force closing curve
//		//
//		else {
//			std::vector< dtPoint2 > pointArray;
//			for (int ii = 0; ii<nControlPoints(); ii++) {
//				pointArray.push_back( controlPoint(ii) );
//			}
//			int order = this->order();        
//			freeCurve( _SISLCurveP );
//			pointConstruct2d( &pointArray, order );        
//		}
//  }  

  void dtSislCurve2d::revert( void ) {
    SISLCurve * twin = copyCurve( _SISLCurveP );
    freeCurve(_SISLCurveP);
    _SISLCurveP = twin;
    s1706( _SISLCurveP );
  }

//  void dtSislCurve2d::trim( float const uuStart, float const uuEnd ) {
//    SISLCurve * trimmedCurve;
//    int errFlag = 0;
//    double uuStartD = (double) uuStart;
//    double uuEndD = (double) uuEnd;
//		double uMinD = static_cast<float>(minPara(0));
//		double uMaxD = static_cast<float>(maxPara(0));
//		
//		bool isMin =  false;
//		bool isMax =  false;
//		
//    if ( uuStartD <= uMinD ) {
//      DTWARNINGWF(trim(),
//              << "Adjust " << DTLOGEVAL(uuStartD) << " to " << DTLOGEVAL(uMinD) );
//      uuStartD = uMinD;
//			isMin = true;
//    }
//    if ( uuEndD >= uMaxD ) {
//      DTWARNINGWF(trim(),
//              << "Adjust " << DTLOGEVAL(uuEndD) << " to " << DTLOGEVAL(uMaxD) );
//      uuEndD = uMaxD;
//			isMax = true;
//    }
//		if ( isMin && isMax ) {
//			return;
//		}
//    
//		if ( uuEndD < uMinD ) {
//      DTWARNINGWF(trim(),
//              << "Adjust " << DTLOGEVAL(uuEndD) << " to " << DTLOGEVAL(uMinD) );
//      uuEndD = uMinD;
//    }
//    if ( uuStartD > uMaxD ) {
//      DTWARNINGWF(trim(),
//              << "Adjust " << DTLOGEVAL(uuStartD) << " to " << DTLOGEVAL(uMaxD) );
//      uuStartD = uMaxD;
//    }
//		
//    s1712(_SISLCurveP, uuStartD, uuEndD, &trimmedCurve, &errFlag);
//    DTINFOWF(trim(),
//						<< DTLOGEVAL(uuStartD) << LOGDEL
//						<< DTLOGEVAL(uuEndD) << LOGDEL
//						<< DTLOGEVAL(uMinD) << LOGDEL
//						<< DTLOGEVAL(uMaxD) );
//		allRight(errFlag, __FILE__, __LINE__);
//    
//		SISLCurve * tmpCurve = _SISLCurveP;
//    _SISLCurveP = copyCurve(trimmedCurve);
//    freeCurve(trimmedCurve);
//    freeCurve(tmpCurve);
//  }

  void dtSislCurve2d::scale( dtPoint2 const norm ) {
		for (int ii=0;ii<nControlPoints();ii++) {
			//
			// get control point
			//
			dtPoint2 cPoint = controlPoint(ii);
			dtPoint2 normPoint(
								cPoint.x() * norm.x(), 
								cPoint.y() * norm.y()
							);
			setControlPoint(ii, normPoint);
		}
  }

//  void dtSislCurve2d::sisl_newCurve(
//    int in, int ik, double *et, double *ecoef, int ikind, int idim, int icopy, 
//    int * etSize, int * rcoefSize, int * ecoefSize
//  ) const {
////  SISLCurve *qnew;		/* Local pointer to new curve.  */
//    int i, j, J, jj, k;		/* loop variables               */
//    int k1,k2;                    /* Superflous knots in the ends. */
//    int kdim;			/* Dimension of space (also including potential
//             homogenous coordinate        */
////    double *st = NULL;		/* Copy of knotvector.          */
////    double *rcoef = NULL;		/* Copy of vertices in rational case.  */
////    double *scoef = NULL;		/* Copy of vertices.            */
//
//
//    /* Allocate space for curve.  */
//
////    if ((qnew = newarray (1, SISLCurve)) == SISL_NULL)
////      goto err101;
//
//    if (ikind == 2 || ikind == 4)
//      kdim = idim + 1;
//    else
//      kdim = idim;
//
//    /* Count superflous knots in the start.  */
//
//    for (k1=0; k1<in; k1++)
//       if (et[ik-1] < et[ik+k1]) break;
//
//    /* Count superflous knots in the end.  */
//
//    for (k2=0; k2<in; k2++)
//       if (et[in] > et[in-1-k2]) break;
//
//    /* Reduce knots and vertices according to k1 and k2.  */
//
//    if (k1 > 0)
//    {
////       memcopy(ecoef,ecoef+k1*kdim,(in-k1)*kdim,DOUBLE);
////       memcopy(et,et+k1,in+ik-k1,DOUBLE);
//    }
//    in -= (k1+k2);
//
//    /* Check if the curve is still valid. Otherwise return zero. */
//
////    if (in < ik) goto err101;
//
//    if (icopy == 1)
//      {
//
//        /* Copy input arrays. First allocate space for new arrays. */
//
////        if ((st = newarray (in +ik, DOUBLE)) == SISL_NULL ||
////      (scoef = newarray (in *kdim, DOUBLE)) == SISL_NULL)
////    goto err101;
//
//        /* Copy contents of arrays.  */
//
////        memcopy (st, et, in +ik, double);
////        memcopy (scoef, ecoef, in *kdim, double);
//      }
//    else
//      {
////        st = et;
////        scoef = ecoef;
//      }
//
//    /* Initialize new curve.  */
//
////    qnew->in = in;
////    qnew->ik = ik;
////    qnew->ikind = ikind;
////    qnew->idim = idim;
////    qnew->icopy = icopy;
////    qnew->et = st;
////    *etOut = st;
//    *etSize = in+ik;
////    qnew->pdir = SISL_NULL;
////    qnew->pbox = SISL_NULL;
//
//    if (ikind == 2 || ikind == 4)
//      {
//        /* Calculate the weighted control points if the object is rational  */
////        rcoef = newarray (in *idim, DOUBLE);
////        if (rcoef == SISL_NULL)
////    goto err101;
//        for (i = 0, j = 0, J = 0, k = idim; i < in; i++, k += kdim)
//    {
//      for (jj = 0; jj < idim; jj++, j++, J++)
//        {
////          rcoef[J] = scoef[j] / scoef[k];
//        }
//      j++;
//    }
////        qnew->ecoef = rcoef;
////        qnew->rcoef = scoef;
////        *ecoefOut = rcoef;
//        *ecoefSize = in*idim;
////        *rcoefOut = scoef;
//        *rcoefSize = in*kdim;
//      }
//    else
//      {
////        qnew->ecoef = scoef;
////        qnew->rcoef = NULL;
////        *ecoefOut = scoef;
//        *ecoefSize = in*kdim;
////        *rcoefOut = NULL;
//        *rcoefSize = 0;        
//      }
//
//
////    /* UJK, 92.03.27 Default value must be set for cuopen */
////    qnew->cuopen = 1;
//
//    /* Task done. */
////    goto out;
//
//    /* Error in space allocation. Return zero. */
//
////  err101:if (qnew != SISL_NULL)
////            { freearray (qnew);  qnew = SISL_NULL;}  
////    err101: if (st != SISL_NULL)
////      freearray (st);
////    if (rcoef != SISL_NULL)
////      freearray (rcoef);
////    if (scoef != SISL_NULL)
////      freearray (scoef);
////    goto out;
////
////  out:return; //(qnew);
//  }
  
//  std::vector< dtPoint2 > dtSislCurve2d::boundingBox( void ) {
//    int errFlag;
//    double * emax;
//    double * emin;
//    s1988(_SISLCurveP, 
//          &emax, 
//          &emin, 
//          &errFlag);
//    
//    allRight(errFlag, __FILE__, __LINE__);
//    
//    std::vector< dtPoint2 > ret;
//    ret.push_back( dtPoint2(emin[0], emin[1]) );
//    ret.push_back( dtPoint2(emax[0], emax[1]) );
//    
//    return ret;
//  }
  
//  std::vector< dtPoint2 > dtSislCurve2d::getIntersectionPoints2d(
//    dtPoint2 const point, 
//    dtVector2 const vector) const {
//    
//    int errFlag;
//    double pointD[2];
//    double vectorD[2];
//    pointD[0] = point.x();
//    pointD[1] = point.y();
//    vectorD[0] = vector.x();
//    vectorD[1] = vector.y();
//    double epsgeD = dtSislBase::refToGeometricResolution();
//    
//    int numintpt;
//    double * intpar;
//    int numintcu;
//    SISLIntcurve ** intcurve;
//    s1850( const_cast< SISLCurve * >(_SISLCurveP), 
//           pointD, 
//           vectorD, 
//           DIMTWOD, 
//           1., //not used 
//           epsgeD, 
//           &numintpt, 
//           &intpar,
//           &numintcu, 
//           &intcurve, 
//           &errFlag
//    );
//    
//    allRight(errFlag, __FILE__, __LINE__);
//    
//    std::vector< dtPoint2 > ret;
//    for (int ii=0;ii<numintpt;ii++) {
//      ret.push_back(
//        this->point( static_cast<float>(intpar[ii]) )
//      );
//    }
//
//    if (numintpt != 0) { 
//      free(intpar);
//    }
//    
//    if (numintcu != 0) { 
//      freeIntcrvlist(intcurve, numintcu);
//    }
//    
//    return ret;
//  }

//  float dtSislCurve2d::getParameterToX( float const xx ) const {
//    int errFlag;
//    double pointD[2];
//    double vectorD[2];
//    pointD[0] = (double) xx;
//    pointD[1] = 0.;
//    vectorD[0] = 1.;
//    vectorD[1] = 0;
//    double epsgeD = dtSislBase::refToGeometricResolution();
//    
//    int numintpt;
//    double * intpar;
//    int numintcu;
//    SISLIntcurve ** intcurve;
//    s1850( const_cast< SISLCurve * >(_SISLCurveP), 
//           pointD, 
//           vectorD, 
//           DIMTWOD, 
//           1., //not used 
//           epsgeD, 
//           &numintpt, 
//           &intpar,
//           &numintcu, 
//           &intcurve, 
//           &errFlag
//    );
//    
//    allRight(errFlag, __FILE__, __LINE__);
//    
//    if ( numintpt > 1) {
//      dt__THROW(getParameterToX(),
//              << "More than one intersection point found.");
//    }
//    
//    float retValue =  (float) intpar[0];
//
//    if (numintpt != 0) {
//      free(intpar);
//    }
//    
//    if (numintcu != 0) {
//      freeIntcrvlist(intcurve, numintcu);
//    }
//    
//    return retValue;
//  }
  
//  float dtSislCurve2d::getFunctionValue2d( float const xx ) const {
//    std::vector< dtPoint2 > pp = getIntersectionPoints2d(
//                   dtPoint2(xx, 0.),
//                   dtVector2(1., 0.)
//                 );
//    
//    if (pp.size() > 1) {
//      DTBUFFERINIT();
//      DTBUFFER( << "More than one intersection point found." << LOGDEL );
//      for (int ii=0;ii<pp.size();ii++) {
//        DTBUFFER( << DTLOGPOI2D(pp[ii]) << LOGDEL );
//      }
//      DTBUFFER( << "Returning point " << DTLOGPOI2D(pp[0]) );
//      DTDEBUGWF_BUFFER( getFunctionValue2d() );
//    }
//    else if (pp.size() == 0) {
//      dt__THROW(getFunctionValue2d(),
//              << "No Point at " << DTLOGEVAL(xx) << " found.");
//    }
//    
//    return pp[0].y();
//  }  

	dtSislCurve2d const * dtSislCurve2d::cast2Sisl( dtCurve2d const * curve ) const {
		dtSislCurve2d const * dtSislCurve2dP;
		dt__MUSTDOWNCAST(curve, dtSislCurve2d const, dtSislCurve2dP);
		return dtSislCurve2dP;
	}
	
  dtSislCurve2d * dtSislCurve2d::cast2Sisl( dtCurve2d * curve ) const {
		dtSislCurve2d * dtSislCurve2dP;
		dt__MUSTDOWNCAST(curve, dtSislCurve2d, dtSislCurve2dP);
		return dtSislCurve2dP;		
	}	
}