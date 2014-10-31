#ifndef ANALYTICGEOMETRYDIRECTOR_H
#define	ANALYTICGEOMETRYDIRECTOR_H

#include "analyticGeometryBuilder.h"
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  /**
   * Director of the analytic geometry builder classes
   * @author A. Tismer
   * @date 27.12.2012
   */
  class analyticGeometryDirector {
    DTCLASSNAMEMETHOD(analyticGeometryDirector);
    DTCLASSLOGMETHOD(analyticGeometryDirector);
  public:
    /**
     * constructor
     */
    analyticGeometryDirector();
    /**
     * destructor
     */
    virtual ~analyticGeometryDirector();
    /**
     * set the pointer to the desired builder
     * @param _builderToSet pointer to builder
     */
    void setBuilder(analyticGeometryBuilder* _builderToSet);
    /**
     * construct the object
     * @param aGeo result of builder process
     * @return error flag
     */
    int construct(vectorHandling< analyticGeometry* >* aGeo);
  private:
    analyticGeometryBuilder* _builder;
  };
}

#endif	/* ANALYTICGEOMETRYDIRECTOR_H */

