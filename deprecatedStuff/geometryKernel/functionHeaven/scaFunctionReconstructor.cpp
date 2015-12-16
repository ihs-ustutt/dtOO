#include "scaFunctionReconstructor.h"
#include "scaFunction.h"
#include "scaOneD.h"
#include <logMe/dtMacros.h>
#include <do/coDoSet.h>

namespace dtOO {
  scaFunctionReconstructor::scaFunctionReconstructor() {
  }

  scaFunctionReconstructor::~scaFunctionReconstructor() {
  }
  
  scaFunction * scaFunctionReconstructor::reconFromCoDoSet ( 
                 covise::coDoSet const * const obj 
               ) const {
    
    scaFunction * ret 
		= 
		scaFunctionFactory::create( obj->getAttribute("factoryType") );
    
		dt__ptrAss(scaOneD * s1d, scaOneD::DownCast(ret));
    
		s1d->unpackFromCoDoSet(obj);
    
    return ret;
  }
}
