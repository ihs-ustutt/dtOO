#ifndef geomVolume_multipleBoundedVolumeAGXmlBuilder_H
#define	geomVolume_multipleBoundedVolumeAGXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class geomVolume_multipleBoundedVolumeAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(geomVolume_multipleBoundedVolumeAGXmlBuilder);
      dt__classSelfCreate(geomVolume_multipleBoundedVolumeAGXmlBuilder);
      geomVolume_multipleBoundedVolumeAGXmlBuilder();
      virtual ~geomVolume_multipleBoundedVolumeAGXmlBuilder();
      virtual void buildPart(
        ::QDomElement const & toBuild,
        baseContainer * const bC,
        lvH_constValue const * const cV,  
        lvH_analyticFunction const * const aF,  
        lvH_analyticGeometry const * const aG,
        lvH_analyticGeometry * result 
      ) const; 
    private: 
      static bool _registrated;          
  };
}
#endif	/* geomVolume_multipleBoundedVolumeAGXmlBuilder_H */

