#include "vec3dThreeD.h"
#include "dtLinearAlgebra.h"
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <interfaceHeaven/twoDArrayHandling.h>
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

  aFY vec3dThreeD::Y(
    dtReal const & x0, dtReal const & x1, dtReal const & x2
  ) const {	
		aFX xx(3,0);
		
		xx[0] = x0;
		xx[1] = x1;
		xx[2] = x2;
		
		return Y(xx);
	}

  dtPoint3 vec3dThreeD::YdtPoint3(aFX const & xx) const {
		return vec3dFunction::YdtPoint3( xx );
	}

  dtPoint3 vec3dThreeD::YdtPoint3(
    dtReal const & x0, dtReal const & x1, dtReal const & x2
  ) const {
		aFX xx(3,0);
		
		xx[0] = x0;
		xx[1] = x1;
		xx[2] = x2;
		
		return vec3dFunction::YdtPoint3( xx );
	}	
	
	dtPoint3 vec3dThreeD::YdtPoint3Percent(aFX const & xx) const {
		return YdtPoint3( x_percent(xx) );
	}
	
	dtPoint3 vec3dThreeD::YdtPoint3Percent(
    dtReal const & x0, dtReal const & x1, dtReal const & x2
  ) const {
		aFX xx(3,0);
		
		xx[0] = x0;
		xx[1] = x1;
		xx[2] = x2;
		
		return YdtPoint3( x_percent(xx) );
	}
	
	aFX vec3dThreeD::x_percent(
    dtReal const & x0, dtReal const & x1, dtReal const & x2
  ) const {
		aFX xx(3, 0.);
    xx[0] = x0;
		xx[1] = x1;
		xx[2] = x2;
		
		return x_percent(xx);
  }  
	
	aFX vec3dThreeD::percent_x(
    dtReal const & x0, dtReal const & x1, dtReal const & x2
  ) const {
		aFX xx(3, 0.);
    xx[0] = x0;
		xx[1] = x1;
		xx[2] = x2;
		
		return percent_x(xx);
  }  
	
	std::vector<dtVector3> vec3dThreeD::DYdtVector3( aFX const & xx ) const {
    aFX xP = percent_x(xx);
    dtReal deltaPer[3];
		deltaPer[0] = 0.0001;
		deltaPer[1] = 0.0001;
		deltaPer[2] = 0.0001;

		std::vector< aFX > uv(3, aFX(2, 0.));    
		for (int ii=0; ii<3; ii++) {
			if (xP[ii]<deltaPer[ii]) {
				uv[ii][0] = 0.;
				uv[ii][1] = deltaPer[ii];
			}
			else if ( (xP[ii]>=deltaPer[ii]) && (xP[ii]<=(1.-deltaPer[ii])) ) {
				uv[ii][0] = xP[ii]-deltaPer[ii];
				uv[ii][1] = xP[ii]+deltaPer[ii];
			}
			else if (xP[ii]>(1.-deltaPer[ii])) {
				uv[ii][0] = 1.-deltaPer[ii];
				uv[ii][1] = 1.;
			}		
		}
		
		twoDArrayHandling< dtPoint3 > yy(3,2);
		yy[0][0] = YdtPoint3( x_percent(uv[0][0], xP[1], xP[2]) );
		yy[0][1] = YdtPoint3( x_percent(uv[0][1], xP[1], xP[2]) );
		yy[1][0] = YdtPoint3( x_percent(xP[0], uv[1][0], xP[2]) );
		yy[1][1] = YdtPoint3( x_percent(xP[0], uv[1][1], xP[2]) );
		yy[2][0] = YdtPoint3( x_percent(xP[0], xP[1], uv[2][0]) );
		yy[2][1] = YdtPoint3( x_percent(xP[0], xP[1], uv[2][1]) );
		
		std::vector<dtVector3> dxdy(3);
		dxdy[0] 
    = 
    (yy[0][1] - yy[0][0]) 
    / 
    ( 
      x_percent(uv[0][1], xP[1], xP[2])[0] 
      - 
      x_percent(uv[0][0], xP[1], xP[2])[0] 
    );
		dxdy[1] 
    = 
    (yy[1][1] - yy[1][0]) 
    / 
    ( 
      x_percent(xP[0], uv[1][1], xP[2])[1] 
      - 
      x_percent(xP[0], uv[1][0], xP[2])[1] 
    );
		dxdy[2] 
    = 
    (yy[2][1] - yy[2][0]) 
    / 
    ( 
      x_percent(xP[0], xP[1], uv[2][1])[2] 
      - 
      x_percent(xP[0], xP[1], uv[2][0])[2] 
    );
		
		return dxdy;
	}
	
  dtReal vec3dThreeD::xMin( int const & dir) const {
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
        dt__throw(xMin(),
              << dt__eval(dir) << std::endl
              << "dir should be 0, 1 or 2.");
    }   
	}
	
  dtReal vec3dThreeD::xMax( int const & dir) const {
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
        dt__throw(xMax(),
              << dt__eval(dir) << std::endl
              << "dir should be 0, 1 or 2.");
    }
	}	

  void vec3dThreeD::setMin(int const & dir, dtReal const & min) {
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
        dt__throw(setMin(),
              << dt__eval(dir) << std::endl
              << "dir should be 0, 1 or 2.");
    }
  }

  void vec3dThreeD::setMax(int const & dir, dtReal const & max) {
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
        dt__throw(setMax(),
              << dt__eval(dir) << std::endl
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
//				dtReal intervalU = (xMax(0) - xMin(0)) / (nU-1);
//				dtReal intervalV = (xMax(1) - xMin(1)) / (nV-1);
//				dtReal intervalW = (xMax(2) - xMin(2)) / (nW-1);
//				for (int ii=0; ii<nU; ii++) {
//					dt__toFloat(dtReal iiF, ii);
//					dt__toFloat(dtReal jjF, jj);
//					dt__toFloat(dtReal kkF, kk);
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
		dtReal intervalU = (xMax(0) - xMin(0)) / (nU-1);
		dtReal intervalV = (xMax(1) - xMin(1)) / (nV-1);		
		dtReal intervalW = (xMax(2) - xMin(2)) / (nW-1);
		for (int ii=0; ii<nU; ii++) {
			dt__toFloat(dtReal iiF, ii);
			aFX xx(3,0);
			xx[0] = xMin(0) + iiF * intervalU;
			xx[1] = xMin(1);
			xx[2] = xMin(2);
			dtPoint3 p3 = YdtPoint3(xx);
			dt__info(getRender(), << dt__point3d(p3) );
			p2.push_back( dtPoint2( p3.x(), p3.y() ) );
		}
		rV.push_back( new solid2dLine(p2) );
		p2.clear();
		for (int jj=0; jj<nV; jj++) {
			dt__toFloat(dtReal jjF, jj);
			aFX xx(3,0);
			xx[0] = xMin(0);
			xx[1] = xMin(1) + jjF * intervalV;
			xx[2] = xMin(2);
			dtPoint3 p3 = YdtPoint3(xx);
			dt__info(getRender(), << dt__point3d(p3) );
			p2.push_back( dtPoint2( p3.x(), p3.y() ) );
		}
		rV.push_back( new solid2dLine(p2) );			
		p2.clear();
		for (int ii=0; ii<nU; ii++) {
			dt__toFloat(dtReal iiF, ii);
			aFX xx(3,0);
			xx[0] = xMin(0) + iiF * intervalU;
			xx[1] = xMax(1);
			xx[2] = xMin(2);
			dtPoint3 p3 = YdtPoint3(xx);
			dt__info(getRender(), << dt__point3d(p3) );
			p2.push_back( dtPoint2( p3.x(), p3.y() ) );
		}
		rV.push_back( new solid2dLine(p2) );
		p2.clear();
		for (int jj=0; jj<nV; jj++) {
			dt__toFloat(dtReal jjF, jj);
			aFX xx(3,0);
			xx[0] = xMax(0);
			xx[1] = xMin(1) + jjF * intervalV;
			xx[2] = xMin(2);
			dtPoint3 p3 = YdtPoint3(xx);
			dt__info(getRender(), << dt__point3d(p3) );
			p2.push_back( dtPoint2( p3.x(), p3.y() ) );
		}
		rV.push_back( new solid2dLine(p2) );		
		p2.clear();
		for (int ii=0; ii<nU; ii++) {
			dt__toFloat(dtReal iiF, ii);
			aFX xx(3,0);
			xx[0] = xMin(0) + iiF * intervalU;
			xx[1] = xMin(1);
			xx[2] = xMax(2);
			dtPoint3 p3 = YdtPoint3(xx);
			dt__info(getRender(), << dt__point3d(p3) );
			p2.push_back( dtPoint2( p3.x(), p3.y() ) );
		}
		rV.push_back( new solid2dLine(p2) );
		p2.clear();
		for (int jj=0; jj<nV; jj++) {
			dt__toFloat(dtReal jjF, jj);
			aFX xx(3,0);
			xx[0] = xMin(0);
			xx[1] = xMin(1) + jjF * intervalV;
			xx[2] = xMax(2);
			dtPoint3 p3 = YdtPoint3(xx);
			dt__info(getRender(), << dt__point3d(p3) );
			p2.push_back( dtPoint2( p3.x(), p3.y() ) );
		}
		rV.push_back( new solid2dLine(p2) );			
		p2.clear();
		for (int ii=0; ii<nU; ii++) {
			dt__toFloat(dtReal iiF, ii);
			aFX xx(3,0);
			xx[0] = xMin(0) + iiF * intervalU;
			xx[1] = xMax(1);
			xx[2] = xMax(2);
			dtPoint3 p3 = YdtPoint3(xx);
			dt__info(getRender(), << dt__point3d(p3) );
			p2.push_back( dtPoint2( p3.x(), p3.y() ) );
		}
		rV.push_back( new solid2dLine(p2) );
		p2.clear();
		for (int jj=0; jj<nV; jj++) {
			dt__toFloat(dtReal jjF, jj);
			aFX xx(3,0);
			xx[0] = xMax(0);
			xx[1] = xMin(1) + jjF * intervalV;
			xx[2] = xMax(2);
			dtPoint3 p3 = YdtPoint3(xx);
			dt__info(getRender(), << dt__point3d(p3) );
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
  
  dt__C_addCloneForpVH(vec3dThreeD);
}
