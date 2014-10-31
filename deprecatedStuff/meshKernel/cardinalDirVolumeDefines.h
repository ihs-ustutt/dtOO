#ifndef CARDINALDIRVOLUMEDEFINES_H
#define	CARDINALDIRVOLUMEDEFINES_H

#include <logMe/dtMacros.h>

namespace dtOO {
  class cardinalDirVolumeDefines {
    public:
      dt__CLASSNAME(cardinalDirVolumeDefines);
      cardinalDirVolumeDefines();
      cardinalDirVolumeDefines(int const offset);
      ~cardinalDirVolumeDefines();
      int transId( int const toTrans ) const;
    private:
      int _indexOffset;
  };
}

#endif	/* CARDINALDIRVOLUMEDEFINES_H */

