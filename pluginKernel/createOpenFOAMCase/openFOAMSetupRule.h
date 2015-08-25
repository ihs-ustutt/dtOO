#ifndef OPENFOAMSETUPRULE_H
#define	OPENFOAMSETUPRULE_H

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <volFieldsFwd.H>

namespace Foam {
  class polyMesh;
  class polyPatch;
  class polyBoundaryMesh;
  class fvMesh;
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
    virtual void executeOnVolVectorField(
      std::vector< std::string > const & rule, ::Foam::volVectorField & field
    ) const;    
    virtual void executeOnVolScalarField(
      std::vector< std::string > const & rule, ::Foam::volScalarField & field
    ) const;        
  protected:
    std::string getRuleOfField( 
      std::string const & fieldName, std::vector< std::string > const & rule
    ) const;
    std::string parseOptionStr(
      std::string const & name, std::string const & str
    ) const;    
    bool parseOptionBool(
      std::string const & name, std::string const & str
    ) const;
    dtVector3 parseOptionDtVector3(
      std::string const & name, std::string const & str
    ) const;
    float parseOptionFloat(
      std::string const & name, std::string const & str
    ) const;    
  private:

  };
}
#endif	/* OPENFOAMSETUPRULE_H */

