#ifndef TRANSFORMERCONTAINER_H
#define	TRANSFORMERCONTAINER_H

#include <dtLinearAlgebra.h>
#include <string>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtTransformer;
  
  class transformerContainer {
    public:    
      dt__CLASSNAME(transformerContainer);
      transformerContainer();
      transformerContainer(const transformerContainer& orig);
      virtual ~transformerContainer();
      void add(dtTransformer const * const dtT );
      dtTransformer const * get( std::string const string ) const;
      bool has(std::string const string) const;
      bool isEmpty( void ) const;
    private:
      vectorHandling< dtTransformer * > _dtT;
  };
}
#endif	/* TRANSFORMERCONTAINER_H */
