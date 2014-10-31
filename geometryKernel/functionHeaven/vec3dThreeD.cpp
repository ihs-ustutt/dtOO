#include "vec3dThreeD.h"
#include "dtLinearAlgebra.h"
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <solid2dLine.h>
#include <discrete2dPoints.h>

namespace dtOO {
	vec3dThreeD::vec3dThreeD() : vec3dFunction() {
	}

	vec3dThreeD::vec3dThreeD(const vec3dThreeD& orig) : vec3dFunction(orig) {
		_min[0] = orig._min[0];
		_min[1] = orig._min[1];
		_min[2] = orig._min[2];
		_max[0] = orig._max[0];
		_max[1] = orig._max[1];
		_max[2] = orig._max[2];
	}

	vec3dThreeD::~vec3dThreeD() {
	}
	
	int vec3dThreeD::xDim( void ) const {
		return 3;
	}

  aFY vec3dThreeD::Y(float const & x0, float const & x1, float const & x2) const {	
		aFX xx(3,0);
		
		xx[0] = x0;
		xx[1] = x1;
		xx[2] = x2;
		
		return Y(xx);
	}

  dtPoint3 vec3dThreeD::YdtPoint3(aFX const & xx) const {
		return vec3dFunction::YdtPoint3( xx );
	}

  dtPoint3 vec3dThreeD::YdtPoint3(float const & x0, float const & x1, float const & x2) const {
		aFX xx(3,0);
		
		xx[0] = x0;
		xx[1] = x1;
		xx[2] = x2;
		
		return vec3dFunction::YdtPoint3( xx );
	}	
	
	dtPoint3 vec3dThreeD::YdtPoint3Percent(aFX const & xx) const {
		return YdtPoint3( x_percent(xx) );
	}
	
	dtPoint3 vec3dThreeD::YdtPoint3Percent(float const & x0, float const & x1, float const & x2) const {
		aFX xx(3,0);
		
		xx[0] = x0;
		xx[1] = x1;
		xx[2] = x2;
		
		return YdtPoint3( x_percent(xx) );
	}
	
	aFX vec3dThreeD::x_percent(aFX const & xx) const {
		aFX ret(3, 0.);
    for (int ii=0; ii<3; ii++) {
      ret[ii] = xMin(ii) +  (xMax(ii) - xMin(ii) ) * xx[ii];
		}
		
		return ret;
  }  
  
  aFX vec3dThreeD::percent_x(aFX const & xx) const {
		aFX ret(3, 0.);
		for (int ii=0; ii<3; ii++) {
      ret[ii] = (xx[ii] - xMin(ii)) / (xMax(ii) - xMin(ii));
		}
		
		return ret;
  }
	
	std::vector<dtVector3> vec3dThreeD::DYdtVector3( aFX const & xx ) const {
    aFX xP = percent_x(xx);
    float deltaPer[3];
		deltaPer[0] = 0.0001;
		deltaPer[1] = 0.0001;
    deltaPer[2] = 0.0001;
 
		aFX x0(3,0);
		aFX x1(3,0);
    
		for (int ii=0; ii<3; ii++) {
			if (xP[ii]<deltaPer[ii]) {
				x0[ii] = 0.;
				x1[ii] = deltaPer[ii];      
			}
			else if ( (xP[ii]>=0.01) && (xP[ii]<=(1.-deltaPer[ii])) ) {
				x0[ii] = xP[ii]-deltaPer[ii];
				x1[ii] = xP[ii]+deltaPer[ii];
			}
			else if (xP[ii]>(1.-deltaPer[ii])) {
				x0[ii] = 1.-deltaPer[ii];
				x1[ii] = 1.;
			}		
		}
		
		dtPoint3 y0 = YdtPoint3( x_percent(x0) );
		dtPoint3 y1 = YdtPoint3( x_percent(x1) );
		
		std::vector<dtVector3> dxdy(2);
		for (int ii=0; ii<3; ii++) {
  		dxdy[ii] = (y1 - y0) / ( x_percent(x1)[ii] - x_percent(x0)[ii] );
		}
		
		return dxdy;
	}
	
  float vec3dThreeD::xMin( int const & dir) const {
    switch (dir) {
      case 0:
        return _min[0];
        break;
      case 1:
        return _min[1];
        break;
      case 2:
        return _min[2];
        break;					
      default:
        dt__THROW(xMin(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0, 1 or 2.");
    }   
	}
	
  float vec3dThreeD::xMax( int const & dir) const {
    switch (dir) {
      case 0:
        return _max[0];
        break;
      case 1:
        return _max[1];
        break;
      case 2:
        return _max[2];
        break;					
      default:
        dt__THROW(xMax(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0, 1 or 2.");
    }
	}	

  void vec3dThreeD::setMin(int const & dir, float const & min) {
    switch (dir) {
      case 0:
        _min[0] = min;
        break;
      case 1:
        _min[1] = min;
				break;
      case 2:
        _min[2] = min;
				break;				
      default:
        dt__THROW(setMin(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0, 1 or 2.");
    }
  }

  void vec3dThreeD::setMax(int const & dir, float const & max) {
    switch (dir) {
      case 0:
        _max[0] = max;
        break;
      case 1:
        _max[1] = max;
				break;
      case 2:
        _max[2] = max;
				break;				
      default:
        dt__THROW(setMax(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0, 1 or 2.");
    }
  }

  vectorHandling< renderInterface * > vec3dThreeD::getRender( void ) const {
		int nU
		=
		staticPropertiesHandler::getInstance()->getOptionInt(
      "function_render_resolution_u"
    );		
		int nV
		=
		staticPropertiesHandler::getInstance()->getOptionInt(
      "function_render_resolution_v"
    );		
		int nW
		=
		staticPropertiesHandler::getInstance()->getOptionInt(
      "function_render_resolution_w"
    );				
		vectorHandling< renderInterface * > rV;//(nV);
//		for (int kk=0; kk<nW; kk++) {
//			for (int jj=0; jj<nV; jj++) {
//				vectorHandling< dtPoint2 > p2(nU);
//				float intervalU = (xMax(0) - xMin(0)) / (nU-1);
//				float intervalV = (xMax(1) - xMin(1)) / (nV-1);
//				float intervalW = (xMax(2) - xMin(2)) / (nW-1);
//				for (int ii=0; ii<nU; ii++) {
//					dt__TOFLOAT(float iiF, ii);
//					dt__TOFLOAT(float jjF, jj);
//					dt__TOFLOAT(float kkF, kk);
//					aFX xx(3,0);
//					xx[0] = xMin(0) + iiF * intervalU;
//					xx[1] = xMin(1) + jjF * intervalV;
//					xx[2] = xMin(2) + kkF * intervalW;
//					dtPoint3 p3 = YdtPoint3(xx);
//					p2[ii] = dtPoint2( p3.x(), p3.y() );
//				}
//				rV.push_back( new solid2dLine(p2) );
//			}
//		}
		vectorHandling< dtPoint2 > p2;
		float intervalU = (xMax(0) - xMin(0)) / (nU-1);
		float intervalV = (xMax(1) - xMin(1)) / (nV-1);		
		float intervalW = (xMax(2) - xMin(2)) / (nW-1);
		for (int ii=0; ii<nU; ii++) {
			dt__TOFLOAT(float iiF, ii);
			aFX xx(3,0);
			xx[0] = xMin(0) + iiF * intervalU;
			xx[1] = xMin(1);
			xx[2] = xMin(2);
			dtPoint3 p3 = YdtPoint3(xx);
			DTINFOWF(getRender(), << DTLOGPOI3D(p3) );
			p2.push_back( dtPoint2( p3.x(), p3.y() ) );
		}
		rV.push_back( new solid2dLine(p2) );
		p2.clear();
		for (int jj=0; jj<nV; jj++) {
			dt__TOFLOAT(float jjF, jj);
			aFX xx(3,0);
			xx[0] = xMin(0);
			xx[1] = xMin(1) + jjF * intervalV;
			xx[2] = xMin(2);
			dtPoint3 p3 = YdtPoint3(xx);
			DTINFOWF(getRender(), << DTLOGPOI3D(p3) );
			p2.push_back( dtPoint2( p3.x(), p3.y() ) );
		}
		rV.push_back( new solid2dLine(p2) );			
		p2.clear();
		for (int ii=0; ii<nU; ii++) {
			dt__TOFLOAT(float iiF, ii);
			aFX xx(3,0);
			xx[0] = xMin(0) + iiF * intervalU;
			xx[1] = xMax(1);
			xx[2] = xMin(2);
			dtPoint3 p3 = YdtPoint3(xx);
			DTINFOWF(getRender(), << DTLOGPOI3D(p3) );
			p2.push_back( dtPoint2( p3.x(), p3.y() ) );
		}
		rV.push_back( new solid2dLine(p2) );
		p2.clear();
		for (int jj=0; jj<nV; jj++) {
			dt__TOFLOAT(float jjF, jj);
			aFX xx(3,0);
			xx[0] = xMax(0);
			xx[1] = xMin(1) + jjF * intervalV;
			xx[2] = xMin(2);
			dtPoint3 p3 = YdtPoint3(xx);
			DTINFOWF(getRender(), << DTLOGPOI3D(p3) );
			p2.push_back( dtPoint2( p3.x(), p3.y() ) );
		}
		rV.push_back( new solid2dLine(p2) );		
		p2.clear();
		for (int ii=0; ii<nU; ii++) {
			dt__TOFLOAT(float iiF, ii);
			aFX xx(3,0);
			xx[0] = xMin(0) + iiF * intervalU;
			xx[1] = xMin(1);
			xx[2] = xMax(2);
			dtPoint3 p3 = YdtPoint3(xx);
			DTINFOWF(getRender(), << DTLOGPOI3D(p3) );
			p2.push_back( dtPoint2( p3.x(), p3.y() ) );
		}
		rV.push_back( new solid2dLine(p2) );
		p2.clear();
		for (int jj=0; jj<nV; jj++) {
			dt__TOFLOAT(float jjF, jj);
			aFX xx(3,0);
			xx[0] = xMin(0);
			xx[1] = xMin(1) + jjF * intervalV;
			xx[2] = xMax(2);
			dtPoint3 p3 = YdtPoint3(xx);
			DTINFOWF(getRender(), << DTLOGPOI3D(p3) );
			p2.push_back( dtPoint2( p3.x(), p3.y() ) );
		}
		rV.push_back( new solid2dLine(p2) );			
		p2.clear();
		for (int ii=0; ii<nU; ii++) {
			dt__TOFLOAT(float iiF, ii);
			aFX xx(3,0);
			xx[0] = xMin(0) + iiF * intervalU;
			xx[1] = xMax(1);
			xx[2] = xMax(2);
			dtPoint3 p3 = YdtPoint3(xx);
			DTINFOWF(getRender(), << DTLOGPOI3D(p3) );
			p2.push_back( dtPoint2( p3.x(), p3.y() ) );
		}
		rV.push_back( new solid2dLine(p2) );
		p2.clear();
		for (int jj=0; jj<nV; jj++) {
			dt__TOFLOAT(float jjF, jj);
			aFX xx(3,0);
			xx[0] = xMax(0);
			xx[1] = xMin(1) + jjF * intervalV;
			xx[2] = xMax(2);
			dtPoint3 p3 = YdtPoint3(xx);
			DTINFOWF(getRender(), << DTLOGPOI3D(p3) );
			p2.push_back( dtPoint2( p3.x(), p3.y() ) );
		}
		rV.push_back( new solid2dLine(p2) );						
		
//		vectorHandling< dtPoint2 > p2Cp(_dtS->nControlPoints());
//		for (int ii=0; ii<p2Cp.size(); ii++) {
//			dtPoint3 p3 = _dtS->controlPoint(ii);
//      p2Cp[ii] = dtPoint2(p3.x(), p3.y());
//    }
//		rV.push_back( new discrete2dPoints(p2Cp) );

		return rV;
  }	
}
