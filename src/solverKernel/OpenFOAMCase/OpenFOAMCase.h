#ifndef CREATEOPENFOAMCASE_H
#define	CREATEOPENFOAMCASE_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

#include "dtCase.h"

class MVertex;
class MElement;

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  class baseContainer;
  class dtGmshModel;
  class dtXmlParser;
  class OpenFOAMSetupRule;
      
  class OpenFOAMCase : public dtCase {
  public:
    dt__class(OpenFOAMCase, dtCase);    
    OpenFOAMCase();
    virtual ~OpenFOAMCase();
    virtual void init( 
      ::QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      vectorHandling< dtCase * > const * const dC
    );    
    virtual void runCurrentState(void);
  private:
    void initMeshVectors( 
      std::vector< ::MVertex * > & allVerts, 
      std::vector< std::pair< ::MElement *, int > > & allElems,
      std::map< int, std::string > & physicalNames    
    );
  private:
    vectorHandling< constValue * > const * _cV;
    std::string _dictRule;
    std::map< std::vector< std::string >, dt__pH(OpenFOAMSetupRule) > _setupRule;
    std::vector< std::vector< std::string > > _fieldRule;
    std::vector< boundedVolume * > _bV;
    std::string _runCommand;    
    std::vector< std::string > _noWriteRule;
  };
}
#endif	/* CREATEOPENFOAMCASE_H */
