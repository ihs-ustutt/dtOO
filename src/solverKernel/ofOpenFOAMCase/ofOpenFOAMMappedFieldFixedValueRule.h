#ifndef ofOpenFOAMMappedFieldFixedValueRule_H
#define	ofOpenFOAMMappedFieldFixedValueRule_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "ofOpenFOAMSetupRule.h"

namespace dtOO {
  class ofOpenFOAMMappedFieldFixedValueRule : public ofOpenFOAMSetupRule {
    public:
      dt__class(ofOpenFOAMMappedFieldFixedValueRule, ofOpenFOAMSetupRule);  
      dt__classSelfCreate(ofOpenFOAMMappedFieldFixedValueRule);
      ofOpenFOAMMappedFieldFixedValueRule();
      virtual ~ofOpenFOAMMappedFieldFixedValueRule();
      virtual std::vector< std::string > factoryAlias( void ) const;
      virtual void executeOnVolVectorField(
        std::vector< std::string > const & rule, ::Foam::volVectorField & field
      ) const;    
      virtual void executeOnVolScalarField(
        std::vector< std::string > const & rule, ::Foam::volScalarField & field
      ) const;
    private:
      static bool _registrated;
  };
}
#endif	/* ofOpenFOAMMappedFieldFixedValueRule_H */

