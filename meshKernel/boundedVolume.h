#ifndef BOUNDEDVOLUME_H
#define	BOUNDEDVOLUME_H

#include <string>
#include <map>
#include <logMe/dtMacros.h>
#include <bVObserver/bVOSubject.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/optionHandling.h>
#include <interfaceHeaven/vectorHandling.h>
#include <interfaceHeaven/renderInterface.h>
#include <dtLinearAlgebra.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;
  class dtGmshFace;
  class dtGmshRegion;
  class dtGmshModel;
  
  class boundedVolume : public bVOSubject,
                        public labelHandling,
                        public optionHandling,
                        public renderInterface {
  public:
    dt__classOnlyName(boundedVolume);
    boundedVolume();
    virtual ~boundedVolume();
    virtual void init( 
      QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const sF,
      vectorHandling< analyticGeometry * > const * const aF,
      vectorHandling< boundedVolume * > const * const bV
    );
    virtual void makeGrid(void) = 0;
    virtual void makePreGrid(void) = 0;
    bool isMeshed( void ) const;
    void setMeshed( void );
    virtual std::vector< std::string > getMeshTags( void ) const;
	  virtual dtGmshFace * getFace( std::string const & tag ) const;
    virtual dtGmshRegion * getRegion( std::string const & tag ) const;
    virtual dtGmshModel * getModel( void ) const;
  private:
    bool _meshed;
  };
}
#endif	/* BOUNDEDVOLUME_H */

