#include "ofOpenFOAMMappedFieldFixedValueRule.h"
#include <interfaceHeaven/stringPrimitive.h>

#include <logMe/logMe.h>

#include <polyMesh.H>
#include <polyBoundaryMesh.H>
#include <polyPatch.H>
#include <lnInclude/mappedFieldFixedValueFvPatchField.H>
#include <volFields.H>
#include <boost/assign.hpp>

namespace dtOO {
  bool ofOpenFOAMMappedFieldFixedValueRule::_registrated 
  =
  ofOpenFOAMSetupRule::registrate(
    dt__tmpPtr(ofOpenFOAMMappedFieldFixedValueRule, new ofOpenFOAMMappedFieldFixedValueRule())
  );
  
  ofOpenFOAMMappedFieldFixedValueRule::ofOpenFOAMMappedFieldFixedValueRule() {
  }

  ofOpenFOAMMappedFieldFixedValueRule::~ofOpenFOAMMappedFieldFixedValueRule() {
  }
  
  std::vector< std::string > ofOpenFOAMMappedFieldFixedValueRule::factoryAlias( void ) const {
    return ::boost::assign::list_of("OpenFOAMMappedFieldFixedValueRule");
  }  
    
  void ofOpenFOAMMappedFieldFixedValueRule::executeOnVolVectorField(
    std::vector< std::string > const & rule, ::Foam::volVectorField & field
  ) const {
    dt__debug(executeOnVolVectorField(), << "Execute: " << rule);
    std::string thisRule = getRuleOfField(field.name(), rule);
    if (thisRule.empty()) return;
        
    ::Foam::volVectorField::Boundary & bF 
    = 
    const_cast<::Foam::volVectorField::Boundary&>(
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
        if ( 
          stringPrimitive::stringContains(
            "mappedFieldFixedValue",
            stringPrimitive::getStringBetween("", "(", thisRule) 
          )
        ) {
          //
          // create and set new patch
          //
          bF.set(
            i, 
            new ::Foam::mappedFieldFixedValueFvPatchField< ::Foam::vector >(
              field.mesh().boundary()[i], 
              field, 
              parseOptionDict("mappedFieldFixedValue", thisRule)
            )
          );
          
          return;
        }
        else ofOpenFOAMSetupRule::executeOnVolVectorField(rule, field);
        
      }
    }
  }
  
  void ofOpenFOAMMappedFieldFixedValueRule::executeOnVolScalarField(
    std::vector< std::string > const & rule, ::Foam::volScalarField & field
  ) const {
    std::string thisRule = getRuleOfField(field.name(), rule);
    if (thisRule.empty()) return;
        
    ::Foam::volScalarField::Boundary & bF 
    = 
    const_cast<::Foam::volScalarField::Boundary&>(
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
        if ( 
          stringPrimitive::stringContains(
            "mappedFieldFixedValue",
            stringPrimitive::getStringBetween("", "(", thisRule)   
          )                
        ) {
          //
          // create and set new patch
          //
          bF.set(
            i, 
            new ::Foam::mappedFieldFixedValueFvPatchField< ::Foam::scalar >(
              field.mesh().boundary()[i], 
              field, 
              parseOptionDict("mappedFieldFixedValue", thisRule)
            )
          );
          
          return;
        }
        else ofOpenFOAMSetupRule::executeOnVolScalarField(rule, field);
      }
    }
  }
  
}

