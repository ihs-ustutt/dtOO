#ifndef BASECONTAINER_H
#define	BASECONTAINER_H

#include <logMe/dtMacros.h>
#include <progHelper.h>


namespace dtOO {
  class pointContainer;
  class vectorContainer;
  class transformerContainer;
  
  class baseContainer {
  public:
    dt__CLASSNAME(baseContainer);    
    baseContainer();
//    dt__pH(pointContainer) & refPtrHPointContainer( void );
//    dt__pH(vectorContainer) & refPtrHVectorContainer( void );
//    dt__pH(transformerContainer) & refPtrHTransformerContainer( void );
    pointContainer * const ptrPointContainer( void );
    vectorContainer * const ptrVectorContainer( void );
    transformerContainer * const ptrTransformerContainer( void );
    virtual ~baseContainer();
  private:
    dt__pH(pointContainer) _pC;
    dt__pH(vectorContainer) _vC;
    dt__pH(transformerContainer) _tC;
  };
}
#endif	/* BASECONTAINER_H */

