#include "scaOneD.h"
#include <logMe/logMe.h>
#include <progHelper.h>

namespace dtOO {
  scaOneD::scaOneD() : scaFunction() {
  }

  scaOneD::scaOneD( scaOneD const & orig) : scaFunction(orig) {
    _xxMax = orig._xxMax;
    _xxMin = orig._xxMin;
  }

  scaOneD::~scaOneD() {
  }

  float scaOneD::YFloat(aFX const & xx) const {
		if (xx.size() != 1) {
			dt__throw(
			  YFloat(), 
				<< dt__eval(xx.size()) << std::endl 
				<< "Should be 1."
			);
		}
		
		return YFloat( xx[0] );
	}
	
  float scaOneD::YFloatPercent( float const & xP ) const {
    return YFloat( x_percent(xP) );
  }	
  
//  float scaOneD::optionFunction(std::string const option, float const yy) const {
//    if (option == "invertSimpleNewton") {
//      return simpleNewton(yy);
//    }
//    else if (option == "getVectorValuePercent().x()") {
//      return getVectorValuePercent(yy)[0];
//    }    
//    if (option == "getVectorValuePercent().y()") {
//      return getVectorValuePercent(yy)[1];
//    }        
//    else {
//      dt__THROW(optionFunction(),
//              << "Unknown " << dt__eval(option));
//    }
//  }
  
//  float scaOneD::simpleNewton( float const yy) const {
//    DTBUFFERINIT();
//    
//    DTBUFFER( << dt__eval(yy) << std::endl);
//    
//    float xx[3];
//    
////    float diff = getMaxX() - getMinX();
//    xx[1] = xMin(0);// + .25 * diff;
//    xx[2] = xMax(0);// - .25 * diff;
//       
//    for (int ii=0;ii<100;ii++) {
//      float diffF = YFloat(xx[1]) - YFloat(xx[2]);
//      if ( diffF != 0. ) {
//        xx[0] = xx[1] - (xx[1] - xx[2])/(diffF) * (YFloat(xx[1]) - yy);
//      }
//      else {
//        DTBUFFER( 
//          << dt__eval(YFloat(xx[1])) << std::endl
//          << dt__eval(YFloat(xx[2])) << std::endl
//          << "Unexpected end of iteration. Set xx[0] = xx[1]"
//          << std::endl );
//        xx[0] = xx[1];
//        break;
//      }
//      float res = xx[0] - xx[1];
//      
//      DTBUFFER( 
//        << dt__eval(ii) << " | "
//        << dt__eval(xx[0]) << " | " 
//        << dt__eval(xx[1]) << " | " 
//        << dt__eval(xx[2]) << " | " 
//        << dt__eval(res) << std::endl ); 
//      xx[2] = xx[1];
//      xx[1] = xx[0];
//      
//      if (res == 0.) {
//        break;
//      }
//    }
//    
//    DTBUFFER( << "f(" << xx[0] << ") = " << YFloat(xx[0]) << std::endl);
//    
//    dt__debug_BUFFER( simpleNewton() );
//    
//    return xx[0];
//  }
  
  int scaOneD::xDim( void ) const {
		return 1;
	}
	
  float scaOneD::xMin( int const & dir) const {
    switch (dir) {
      case 0:
        return _xxMin;
        break;
      default:
        dt__throw(xMin(),
              << dt__eval(dir) << std::endl
              << "dir should be 0.");
    }   
	}
	
  float scaOneD::xMax( int const & dir) const {
    switch (dir) {
      case 0:
        return _xxMax;
        break;
      default:
        dt__throw(xMax(),
              << dt__eval(dir) << std::endl
              << "dir should be 0.");
    }
	}
  
  void scaOneD::setMinMax( float const min, float const max) {
    _xxMin = min;
    _xxMax = max;
  }
  
  void scaOneD::setMin(float const min) {
    _xxMin = min;
  }

  void scaOneD::setMax(float const max) {
    _xxMax = max;
  }  
  
	float scaOneD::x_percent(float const & xx) const {
    return (xMin(0) +  (xMax(0) - xMin(0) ) * xx);
  }  
  
  float scaOneD::percent_x(float const & xx) const {
    return ( (xx - xMin(0)) / (xMax(0) - xMin(0)) );
  }
	
  float scaOneD::DYFloat( float const & xx ) const {
    float xP = percent_x(xx);
    float const deltaPer = 0.01;

    if (xP<0.01) {
      return (
        (YFloatPercent(deltaPer) - YFloatPercent(0.))
        /
        (x_percent(deltaPer) - x_percent(0.) )
      );      
    }
    else if ( (xP>=deltaPer) && (xP<=(1.-deltaPer)) ) {
      return (
        ( YFloatPercent(xP+deltaPer) - YFloatPercent(xP-deltaPer) )
        /
        ( x_percent(xP+deltaPer) - x_percent(xP-deltaPer) )
      );
    }
    else if (xP>0.99) {
      return (
        (YFloatPercent(1.) - YFloatPercent(1.-deltaPer))
        /
        (x_percent(1.) - x_percent(1.-deltaPer) )
      );      
    }
  }
		
  float scaOneD::getLength( void ) const {
    float epsge = 1.e-08;
    float curLength = 0.;
    float value[2];
    float valueOld[2];
    value[0] = scaOneD::xMin(0);
    value[1] = YFloat( value[0] );
    
    float jjF = 0.;
    while (value[0] <= scaOneD::xMax(0)) {
      valueOld[0] = value[0];
      valueOld[1] = value[1];
      jjF = jjF + 1.;
      value[0] = scaOneD::xMin(0) 
                 + (
                     scaOneD::xMax(0) - scaOneD::xMin(0)
                   ) * jjF * epsge;
      value[1] = YFloat( value[0] );
      
      if ( value[0] > scaOneD::xMax(0) ) {
        value[0] = scaOneD::xMax(0);
        break;
      }

      // calculate current length
      curLength = curLength
                  + sqrt( (value[0] - valueOld[0])*(value[0] - valueOld[0]) 
                          + (value[1] - valueOld[1])*(value[1] - valueOld[1]) 
                        );
    }

    //
    // last point
    //
    value[0] = scaOneD::xMax(0);
    value[1] = YFloat( value[0] );

    // calculate current length
    curLength = curLength 
                + sqrt( (value[0] - valueOld[0])*(value[0] - valueOld[0]) 
                        + (value[1] - valueOld[1])*(value[1] - valueOld[1]) 
                      );
      
    return curLength;
  }
  
  dt__C_addCloneForpVH(scaOneD);        
}