#include "openFOAMSetupRule.h"

#include <logMe/logMe.h>
#include "openFOAMWallRule.h"
#include "openFOAMGgiRule.h"
#include "openFOAMCyclicGgiRule.h"
#include "openFOAMEmptyRule.h"
#include "openFOAMInletRule.h"
#include "openFOAMOutletRule.h"
#include "openFOAMCellZoneRule.h"
#include <xmlHeaven/qtXmlBase.h>
#include <interfaceHeaven/stringPrimitive.h>

#include <criticalHeaven/prepareOpenFOAM.h>
#include <polyMesh.H>
#include <polyBoundaryMesh.H>
#include <polyPatch.H>
#include <volFields.H>
#include <fixedValueFvPatchField.H>
#include <zeroGradientFvPatchField.H>
#include <epsilonWallFunctionFvPatchScalarField.H>
#include <kqRWallFunctionFvPatchField.H>
#include <nutWallFunctionFvPatchScalarField.H>
#include <dictionary.H>

namespace dtOO {
  openFOAMSetupRule::openFOAMSetupRule() {
  }

  openFOAMSetupRule::~openFOAMSetupRule() {
  }
  
  openFOAMSetupRule * openFOAMSetupRule::create( 
    std::string const & name 
  ) {
    if (name == "openFOAMWallRule") return new openFOAMWallRule();
    if (name == "openFOAMGgiRule") return new openFOAMGgiRule();
    if (name == "openFOAMEmptyRule") return new openFOAMEmptyRule();
    if (name == "openFOAMInletRule") return new openFOAMInletRule();
    if (name == "openFOAMOutletRule") return new openFOAMOutletRule();
    if (name == "openFOAMCyclicGgiRule") return new openFOAMCyclicGgiRule();
    if (name == "openFOAMCellZoneRule") return new openFOAMCellZoneRule();
    
    dt__throw(create(), << "Cannot create " << name);
  }
  
  void openFOAMSetupRule::executeOnVolVectorField(
    std::vector< std::string > const & rule, ::Foam::volVectorField & field
  ) const {
    std::string thisRule = getRuleOfField(field.name(), rule);
    if (thisRule.empty()) return;
        
    ::Foam::volVectorField::GeometricBoundaryField & bF 
    = 
    const_cast<::Foam::volVectorField::GeometricBoundaryField&>(
      field.boundaryField()
    );

    forAll(bF, i) {
      //
      // get patch according to rule
      //
      if ( field.mesh().boundary()[i].name() ==  rule[1] ) {
        dt__info(
          executeOnVolVectorField(),
          << "Executing" << std::endl
          << dt__eval(thisRule) << std::endl
          << "on" << std::endl
          << "field.mesh().boundary()[" << i << "].name() = " 
          << field.mesh().boundary()[i].name() << std::endl
          << "of" << std::endl
          << dt__eval(field.name())
        );        
        if ( stringPrimitive::stringContains("fixedValue", thisRule) ) {
          //
          // create and set new patch
          //
          bF.set(
            i, 
            new ::Foam::fixedValueFvPatchField< ::Foam::vector >(
              field.mesh().boundary()[i], 
              field, 
              parseOptionDict("fixedValue", thisRule)
            )
          );
          
          return;
        }
        else if ( stringPrimitive::stringContains("zeroGradient", thisRule) ) {
          //
          // create and set new patch
          //
          bF.set(
            i, 
            new ::Foam::zeroGradientFvPatchField< ::Foam::vector >(
              field.mesh().boundary()[i], 
              field, 
              parseOptionDict("zeroGradient", thisRule)
            )            
          );
          
          return;
        }
        else dt__throwUnexpected(executeOnVolVectorField());
      }
    }
  }
    
  void openFOAMSetupRule::executeOnVolScalarField(
    std::vector< std::string > const & rule, ::Foam::volScalarField & field
  ) const {
    std::string thisRule = getRuleOfField(field.name(), rule);
    if (thisRule.empty()) return;
        
    ::Foam::volScalarField::GeometricBoundaryField & bF 
    = 
    const_cast<::Foam::volScalarField::GeometricBoundaryField&>(
      field.boundaryField()
    );

    forAll(bF, i) {
      //
      // get patch according to rule
      //
      if ( field.mesh().boundary()[i].name() ==  rule[1] ) {
        dt__info(
          executeOnVolScalarField(),
          << "Executing" << std::endl
          << dt__eval(thisRule) << std::endl
          << "on" << std::endl
          << "field.mesh().boundary()[" << i << "].name() = " 
          << field.mesh().boundary()[i].name() << std::endl
          << "of" << std::endl
          << dt__eval(field.name())
        );
        if ( stringPrimitive::stringContains("fixedValue", thisRule) ) {
          //
          // create and set new patch
          //
          bF.set(
            i, 
            new ::Foam::fixedValueFvPatchField< ::Foam::scalar >(
              field.mesh().boundary()[i], 
              field, 
              parseOptionDict("fixedValue", thisRule)
            )
          );
          
          return;
        }
        else if ( stringPrimitive::stringContains("zeroGradient", thisRule) ) {
          //
          // create and set new patch
          //
          bF.set(
            i, 
            new ::Foam::zeroGradientFvPatchField< ::Foam::scalar >(
              field.mesh().boundary()[i], 
              field, 
              parseOptionDict("zeroGradient", thisRule)
            )
          );
          
          return;
        }
        else if( stringPrimitive::stringContains("kqRWallFunction", thisRule) ) {
          //
          // create and set new patch
          //
          bF.set(
            i, 
            new ::Foam::incompressible::RASModels
              ::kqRWallFunctionFvPatchField< ::Foam::scalar >(
                field.mesh().boundary()[i], 
                field, 
                parseOptionDict("kqRWallFunction", thisRule)
            )
          );
          
          return;          
        }
        else if( stringPrimitive::stringContains("epsilonWallFunction", thisRule) ) {
          //
          // create and set new patch
          //
          bF.set(
            i, 
            new ::Foam::incompressible::RASModels
              ::epsilonWallFunctionFvPatchScalarField(
                field.mesh().boundary()[i], 
                field, 
                parseOptionDict("epsilonWallFunction", thisRule)
            ) 
          );
          
          return;          
        }      
        else if( stringPrimitive::stringContains("nutWallFunction", thisRule) ) {
          //
          // create and set new patch
          //
          bF.set(
            i, 
            new ::Foam::incompressible::RASModels
              ::nutWallFunctionFvPatchScalarField(
                field.mesh().boundary()[i], 
                field, 
                parseOptionDict("nutWallFunction", thisRule)
            ) 
          );
          
          return;          
        }                
        else dt__throwUnexpected(executeOnVolScalarField());
      }
    }
  }
    
  std::string openFOAMSetupRule::getRuleOfField(
    std::string const & fieldName, std::vector< std::string > const & rule
  ) {  
    //
    // start at position 3
    //
    for(int ii = 3; ii < rule.size(); ii=ii+2) {
      //
      // if rule[ii] is equal field name return
      //
      if ( rule[ii] == fieldName ) {
        dt__throwIf( (ii+1)>(rule.size()-1), getRuleOfField());
        return rule[ii+1];
      }
    }
    return std::string("");    
  }
  
  std::string openFOAMSetupRule::parseOptionStr(
    std::string const & name, std::string const & str
  ) {
    if ( stringPrimitive::stringContains(name, str) ) {
      return stringPrimitive::getStringBetween(
        "(", ")", str.substr(str.find(name))
      );
    
    }
    dt__warning(
      parseOptionStr(),
      << "Cannot find option. Return empty string." << std::endl
      << dt__eval(name) << std::endl
      << dt__eval(str)
    );
    return std::string("");
  }    
  
  bool openFOAMSetupRule::parseOptionBool(
    std::string const & name, std::string const & str
  ) {
    if ( parseOptionStr(name, str) == "true") return true;
    return false;    
  }
   
  ::Foam::vector openFOAMSetupRule::parseOptionVector(
    std::string const & name, std::string const & str
  ) {
    std::vector< float > ff
    =
    qtXmlBase::muParseCSString( parseOptionStr(name, str) );
    
    dt__throwIf(ff.size()!=3, parseOptionVector());
    
    return ::Foam::vector(ff[0], ff[1], ff[2]);
  }

  ::Foam::scalar openFOAMSetupRule::parseOptionScalar(
    std::string const & name, std::string const & str
  ) {
    return ::Foam::scalar(
      qtXmlBase::muParseString( parseOptionStr(name, str) )
    );
  }  

  ::Foam::dictionary openFOAMSetupRule::parseOptionDict(
    std::string const & name, std::string const & str
  ) {
    ::Foam::IStringStream is(
      "{"
      +
      parseOptionStr(name, str)
      +
      "}"
    );
    
    return ::Foam::dictionary(is());
  }    
}