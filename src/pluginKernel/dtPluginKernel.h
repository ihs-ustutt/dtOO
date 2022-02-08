#ifndef dtPluginKernel_H
#define	dtPluginKernel_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <pluginEngine/pugg/Kernel.h>

namespace dtOO {
  class dtPluginKernel : public ::pugg::Kernel {
    public:
      dt__classOnlyName(dtPluginKernel);       
      ~dtPluginKernel();
    private:

  };
}

#endif	/* dtPluginKernel_H */

