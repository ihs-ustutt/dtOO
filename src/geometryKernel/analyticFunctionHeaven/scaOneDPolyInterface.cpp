#include "scaOneDPolyInterface.h"

namespace dtOO {
  scaOneDPolyInterface::scaOneDPolyInterface() : scaOneD() {
  }

  scaOneDPolyInterface::scaOneDPolyInterface( 
    scaOneDPolyInterface const & orig
  ) : scaOneD(orig) {
    
  }
  
  scaOneDPolyInterface::~scaOneDPolyInterface() {
  }
}
