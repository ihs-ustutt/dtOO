#ifndef GMSHBOUNDEDVOLUME_H
#define	GMSHBOUNDEDVOLUME_H

#include <logMe/dtMacros.h>
#include <interfaceHeaven/twoDArrayHandling.h>
#include "boundedVolume.h"

class MVertex;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;
  class dtGmshFace;
  class dtGmshRegion;
  class dtGmshModel;
  
  class gmshBoundedVolume : public boundedVolume {
  public:
    dt__CLASSSTD(gmshBoundedVolume, boundedVolume);      
    gmshBoundedVolume();
    virtual ~gmshBoundedVolume();
    virtual void init( 
      QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV
    );
  	virtual vectorHandling< renderInterface * > getRender( void ) const;
  	virtual vectorHandling< renderInterface * > getExtRender( void ) const;        
    virtual std::vector< std::string > getMeshTags( void ) const;
	  virtual dtGmshFace * getFace( std::string const & tag ) const;
	  virtual dtGmshRegion * getRegion( std::string const & tag ) const;
    virtual dtGmshModel * getModel( void ) const;
  protected:
    void updatePhysicals( void );
  private:
    twoDArrayHandling< std::string > _physLabels;    
  protected:
    dt__pH(dtGmshModel) _gm;
  };
}
#endif	/* GMSHBOUNDEDVOLUME_H */

