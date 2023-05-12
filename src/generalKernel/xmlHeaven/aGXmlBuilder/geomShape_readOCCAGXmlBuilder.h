#ifndef geomShape_readOCCAGXmlBuilder_H
#define	geomShape_readOCCAGXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class geomShape_readOCCAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(geomShape_readOCCAGXmlBuilder);
      dt__classSelfCreate(geomShape_readOCCAGXmlBuilder);
      geomShape_readOCCAGXmlBuilder();
      virtual ~geomShape_readOCCAGXmlBuilder();
      virtual std::vector< std::string > factoryAlias( void ) const;
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
#endif	/* geomShape_readOCCAGXmlBuilder_H */

