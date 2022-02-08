#ifndef aGXmlBuilder_H
#define	aGXmlBuilder_H

#include <dtOOTypeDef.h>

#include "dtXmlParserBase.h"
#include <vector>
#include <string>
#include <logMe/dtMacros.h>

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class aGXmlBuilder {
    public:
      dt__classOnlyName(aGXmlBuilder);
      aGXmlBuilder();
      virtual ~aGXmlBuilder();
      virtual aGXmlBuilder * create( void ) const = 0;      
      virtual void buildPart(
        ::QDomElement const & toBuild,
        baseContainer * const bC,   
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG,
        aGPtrVec * result 
      ) const = 0;
      virtual void buildPartCompound(
        ::QDomElement const & toBuild,
        baseContainer * const bC,  
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG,
        aGPtrVec * result 
      ) const;
      virtual std::vector< std::string > factoryAlias( void ) const;      
  };
}

#endif	/* aGXmlBuilder_H */

