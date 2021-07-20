#include "OpenFOAMTurboRule.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/stringPrimitive.h>

#include <criticalHeaven/prepareOpenFOAM.h>
#include <polyMesh.H>
#include <polyBoundaryMesh.H>
#include <polyPatch.H>
#include <volFields.H>
#include <profile1DfixedValueFvPatchField.H>

namespace dtOO {
  OpenFOAMTurboRule::OpenFOAMTurboRule() {
  }

  OpenFOAMTurboRule::~OpenFOAMTurboRule() {
  }
  
//  OpenFOAMSetupRule * OpenFOAMSetupRule::create( 
//    std::string const & name 
//  ) {
//    if (name == "OpenFOAMSetupRule") return new OpenFOAMSetupRule();    
//    if (name == "OpenFOAMWallRule") return new OpenFOAMWallRule();
//    if (name == "OpenFOAMGgiRule") return new OpenFOAMGgiRule();
//    if (name == "OpenFOAMEmptyRule") return new OpenFOAMEmptyRule();
//    if (name == "OpenFOAMCyclicGgiRule") return new OpenFOAMCyclicGgiRule();
//    if (name == "OpenFOAMCellZoneRule") return new OpenFOAMCellZoneRule();
//    if (name == "OpenFOAMCylindricalInletRule") {
//      return new OpenFOAMCylindricalInletRule();
//    }
//    if (name == "OpenFOAMMixingPlaneRule" ) {
//      return new OpenFOAMMixingPlaneRule();
//    }
//    dt__throw(create(), << "Cannot create " << name);
//  }

//  void OpenFOAMSetupRule::init(
//    baseContainer const * const bC,
//    cVPtrVec const * const cV,
//    aFPtrVec const * const aF,
//    aGPtrVec const * const aG,
//    bVPtrVec const * const bV,
//    dCPtrVec const * const dC
//  ) {
//    _bC = bC;
//    _cV = cV;
//    _aF = aF;
//    _aG = aG;
//    _bV = bV;
//    _dC = dC;
//  }
    
//  void OpenFOAMSetupRule::executeOnMesh(
//    std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
//  ) const {
//    //
//    // get boundary patch and id
//    //
//    ::Foam::polyBoundaryMesh & bM
//    =
//    const_cast<::Foam::polyBoundaryMesh&>(mesh.boundaryMesh());
//    ::Foam::label id = bM.findPatchID( rule[1] );
//    dt__throwIfWithMessage(
//      id<0, 
//      executeOnMesh(),
//      << "rule = " << rule
//    );
//    
//    //
//    // replace old patch
//    //
//    bM.set(
//      id,
//      new ::Foam::polyPatch(
//        bM[ id ].name(), 
//        bM[ id ].size(),
//        bM[ id ].start(), 
//        bM[ id ].index(),
//        bM[ id ].boundaryMesh()
//      )
//    );
//  }  
  
  void OpenFOAMTurboRule::executeOnVolVectorField(
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
        if ( 
          stringPrimitive::getStringBetween("", "(", thisRule) 
          == 
          "profile1DfixedValue"           
        ) {
          //
          // create and set new patch
          //
          bF.set(
            i, 
            new ::Foam::profile1DfixedValueFvPatchField< ::Foam::vector >(
              field.mesh().boundary()[i], 
              field, 
              parseOptionDict("profile1DfixedValue", thisRule)
            )            
          );
          
          return;
        }
        else dt__throwUnexpected(executeOnVolVectorField());
      }
    }
  }
    
  void OpenFOAMTurboRule::executeOnVolScalarField(
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
        if ( 
          stringPrimitive::getStringBetween("", "(", thisRule) 
          == 
          "profile1DfixedValue"           
        ) {
          //
          // create and set new patch
          //
          bF.set(
            i, 
            new ::Foam::profile1DfixedValueFvPatchField< ::Foam::scalar >(
              field.mesh().boundary()[i], 
              field, 
              parseOptionDict("profile1DfixedValue", thisRule)
            )            
          );
          
          return;
        }            
        else dt__throwUnexpected(executeOnVolScalarField());
      }
    }
  }
}
