#ifndef boundedVolume_H
#define	boundedVolume_H

#include <dtOOTypeDef.h>

#include <string>
#include <map>
#include <logMe/dtMacros.h>
#include <bVObserver/bVOSubject.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/optionHandling.h>
#include <interfaceHeaven/renderInterface.h>
#include <dtLinearAlgebra.h>
#include <mainConceptFwd.h>
#include <interfaceHeaven/labeledVectorHandling.h>

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
      virtual boundedVolume * create( void ) const = 0;     
      virtual std::vector< std::string > factoryAlias( void ) const;    
      virtual void init( 
        ::QDomElement const & element,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const sF,
        lvH_analyticGeometry const * const aF,
        lvH_boundedVolume const * const bV
      );
      virtual void makeGrid(void) = 0;
      virtual void makePreGrid(void) = 0;
      bool isMeshed( void ) const;
      void setMeshed( void );
      bool isPreMeshed( void ) const;
      void setPreMeshed( void );    
      virtual std::vector< std::string > getMeshTags( void ) const = 0;
      virtual dtGmshFace * getFace( std::string const & tag ) const = 0;
      virtual dtGmshRegion * getRegion( std::string const & tag ) const = 0;
      virtual dtGmshModel * getModel( void ) const = 0;
    private:
      bool _meshed;
      bool _preMeshed;
  };
}
#endif	/* boundedVolume_H */

