#ifndef OPENFOAMSETUPRULE_H
#define	OPENFOAMSETUPRULE_H

#include <logMe/dtMacros.h>
#include <DynamicList.H>

namespace Foam {
  class polyMesh;
  class polyPatch;
  class polyBoundaryMesh;
};

namespace dtOO {
  class openFOAMSetupRule {
  public:
    dt__classOnlyName(openFOAMSetupRule);    
    openFOAMSetupRule();
    virtual ~openFOAMSetupRule();
    static openFOAMSetupRule * create( std::string const & name );
    virtual void executeOnMesh(
      std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
    ) const = 0;
  private:

  };
}
#endif	/* OPENFOAMSETUPRULE_H */

