#ifndef CREATEOPENFOAMCASE_H
#define	CREATEOPENFOAMCASE_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

#include "dtPlugin.h"

class MVertex;
class MElement;

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  class baseContainer;
  
  class dtGmshModel;
  
  class createOpenFOAMCase : public dtPlugin {
  public:
    dt__class(createOpenFOAMCase, dtPlugin);    
    createOpenFOAMCase();
    virtual ~createOpenFOAMCase();
    virtual void init( 
      ::QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      vectorHandling< dtPlugin * > const * const pL
    );    
    virtual void apply(void);
  private:
    void initMeshVectors( 
      std::vector< ::MVertex * > & allVerts, 
      std::vector< std::pair< ::MElement *, int > > & allElems,
      std::map< int, std::string > & physicalNames    
    );
  private:
    std::string _workingDirectory;
    std::vector< boundedVolume * > _bV;
  };
}
#endif	/* CREATEOPENFOAMCASE_H */
