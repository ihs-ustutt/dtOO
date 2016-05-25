#include "scaThreeD.h"
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/twoDArrayHandling.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <solid2dLine.h>
#include <discrete2dPoints.h>

namespace dtOO {
  scaThreeD::scaThreeD() : scaFunction() {
  }

  scaThreeD::scaThreeD( scaThreeD const & orig) : scaFunction(orig) {
		_min[0] = orig._min[0];
		_min[1] = orig._min[1];
    _min[2] = orig._min[2];
		_max[0] = orig._max[0];
		_max[1] = orig._max[1];
    _max[2] = orig._max[2];
  }

  scaThreeD::~scaThreeD() {
  }

  float scaThreeD::YFloat(aFX const & xx) const {
		dt__throwIf(xx.size()!=3, YFloat());	
		
		return YFloat(xx[0], xx[1], xx[2]);
	}
	
  float scaThreeD::YFloatPercent( 
    float const & xP0, float const & xP1, float const & xP2
  ) const {
    return YFloat( x_percent(xP0, xP1, xP2) );
  }
  
  int scaThreeD::xDim( void ) const {
		return 3;
	}
	
  float scaThreeD::xMin( int const & dir) const {
    switch (dir) {
      case 0:
        return _min[0];
      case 1:
        return _min[1];
      case 2:
        return _min[2];        
    }

    dt__throw(
      xMin(),
      << dt__eval(dir) << std::endl
      << "dir should be 0, 1 or 2."
    );    
	}

  float scaThreeD::xMax( int const & dir) const {
    switch (dir) {
      case 0:
        return _max[0];
      case 1:
        return _max[1];
      case 2:
        return _max[2];        
    }
    
    dt__throw(
      xMax(),
      << dt__eval(dir) << std::endl
      << "dir should be 0, 1 or 2."
    );
    
	}	
	
  void scaThreeD::setMin(int const & dir, float const & min) {
    switch (dir) {
      case 0:
        _min[0] = min;
        return;
      case 1:
        _min[1] = min;
				return;
      case 2:
        _min[2] = min;
				return;        
    }
    
    dt__throw(
      setMin(),
      << dt__eval(dir) << std::endl
      << "dir should be 0, 1 or 2."
    );    
  }
   
  void scaThreeD::setMax(int const & dir, float const & max) {
    switch (dir) {
      case 0:
        _max[0] = max;
        return;
      case 1:
        _max[1] = max;
				return;
      case 2:
        _max[2] = max;
				return;        
    }
    
    dt__throw(
      setMax(),
      << dt__eval(dir) << std::endl
      << "dir should be 0, 1 or 2."
    );        
  }
  
	aFX scaThreeD::x_percent(
    float const & x0, float const & x1, float const & x2
  ) const {	
		return x_percent( analyticFunction::aFXThreeD(x0, x1, x2) );
  }  
	
	aFX scaThreeD::percent_x(
    float const & x0, float const & x1, float const & x2
  ) const {
		return percent_x( analyticFunction::aFXThreeD(x0, x1, x2) );
  }  	
  
	std::vector<float> scaThreeD::DYFloat( aFX const & xx ) const {
    aFX xP = percent_x(xx);
    float deltaPer[3];
		deltaPer[0] = 0.0001;
		deltaPer[1] = 0.0001;
		deltaPer[2] = 0.0001;

		std::vector< aFX > uv(3, aFX(2));    
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
		
		twoDArrayHandling< float > yy(3,2);
		yy[0][0] = YFloat( x_percent(uv[0][0], xP[1], xP[2]) );
		yy[0][1] = YFloat( x_percent(uv[0][1], xP[1], xP[2]) );
		yy[1][0] = YFloat( x_percent(xP[0], uv[1][0], xP[2]) );
		yy[1][1] = YFloat( x_percent(xP[0], uv[1][1], xP[2]) );
		yy[2][0] = YFloat( x_percent(xP[0], xP[1], uv[2][0]) );
		yy[2][1] = YFloat( x_percent(xP[0], xP[1], uv[2][1]) );
		
		std::vector< float > dxdy(3);
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
	
	std::vector<float> scaThreeD::DYFloat( 
    float const & x0, float const & x1, float const & x2
  ) const {
		aFX xx(2,0);
		xx[0] = x0;
		xx[1] = x1;
    xx[2] = x2;

		return DYFloat(xx);
	}	
	
  vectorHandling< renderInterface * > scaThreeD::getRender( void ) const {
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
	
		vectorHandling< dtPoint2 > p2;
		float intervalU = (xMax(0) - xMin(0)) / (nU-1);
		float intervalV = (xMax(1) - xMin(1)) / (nV-1);
    float intervalW = (xMax(2) - xMin(2)) / (nW-1);

		for (int kk=0; kk<nW; kk++) {
			dt__toFloat(float kkF, kk);			
			float constW = xMin(2) + kkF * intervalW;
      for (int jj=0; jj<nV; jj++) {
        dt__toFloat(float jjF, jj);			
        float constV = xMin(1) + jjF * intervalV;

        for (int ii=0; ii<nU; ii++) {
          dt__toFloat(float iiF, ii);
          aFX xx(3,0);
          xx[0] = xMin(0) + iiF * intervalU;
          xx[1] = constV;
          xx[2] = constW;
          p2.push_back( dtPoint2( xx[0], YFloat(xx) ) );
        }
        rV.push_back( new solid2dLine(p2) );	
        p2.clear();
      }
    }
		return rV;
  }	
  
  dt__C_addCloneForpVH(scaThreeD);    
}