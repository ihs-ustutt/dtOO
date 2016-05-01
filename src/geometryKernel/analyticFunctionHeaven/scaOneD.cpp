#include "scaOneD.h"
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>

#include <interfaceHeaven/staticPropertiesHandler.h>
#include <solid2dLine.h>

namespace dtOO {
  scaOneD::scaOneD() : scaFunction() {
    _xxMax = std::numeric_limits<float>::min();
    _xxMin = std::numeric_limits<float>::max();
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
  
  float scaOneD::invYFloat(float const & yy) const {
		return invY( analyticFunction::aFYOneD(yy) )[0];
  }
	
  float scaOneD::YFloatPercent( float const & xP ) const {
    return YFloat( x_percent(xP) );
  }	
  
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
  
  vectorHandling< renderInterface * > scaOneD::getRender( void ) const {
		int nU
		=
		staticPropertiesHandler::getInstance()->getOptionInt(
      "function_render_resolution_u"
    );		
		
		vectorHandling< dtPoint2 > p2(nU);
    float interval = (xMax(0) - xMin(0)) / (nU-1);
    for (int ii=0;ii<nU;ii++) {
			float iiF = static_cast<float>(ii);
      float xx = xMin(0) + iiF * interval;
      float yy = YFloat(xx);
			p2[ii] = dtPoint2(xx, yy);
    }
		
		vectorHandling< renderInterface * > rV(1);
		rV[0] = new solid2dLine(p2);
		
		return rV;
  }  
  dt__C_addCloneForpVH(scaOneD);        
}