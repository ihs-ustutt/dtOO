#ifndef boundedVolume_H
#define	boundedVolume_H

#include <string>
#include <map>
#include <logMe/dtMacros.h>
#include <bVObserver/bVOSubject.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/optionHandling.h>
#include <interfaceHeaven/renderInterface.h>
#include <dtLinearAlgebra.h>
#include <mainConceptFwd.h>

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
    dt__class(boundedVolume, boundedVolume);
    boundedVolume();
    virtual ~boundedVolume();
    virtual void init( 
      ::QDomElement const & element,
      baseContainer const * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const sF,
      aGPtrVec const * const aF,
      bVPtrVec const * const bV
    );
    virtual void makeGrid(void) = 0;
    virtual void makePreGrid(void) = 0;
    bool isMeshed( void ) const;
    void setMeshed( void );
    virtual std::vector< std::string > getMeshTags( void ) const = 0;
	  virtual dtGmshFace * getFace( std::string const & tag ) const = 0;
    virtual dtGmshRegion * getRegion( std::string const & tag ) const = 0;
    virtual dtGmshModel * getModel( void ) const = 0;
  private:
    bool _meshed;
  };
}
#endif	/* boundedVolume_H */

