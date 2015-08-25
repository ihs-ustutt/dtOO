#include "openFOAMWallRule.h"
#include "interfaceHeaven/stringPrimitive.h"

#include <logMe/logMe.h>

#include <polyMesh.H>
#include <polyBoundaryMesh.H>
#include <polyPatch.H>
#include <wallPolyPatch.H>
#include <volFields.H>
#include <fixedValueFvPatchField.H>
//#include <UniformDimensionedField.H>

namespace dtOO {
  openFOAMWallRule::openFOAMWallRule() {
  }

  openFOAMWallRule::~openFOAMWallRule() {
  }
  
  void openFOAMWallRule::executeOnMesh(
    std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
  ) const {
    //
    // get boundary patch and id
    //
    ::Foam::polyBoundaryMesh & bM
    =
    const_cast<::Foam::polyBoundaryMesh&>(mesh.boundaryMesh());
    ::Foam::label id = bM.findPatchID( rule[1] );
    dt__throwIf(id<0, executeOnMesh());
    
    //
    // replace old patch
    //
    bM.set(
      id,
      new ::Foam::wallPolyPatch(
        bM[ id ].name(), 
        bM[ id ].size(),
        bM[ id ].start(), 
        bM[ id ].index(),
        bM[ id ].boundaryMesh()
      )
    );
  }    

  void openFOAMWallRule::executeOnVolVectorField(
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
      }
    }
  }  
}

