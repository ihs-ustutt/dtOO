#ifndef analyticFunction_H
#define	analyticFunction_H

#include <logMe/dtMacros.h>
#include <interfaceHeaven/optionHandling.h>
#include <interfaceHeaven/labelHandling.h>
#include <vector>
#include <interfaceHeaven/renderInterface.h>
#include <dtLinearAlgebra.h>

#include "aFX.h"
#include "aFY.h"

#include <mainConceptFwd.h>

namespace dtOO {
  class dtTransformer;
  
  class analyticFunction : public optionHandling,
                           public labelHandling,
                           public renderInterface {
  public:
    dt__class(analyticFunction, analyticFunction);
    analyticFunction();
    analyticFunction(const analyticFunction& orig);
    virtual ~analyticFunction();
    virtual analyticFunction * clone( void ) const = 0;
    virtual analyticFunction * weakClone( void ) const;
    virtual analyticFunction * cloneTransformed( 
      dtTransformer const * const dtT 
    ) const = 0;
    virtual analyticFunction * create( void ) const = 0;
    virtual void dump(void) const;    
    virtual std::string dumpToString(void) const;
    virtual aFY Y(aFX const & xx) const = 0;
    virtual int xDim( void ) const = 0;
    virtual int yDim( void ) const = 0;
    virtual float xMin( int const & dir) const = 0;
    virtual float xMax( int const & dir) const = 0;
    virtual aFX xMin( void ) const;
    virtual aFX xMax( void ) const;        
	  virtual aFX x_percent( aFX const & xx ) const;
    virtual aFX percent_x( aFX const & xx ) const;    
    virtual aFX invY(aFY const & yy) const;    
    virtual bool isCompound( void ) const;
    virtual vectorHandling< analyticFunction * > const & vecRef( void ) const;
    virtual std::map< int, int > const & mapRef( void ) const;
    virtual bool isTransformed( void ) const;
    virtual std::pair< aFY, aFY > yBoundingBox( void ) const;
    virtual bool yBounded( void ) const;
    static aFX aFXZeroD( void );
    static aFX aFXOneD( float const & x0 );
    static aFX aFXTwoD( float const & x0, float const & x1 );
    static aFX aFXThreeD( 
      float const & x0, float const & x1, float const & x2 
    );
    static aFX aFXTwoD( dtPoint2 const & pp );    
    static aFX aFXThreeD( dtPoint3 const & pp );
    static aFY aFYZeroD( void );    
    static aFY aFYOneD( float const & x0 );
    static aFY aFYTwoD( float const & x0, float const & x1 );
    static aFY aFYThreeD( 
      float const & x0, float const & x1, float const & x2 
    );    
    static aFY aFYTwoD( dtPoint2 const & pp );    
    static aFY aFYThreeD( dtPoint3 const & pp );    
//    static aFX aFX( std::vector< float > const & xx );    
//    static aFY aFY( std::vector< float > const & xx );
    static aFY aFY_aFX( aFX const & xx );    
    static aFX aFX_aFY( aFY const & yy );
    static float distance( aFX const & x0, aFX const & x1 );
  private:
	  double F(double const * xx) const;
    mutable aFY _invY;
  };
  dt__H_addCloneForpVH(analyticFunction);
}
#endif	/* analyticFunction_H */

