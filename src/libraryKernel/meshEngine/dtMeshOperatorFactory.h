#ifndef dtMeshOperatorFactory_H
#define	dtMeshOperatorFactory_H

#include <dtOOTypeDef.h>

#include <string>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtMeshOperator;
  
  class dtMeshOperatorFactory {
    public:
      dt__classOnlyName(dtMeshOperatorFactory);
      virtual ~dtMeshOperatorFactory();
      static bool registrate( dtMeshOperator const * const );
      static dtMeshOperator * create(char const * const str);
      static dtMeshOperator * create( std::string const str );
    private:
      dtMeshOperatorFactory();
    private:
      static dt__pVH(dtMeshOperator) _product;
  };
}

#endif	/* dtMeshOperatorFactory_H */

