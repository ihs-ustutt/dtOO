#include "OpenFOAMCellZoneRule.h"
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
  OpenFOAMCellZoneRule::OpenFOAMCellZoneRule() {
  }

  OpenFOAMCellZoneRule::~OpenFOAMCellZoneRule() {
  }
  
  void OpenFOAMCellZoneRule::executeOnMesh(
    std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
  ) const {
    std::string min = OpenFOAMSetupRule::parseOptionStr("min", rule[2]);
    std::string max = OpenFOAMSetupRule::parseOptionStr("max", rule[2]);
    
    if (min == "" && max == "") {
      dtFoamLibrary::boxToCellZone(
        mesh.bounds().min(), mesh.bounds().max(), rule[1], mesh
      );
    }
    else dt__throwUnexpected(executeOnMesh());
  }    
}

