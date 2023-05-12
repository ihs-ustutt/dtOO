#ifndef DTMESHOPERATOR_H
#define	DTMESHOPERATOR_H

#include <logMe/dtMacros.h>
#include <dtOOTypeDef.h>

#include <jsonHeaven/jsonPrimitive.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/optionHandling.h>
#include <mainConceptFwd.h>

class QDomElement;

namespace dtOO {
  class baseContainer;
  
  class dtMeshOperator : public labelHandling, public optionHandling {
    public:
      dt__class(dtMeshOperator, labelHandling);    
      dtMeshOperator();
      virtual ~dtMeshOperator();
      virtual dtMeshOperator * create( void ) const = 0;     
      virtual std::vector< std::string > factoryAlias( void ) const;    
      virtual void jInit(
        jsonPrimitive const & jE,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        lvH_dtMeshOperator const * const mO
      );
      virtual void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        lvH_dtMeshOperator const * const mO
      );
    protected:
      jsonPrimitive & config( void );
      jsonPrimitive const & config( void ) const;
    private:
      jsonPrimitive _config;
  };
  dt__I_addCloneForpVHNotImpl(dtMeshOperator);
}
#endif	/* DTMESHOPERATOR_H */

