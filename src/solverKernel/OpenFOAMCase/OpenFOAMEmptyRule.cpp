#include "OpenFOAMEmptyRule.h"
#include "fvMesh.H"

#include <logMe/logMe.h>
#include <interfaceHeaven/stringPrimitive.h>

#include <meshEngine/dtFoamLibrary.h>

#include <typeInfo.H>
#include <polyMesh.H>
#include <polyBoundaryMesh.H>
#include <polyPatch.H>
#include <emptyPolyPatch.H>

namespace dtOO {
  OpenFOAMEmptyRule::OpenFOAMEmptyRule() {
  }

  OpenFOAMEmptyRule::~OpenFOAMEmptyRule() {
  }
  
  void OpenFOAMEmptyRule::executeOnMesh(
    std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
  ) const {
    
    //
    // get corresponding patch ids
    //
    ::Foam::polyBoundaryMesh & bM
    = 
    const_cast<::Foam::polyBoundaryMesh&>(mesh.boundaryMesh());      
    forAll(bM, i) {
      if ( bM[ i ].size() != 0 ) continue;

      //
      // replace old patches with new ggi patches
      //
      bM.set( 
        i,       
        new ::Foam::emptyPolyPatch(
          bM[ i ].name(), 
          bM[ i ].size(),
          bM[ i ].start(), 
          bM[ i ].index(),
          bM[ i ].boundaryMesh()
        )
      );
    }
  }    
}

