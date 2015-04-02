#ifndef SCAFUNCTIONBUILDERFACTORY_H
#define	SCAFUNCTIONBUILDERFACTORY_H

#include <logMe/dtMacros.h>

namespace dtOO {
  class scaFunctionBuilder;

  class scaFunctionBuilderFactory {
    dt__classOnlyName(scaFunctionBuilderFactory);
  public:
    /**
     * constructor
     */
    scaFunctionBuilderFactory();
    /**
     * destructor
     */
    virtual ~scaFunctionBuilderFactory();
    /**
     * create a special builder
     * @param str character id of special builder
     * @return pointer to desired builder
     */
    scaFunctionBuilder* create(char const * const str) const;
  private:

  };
}
#endif	/* SCAFUNCTIONBUILDERFACTORY_H */

