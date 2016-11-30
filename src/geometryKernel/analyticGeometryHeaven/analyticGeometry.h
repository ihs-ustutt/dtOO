#ifndef analyticGeometry_H
#define	analyticGeometry_H

#include <dtLinearAlgebra.h>
#include <vector>
#include <string>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/optionHandling.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/renderInterface.h>
#include <interfaceHeaven/vectorHandling.h>
#include <interfaceHeaven/labeledVectorHandling.h>
#include <interfaceHeaven/threadSafeRoot.h>
#include <mainConceptFwd.h>

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
    dt__class(analyticGeometry, analyticGeometry);
    analyticGeometry();
    virtual ~analyticGeometry();
    analyticGeometry( analyticGeometry const & orig );
    dtPoint3 getPointPercent( float const * const uvw ) const;    
    //
    // overload
    //
    virtual analyticGeometry * clone( void ) const = 0;
    virtual analyticGeometry * create( void ) const = 0;
    virtual int dim( void ) const = 0;
    virtual bool isClosed( int const & dir) const = 0;
    virtual float getMin( int const & dir) const = 0;
    virtual float getMax( int const & dir) const = 0;
    virtual dtPoint3 getPoint( float const * const uvw ) const = 0;
    //
    // optional overload
    //
    virtual std::string dumpToString(void) const;
    virtual bool isTransformed( void ) const;
    virtual bool isCompound( void ) const;
	  virtual vectorHandling< analyticGeometry const * > compoundInternal( 
      void 
    ) const;
    std::pair< dtPoint3, dtPoint3 > boundingBox( void ) const;
    void boundingBox( std::pair< dtPoint3, dtPoint3 > boundingBox);
    void updateBoundingBox( void ) const;
    float characteristicLength( void ) const;
    //
    //
    //
    static bool inXYZTolerance(
      dtPoint3 const & p0, dtPoint3 const & p1, 
      bool output = false, float inc = 1.
    );
    //
    //
    //
    void dump(void) const;    
    void setRenderResolution(int const & dir, int const & value) const;
    int getRenderResolution(int const & dir) const;    
  private:
    mutable int _resU;
    mutable int _resV;
    mutable int _resW;
    mutable float _characteristicLength;
    mutable std::pair< dtPoint3, dtPoint3 > _boundingBox;
    threadSafeRoot _threadRoot;
  };
  
	//
	// boost requires this method in ptr_vector
	//  
  analyticGeometry * new_clone(analyticGeometry const & aG);
}
#endif	/* analyticGeometry_H */

