#ifndef bVODumpModel_H
#define	bVODumpModel_H

#include <dtOOTypeDef.h>

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
      dt__classSelfCreate(bVODumpModel);
      bVODumpModel();
      virtual ~bVODumpModel();
      virtual void preUpdate( void );
      virtual void postUpdate( void );
      using bVOInterface::postUpdate;
    private:
      std::vector< std::string > _regionLabel;
      static bool _registrated;
  };
}
#endif	/* bVODumpModel_H */

