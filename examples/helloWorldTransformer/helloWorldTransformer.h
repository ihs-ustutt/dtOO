#ifndef helloWorldTransformer_H
#define	helloWorldTransformer_H

#include <mainConceptFwd.h>
#include <pluginEngine/pugg/Driver.h>
#include <dtTransformerHeaven/dtTransformerInvThreeD.h>
#include <dtTransformerHeaven/dtTransformerDriver.h>

#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticGeometry;
  class analyticFunction; 
  
  class helloWorldTransformer : public dtTransformerInvThreeD {
    public:
      dt__class(helloWorldTransformer, dtTransformerInvThreeD);
      helloWorldTransformer();
      virtual ~helloWorldTransformer();
      helloWorldTransformer(helloWorldTransformer const & orig);
      virtual helloWorldTransformer * clone( void ) const;
      virtual helloWorldTransformer * create( void ) const;
      virtual void init( 
        ::QDomElement const * tE, 
        baseContainer * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG 
      );
      virtual bool isNecessary( void ) const;
      virtual std::vector< dtPoint3 > apply( 
        std::vector< dtPoint3 > const * const toTrans 
      ) const;      
      virtual std::vector< dtPoint3 > retract( 
        std::vector< dtPoint3 > const * const toTrans 
      ) const;       
      virtual std::vector< dtVector3 > apply( 
        std::vector< dtVector3 > const * const toTrans 
      ) const;      
      virtual std::vector< dtVector3 > retract( 
        std::vector< dtVector3 > const * const toTrans 
      ) const;           
      virtual aGPtrVec apply( aGPtrVec const * const toTrans ) const;      
    private:
      float _float;
  };
}

class helloWorldTransformerDriver : public dtOO::dtTransformerDriver {
public:
    helloWorldTransformerDriver();
    dtOO::dtTransformer * create();
};

#endif	/* helloWorldTransformer_H */