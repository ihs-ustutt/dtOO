#include "vec3dTwoD.h"
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/twoDArrayHandling.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <solid2dLine.h>
#include <discrete2dPoints.h>

namespace dtOO {
	vec3dTwoD::vec3dTwoD() : vec3dFunction() {
	}

	vec3dTwoD::vec3dTwoD(const vec3dTwoD& orig) : vec3dFunction(orig) {
		_min[0] = orig._min[0];
		_min[1] = orig._min[1];
		_max[0] = orig._max[0];
		_max[1] = orig._max[1];
	}

	vec3dTwoD::~vec3dTwoD() {
	}
	
	int vec3dTwoD::xDim( void ) const {
		return 2;
	}

  aFY vec3dTwoD::Y(float const & x0, float const & x1) const {	
		aFX xx(2,0);
		
		xx[0] = x0;
		xx[1] = x1;
		
		return Y(xx);
	}

  dtPoint3 vec3dTwoD::YdtPoint3(aFX const & xx) const {
		return vec3dFunction::YdtPoint3( xx );
	}

  dtPoint3 vec3dTwoD::YdtPoint3(float const & x0, float const & x1) const {
		aFX xx(2,0);
		
		xx[0] = x0;
		xx[1] = x1;
		
		return vec3dFunction::YdtPoint3( xx );
	}	
	
	dtPoint3 vec3dTwoD::YdtPoint3Percent(aFX const & xx) const {
		return YdtPoint3( x_percent(xx) );
	}
	
	dtPoint3 vec3dTwoD::YdtPoint3Percent(float const & x0, float const & x1) const {
		aFX xx(2,0);
		
		xx[0] = x0;
		xx[1] = x1;
		
		return YdtPoint3( x_percent(xx) );
	}
	
//	aFX vec3dTwoD::x_percent(aFX const & xx) const {
//		aFX ret(2, 0.);
//    ret[0] = xMin(0) +  (xMax(0) - xMin(0) ) * xx[0];
//		ret[1] = xMin(1) +  (xMax(1) - xMin(1) ) * xx[1];
//		
//		return ret;
//  }

	aFX vec3dTwoD::x_percent(float const & x0, float const & x1) const {
		aFX xx(2, 0.);
    xx[0] = x0;
		xx[1] = x1;
		
		return x_percent(xx);
  }  
	
	aFX vec3dTwoD::percent_x(float const & x0, float const & x1) const {
		aFX xx(2, 0.);
    xx[0] = x0;
		xx[1] = x1;
		
		return percent_x(xx);
  }  	
  
//  aFX vec3dTwoD::percent_x(aFX const & xx) const {
//		aFX ret(2, 0.);
//    ret[0] = (xx[0] - xMin(0)) / (xMax(0) - xMin(0));
//		ret[1] = (xx[1] - xMin(1)) / (xMax(1) - xMin(1));
//		
//		return ret;
//  }
	
	std::vector<dtVector3> vec3dTwoD::DYdtVector3( aFX const & xx ) const {
		/*
		 *                      (xP[0], uv[1][1]) -> yy[1][1]
		 *                               o
		 *                               |
		 *                               |
		 *                               |
		 *  (uv[0][0], xP[1]) o---------------------o (uv[0][1], xP[1])
		 *     -> yy[0][0]               |               -> yy[0][1]
		 *                               |
		 *                               |
		 *                               o
		 *                      (xP[0], uv[1][0]) -> yy[1][0] 
		 */
    aFX xP = percent_x(xx);
    float deltaPer[2];
		deltaPer[0] = 0.0001;
		deltaPer[1] = 0.0001;

		std::vector< aFX > uv(2, aFX(2));    
		for (int ii=0; ii<2; ii++) {
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
		
		twoDArrayHandling< dtPoint3 > yy(2,2);
		yy[0][0] = YdtPoint3( x_percent(uv[0][0], xP[1]) );
		yy[0][1] = YdtPoint3( x_percent(uv[0][1], xP[1]) );
		yy[1][0] = YdtPoint3( x_percent(xP[0], uv[1][0]) );
		yy[1][1] = YdtPoint3( x_percent(xP[0], uv[1][1]) );
		
		std::vector<dtVector3> dxdy(2);
		dxdy[0] = (yy[0][1] - yy[0][0]) / ( x_percent(uv[0][1], xP[1])[0] - x_percent(uv[0][0], xP[1])[0] );
		dxdy[1] = (yy[1][1] - yy[1][0]) / ( x_percent(xP[0], uv[1][1])[1] - x_percent(xP[0], uv[1][0])[1] );
		
		return dxdy;
	}
		
	std::vector<dtVector3> vec3dTwoD::DYdtVector3( float const & x0, float const & x1 ) const {
		aFX xx(2,0);
		xx[0] = x0;
		xx[1] = x1;

		return DYdtVector3(xx);
	}
		
	dtVector3 vec3dTwoD::unitNdtVector3( aFX const & xx ) const {
		std::vector<dtVector3> dxdy = DYdtVector3(xx);
		
		dtVector3 nn = dtLinearAlgebra::crossProduct(dxdy[0], dxdy[1]);
		
		return dtLinearAlgebra::normalize(nn);
	}

	dtVector3 vec3dTwoD::unitNdtVector3Percent( aFX const & xx ) const {
		return unitNdtVector3( x_percent(xx) );
	}
	
  float vec3dTwoD::xMin( int const & dir) const {
    switch (dir) {
      case 0:
        return _min[0];
        break;
      case 1:
        return _min[1];
        break;				
      default:
        dt__throw(xMin(),
              << dt__eval(dir) << std::endl
              << "dir should be 0 or 1.");
    }   
	}
	
  float vec3dTwoD::xMax( int const & dir) const {
    switch (dir) {
      case 0:
        return _max[0];
        break;
      case 1:
        return _max[1];
        break;
      default:
        dt__throw(xMax(),
              << dt__eval(dir) << std::endl
              << "dir should be 0 or 1.");
    }
	}	

  void vec3dTwoD::setMin(int const & dir, float const & min) {
    switch (dir) {
      case 0:
        _min[0] = min;
        break;
      case 1:
        _min[1] = min;
				break;
      default:
        dt__throw(setMin(),
              << dt__eval(dir) << std::endl
              << "dir should be 0 or 1.");
    }
  }

  void vec3dTwoD::setMax(int const & dir, float const & max) {
    switch (dir) {
      case 0:
        _max[0] = max;
        break;
      case 1:
        _max[1] = max;
				break;
      default:
        dt__throw(setMax(),
              << dt__eval(dir) << std::endl
              << "dir should be 0 or 1.");
    }
  }

  vectorHandling< renderInterface * > vec3dTwoD::getRender( void ) const {
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
		vectorHandling< renderInterface * > rV;//(nV);
	
		vectorHandling< dtPoint2 > p2;
		float intervalU = (xMax(0) - xMin(0)) / (nU-1);
		float intervalV = (xMax(1) - xMin(1)) / (nV-1);
		
		for (int ii=0; ii<nU; ii++) {
			dt__toFloat(float iiF, ii);
			aFX xx(2,0);
			xx[0] = xMin(0) + iiF * intervalU;
			xx[1] = xMin(1);
			dtPoint3 p3 = YdtPoint3(xx);
			dt__info(getRender(), << dt__point3d(p3) );
			p2.push_back( dtPoint2( p3.x(), p3.y() ) );
		}
		rV.push_back( new solid2dLine(p2) );
		p2.clear();
		for (int jj=0; jj<nV; jj++) {
			dt__toFloat(float jjF, jj);
			aFX xx(2,0);
			xx[0] = xMin(0);
			xx[1] = xMin(1) + jjF * intervalV;
			dtPoint3 p3 = YdtPoint3(xx);
			dt__info(getRender(), << dt__point3d(p3) );
			p2.push_back( dtPoint2( p3.x(), p3.y() ) );
		}
		rV.push_back( new solid2dLine(p2) );			
		p2.clear();
		for (int ii=0; ii<nU; ii++) {
			dt__toFloat(float iiF, ii);
			aFX xx(2,0);
			xx[0] = xMin(0) + iiF * intervalU;
			xx[1] = xMax(1);
			dtPoint3 p3 = YdtPoint3(xx);
			dt__info(getRender(), << dt__point3d(p3) );
			p2.push_back( dtPoint2( p3.x(), p3.y() ) );
		}
		rV.push_back( new solid2dLine(p2) );
		p2.clear();
		for (int jj=0; jj<nV; jj++) {
			dt__toFloat(float jjF, jj);
			aFX xx(2,0);
			xx[0] = xMax(0);
			xx[1] = xMin(1) + jjF * intervalV;
			dtPoint3 p3 = YdtPoint3(xx);
			dt__info(getRender(), << dt__point3d(p3) );
			p2.push_back( dtPoint2( p3.x(), p3.y() ) );
		}
		rV.push_back( new solid2dLine(p2) );			

		
		return rV;
  }		

	aFX vec3dTwoD::operator%(const aFX &percent) const {
		return x_percent(percent);
	}	
  
//  float vec3dTwoD::length( 
//    int const & nP, aFX const & x0, aFX const & x1 
//  ) const {
//		std::vector< dtPoint2 > glp = dtLinearAlgebra::getGaussLegendre(nP);
//		float L = 0.0;
//		dtPoint2 const u0(x0[0], x0[1]);
//		dtPoint2 const u1(x1[0], x1[1]);
//		dtVector2 const normalDir( dtLinearAlgebra::normalize(u1-u0) );
//		const dtVector2 rapJ = .5 * (u1 - u0);
//		for (int i = 0; i < nP; i++) {
//			float const tt = glp[i].x();
//			float const ww = glp[i].y();
//			dtPoint2 const ui(
//        .5 * (1. - tt) * u0.x()  + .5 * (1. + tt) * u1.x(),
//        .5 * (1. - tt) * u0.y()  + .5 * (1. + tt) * u1.y()
//      );
//			std::vector< dtVector3 > der = DYdtVector3(ui.x(), ui.y());
//			const float d 
//      = 
//      dtLinearAlgebra::length(normalDir.x()*der[0]+normalDir.y()*der[1] );
//			L += d * ww * dtLinearAlgebra::length(rapJ);
//		}
//		return L;    
//  }
  
//  float vec3dTwoD::length( aFX const & x0, aFX const & x1 ) const {	
//    return length(20, x0, x1);
//  }
  dt__C_addCloneForpVH(vec3dTwoD);      
}
