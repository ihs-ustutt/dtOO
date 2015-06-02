#include "vec2dMultiBiLinearTwoD.h"
#include "interfaceHeaven/intHandling.h"

#include <logMe/logMe.h>

namespace dtOO {
  vec2dMultiBiLinearTwoD::vec2dMultiBiLinearTwoD() : vec2dTwoD() {
  }

  vec2dMultiBiLinearTwoD::vec2dMultiBiLinearTwoD(
    vec2dMultiBiLinearTwoD const & orig
  ) : vec2dTwoD(orig) {
    _piece = twoDArrayHandling< vec2dBiLinearTwoD >(orig._piece);
    
		for (int ii=0; ii<2; ii++) {
			setMin( ii, orig.xMin(ii) );
			setMax( ii, orig.xMax(ii) );
		}		
  }

  vec2dMultiBiLinearTwoD::vec2dMultiBiLinearTwoD(
      twoDArrayHandling< dtPoint2 > const & pp
	) : vec2dTwoD() {
    _piece = twoDArrayHandling< vec2dBiLinearTwoD >(pp.size(0)-1, pp.size(1)-1);
    
    dt__forAllIndex(_piece, ii) {
      dt__forAllIndex(_piece[ii], jj) {      
        _piece[ii][jj] 
        = 
        vec2dBiLinearTwoD(
          pp[ii][jj], pp[ii+1][jj], pp[ii+1][jj+1], pp[ii][jj+1]
        );
      }
    }
		for (int ii=0; ii<2; ii++) {
			setMin( ii, 0. );
			setMax( ii, 1. );
		}				
    
    twoDArrayHandling< float > rangeY0(_piece.size(0)+1, _piece.size(1)+1);
    twoDArrayHandling< float > rangeY1(_piece.size(0)+1, _piece.size(1)+1);
    dt__forAllIndex(_piece, ii) {
      dt__forAllIndex(_piece[ii], jj) {    
        aFY y0 = _piece[ii][jj].Y( analyticFunction::aFXTwoD(0., 0.) );
        aFY y1 = _piece[ii][jj].Y( analyticFunction::aFXTwoD(1., 0.) );
        aFY y2 = _piece[ii][jj].Y( analyticFunction::aFXTwoD(1., 1.) );
        aFY y3 = _piece[ii][jj].Y( analyticFunction::aFXTwoD(0., 1.) );

        rangeY0[ii  ][jj  ] = y0[0];
        rangeY0[ii+1][jj  ] = y1[0];
        rangeY0[ii+1][jj+1] = y2[0];
        rangeY0[ii  ][jj+1] = y3[0];
        rangeY1[ii  ][jj  ] = y0[1];
        rangeY1[ii+1][jj  ] = y1[1];
        rangeY1[ii+1][jj+1] = y2[1];
        rangeY1[ii  ][jj+1] = y3[1];        
      }
    }    
    dt__info(
      vec2dMultiBiLinearTwoD(),
      << "y0" << std::endl
      << logMe::floatMatrixToString(rangeY0) << std::endl
      << "y1" << std::endl
      << logMe::floatMatrixToString(rangeY1)
    );    
  }
 
  vec2dMultiBiLinearTwoD::~vec2dMultiBiLinearTwoD() {
    _piece.clear();
  }

  aFY vec2dMultiBiLinearTwoD::Y( aFX const & xx ) const {
    dt__throwIf(xx.size()!=2, Y());
   
    float distU = 1./_piece.size(0);
    float distV = 1./_piece.size(1);
    int ii = xx[0] / distU;
    int jj = xx[1] / distV;
    
    ii = intHandling::bound(ii, 0, _piece.size(0)-1);
    jj = intHandling::bound(jj, 0, _piece.size(1)-1);
    
    aFX xxLocal 
    = 
    analyticFunction::aFXTwoD( 
      (xx[0] - ii*distU)/distU, (xx[1] - jj*distV)/distV 
    );
    return _piece[ii][jj].Y(xxLocal);	
	}
	
	bool vec2dMultiBiLinearTwoD::closed( int const & dir ) const {
		dt__throwIf( (dir!=0) && (dir!=1), closed );
		return false;
	}
	
  vec2dMultiBiLinearTwoD * vec2dMultiBiLinearTwoD::clone( void ) const {
    return new vec2dMultiBiLinearTwoD( *this );
  }
  
  vec2dMultiBiLinearTwoD * vec2dMultiBiLinearTwoD::create( void ) const {
    return new vec2dMultiBiLinearTwoD();
  }

  aFX vec2dMultiBiLinearTwoD::invY( aFY const & yy ) const {
    float distU = 1./_piece.size(0);
    float distV = 1./_piece.size(1);
    
    dt__forAllIndex(_piece, ii) {
      dt__forAllIndex(_piece[ii], jj) {    
        if (_piece[ii][jj].insideY(yy)) {   
          aFX xxLocal = _piece[ii][jj].invY(yy);
          return analyticFunction::aFXTwoD(
            ii*distU + xxLocal[0] * distU,
            jj*distV + xxLocal[1] * distV
          );
        }
      } 
    }
    twoDArrayHandling< float > rangeX0(_piece.size(0)+1, _piece.size(1)+1);
    twoDArrayHandling< float > rangeX1(_piece.size(0)+1, _piece.size(1)+1);
    
    dt__forAllIndex(_piece, ii) {
      dt__forAllIndex(_piece[ii], jj) {    
        aFX x0 = _piece[ii][jj].Y( analyticFunction::aFXTwoD(0., 0.) );
        aFX x1 = _piece[ii][jj].Y( analyticFunction::aFXTwoD(1., 0.) );
        aFX x2 = _piece[ii][jj].Y( analyticFunction::aFXTwoD(1., 1.) );
        aFX x3 = _piece[ii][jj].Y( analyticFunction::aFXTwoD(0., 1.) );

        rangeX0[ii  ][jj  ] = x0[0];
        rangeX0[ii+1][jj  ] = x1[0];
        rangeX0[ii+1][jj+1] = x2[0];
        rangeX0[ii  ][jj+1] = x3[0];
        rangeX1[ii  ][jj  ] = x0[1];
        rangeX1[ii+1][jj  ] = x1[1];
        rangeX1[ii+1][jj+1] = x2[1];
        rangeX1[ii  ][jj+1] = x3[1];        
      }
    }
    
    dt__throw(
      invY(),
      << dt__eval(yy) << std::endl
      << "rangeX0" << std::endl
      << logMe::floatMatrixToString(rangeX0) << std::endl
      << "rangeX1" << std::endl
      << logMe::floatMatrixToString(rangeX1) << std::endl
      << "Cannot determine interval."
    );
    
  }
}