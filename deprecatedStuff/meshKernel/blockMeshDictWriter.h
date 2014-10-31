#ifndef BLOCKMESHDICTWRITER_H
#define	BLOCKMESHDICTWRITER_H

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <vector>
#include "simpleBlockStructure.h"

namespace covise {
  class coDoSet;
}

namespace dtOO {
  class blockMeshDictWriter : public simpleBlockStructure {
  public:
    DTCLASSLOGMETHOD(blockMeshDictWriter);
    DTCLASSNAMEMETHOD(blockMeshDictWriter);
    blockMeshDictWriter();
    blockMeshDictWriter(const blockMeshDictWriter& orig);
    virtual ~blockMeshDictWriter();
    void writeDict( std::string const casePath ) const;
  };
}
#endif	/* BLOCKMESHDICTWRITER_H */

