#include "dtSislCurve.h"
#include "progHelper.h"

#include <logMe/logMe.h>
#include <stdio.h>
#include <sisl.h>
#include <sisl-copyright.h>
#include <sislP.h>

#include <interfaceHeaven/floatHandling.h>

namespace dtOO { 
  dtSislCurve::dtSislCurve() : dtSislBase(), dtCurve() {
    _SISLCurveP = NULL;
    _leftknot = 0;
    
  }

  dtSislCurve::dtSislCurve( dtSislBase const * base ) : dtSislBase(), dtCurve() {
    _SISLCurveP = NULL;
    dtSislCurve const * dC;
    dt__MUSTDOWNCAST(base, dtSislCurve const, dC);
    
    _SISLCurveP = copyCurve( const_cast< SISLCurve * >(dC->_SISLCurveP) );
    _leftknot = 0;
  }
  
  dtSislCurve::dtSislCurve( SISLCurve const & curve) : dtSislBase(), dtCurve() {
    _SISLCurveP = NULL;
    _SISLCurveP = copyCurve( const_cast< SISLCurve * > (&curve) );
    _leftknot = 0;
  }

  dtSislCurve::dtSislCurve( std::vector < dtPoint3 > const * const pointV, int const order ) : dtSislBase(), dtCurve() {
    _SISLCurveP = NULL;
    pointConstruct3d(pointV, order);
    _leftknot = 0;
  }

  dtSislCurve::dtSislCurve(const dtSislCurve& orig) : dtSislBase(), dtCurve() {
    _SISLCurveP = NULL;
    _SISLCurveP = copyCurve( orig._SISLCurveP );
    _leftknot = orig._leftknot;
  }

  dtSislCurve::dtSislCurve( dtSislCurve const * const orig) : dtSislBase(), dtCurve() {
    _SISLCurveP = NULL;
    _SISLCurveP = copyCurve( orig->_SISLCurveP );
    _leftknot = orig->_leftknot;
  }
  	
	dtSislCurve * dtSislCurve::clone( void ) const {
		return new dtSislCurve( *this );
	}
  
  dtSislCurve::~dtSislCurve() {
    if (_SISLCurveP) freeCurve(_SISLCurveP);
  //  errStr.clear();
  }
  
  void dtSislCurve::pointConstruct3d( std::vector < dtPoint3 > const * const pointV, int const order ) {
    //
    // open/close check
    //
    int openFlag;
    int nPoints = pointV->size();;

//    dtVector3 dist = pointV->at(0) - pointV->at(pointV->size()-1);
//    DTDEBUGWF(pointConstruct3d(),
//            << DTLOGEVAL( sqrt(dist.squared_length()) ) );
    
    if ( CGAL::compare_xyz(pointV->at(0), pointV->at(pointV->size()-1)) == CGAL::EQUAL ) {
      if ( nPoints == 2 ) {
        dt__THROW(pointConstruct3d(),
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
    int numPoints = pointV->size();
    double * pointsArray = new double[ nPoints * DIMTHREED ];    
    int counter  = 0;
    pointsArray[counter] = pointV->at(0).x();
    pointsArray[counter+1] = pointV->at(0).y();
    pointsArray[counter+2] = pointV->at(0).z();
    counter =  counter+3;
    for (int ii=1;ii<nPoints;ii++ ) {
      pointsArray[counter] = pointV->at(ii).x();
      pointsArray[counter+1] = pointV->at(ii).y();
      pointsArray[counter+2] = pointV->at(ii).z();
      if (     (pointsArray[counter+0] == pointsArray[counter+0-3])
            && (pointsArray[counter+1] == pointsArray[counter+1-3])
            && (pointsArray[counter+2] == pointsArray[counter+2-3])
         ) {
        numPoints--;
        DTINFOWF(pointConstruct3d(),
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
    s1630( pointsArray, //epoint[]
           numPoints, //inbpnt;
           startPara, //astpar;
           openFlag, //iopen;
           DIMTHREED, //idim;
           order, //ik;
           &_SISLCurveP, //**rc;
           &errFlag);//*jstat;
    allRight(errFlag, __FILE__, __LINE__);
    
    delete [] pointsArray;
  }

  float dtSislCurve::getMin( int const & dir) const {
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
        dt__THROW(getMin(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0.");
    }    
  }
	
  float dtSislCurve::getMax( int const & dir) const {
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
        dt__THROW(getMax(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0.");
    }    
  }

  int dtSislCurve::getKind( void ) const {
    return _SISLCurveP->ikind;
  }

  int dtSislCurve::getOrder( void ) const {
    return _SISLCurveP->ik;
  }  

  bool dtSislCurve::isClosed( void ) const {
    if (_SISLCurveP->cuopen == 0) {
      return true;
    }
    return false;
  }  

  dtPoint3 dtSislCurve::getPoint3d(float const uu) const {
    int der = 0;
    double dbl_para = (double) uu;
    int leftknot = 0;
    int errFlag;
    double dbl_point[3];

    s1227(_SISLCurveP, //SISLCurve*
          der,
          dbl_para, //parvalue
          &_leftknot, //leftknot*
          dbl_point, //derive[]
          &errFlag); //*stat;
    
    allRight(errFlag, __FILE__, __LINE__);
    
    return dtPoint3( dbl_point[0] , dbl_point[1], dbl_point[2] );
  }

  dtVector3 dtSislCurve::firstDer(float const uu) const {
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
    
    return dtVector3( dbl_pointLeft[3], dbl_pointLeft[4], dbl_pointLeft[5] );    
  }
 
  /**
   * @todo check for min bound 
   */
	float dtSislCurve::l_u( float const uu ) const {
		float uuMin = getMin(0);
		float uuMax = getMax(0);
    if (uu == uuMin) {
      return 0.;
    }

    double length;
    if (uu == uuMax) {
      int errFlag;
      s1240(
			  const_cast<SISLCurve*>(_SISLCurveP), 
				dtSislBase::refToGeometricResolution(), 
				&length, 
				&errFlag
			);			
      allRight(errFlag, __FILE__, __LINE__);			
    }
    else {
      if (uu == uuMin) {
        return 0.;
      }
			SISLCurve * trimmedCurve;
      double uuD = static_cast<double>(uu);
			int errFlag;
      s1712(_SISLCurveP, uuMin, uuD, &trimmedCurve, &errFlag);
      allRight(errFlag, __FILE__, __LINE__);

      s1240(trimmedCurve, dtSislBase::refToGeometricResolution(), &length, &errFlag);
		  allRight(errFlag, __FILE__, __LINE__);
			
			freeCurve( trimmedCurve );
    }
    return ( static_cast<float>(length) ); 
  }
 
  float dtSislCurve::u_l( float const length ) const {
    int const nIter = 100;
    std::vector< float > uu(nIter,0.);
    std::vector< float > lengthUU(nIter,0.);
    std::vector< float > delta(nIter,0.);
    std::vector< float > res(nIter,0.);

    float uuMin = getMin(0);
    float uuMax = getMax(0);		
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
    uu[0] = getUMin() + (length / maxLength) * getUMax();
    lengthUU[0] = l_u(uu[0]);
    delta[0] = lengthUU[0] / length;
    res[0] = (lengthUU[0] - 0.) / lengthUU[0];
     
    //
    // iteration
    //
		std::vector<float> iVal;
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
			float deltaAbs = lengthUU[ii] - length;
      res[ii] = (lengthUU[ii] - lengthUU[ii-1]);

			iVal.push_back(uu[ii]);iVal.push_back(uuIterMin); iVal.push_back(uuIterMax);
			iVal.push_back(lengthUU[ii]); iVal.push_back(res[ii]); iVal.push_back(delta[ii]);
			iVal.push_back(deltaAbs);
      //
      // convergence check
      //
      if ( fabs(deltaAbs) <= dtSislBase::refToGeometricResolution() ) {
//        DTDEBUGWF(u_l(),
//                << "Convergence after " << counter << " iterations.");
        break;
      }
    }
    
		std::vector< std::string > header;
		header.push_back("u"); header.push_back("u_min"); header.push_back("u_max");
		header.push_back("l(u)"); header.push_back("l(u)/l(u-1)"); header.push_back("l(u)/l");
		header.push_back("l(u)-l");
		
		DTINFOWF(u_l(),
						<< floatVecToTable(header, iVal) << LOGDEL
            << "Result after " << counter << " iterations." << LOGDEL
            << DTLOGEVAL(length) << LOGDEL
            << DTLOGEVAL(maxLength) << LOGDEL
            << DTLOGEVAL(lengthUU[counter]) << " => " << delta[counter] << " * length" << LOGDEL
            << "uu = " << uu[counter]);
            
    
    return uu[counter];
  }
	
	float dtSislCurve::reparam(dtPoint3 const point) const {
		DTFUNCTIONNOTI(reparam);
	}
    
  SISLCurve const & dtSislCurve::SISLRef( void ) const {
		return *_SISLCurveP;
	}
	
  SISLCurve const * dtSislCurve::getSISLCurve( void ) const{
    return _SISLCurveP;
  }

  int dtSislCurve::getNControlPoints( void ) const {
    return _SISLCurveP->in;
  }

  dtPoint3 dtSislCurve::getControlPoint3d(int const nPoint) const {
    if (_SISLCurveP->idim != DIMTHREED) {
      dt__THROW(getControlPoint3d(),
              << DTLOGEVAL(_SISLCurveP->idim) << LOGDEL
              << "Should be 3");
    }    
    int pIndex = nPoint * _SISLCurveP->idim;
    dtPoint3 point = dtPoint3( _SISLCurveP->ecoef[pIndex], 
                             _SISLCurveP->ecoef[pIndex+1], 
                             _SISLCurveP->ecoef[pIndex+2] );
    
    return point;
  }

  void dtSislCurve::setControlPoint3d(int const nPoint, dtPoint3 const point) {
    if (_SISLCurveP->idim != DIMTHREED) {
      dt__THROW(setControlPoint3d(),
              << DTLOGEVAL(_SISLCurveP->idim) << LOGDEL
              << "Should be 3");
    }        
    int pIndex = nPoint * _SISLCurveP->idim;
    _SISLCurveP->ecoef[pIndex] = point.x();
    _SISLCurveP->ecoef[pIndex+1] = point.y();
    _SISLCurveP->ecoef[pIndex+2] = point.z();
  }  
  
  void dtSislCurve::rotate( dtPoint3 const origin, dtVector3 const vector, const float angle ) {
    if (_SISLCurveP->idim != DIMTHREED ) {
      dt__THROW(rotate(),
              << DTLOGEVAL(_SISLCurveP->idim) << LOGDEL
              << " should be 3");
      return;
    }
    
    dtAffTransformation3 rotation = dtLinearAlgebra::getRotation(vector, angle);
    
    for (int ii=0;ii<getNControlPoints();ii++) {
      int pIndex = ii * _SISLCurveP->idim;
      dtVector3 toRot = getControlPoint3d(ii) - origin;
      toRot = rotation.transform(toRot);
      dtPoint3 rotPoint = origin + toRot;
       _SISLCurveP->ecoef[pIndex] = rotPoint.x();//_SISLCurveP->ecoef[pIndex] + transVec.x(); 
       _SISLCurveP->ecoef[pIndex+1] = rotPoint.y();//    _SISLCurveP->ecoef[pIndex+1] = _SISLCurveP->ecoef[pIndex+1] + transVec.y(); 
       _SISLCurveP->ecoef[pIndex+2] = rotPoint.z();//    _SISLCurveP->ecoef[pIndex+2] = _SISLCurveP->ecoef[pIndex+2] + transVec.z(); 
    }
  }

  void dtSislCurve::dump( void ) {
    DTDEBUGWF(dump(),
            << "uu = [ " <<  getUMin() << ", " << getUMax() << "]" << LOGDEL
            << "kind = " << _SISLCurveP->ikind << LOGDEL
            << "dim = " << _SISLCurveP->idim << LOGDEL
            << DTLOGEVAL(_SISLCurveP->ik) << LOGDEL
            << DTLOGEVAL(_SISLCurveP->in) << LOGDEL
            << DTLOGEVAL(_SISLCurveP->ikind) << LOGDEL
            << DTLOGEVAL(_SISLCurveP->idim) << LOGDEL
            << DTLOGEVAL(_SISLCurveP->icopy) << LOGDEL
            << DTLOGEVAL(_SISLCurveP->cuopen) );
    DTBUFFERINIT();
    for (int ii=0; ii<getNControlPoints(); ii++) {
//      if (getDimension() == DIMTWOD) {
//       DTBUFFER(<< DTLOGPOI2D( getControlPoint2d(ii) ) << LOGDEL);
//      }
//      else {
        DTBUFFER(<< DTLOGPOI3D( getControlPoint3d(ii) ) << LOGDEL);
//      }
    }
    DTINFOWF_BUFFER(dump());
  }

  void dtSislCurve::closeStraight( void ) {
    dtSislCurve * endExt;
		dtPoint3 endPoint = getPointPercent3d(0.);
		dtPoint3 startPoint = getPointPercent3d(1.);

		if ( CGAL::compare_xyz(endPoint, startPoint) != CGAL::EQUAL ) {
			DTWARNINGWF(closeStraight(),
							<< DTLOGPOI3D(endPoint) << LOGDEL
							<< DTLOGPOI3D(startPoint) << ")" << LOGDEL
							<< "Gap between endPoint and startPoint is filled with a linear spline!");      
			std::vector< dtPoint3 > pointArray(2, dtPoint3(0,0,0));
			pointArray[0] = endPoint;
			pointArray[1] = startPoint;    
			int order = 2;
			endExt = new dtSislCurve(&pointArray, order);
			SISLCurve * tmpCurve;
			int errFlag;
			s1715(
				const_cast< SISLCurve * >(
					getSISLCurve()
				), 
				const_cast< SISLCurve * >(
					endExt->getSISLCurve()
				),
				1,
				0,
				&tmpCurve, 
				&errFlag
			);
			allRight(errFlag, __FILE__, __LINE__);
			delete endExt;
			freeCurve(_SISLCurveP);        
			_SISLCurveP = tmpCurve;
		}
  }
  
  void dtSislCurve::connectArithmetic( dtCurve const * const toConnect ) {
    dtSislCurve * endExt = NULL;

		if ( !cast2Sisl(toConnect) ) {
			dt__THROW(connectArithmetic(),
							<< "Curve to connect with is not a dtSislCurve.");
		}

		dtPoint3 endPoint = getPointPercent3d(1.);
		dtPoint3 startPoint = toConnect->getPointPercent3d(0.);

		DTWARNINGWF(connectArithmetic(),
						<< DTLOGPOI3D(endPoint) << LOGDEL
						<< DTLOGPOI3D(startPoint) << ")" );

		//
		// create startExt
		//
		std::vector< dtPoint3 > pointArray;
		for (int ii = 0; ii<(getNControlPoints()-1); ii++) {
			pointArray.push_back( getControlPoint3d(ii) );
		}

//			//
//			// create arithmetic point
//			//
//			pointArray.push_back(startPoint+.5*(endPoint - startPoint));
			
		//
		// create endExt
		//
		for (int ii = 1; ii < toConnect->getNControlPoints(); ii++) {
			pointArray.push_back( toConnect->getControlPoint3d(ii) );
		}
		endExt = new dtSislCurve( &pointArray, toConnect->getOrder() );
		pointArray.clear();

    freeCurve( _SISLCurveP );
    _SISLCurveP = copyCurve(
                    const_cast< SISLCurve * >(
                      endExt->getSISLCurve()
                    ) 
                 );

    delete endExt;
  }  
  
  void dtSislCurve::closeArithmetic( void ) {
    //
    // check if curve is already closed
    //
    if (isClosed()) {
      return;
    }
    
		dtPoint3 endPoint = getPointPercent3d(1.);
		dtPoint3 startPoint = getPointPercent3d(0.);

		if ( CGAL::compare_xyz(endPoint, startPoint) != CGAL::EQUAL ) {
			DTWARNINGWF(connectArithmetic(),
							<< DTLOGPOI3D(endPoint) << LOGDEL
							<< DTLOGPOI3D(startPoint) << ")" );
			dtPoint3 newPoint = dtPoint3(
													 .5 * (startPoint.x() + endPoint.x()), 
													 .5 * (startPoint.y() + endPoint.y()),
													 .5 * (startPoint.z() + endPoint.z())
												 );

			//
			// create new curve
			//
			std::vector< dtPoint3 > pointArray;
			pointArray.push_back( newPoint );        
			for (int ii = 1; ii<getNControlPoints(); ii++) {
				pointArray.push_back( getControlPoint3d(ii) );
			}
			pointArray[getNControlPoints()-1] = newPoint;
			int order = getOrder();        
			freeCurve( _SISLCurveP );
			pointConstruct3d( &pointArray, order );
		}
		//
		// force closing curve
		//
		else {
			std::vector< dtPoint3 > pointArray;
			for (int ii = 0; ii<getNControlPoints(); ii++) {
				pointArray.push_back( getControlPoint3d(ii) );
			}
			int order = getOrder();
			freeCurve( _SISLCurveP );
			pointConstruct3d( &pointArray, order );        
		}
  }  
    
//  int dtSislCurve::getDimension( void ) const {
//    return _SISLCurveP->idim;
//  }

  void dtSislCurve::revert( void ) {
    SISLCurve * twin = copyCurve( _SISLCurveP );
    freeCurve(_SISLCurveP);
    _SISLCurveP = twin;
    s1706( _SISLCurveP );
  }

  void dtSislCurve::trim( float const uuStart, float const uuEnd ) {
    SISLCurve * trimmedCurve;
    int errFlag = 0;
    double uuStartD = (double) uuStart;
    double uuEndD = (double) uuEnd;
		double uMinD = static_cast<float>(getMin(0));
		double uMaxD = static_cast<float>(getMax(0));
		
		bool isMin =  false;
		bool isMax =  false;
		
    if ( uuStartD <= uMinD ) {
      DTWARNINGWF(trim(),
              << "Adjust " << DTLOGEVAL(uuStartD) << " to " << DTLOGEVAL(uMinD) );
      uuStartD = uMinD;
			isMin = true;
    }
    if ( uuEndD >= uMaxD ) {
      DTWARNINGWF(trim(),
              << "Adjust " << DTLOGEVAL(uuEndD) << " to " << DTLOGEVAL(uMaxD) );
      uuEndD = uMaxD;
			isMax = true;
    }
		if ( isMin && isMax ) {
			return;
		}
    
		if ( uuEndD < uMinD ) {
      DTWARNINGWF(trim(),
              << "Adjust " << DTLOGEVAL(uuEndD) << " to " << DTLOGEVAL(uMinD) );
      uuEndD = uMinD;
    }
    if ( uuStartD > uMaxD ) {
      DTWARNINGWF(trim(),
              << "Adjust " << DTLOGEVAL(uuStartD) << " to " << DTLOGEVAL(uMaxD) );
      uuStartD = uMaxD;
    }
		
    s1712(_SISLCurveP, uuStartD, uuEndD, &trimmedCurve, &errFlag);
    DTINFOWF(trim(),
						<< DTLOGEVAL(uuStartD) << LOGDEL
						<< DTLOGEVAL(uuEndD) << LOGDEL
						<< DTLOGEVAL(uMinD) << LOGDEL
						<< DTLOGEVAL(uMaxD) );
		allRight(errFlag, __FILE__, __LINE__);
    
		SISLCurve * tmpCurve = _SISLCurveP;
    _SISLCurveP = copyCurve(trimmedCurve);
    freeCurve(trimmedCurve);
    freeCurve(tmpCurve);
  }

	dtSislCurve const * dtSislCurve::cast2Sisl( dtCurve const * curve ) const {
		dtSislCurve const * dtSislCurveP;
		dt__MUSTDOWNCAST(curve, dtSislCurve const, dtSislCurveP);
		return dtSislCurveP;
	}
	
  dtSislCurve * dtSislCurve::cast2Sisl( dtCurve * curve ) const {
		dtSislCurve * dtSislCurveP;
		dt__MUSTDOWNCAST(curve, dtSislCurve, dtSislCurveP);
		return dtSislCurveP;		
	}	
}