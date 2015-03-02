#ifndef ANALYTICGEOMETRY_H
#define	ANALYTICGEOMETRY_H

#include <dtLinearAlgebra.h>
#include <vector>
#include <string>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/optionHandling.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/renderInterface.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO { 
  /**
   * @author A. Tismer
   * @date 05.11.2012
   * @brief Abstract analytic geometry class
   * Defines basic interfaces of an analyticGeometry.
   */
  class analyticGeometry : public optionHandling,
                           public labelHandling,
                           public renderInterface {
  public:  
    dt__CLASSSTD(analyticGeometry, analyticGeometry);
    analyticGeometry();
    virtual ~analyticGeometry();
    analyticGeometry( analyticGeometry const & orig );
    void dump(void) const;
    virtual std::string dumpToString(void) const;
    void setRenderResolution(int const & dir, int const & value) const;
    int getRenderResolution(int const & dir) const;
    virtual analyticGeometry * clone( void ) const = 0;
    virtual analyticGeometry * create( void ) const = 0;
//    virtual void packToExtInfoContainer( extInfoContainer * const eIC ) const;
    virtual bool isCompound( void ) const;
	  virtual vectorHandling< analyticGeometry const * > compoundInternal( void ) const;
  private:
    mutable int _resU;
    mutable int _resV;
    mutable int _resW;
    std::vector< std::string > _attType;
    std::vector< std::string > _attValue;
  };
  
	//
	// boost requires this method in ptr_vector
	//  
  analyticGeometry * new_clone(analyticGeometry const & aG);
}
#endif	/* ANALYTICGEOMETRY_H */

