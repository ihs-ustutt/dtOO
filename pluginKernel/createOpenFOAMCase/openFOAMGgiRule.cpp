#include "openFOAMGgiRule.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/stringPrimitive.h>

#include <meshEngine/dtFoamLibrary.h>

#include <polyMesh.H>
#include <polyBoundaryMesh.H>
#include <polyPatch.H>
#include <ggiPolyPatch.H>

namespace dtOO {
  openFOAMGgiRule::openFOAMGgiRule() {
  }

  openFOAMGgiRule::~openFOAMGgiRule() {
  }
  
  void openFOAMGgiRule::executeOnMesh(
    std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
  ) const {
    //
    // get ggi patch labels
    //
    std::vector< std::string > csvString
    =
    stringPrimitive::convertToCSVStringVector(rule[1]);
    dt__throwIf(csvString.size()!=2, executeOnPolyPatch());
    
    //
    // get corresponding patch ids
    //
    ::Foam::polyBoundaryMesh & bM
    = 
    const_cast<::Foam::polyBoundaryMesh&>(mesh.boundaryMesh());      
    ::Foam::label id0 = bM.findPatchID(csvString[0]);
    ::Foam::label id1 = bM.findPatchID(csvString[1]);
    dt__throwIf(id0<0, executeOnMesh());
    dt__throwIf(id1<0, executeOnMesh());
    
    //
    // create faceZones
    //
    dtFoamLibrary::patchToFaceZoneNoFlipMap(
      bM[ id0 ], csvString[0]+"_zone", mesh
    );
    dtFoamLibrary::patchToFaceZoneNoFlipMap(
      bM[ id1 ], csvString[1]+"_zone", mesh
    );    
    
    bool bridgeOverlap = false;
    if ( stringPrimitive::stringContains("bridgeOverlap", rule[2]) ) {
      if ( stringPrimitive::getStringBetween("(", ")", rule[2]) == "true") {
        bridgeOverlap = true;
      }
    }
    //
    // create new ggi patches
    //
    ::Foam::ggiPolyPatch * ggi0    
    =
    new ::Foam::ggiPolyPatch(
      bM[ id0 ].name(), 
      bM[ id0 ].size(),
      bM[ id0 ].start(), 
      bM[ id0 ].index(),
      bM[ id0 ].boundaryMesh(),
      csvString[1],
      csvString[0]+"_zone",
      bridgeOverlap
    );
    ::Foam::ggiPolyPatch * ggi1    
    =
    new ::Foam::ggiPolyPatch(
      bM[ id1 ].name(), 
      bM[ id1 ].size(),
      bM[ id1 ].start(), 
      bM[ id1 ].index(),
      bM[ id1 ].boundaryMesh(),
      csvString[0],
      csvString[1]+"_zone",
      bridgeOverlap
    );

    //
    // replace old patches with new ggi patches
    //
    bM.set( id0, ggi0);
    bM.set( id1, ggi1);
  }    
}

