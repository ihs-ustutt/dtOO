#ifndef bVODumpModel_H
#define	bVODumpModel_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <interfaceHeaven/twoDArrayHandling.h>

namespace dtOO {
  class boundedVolume;
  class dtGmshEdge;
  
  class bVODumpModel : public bVOInterface {
  public:
    dt__class(bVODumpModel, bVOInterface);
    bVODumpModel();
    virtual ~bVODumpModel();
    virtual void preUpdate( void );
    using bVOInterface::postUpdate;
  private:
    std::vector< std::string > _regionLabel;
  };
}
#endif	/* bVODumpModel_H */
