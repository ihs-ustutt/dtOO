#include "cardinalDirVolumeDefines.h"

#include <logMe/logMe.h>

namespace dtOO {  
  cardinalDirVolumeDefines::cardinalDirVolumeDefines() {
    _indexOffset = 0;
  }

  cardinalDirVolumeDefines::cardinalDirVolumeDefines(int const offset) {
    _indexOffset = offset;
  }
  
  cardinalDirVolumeDefines::~cardinalDirVolumeDefines() {
    
  }
  
  int cardinalDirVolumeDefines::transId( int const toTrans ) const {
    return toTrans + _indexOffset;
  }
}
