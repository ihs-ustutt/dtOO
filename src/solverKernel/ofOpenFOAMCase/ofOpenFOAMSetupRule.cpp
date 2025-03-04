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

#include "ofOpenFOAMSetupRule.h"

#include <interfaceHeaven/stringPrimitive.h>
#include <logMe/logMe.h>
#include <xmlHeaven/qtXmlBase.h>

#include <boost/assign.hpp>
#include <criticalHeaven/prepareOpenFOAM.h>
#include <dictionary.H>
#include <epsilonWallFunctionFvPatchScalarField.H>
#include <fixedValueFvPatchField.H>
#include <flowRateInletVelocityFvPatchVectorField.H>
#include <inletOutletFvPatchField.H>
#include <kqRWallFunctionFvPatchField.H>
#include <movingWallVelocityFvPatchVectorField.H>
#include <nutkWallFunctionFvPatchScalarField.H>
#include <omegaWallFunctionFvPatchScalarField.H>
#include <polyBoundaryMesh.H>
#include <polyMesh.H>
#include <polyPatch.H>
#include <rotatingWallVelocityFvPatchVectorField.H>
#include <slipFvPatchField.H>
#include <surfaceFields.H>
#include <turbulentIntensityKineticEnergyInletFvPatchScalarField.H>
#include <turbulentMixingLengthDissipationRateInletFvPatchScalarField.H>
#include <turbulentMixingLengthFrequencyInletFvPatchScalarField.H>
#include <volFields.H>
#include <zeroGradientFvPatchField.H>

namespace dtOO {
dt__pVH(ofOpenFOAMSetupRule) ofOpenFOAMSetupRule::_rules;

bool ofOpenFOAMSetupRule::_registrated = ofOpenFOAMSetupRule::registrate(
  dt__tmpPtr(ofOpenFOAMSetupRule, new ofOpenFOAMSetupRule())
);

ofOpenFOAMSetupRule::ofOpenFOAMSetupRule() {}

ofOpenFOAMSetupRule::~ofOpenFOAMSetupRule() {}

std::vector<std::string> ofOpenFOAMSetupRule::factoryAlias(void) const
{
  return std::vector<std::string>(
  ); //::boost::assign::list_of("OpenFOAMSetupRule");
}

bool ofOpenFOAMSetupRule::registrate(ofOpenFOAMSetupRule const *const rule)
{
  dt__forAllRefAuto(_rules, aRule)
  {
    if (aRule.virtualClassName() == rule->virtualClassName())
    {
      return false;
    }
  }
  _rules.push_back(rule->create());

  return true;
}

ofOpenFOAMSetupRule *ofOpenFOAMSetupRule::create(std::string const &str)
{
  dt__info(create(), << "str = " << str);
  dt__forAllRefAuto(_rules, aRule)
  {
    //
    // check virtual class name
    //
    if (aRule.virtualClassName() == str)
    {
      return aRule.create();
    }

    //
    // check alias
    //
    dt__forAllRefAuto(aRule.factoryAlias(), anAlias)
    {
      if (anAlias == str)
        return aRule.create();
    }
  }

  std::vector<std::string> av;
  dt__forAllRefAuto(_rules, aRule)
  {
    av.push_back(aRule.virtualClassName());
    dt__forAllRefAuto(aRule.factoryAlias(), anAlias)
    {
      av.push_back("  -> " + anAlias);
    }
  }
  dt__throw(
    create(),
    << str << " could not be created." << std::endl
    << "Implemented rules:" << std::endl
    << logMe::vecToString(av, 1) << std::endl
  );
}

void ofOpenFOAMSetupRule::init(
  baseContainer const *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV,
  lvH_dtCase const *const dC
)
{
  _bC = bC;
  _cV = cV;
  _aF = aF;
  _aG = aG;
  _bV = bV;
  _dC = dC;
}

void ofOpenFOAMSetupRule::executeOnMesh(
  std::vector<std::string> const &rule, ::Foam::polyMesh &mesh
) const
{
  //
  // get boundary patch and id
  //
  ::Foam::polyBoundaryMesh &bM =
    const_cast<::Foam::polyBoundaryMesh &>(mesh.boundaryMesh());
  ::Foam::label id = bM.findPatchID(rule[1]);
  dt__throwIfWithMessage(id < 0, executeOnMesh(), << "rule = " << rule);

  //
  // replace old patch
  //
  bM.set(
    id,
    new ::Foam::polyPatch(
      bM[id].name(),
      bM[id].size(),
      bM[id].start(),
      bM[id].index(),
      bM[id].boundaryMesh(),
      bM[id].physicalType(),
      bM[id].inGroups()
    )
  );
}

void ofOpenFOAMSetupRule::executeOnVolVectorField(
  std::vector<std::string> const &rule, ::Foam::volVectorField &field
) const
{
  dt__debug(executeOnVolVectorField(), << "Execute: " << rule);
  std::string thisRule = getRuleOfField(field.name(), rule);
  if (thisRule.empty())
    return;

  ::Foam::volVectorField::Boundary &bF =
    const_cast<::Foam::volVectorField::Boundary &>(field.boundaryField());

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
      if (stringPrimitive::stringContains(
            "fixedValue", stringPrimitive::getStringBetween("", "(", thisRule)
          ))
      {
        //
        // create and set new patch
        //
        bF.set(
          i,
          new ::Foam::fixedValueFvPatchField<::Foam::vector>(
            field.mesh().boundary()[i],
            field,
            parseOptionDict("fixedValue", thisRule)
          )
        );

        return;
      }
      else if (stringPrimitive::stringContains(
                 "zeroGradient",
                 stringPrimitive::getStringBetween("", "(", thisRule)
               ))
      {
        //
        // create and set new patch
        //
        bF.set(
          i,
          new ::Foam::zeroGradientFvPatchField<::Foam::vector>(
            field.mesh().boundary()[i],
            field,
            parseOptionDict("zeroGradient", thisRule)
          )
        );

        return;
      }
      else if (stringPrimitive::stringContains(
                 "inletOutlet",
                 stringPrimitive::getStringBetween("", "(", thisRule)
               ))
      {
        //
        // create and set new patch
        //
        bF.set(
          i,
          new ::Foam::inletOutletFvPatchField<::Foam::vector>(
            field.mesh().boundary()[i],
            field,
            parseOptionDict("inletOutlet", thisRule)
          )
        );

        return;
      }
      else if (stringPrimitive::stringContains(
                 "rotatingWallVelocity",
                 stringPrimitive::getStringBetween("", "(", thisRule)
               ))
      {
        //
        // create and set new patch
        //
        bF.set(
          i,
          new ::Foam::rotatingWallVelocityFvPatchVectorField(
            field.mesh().boundary()[i],
            field,
            parseOptionDict("rotatingWallVelocity", thisRule)
          )
        );

        return;
      }
      else if (stringPrimitive::stringContains(
                 "flowRateInletVelocity",
                 stringPrimitive::getStringBetween("", "(", thisRule)
               ))
      {
        //
        // create and set new patch
        //
        bF.set(
          i,
          new ::Foam::flowRateInletVelocityFvPatchVectorField(
            field.mesh().boundary()[i],
            field,
            parseOptionDict("flowRateInletVelocity", thisRule)
          )
        );

        return;
      }
      else if (stringPrimitive::stringContains(
                 "slip", stringPrimitive::getStringBetween("", "(", thisRule)
               ))
      {
        //
        // create and set new patch
        //
        bF.set(
          i,
          new ::Foam::slipFvPatchField<::Foam::vector>(
            field.mesh().boundary()[i], field, parseOptionDict("slip", thisRule)
          )
        );

        return;
      }
      else if (stringPrimitive::stringContains(
                 "movingWallVelocity",
                 stringPrimitive::getStringBetween("", "(", thisRule)
               ))
      {
        //
        // create and set new patch
        //
        bF.set(
          i,
          new ::Foam::movingWallVelocityFvPatchVectorField(
            field.mesh().boundary()[i],
            field,
            parseOptionDict("movingWallVelocity", thisRule)
          )
        );

        return;
      }
      else
        dt__throwUnexpected(executeOnVolVectorField());
    }
  }
}

void ofOpenFOAMSetupRule::executeOnVolScalarField(
  std::vector<std::string> const &rule, ::Foam::volScalarField &field
) const
{
  std::string thisRule = getRuleOfField(field.name(), rule);
  if (thisRule.empty())
    return;

  ::Foam::volScalarField::Boundary &bF =
    const_cast<::Foam::volScalarField::Boundary &>(field.boundaryField());

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
      if (stringPrimitive::stringContains(
            "fixedValue", stringPrimitive::getStringBetween("", "(", thisRule)
          ))
      {
        //
        // create and set new patch
        //
        bF.set(
          i,
          new ::Foam::fixedValueFvPatchField<::Foam::scalar>(
            field.mesh().boundary()[i],
            field,
            parseOptionDict("fixedValue", thisRule)
          )
        );

        return;
      }
      else if (stringPrimitive::stringContains(
                 "zeroGradient",
                 stringPrimitive::getStringBetween("", "(", thisRule)
               ))
      {
        //
        // create and set new patch
        //
        bF.set(
          i,
          new ::Foam::zeroGradientFvPatchField<::Foam::scalar>(
            field.mesh().boundary()[i],
            field,
            parseOptionDict("zeroGradient", thisRule)
          )
        );

        return;
      }
      else if (stringPrimitive::stringContains(
                 "kqRWallFunction",
                 stringPrimitive::getStringBetween("", "(", thisRule)
               ))
      {
        //
        // create and set new patch
        //
        bF.set(
          i,
          new ::Foam::kqRWallFunctionFvPatchField<::Foam::scalar>(
            field.mesh().boundary()[i],
            field,
            parseOptionDict("kqRWallFunction", thisRule)
          )
        );

        return;
      }
      else if (stringPrimitive::stringContains(
                 "epsilonWallFunction",
                 stringPrimitive::getStringBetween("", "(", thisRule)
               ))
      {
        //
        // create and set new patch
        //
        bF.set(
          i,
          new ::Foam::epsilonWallFunctionFvPatchScalarField(
            field.mesh().boundary()[i],
            field,
            parseOptionDict("epsilonWallFunction", thisRule)
          )
        );

        return;
      }
      else if (stringPrimitive::stringContains(
                 "omegaWallFunction",
                 stringPrimitive::getStringBetween("", "(", thisRule)
               ))
      {
        //
        // create and set new patch
        //
        bF.set(
          i,
          new ::Foam::omegaWallFunctionFvPatchScalarField(
            field.mesh().boundary()[i],
            field,
            parseOptionDict("omegaWallFunction", thisRule)
          )
        );

        return;
      }
      else if (stringPrimitive::stringContains(
                 "nutkWallFunction",
                 stringPrimitive::getStringBetween("", "(", thisRule)
               ))
      {
        //
        // create and set new patch
        //
        bF.set(
          i,
          new ::Foam::nutkWallFunctionFvPatchScalarField(
            field.mesh().boundary()[i],
            field,
            parseOptionDict("nutkWallFunction", thisRule)
          )
        );

        return;
      }
      else if (stringPrimitive::stringContains(
                 "turbulentIntensityKineticEnergyInlet",
                 stringPrimitive::getStringBetween("", "(", thisRule)
               ))
      {
        //
        // create and set new patch
        //
        bF.set(
          i,
          new ::Foam::turbulentIntensityKineticEnergyInletFvPatchScalarField(
            field.mesh().boundary()[i],
            field,
            parseOptionDict("turbulentIntensityKineticEnergyInlet", thisRule)
          )
        );

        return;
      }
      else if (stringPrimitive::stringContains(
                 "turbulentMixingLengthDissipationRateInlet",
                 stringPrimitive::getStringBetween("", "(", thisRule)
               ))
      {
        //
        // create and set new patch
        //
        bF.set(
          i,
          new ::Foam::
            turbulentMixingLengthDissipationRateInletFvPatchScalarField(
              field.mesh().boundary()[i],
              field,
              parseOptionDict(
                "turbulentMixingLengthDissipationRateInlet", thisRule
              )
            )
        );

        return;
      }
      else if (stringPrimitive::stringContains(
                 "turbulentMixingLengthFrequencyInlet",
                 stringPrimitive::getStringBetween("", "(", thisRule)
               ))
      {
        //
        // create and set new patch
        //
        bF.set(
          i,
          new ::Foam::turbulentMixingLengthFrequencyInletFvPatchScalarField(
            field.mesh().boundary()[i],
            field,
            parseOptionDict("turbulentMixingLengthFrequencyInlet", thisRule)
          )
        );

        return;
      }

      else if (stringPrimitive::stringContains(
                 "slip", stringPrimitive::getStringBetween("", "(", thisRule)
               ))
      {
        //
        // create and set new patch
        //
        bF.set(
          i,
          new ::Foam::slipFvPatchField<::Foam::scalar>(
            field.mesh().boundary()[i], field, parseOptionDict("slip", thisRule)
          )
        );

        return;
      }
      else
        dt__throw(
          executeOnVolScalarField(),
          << "Not supported: "
          << stringPrimitive::getStringBetween("", "(", thisRule)
        );
    }
  }
}

std::string ofOpenFOAMSetupRule::getRuleOfField(
  std::string const &fieldName, std::vector<std::string> const &rule
)
{
  //
  // start at position 3
  //
  for (int ii = 3; ii < rule.size(); ii = ii + 2)
  {
    //
    // if rule[ii] is equal field name return
    //
    if (rule[ii] == fieldName)
    {
      dt__throwIf((ii + 1) > (rule.size() - 1), getRuleOfField());
      return rule[ii + 1];
    }
  }
  return std::string("");
}

std::vector<std::string>
ofOpenFOAMSetupRule::optionVector(std::string const &str)
{
  return stringPrimitive::convertToCSVStringVector(str);
}

std::vector<std::string>
ofOpenFOAMSetupRule::availableOptions(std::string const &str)
{
  std::vector<std::string> names;
  dt__forAllRefAuto(optionVector(str), aOpt)
  {
    names.push_back(stringPrimitive::getStringBetween("", "(", aOpt));
  }
  return names;
}

std::string ofOpenFOAMSetupRule::parseOptionStr(
  std::string const &name, std::string const &str
)
{
  if (stringPrimitive::stringContains(name, str))
  {
    return stringPrimitive::getStringBetweenRespectOcc(
      "(", ")", str.substr(str.find(name))
    );
  }
  dt__warning(
    parseOptionStr(),
    << "Cannot find option. Return empty string." << std::endl
    << dt__eval(name) << std::endl
    << dt__eval(str)
  );
  return std::string("");
}

bool ofOpenFOAMSetupRule::parseOptionBool(
  std::string const &name, std::string const &str
)
{
  if (parseOptionStr(name, str) == "true")
    return true;
  return false;
}

::Foam::vector ofOpenFOAMSetupRule::parseOptionVector(
  std::string const &name, std::string const &str
)
{
  std::vector<dtReal> ff =
    qtXmlBase::muParseCSString(parseOptionStr(name, str));

  dt__throwIf(ff.size() != 3, parseOptionVector());

  return ::Foam::vector(ff[0], ff[1], ff[2]);
}

::Foam::scalar ofOpenFOAMSetupRule::parseOptionScalar(
  std::string const &name, std::string const &str
)
{
  return ::Foam::scalar(qtXmlBase::muParseString(parseOptionStr(name, str)));
}

::Foam::dictionary ofOpenFOAMSetupRule::parseOptionDict(
  std::string const &name, std::string const &str
)
{
  ::Foam::IStringStream is("{" + parseOptionStr(name, str) + "}");

  return ::Foam::dictionary(is());
}

lvH_analyticFunction const &ofOpenFOAMSetupRule::refAF(void) const
{
  return *_aF;
}
} // namespace dtOO
