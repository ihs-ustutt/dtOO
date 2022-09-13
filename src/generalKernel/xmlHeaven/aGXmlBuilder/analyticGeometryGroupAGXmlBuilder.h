#ifndef analyticGeometryGroupAGXmlBuilder_H
#define	analyticGeometryGroupAGXmlBuilder_H

#include <dtOOTypeDef.h>

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
        lvH_constValue const * const cV,  
        lvH_analyticFunction const * const aF,  
        lvH_analyticGeometry const * const aG,
        lvH_analyticGeometry * result 
      ) const;
    private: 
      static bool _registrated;            
  };
}
#endif	/* analyticGeometryGroupAGXmlBuilder_H */

