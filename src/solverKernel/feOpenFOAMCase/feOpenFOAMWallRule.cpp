/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "feOpenFOAMWallRule.h"
#include <interfaceHeaven/stringPrimitive.h>

#include <logMe/logMe.h>

#include <polyMesh.H>
#include <polyBoundaryMesh.H>
#include <polyPatch.H>
#include <wallPolyPatch.H>
#include <volFields.H>
#include <boost/assign.hpp>

namespace dtOO {
  bool feOpenFOAMWallRule::_registrated 
  =
  feOpenFOAMSetupRule::registrate(
    dt__tmpPtr(feOpenFOAMWallRule, new feOpenFOAMWallRule())
  );
  
  feOpenFOAMWallRule::feOpenFOAMWallRule() {
  }

  feOpenFOAMWallRule::~feOpenFOAMWallRule() {
  }
  
  std::vector< std::string > feOpenFOAMWallRule::factoryAlias( void ) const {
    return ::boost::assign::list_of("OpenFOAMWallRule");
  }  
    
  void feOpenFOAMWallRule::executeOnMesh(
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
