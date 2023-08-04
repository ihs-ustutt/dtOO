#ifndef TRANSFORMERCONTAINER_H
#define	TRANSFORMERCONTAINER_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <string>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/labeledVectorHandling.h>

namespace dtOO {
  class dtTransformer;
  
  class transformerContainer {
    public:    
      dt__classOnlyName(transformerContainer);
      transformerContainer();
      transformerContainer(const transformerContainer& orig);
      virtual ~transformerContainer();
      void add(dtTransformer const * const dtT );
      dtTransformer const * get( std::string const string ) const;
      dtTransformer const * operator[]( std::string const label ) const;
      bool has(std::string const string) const;
      bool isEmpty( void ) const;
      std::vector< std::string > labels( void ) const;
      labeledVectorHandling< dtTransformer * > & lVH( void );
      const labeledVectorHandling< dtTransformer * > & lVH( void ) const;
    private:
      labeledVectorHandling< dtTransformer * > _dtT;
  };
}
#endif	/* TRANSFORMERCONTAINER_H */

