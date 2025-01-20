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

#include "ofOpenFOAMEmptyRule.h"
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
  bool ofOpenFOAMEmptyRule::_registrated 
  =
  ofOpenFOAMSetupRule::registrate(
    dt__tmpPtr(ofOpenFOAMEmptyRule, new ofOpenFOAMEmptyRule())
  );
  
  ofOpenFOAMEmptyRule::ofOpenFOAMEmptyRule() {
  }

  ofOpenFOAMEmptyRule::~ofOpenFOAMEmptyRule() {
  }
  
  void ofOpenFOAMEmptyRule::executeOnMesh(
    std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
  ) const {
    //
    // get corresponding patch ids
    //
    ::Foam::polyBoundaryMesh & bM
    = 
    const_cast<::Foam::polyBoundaryMesh&>(mesh.boundaryMesh());
    std::vector< std::string > patchNames
    =
    stringPrimitive::convertToCSVStringVector(rule[1]);

    forAll(bM, i) {
      bool setThisPatchEmpty = false;
      dt__forAllRefAuto(patchNames, patchName) {
        if (bM[ i ].name()==patchName) {
          setThisPatchEmpty = true;
          break;
        }
      }
      // patch is empty
      if ( setThisPatchEmpty || (bM[ i ].size() == 0) ) {
        dt__info(
          executeOnMesh(),
          << "Set " << bM[ i ].name() << " to empty."
        );
        bM.set( 
          i,       
          new ::Foam::emptyPolyPatch(
            bM[ i ].name(), 
            bM[ i ].size(),
            bM[ i ].start(), 
            bM[ i ].index(),
            bM[ i ].boundaryMesh(),
            bM[ i ].type()
          )
        );
      }
    }
  }    
}
