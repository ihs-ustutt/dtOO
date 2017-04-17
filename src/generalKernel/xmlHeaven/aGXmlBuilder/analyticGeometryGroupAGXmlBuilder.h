#ifndef analyticGeometryGroupAGXmlBuilder_H
#define	analyticGeometryGroupAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>

namespace dtOO {
  class analyticGeometryGroupAGXmlBuilder : public aGXmlBuilder {
  public:  
    dt__classOnlyName(analyticGeometryGroupAGXmlBuilder);
    dt__classSelfCreate(analyticGeometryGroupAGXmlBuilder);
    analyticGeometryGroupAGXmlBuilder();
    virtual ~analyticGeometryGroupAGXmlBuilder();
    virtual std::vector< std::string > factoryAlias( void ) const;
    virtual void buildPart(
      ::QDomElement const & toBuild,
      baseContainer * const bC,
      cVPtrVec const * const cV,  
      aFPtrVec const * const aF,  
      aGPtrVec const * const aG,
      aGPtrVec * result 
    ) const;
  };
}
#endif	/* analyticGeometryGroupAGXmlBuilder_H */

