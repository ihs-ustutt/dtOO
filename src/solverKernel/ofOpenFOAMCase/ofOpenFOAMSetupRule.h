/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#ifndef ofOpenFOAMSetupRule_H
#define	ofOpenFOAMSetupRule_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/vectorHandling.h>
#include <volFieldsFwd.H>
#include <mainConceptFwd.h>

namespace Foam {
  class polyMesh;
  class polyPatch;
  class polyBoundaryMesh;
  class fvMesh;
};

namespace dtOO {
  class baseContainer;
  
  class ofOpenFOAMSetupRule {
    public:
      dt__classOnlyName(ofOpenFOAMSetupRule);
      dt__classSelfCreate(ofOpenFOAMSetupRule);
      ofOpenFOAMSetupRule();
      virtual ~ofOpenFOAMSetupRule();
      static bool registrate( ofOpenFOAMSetupRule const * const );
      virtual std::vector< std::string > factoryAlias( void ) const;
      static ofOpenFOAMSetupRule * create( std::string const & str );
      virtual void init(       
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        lvH_dtCase const * const dC
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
      static std::vector< std::string > optionVector(std::string const & str);
      static std::vector< std::string > availableOptions(
        std::string const & str
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
      static dtReal parseOptionFloat(
        std::string const & name, std::string const & str
      );    
      static ::Foam::dictionary parseOptionDict(
        std::string const & name, std::string const & str
      );    
    protected:
      lvH_analyticFunction const & refAF( void ) const;
    private:
      baseContainer const * _bC;
      lvH_constValue const * _cV;
      lvH_analyticFunction const * _aF;
      lvH_analyticGeometry const * _aG;
      lvH_boundedVolume const * _bV;
      lvH_dtCase const * _dC;
    private:
      static dt__pVH(ofOpenFOAMSetupRule) _rules;
      static bool _registrated;           
  };
}
#endif	/* ofOpenFOAMSetupRule_H */
