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

#include "feOpenFOAMTurboRule.h"

#include <interfaceHeaven/stringPrimitive.h>
#include <logMe/logMe.h>

#include <boost/assign.hpp>
#include <criticalHeaven/prepareOpenFOAM.h>
#include <polyBoundaryMesh.H>
#include <polyMesh.H>
#include <polyPatch.H>
#include <profile1DfixedValueFvPatchField.H>
#include <volFields.H>

namespace dtOO {
bool feOpenFOAMTurboRule::_registrated = feOpenFOAMSetupRule::registrate(
  dt__tmpPtr(feOpenFOAMTurboRule, new feOpenFOAMTurboRule())
);

feOpenFOAMTurboRule::feOpenFOAMTurboRule() {}

feOpenFOAMTurboRule::~feOpenFOAMTurboRule() {}

std::vector<std::string> feOpenFOAMTurboRule::factoryAlias(void) const
{
  return ::boost::assign::list_of("OpenFOAMTurboRule");
}

void feOpenFOAMTurboRule::executeOnVolVectorField(
  std::vector<std::string> const &rule, ::Foam::volVectorField &field
) const
{
  std::string thisRule = getRuleOfField(field.name(), rule);
  if (thisRule.empty())
    return;

  ::Foam::volVectorField::GeometricBoundaryField &bF =
    const_cast<::Foam::volVectorField::GeometricBoundaryField &>(
      field.boundaryField()
    );

  forAll(bF, i)
  {
    //
    // get patch according to rule
    //
    if (field.mesh().boundary()[i].name() == rule[1])
    {
      dt__info(
        executeOnVolVectorField(),
        << "Executing" << std::endl
        << dt__eval(thisRule) << std::endl
        << "on" << std::endl
        << "field.mesh().boundary()[" << i
        << "].name() = " << field.mesh().boundary()[i].name() << std::endl
        << "of" << std::endl
        << dt__eval(field.name())
      );
      if (stringPrimitive::getStringBetween("", "(", thisRule) ==
          "profile1DfixedValue")
      {
        //
        // create and set new patch
        //
        bF.set(
          i,
          new ::Foam::profile1DfixedValueFvPatchField<::Foam::vector>(
            field.mesh().boundary()[i],
            field,
            parseOptionDict("profile1DfixedValue", thisRule)
          )
        );

        return;
      }
      else
        dt__throwUnexpected(executeOnVolVectorField());
    }
  }
}

void feOpenFOAMTurboRule::executeOnVolScalarField(
  std::vector<std::string> const &rule, ::Foam::volScalarField &field
) const
{
  std::string thisRule = getRuleOfField(field.name(), rule);
  if (thisRule.empty())
    return;

  ::Foam::volScalarField::GeometricBoundaryField &bF =
    const_cast<::Foam::volScalarField::GeometricBoundaryField &>(
      field.boundaryField()
    );

  forAll(bF, i)
  {
    //
    // get patch according to rule
    //
    if (field.mesh().boundary()[i].name() == rule[1])
    {
      dt__info(
        executeOnVolScalarField(),
        << "Executing" << std::endl
        << dt__eval(thisRule) << std::endl
        << "on" << std::endl
        << "field.mesh().boundary()[" << i
        << "].name() = " << field.mesh().boundary()[i].name() << std::endl
        << "of" << std::endl
        << dt__eval(field.name())
      );
      if (stringPrimitive::getStringBetween("", "(", thisRule) ==
          "profile1DfixedValue")
      {
        //
        // create and set new patch
        //
        bF.set(
          i,
          new ::Foam::profile1DfixedValueFvPatchField<::Foam::scalar>(
            field.mesh().boundary()[i],
            field,
            parseOptionDict("profile1DfixedValue", thisRule)
          )
        );

        return;
      }
      else
        dt__throwUnexpected(executeOnVolScalarField());
    }
  }
}
} // namespace dtOO
