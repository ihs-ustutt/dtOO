#ifndef feOpenFOAMCase_H
#define	feOpenFOAMCase_H

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
  class feOpenFOAMSetupRule;
      
  class feOpenFOAMCase : public dtCase {
    public:
      dt__class(feOpenFOAMCase, dtCase);
      dt__classSelfCreate(feOpenFOAMCase);
      feOpenFOAMCase();
      virtual ~feOpenFOAMCase();
      virtual std::vector< std::string > factoryAlias( void ) const;
      virtual void init( 
        ::QDomElement const & element,
        baseContainer const * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG,
        bVPtrVec const * const bV,
        dCPtrVec const * const dC
      );    
      virtual void runCurrentState(void);
    private:
      void initMeshVectors( 
        std::vector< ::MVertex * > & allVerts, 
        std::vector< std::pair< ::MElement *, int > > & allElems,
        std::map< int, std::string > & physicalNames    
      );
    private:
      cVPtrVec const * _cV;
      std::string _dictRule;
      std::map< std::vector< std::string >, dt__pH(feOpenFOAMSetupRule) > _setupRule;
      std::vector< std::vector< std::string > > _fieldRule;
      std::vector< boundedVolume * > _bV;
      std::string _runCommand;    
      std::vector< std::string > _noWriteRule;
      static bool _registrated;
  };
}
#endif	/* feOpenFOAMCase_H */
