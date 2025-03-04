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

#include "feOpenFOAMCyclicGgiRule.h"
#include "fvMesh.H"

#include <interfaceHeaven/stringPrimitive.h>
#include <logMe/logMe.h>

#include <meshEngine/dtFoamLibrary.h>

#include <boost/assign.hpp>
#include <cyclicGgiPolyPatch.H>
#include <polyBoundaryMesh.H>
#include <polyMesh.H>
#include <polyPatch.H>
#include <typeInfo.H>
#include <volFields.H>

namespace dtOO {
bool feOpenFOAMCyclicGgiRule::_registrated = feOpenFOAMSetupRule::registrate(
  dt__tmpPtr(feOpenFOAMCyclicGgiRule, new feOpenFOAMCyclicGgiRule())
);

feOpenFOAMCyclicGgiRule::feOpenFOAMCyclicGgiRule() {}

feOpenFOAMCyclicGgiRule::~feOpenFOAMCyclicGgiRule() {}

std::vector<std::string> feOpenFOAMCyclicGgiRule::factoryAlias(void) const
{
  return ::boost::assign::list_of("OpenFOAMCyclicGgiRule");
}

void feOpenFOAMCyclicGgiRule::executeOnMesh(
  std::vector<std::string> const &rule, ::Foam::polyMesh &mesh
) const
{
  //
  // get ggi patch labels
  //
  std::vector<std::string> csvString =
    stringPrimitive::convertToCSVStringVector(rule[1]);
  dt__throwIf(csvString.size() != 2, executeOnPolyPatch());

  //
  // get corresponding patch ids
  //
  ::Foam::polyBoundaryMesh &bM =
    const_cast<::Foam::polyBoundaryMesh &>(mesh.boundaryMesh());
  ::Foam::label id0 = bM.findPatchID(csvString[0]);
  ::Foam::label id1 = bM.findPatchID(csvString[1]);
  dt__throwIf(id0 < 0, executeOnMesh());
  dt__throwIf(id1 < 0, executeOnMesh());

  //
  // create faceZones
  //
  dtFoamLibrary::patchToFaceZoneNoFlipMap(
    bM[id0], csvString[0] + "_zone", mesh
  );
  dtFoamLibrary::patchToFaceZoneNoFlipMap(
    bM[id1], csvString[1] + "_zone", mesh
  );

  //
  // create new ggi patches
  //
  ::Foam::cyclicGgiPolyPatch *ggi0 = new ::Foam::cyclicGgiPolyPatch(
    bM[id0].name(),
    bM[id0].size(),
    bM[id0].start(),
    bM[id0].index(),
    bM[id0].boundaryMesh(),
    csvString[1],
    csvString[0] + "_zone",
    parseOptionBool("bridgeOverlap", rule[2]),
    parseOptionVector("separationOffset", rule[2]),
    parseOptionVector("rotationAxis", rule[2]),
    parseOptionScalar("rotationAngle", rule[2])
  );
  ::Foam::cyclicGgiPolyPatch *ggi1 = new ::Foam::cyclicGgiPolyPatch(
    bM[id1].name(),
    bM[id1].size(),
    bM[id1].start(),
    bM[id1].index(),
    bM[id1].boundaryMesh(),
    csvString[0],
    csvString[1] + "_zone",
    parseOptionBool("bridgeOverlap", rule[2]),
    parseOptionVector("separationOffset", rule[2]),
    parseOptionVector("rotationAxis", rule[2]),
    -1. * parseOptionScalar("rotationAngle", rule[2])
  );

  //
  // replace old patches with new ggi patches
  //
  bM.set(id0, ggi0);
  bM.set(id1, ggi1);
}

void feOpenFOAMCyclicGgiRule::executeOnVolVectorField(
  std::vector<std::string> const &rule, ::Foam::volVectorField &field
) const
{
  feOpenFOAMSetupRule::executeOnVolVectorField(rule, field);
  //
  // get ggi patch labels
  //
  std::vector<std::string> csvString =
    stringPrimitive::convertToCSVStringVector(rule[1]);
  dt__throwIf(csvString.size() != 2, executeOnVolVectorField());

  ::Foam::volVectorField::GeometricBoundaryField &bF =
    const_cast<::Foam::volVectorField::GeometricBoundaryField &>(
      field.boundaryField()
    );

  forAll(bF, i)
  {
    //
    // get patch according to rule
    //
    if ((field.mesh().boundary()[i].name() == csvString[0]) ||
        (field.mesh().boundary()[i].name() == csvString[1]))
    {
      bF[i] = field.internalField();
    }
  }
}

void feOpenFOAMCyclicGgiRule::executeOnVolScalarField(
  std::vector<std::string> const &rule, ::Foam::volScalarField &field
) const
{
  feOpenFOAMSetupRule::executeOnVolScalarField(rule, field);
  //
  // get ggi patch labels
  //
  std::vector<std::string> csvString =
    stringPrimitive::convertToCSVStringVector(rule[1]);
  dt__throwIf(csvString.size() != 2, executeOnVolScalarField());

  ::Foam::volScalarField::GeometricBoundaryField &bF =
    const_cast<::Foam::volScalarField::GeometricBoundaryField &>(
      field.boundaryField()
    );

  forAll(bF, i)
  {
    //
    // get patch according to rule
    //
    if ((field.mesh().boundary()[i].name() == csvString[0]) ||
        (field.mesh().boundary()[i].name() == csvString[1]))
    {
      bF[i] = field.internalField();
    }
  }
}
} // namespace dtOO
