#ifndef BASECONTAINER_H
#define	BASECONTAINER_H

#include <logMe/dtMacros.h>
#include <logMe/dtMacros.h>


namespace dtOO {
  class pointContainer;
  class vectorContainer;
  class transformerContainer;
  
  class baseContainer {
  public:
    dt__classOnlyName(baseContainer);    
    baseContainer();
    pointContainer * const ptrPointContainer( void );
	  pointContainer const * const constPtrPointContainer( void ) const;
    vectorContainer * const ptrVectorContainer( void );
    vectorContainer const * const constPtrVectorContainer( void ) const;
    transformerContainer * const ptrTransformerContainer( void );
    transformerContainer const * const constPtrTransformerContainer( 
      void 
    ) const;
    virtual ~baseContainer();
    void clear( void );
  private:
    dt__pH(pointContainer) _pC;
    dt__pH(vectorContainer) _vC;
    dt__pH(transformerContainer) _tC;
  };
}
#endif	/* BASECONTAINER_H */

