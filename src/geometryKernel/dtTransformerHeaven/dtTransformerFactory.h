#ifndef DTTRANSFORMERFACTORY_H
#define	DTTRANSFORMERFACTORY_H

#include <dtOOTypeDef.h>

#include <string>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtTransformer;

  class dtTransformerFactory {
    public:
      dt__classOnlyName(dtTransformerFactory);
      virtual ~dtTransformerFactory();
      static bool registrate( dtTransformer const * const );     
      static dtTransformer * create(char const * const str);
      static dtTransformer * create( std::string const str); 
      static dtTransformer * createFromPlugin(
        std::string const & className, 
        std::string const & pluginName, 
        std::string const & pluginDriver
      );    
    private:
      dtTransformerFactory();    
    private:
      static dt__pVH(dtTransformer) _transformer;     
  };
}

#endif	/* DTTRANSFORMERFACTORY_H */

