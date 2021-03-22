#ifndef geomVolume_multipleBoundedVolumeAGXmlBuilder_H
#define	geomVolume_multipleBoundedVolumeAGXmlBuilder_H

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
        cVPtrVec const * const cV,  
        aFPtrVec const * const aF,  
        aGPtrVec const * const aG,
        aGPtrVec * result 
      ) const; 
    private: 
      static bool _registrated;          
  };
}
#endif	/* geomVolume_multipleBoundedVolumeAGXmlBuilder_H */

