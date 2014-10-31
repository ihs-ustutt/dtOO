#ifndef ANALYTICGEOMETRYBUILDERFACTORY_H
#define	ANALYTICGEOMETRYBUILDERFACTORY_H

#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometryBuilder;

  /**
   * Analytic geometry builder factory
   * @author A. Tismer
   * @date 27.12.2012
   */
  class analyticGeometryBuilderFactory {
    DTCLASSNAMEMETHOD(analyticGeometryBuilderFactory);
    DTCLASSLOGMETHOD(analyticGeometryBuilderFactory);
  public:
    /**
     * constructor
     */
    analyticGeometryBuilderFactory();
    /**
     * destructor
     */
    virtual ~analyticGeometryBuilderFactory();
    /**
     * create a special builder
     * @param str character id of special builder
     * @return pointer to desired builder
     */
    analyticGeometryBuilder* create(char const * const str) const;
  private:

  };
}
#endif	/* ANALYTICGEOMETRYBUILDERFACTORY_H */

