#include "scaOneD.h"
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>

#include <interfaceHeaven/staticPropertiesHandler.h>
#include <solid2dLine.h>

namespace dtOO {
  scaOneD::scaOneD() : scaFunction() {
    _xxMax = std::numeric_limits<dtReal>::min();
    _xxMin = std::numeric_limits<dtReal>::max();
  }

  scaOneD::scaOneD( scaOneD const & orig) : scaFunction(orig) {
    _xxMax = orig._xxMax;
    _xxMin = orig._xxMin;
  }

  scaOneD::~scaOneD() {
  }

  dtReal scaOneD::YFloat(aFX const & xx) const {
		if (xx.size() != 1) {
			dt__throw(
			  YFloat(), 
				<< dt__eval(xx.size()) << std::endl 
				<< "Should be 1."
			);
		}
		
		return YFloat( xx[0] );
	}
  
  dtReal scaOneD::invYFloat(dtReal const & yy) const {
		return invY( analyticFunction::aFYOneD(yy) )[0];
  }
	
  dtReal scaOneD::YFloatPercent( dtReal const & xP ) const {
    return YFloat( x_percent(xP) );
  }	
  
  int scaOneD::xDim( void ) const {
		return 1;
	}
	
  dtReal scaOneD::xMin( int const & dir) const {
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
	
  dtReal scaOneD::xMax( int const & dir) const {
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
  
  void scaOneD::setMinMax( dtReal const min, dtReal const max) {
    _xxMin = min;
    _xxMax = max;
  }
  
  void scaOneD::setMin(dtReal const min) {
    _xxMin = min;
  }

  void scaOneD::setMax(dtReal const max) {
    _xxMax = max;
  }  
  
	dtReal scaOneD::x_percent(dtReal const & xx) const {
    return (xMin(0) +  (xMax(0) - xMin(0) ) * xx);
  }  
  
  dtReal scaOneD::percent_x(dtReal const & xx) const {
    return ( (xx - xMin(0)) / (xMax(0) - xMin(0)) );
  }
	
  dtReal scaOneD::DYFloat( dtReal const & xx ) const {
    dtReal xP = percent_x(xx);
    dtReal const deltaPer = 0.01;

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
		
  dtReal scaOneD::getLength( void ) const {
    dtReal epsge = 1.e-08;
    dtReal curLength = 0.;
    dtReal value[2];
    dtReal valueOld[2];
    value[0] = scaOneD::xMin(0);
    value[1] = YFloat( value[0] );
    
    dtReal jjF = 0.;
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
    dtReal interval = (xMax(0) - xMin(0)) / (nU-1);
    for (int ii=0;ii<nU;ii++) {
			dtReal iiF = static_cast<dtReal>(ii);
      dtReal xx = xMin(0) + iiF * interval;
      dtReal yy = YFloat(xx);
			p2[ii] = dtPoint2(xx, yy);
    }
		
		vectorHandling< renderInterface * > rV(1);
		rV[0] = new solid2dLine(p2);
		
		return rV;
  }  
  dt__C_addCloneForpVH(scaOneD);        
}