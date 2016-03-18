#ifndef OPENFOAMSETUPRULE_H
#define	OPENFOAMSETUPRULE_H

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/vectorHandling.h>
#include <volFieldsFwd.H>

namespace Foam {
  class polyMesh;
  class polyPatch;
  class polyBoundaryMesh;
  class fvMesh;
};

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  class dtCase;
  
  class OpenFOAMSetupRule {
  public:
    dt__classOnlyName(OpenFOAMSetupRule);    
    OpenFOAMSetupRule();
    virtual ~OpenFOAMSetupRule();
    static OpenFOAMSetupRule * create( std::string const & name );
    virtual void init(       
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      vectorHandling< dtCase * > const * const dC
    );
    virtual void executeOnMesh(
      std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
    ) const;
    virtual void executeOnVolVectorField(
      std::vector< std::string > const & rule, ::Foam::volVectorField & field
    ) const;    
    virtual void executeOnVolScalarField(
      std::vector< std::string > const & rule, ::Foam::volScalarField & field
    ) const;        
  public:
    static std::string getRuleOfField( 
      std::string const & fieldName, std::vector< std::string > const & rule
    );
    static std::string parseOptionStr(
      std::string const & name, std::string const & str
    );    
    static bool parseOptionBool(
      std::string const & name, std::string const & str
    );
    static ::Foam::vector parseOptionVector(    
      std::string const & name, std::string const & str
    );    
    static ::Foam::scalar parseOptionScalar(
      std::string const & name, std::string const & str
    );
    static float parseOptionFloat(
      std::string const & name, std::string const & str
    );    
    static ::Foam::dictionary parseOptionDict(
      std::string const & name, std::string const & str
    );    
  protected:
    vectorHandling< analyticFunction * > const & refAF( void ) const;
  private:
    baseContainer const * _bC;
    vectorHandling< constValue * > const * _cV;
    vectorHandling< analyticFunction * > const * _aF;
    vectorHandling< analyticGeometry * > const * _aG;
    vectorHandling< boundedVolume * > const * _bV;
    vectorHandling< dtCase * > const * _dC;
  };
}
#endif	/* OPENFOAMSETUPRULE_H */

