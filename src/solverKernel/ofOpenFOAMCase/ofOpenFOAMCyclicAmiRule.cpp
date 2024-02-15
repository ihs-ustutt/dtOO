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

#include "ofOpenFOAMCyclicAmiRule.h"
#include "fvMesh.H"

#include <logMe/logMe.h>
#include <interfaceHeaven/stringPrimitive.h>

#include <meshEngine/dtFoamLibrary.h>

#include <typeInfo.H>
#include <polyMesh.H>
#include <polyBoundaryMesh.H>
#include <polyPatch.H>
#include <cyclicAMIPolyPatch.H>
#include <volFields.H>
#include <boost/assign.hpp>

namespace dtOO {
  bool ofOpenFOAMCyclicAmiRule::_registrated 
  =
  ofOpenFOAMSetupRule::registrate(
    dt__tmpPtr(ofOpenFOAMCyclicAmiRule, new ofOpenFOAMCyclicAmiRule())
  );
  
  ofOpenFOAMCyclicAmiRule::ofOpenFOAMCyclicAmiRule() {
  }

  ofOpenFOAMCyclicAmiRule::~ofOpenFOAMCyclicAmiRule() {
  }
  
  void ofOpenFOAMCyclicAmiRule::executeOnMesh(
    std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
  ) const {
    //
    // get ami patch labels
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
    // create new ami patches
    //
    ::Foam::dictionary dd0 = parseOptionDict(csvString[0], rule[2]);   
    dd0.add("nFaces", bM[ id0 ].size());
    dd0.add("startFace", bM[ id0 ].start());
    dd0.add("neighbourPatch", ::Foam::word(csvString[1]));
    
    ::Foam::cyclicAMIPolyPatch * ami0    
    =
    new ::Foam::cyclicAMIPolyPatch(        
      bM[ id0 ].name(), 
      dd0,
      bM[ id0 ].index(),
      bM[ id0 ].boundaryMesh(),
      "cyclicAMI"
    );

    ::Foam::dictionary dd1 = parseOptionDict(csvString[1], rule[2]);   
    dd1.add("nFaces", bM[ id1 ].size());
    dd1.add("startFace", bM[ id1 ].start());
    dd1.add("neighbourPatch", ::Foam::word(csvString[0]));    
    ::Foam::cyclicAMIPolyPatch * ami1
    =
    new ::Foam::cyclicAMIPolyPatch(        
        bM[ id1 ].name(), 
        dd1,
        bM[ id1 ].index(),
        bM[ id1 ].boundaryMesh(),
        "cyclicAMI"
    );
    //
    // replace old patches with new ami patches
    //
    bM.set( id0, ami0);
    bM.set( id1, ami1);
  }
  
  void ofOpenFOAMCyclicAmiRule::executeOnVolVectorField(
    std::vector< std::string > const & rule, ::Foam::volVectorField & field
  ) const {
    ofOpenFOAMSetupRule::executeOnVolVectorField(rule, field);
    //
    // get ami patch labels
    //
    std::vector< std::string > csvString
    =
    stringPrimitive::convertToCSVStringVector(rule[1]);
    dt__throwIf(csvString.size()!=2, executeOnVolVectorField());
    
    ::Foam::volVectorField::Boundary & bF 
    = 
    const_cast<::Foam::volVectorField::Boundary&>(
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
  
  void ofOpenFOAMCyclicAmiRule::executeOnVolScalarField(
    std::vector< std::string > const & rule, ::Foam::volScalarField & field
  ) const {
    ofOpenFOAMSetupRule::executeOnVolScalarField(rule, field);
    //
    // get ami patch labels
    //
    std::vector< std::string > csvString
    =
    stringPrimitive::convertToCSVStringVector(rule[1]);
    dt__throwIf(csvString.size()!=2, executeOnVolScalarField());
    
    ::Foam::volScalarField::Boundary & bF 
    = 
    const_cast<::Foam::volScalarField::Boundary&>(
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
