#include "openFOAMSetupRule.h"

#include <logMe/logMe.h>
#include "openFOAMWallRule.h"
#include "openFOAMGgiRule.h"
#include "openFOAMEmptyRule.h"
#include "openFOAMInletRule.h"
#include "openFOAMOutletRule.h"
#include <xmlHeaven/qtXmlBase.h>
#include <interfaceHeaven/stringPrimitive.h>

#include <criticalHeaven/prepareOpenFOAM.h>
#include <polyMesh.H>
#include <polyBoundaryMesh.H>
#include <polyPatch.H>
#include <volFields.H>
#include <fixedValueFvPatchField.H>
#include <zeroGradientFvPatchField.H>

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
        if ( stringPrimitive::stringContains("uniform", thisRule) ) {
          //
          // create a new fixedValue patch
          //
          ::Foam::fixedValueFvPatchField< ::Foam::vector > * fix
          = 
          new ::Foam::fixedValueFvPatchField< ::Foam::vector >(
            field.mesh().boundary()[i], field
          );

          //
          // set new patch to boundaryField
          //
          bF.set(i, fix);
          
          //
          // set fixed value to patch
          //        
          dtVector3 uVal = parseOptionDtVector3("uniform", thisRule);
          bF[i] == ::Foam::vector(uVal.x(), uVal.y(), uVal.z());
          
          return;
        }
        else if ( stringPrimitive::stringContains("zeroGradient", thisRule) ) {
          //
          // create a new fixedValue patch
          //
          ::Foam::zeroGradientFvPatchField< ::Foam::vector > * zeroGradient
          = 
          new ::Foam::zeroGradientFvPatchField< ::Foam::vector >(
            field.mesh().boundary()[i], field
          );

          //
          // set new patch to boundaryField
          //
          bF.set(i, zeroGradient);
          
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
        if ( stringPrimitive::stringContains("uniform", thisRule) ) {
          //
          // create a new fixedValue patch
          //
          ::Foam::fixedValueFvPatchField< ::Foam::scalar > * fix
          = 
          new ::Foam::fixedValueFvPatchField< ::Foam::scalar >(
            field.mesh().boundary()[i], field
          );

          //
          // set new patch to boundaryField
          //
          bF.set(i, fix);
          
          //
          // set fixed value to patch
          //        
          float uVal = parseOptionFloat("uniform", thisRule);
          bF[i] == ::Foam::scalar(uVal);
          
          return;
        }
        else if ( stringPrimitive::stringContains("zeroGradient", thisRule) ) {
          //
          // create a new fixedValue patch
          //
          ::Foam::zeroGradientFvPatchField< ::Foam::scalar > * zeroGradient
          = 
          new ::Foam::zeroGradientFvPatchField< ::Foam::scalar >(
            field.mesh().boundary()[i], field
          );

          //
          // set new patch to boundaryField
          //
          bF.set(i, zeroGradient);
          
          return;
        }
        else dt__throwUnexpected(executeOnVolVectorField());
      }
    }
  }
    
  std::string openFOAMSetupRule::getRuleOfField(
    std::string const & fieldName, std::vector< std::string > const & rule
  ) const {  
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
  ) const {
    if ( stringPrimitive::stringContains(name, str) ) {
      return stringPrimitive::getStringBetween("(", ")", str);
    }
    return std::string("");
  }    
  
  bool openFOAMSetupRule::parseOptionBool(
    std::string const & name, std::string const & str
  ) const {
    if ( parseOptionStr(name, str) == "true") return true;
    return false;    
  }
   
  dtVector3 openFOAMSetupRule::parseOptionDtVector3(
    std::string const & name, std::string const & str
  ) const {
    std::vector< float > ff
    =
    qtXmlBase::muParseCSString( parseOptionStr(name, str) );
    
    dt__throwIf(ff.size()!=3, parseOptionDtVector3());
    
    return dtVector3(ff[0], ff[1], ff[2]);
  }

  float openFOAMSetupRule::parseOptionFloat(
    std::string const & name, std::string const & str
  ) const {
    float ff = qtXmlBase::muParseString( parseOptionStr(name, str) );
    
    return ff;
  }  
}
