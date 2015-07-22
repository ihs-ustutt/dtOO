#ifndef dtMeshOperatorFactory_H
#define	dtMeshOperatorFactory_H

#include <string>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtMeshOperator;
  
  class dtMeshOperatorFactory {
  public:
    dt__classOnlyName(dtMeshOperatorFactory);
    dtMeshOperatorFactory();
    virtual ~dtMeshOperatorFactory();
    static dtMeshOperator * create( std::string const str );
  private:

  };
}

#endif	/* dtMeshOperatorFactory_H */

