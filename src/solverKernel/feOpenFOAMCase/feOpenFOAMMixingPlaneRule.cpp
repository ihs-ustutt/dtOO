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

#include "feOpenFOAMMixingPlaneRule.h"
#include "fvMesh.H"

#include <logMe/logMe.h>
#include <interfaceHeaven/stringPrimitive.h>

#include <meshEngine/dtFoamLibrary.h>

#include <typeInfo.H>
#include <polyMesh.H>
#include <polyBoundaryMesh.H>
#include <polyPatch.H>
#include <mixingPlaneFvPatchField.H>
#include <mixingPlaneFvPatch.H>
#include <mixingPlanePolyPatch.H>
#include <cylindricalCS.H>
#include <volFields.H>
#include <boost/assign.hpp>

namespace dtOO {
  bool feOpenFOAMMixingPlaneRule::_registrated 
  =
  feOpenFOAMSetupRule::registrate(
    dt__tmpPtr(feOpenFOAMMixingPlaneRule, new feOpenFOAMMixingPlaneRule())
  );
  
  feOpenFOAMMixingPlaneRule::feOpenFOAMMixingPlaneRule() {
  }

  feOpenFOAMMixingPlaneRule::~feOpenFOAMMixingPlaneRule() {
  }
  
  std::vector< std::string > feOpenFOAMMixingPlaneRule::factoryAlias( 
    void 
  ) const {
    return ::boost::assign::list_of("OpenFOAMMixingPlaneRule");
  }  
  
  void feOpenFOAMMixingPlaneRule::executeOnMesh(
    std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
  ) const {
    //
    // get patch labels
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
    dt__throwIfWithMessage( id0<0, executeOnMesh(), << csvString[0] );
    dt__throwIfWithMessage( id1<0, executeOnMesh(), << csvString[1] );
    
    //
    // create faceZones
    //
    dtFoamLibrary::patchToFaceZoneNoFlipMap(
      bM[ id0 ], csvString[0]+"_zone", mesh
    );
    dtFoamLibrary::patchToFaceZoneNoFlipMap(
      bM[ id1 ], csvString[1]+"_zone", mesh
    );    
    
    //
    // create new mixingPlane patches
    //
    ::Foam::mixingPlanePolyPatch * mix0    
    =
    new ::Foam::mixingPlanePolyPatch(
      bM[ id0 ].name(), 
      bM[ id0 ].size(),
      bM[ id0 ].start(), 
      bM[ id0 ].index(),
      bM[ id0 ].boundaryMesh(),
      csvString[1],
      csvString[0]+"_zone",
      ::Foam::cylindricalCS( 
        "mixingCS", parseOptionDict("coordinateSystem", rule[2]) 
      ),
      ::Foam::mixingPlaneInterpolation::discretisationNames_[
        parseOptionStr("discretisation", rule[2])
      ],
      ::Foam::mixingPlaneInterpolation::sweepAxisNames_[
        parseOptionStr("sweepAxis", rule[2])
      ],
      ::Foam::mixingPlaneInterpolation::stackAxisNames_[
        parseOptionStr("stackAxis", rule[2])
      ],
      ::Foam::fileName::null
    );
    ::Foam::mixingPlanePolyPatch * mix1    
    =
    new ::Foam::mixingPlanePolyPatch(
      bM[ id1 ].name(), 
      bM[ id1 ].size(),
      bM[ id1 ].start(), 
      bM[ id1 ].index(),
      bM[ id1 ].boundaryMesh(),
      csvString[0],
      csvString[1]+"_zone",
      ::Foam::cylindricalCS( 
        "mixingCS", parseOptionDict("coordinateSystem", rule[2]) 
      ),
      ::Foam::mixingPlaneInterpolation::discretisationNames_[
        parseOptionStr("discretisation", rule[2])
      ],
      ::Foam::mixingPlaneInterpolation::sweepAxisNames_[
        parseOptionStr("sweepAxis", rule[2])
      ],
      ::Foam::mixingPlaneInterpolation::stackAxisNames_[
        parseOptionStr("stackAxis", rule[2])
      ],
      ::Foam::fileName::null
    );

    //
    // replace old patches with new patches
    //
    bM.set( id0, mix0);
    bM.set( id1, mix1);
  }
  
  void feOpenFOAMMixingPlaneRule::executeOnVolVectorField(
    std::vector< std::string > const & rule, ::Foam::volVectorField & field
  ) const {
    feOpenFOAMSetupRule::executeOnVolVectorField(rule, field);
    //
    // get ggi patch labels
    //
    std::vector< std::string > csvString
    =
    stringPrimitive::convertToCSVStringVector(rule[1]);
    dt__throwIf(csvString.size()!=2, executeOnVolVectorField());
    
    ::Foam::volVectorField::GeometricBoundaryField & bF 
    = 
    const_cast<::Foam::volVectorField::GeometricBoundaryField&>(
      field.boundaryField()
    );
    
    forAll(bF, i) {
      //
      // get patch according to rule
      //
      if ( 
        (field.mesh().boundary()[i].name() ==  csvString[0])
        ||
        (field.mesh().boundary()[i].name() ==  csvString[1])
      ) {
        bF[i] = field.internalField();
      } 
    }
  }
  
  void feOpenFOAMMixingPlaneRule::executeOnVolScalarField(
    std::vector< std::string > const & rule, ::Foam::volScalarField & field
  ) const {
    feOpenFOAMSetupRule::executeOnVolScalarField(rule, field);
    //
    // get ggi patch labels
    //
    std::vector< std::string > csvString
    =
    stringPrimitive::convertToCSVStringVector(rule[1]);
    dt__throwIf(csvString.size()!=2, executeOnVolScalarField());
    
    ::Foam::volScalarField::GeometricBoundaryField & bF 
    = 
    const_cast<::Foam::volScalarField::GeometricBoundaryField&>(
      field.boundaryField()
    );
    
    forAll(bF, i) {
      //
      // get patch according to rule
      //
      if ( 
        (field.mesh().boundary()[i].name() ==  csvString[0])
        ||
        (field.mesh().boundary()[i].name() ==  csvString[1])
      ) {
        bF[i] = field.internalField();
      } 
    }
  }  
}
