#include "openFOAMWallRule.h"
#include "interfaceHeaven/stringPrimitive.h"

#include <logMe/logMe.h>

#include <polyMesh.H>
#include <polyBoundaryMesh.H>
#include <polyPatch.H>
#include <wallPolyPatch.H>
#include <volFields.H>

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
}
