#include "feOpenFOAMCellZoneRule.h"
#include "fvMesh.H"

#include <logMe/logMe.h>
#include <interfaceHeaven/stringPrimitive.h>

#include <meshEngine/dtFoamLibrary.h>

#include <typeInfo.H>
#include <polyMesh.H>
#include <polyBoundaryMesh.H>
#include <polyPatch.H>
#include <emptyPolyPatch.H>
#include <boost/assign.hpp>

namespace dtOO {
  bool feOpenFOAMCellZoneRule::_registrated 
  =
  feOpenFOAMSetupRule::registrate(
    dt__tmpPtr(feOpenFOAMCellZoneRule, new feOpenFOAMCellZoneRule())
  );
  
  feOpenFOAMCellZoneRule::feOpenFOAMCellZoneRule() {
  }

  feOpenFOAMCellZoneRule::~feOpenFOAMCellZoneRule() {
  }
  
  std::vector< std::string > feOpenFOAMCellZoneRule::factoryAlias( void ) const {
    return ::boost::assign::list_of("OpenFOAMCellZoneRule");
  }  
  
  void feOpenFOAMCellZoneRule::executeOnMesh(
    std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
  ) const {
    std::string min = feOpenFOAMSetupRule::parseOptionStr("min", rule[2]);
    std::string max = feOpenFOAMSetupRule::parseOptionStr("max", rule[2]);
    
    if (min == "" && max == "") {
      dtFoamLibrary::boxToCellZone(
        mesh.bounds().min(), mesh.bounds().max(), rule[1], mesh
      );
    }
    else dt__throwUnexpected(executeOnMesh());
  }    
}

