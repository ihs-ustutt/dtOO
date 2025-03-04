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

#include "volScalarFieldRange.h"

#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/scaOneD.h>
#include <analyticFunctionHeaven/scaThreeD.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <baseContainerHeaven/baseContainer.h>
#include <boundedVolume.h>
#include <constValueHeaven/constValue.h>
#include <dtCase.h>
#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParser.h>

#include <interfaceHeaven/systemHandling.h>
#include <logMe/dtParMacros.h>
#include <xmlHeaven/dtXmlParser.h>

#include "dtPluginFactory.h"
#include <Time.H>
#include <argList.H>
#include <criticalHeaven/prepareOpenFOAM.h>
#include <meshEngine/dtFoamLibrary.h>
#include <polyMesh.H>
#include <volFields.H>

namespace dtOO {
bool volScalarFieldRange::_registrated = dtPluginFactory::registrate(
  dt__tmpPtr(volScalarFieldRange, new volScalarFieldRange())
);

volScalarFieldRange::volScalarFieldRange() {}

volScalarFieldRange::~volScalarFieldRange() {}

void volScalarFieldRange::init(
  ::QDomElement const &element,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV,
  lvH_dtCase const *const dC,
  lvH_dtPlugin const *const pL
)
{
  dtPlugin::init(element, bC, cV, aF, aG, bV, dC, pL);

  //	<plugin
  //		name="volScalarFieldRange"
  //		label="volScalarFieldRange"
  //    field="U"
  //    inRangeFunction="myFun"
  //	>
  //    <case label="myCase"/>
  //  </plugin>
  dt__throwIf(
    !dtXmlParser::hasChild("case", element) &&
      !dtXmlParser::hasAttribute("field", element),
    init()
  );

  //
  // get parser and case
  //
  _case = dC->get(dtXmlParser::getAttributeStr(
    "label", dtXmlParser::getChild("case", element)
  ));

  //
  // get analyticFunctions
  //
  if (dtXmlParser::hasAttribute("inRangeFunction", element))
  {
    _inRange.reset(scaOneD::MustDownCast(aF->get(dtXmlParser::getAttributeStr(
                                           "inRangeFunction", element
                                         )))
                     ->clone());
  }
  if (dtXmlParser::hasAttribute("inPositionFunction", element))
  {
    _inPosition.reset(
      scaThreeD::MustDownCast(
        aF->get(dtXmlParser::getAttributeStr("inPositionFunction", element))
      )
        ->clone()
    );
  }

  //
  // get field label
  //
  _field = dtXmlParser::getAttributeStr("field", element);
}

void volScalarFieldRange::apply(void)
{
  //
  // get directory
  //
  std::string wDir = _case->getDirectory(dtXmlParser::ptr()->currentState());

  dt__onlyMaster
  {
    dt__throwIf(!systemHandling::directoryExists(wDir), apply());

    try
    {
      dt__pH(::Foam::argList) args = dtFoamLibrary::initCase(getLabel(), wDir);

      //
      // create time
      //
      ::Foam::Time runTime(
        Foam::Time::controlDictName,
        args->rootPath(),
        args->caseName(),
        "system",
        "constant",
        !args->optionFound("noFunctionObjects")
      );

      //
      // create mesh
      //
      Foam::fvMesh mesh(Foam::IOobject(
        Foam::fvMesh::defaultRegion,
        runTime.timeName(),
        runTime,
        Foam::IOobject::MUST_READ
      ));

      //
      // lastTime
      //
      runTime.setTime(runTime.times().last(), runTime.times().size() - 1);

      //
      // read field header
      //
      ::Foam::IOobject fieldHeader(
        _field, runTime.timeName(), mesh, ::Foam::IOobject::MUST_READ
      );

      //
      // only volScalarField
      //
      dt__throwIf(
        !fieldHeader.headerOk() ||
          fieldHeader.headerClassName() != "volScalarField",
        apply()
      );

      //
      // read desired field
      //
      ::Foam::volScalarField volField(fieldHeader, mesh);

      //
      // update field
      //
      mesh.readUpdate();

      //
      // get values
      //
      std::vector<dtPoint3> coord;
      std::vector<dtReal> val;
      std::vector<dtReal> vol;

      forAll(volField, cId)
      {
        ::Foam::scalar volFieldValue = volField[cId];

        if (_inRange && (_inRange->YFloat(volFieldValue) == 0))
          continue;
        if (_inPosition && (_inPosition->YFloat(
                              mesh.C()[cId].component(0),
                              mesh.C()[cId].component(1),
                              mesh.C()[cId].component(2)
                            ) == 0))
          continue;

        //
        // get location
        //
        coord.push_back(dtPoint3(
          mesh.C()[cId].component(0),
          mesh.C()[cId].component(1),
          mesh.C()[cId].component(2)
        ));

        //
        // assign values
        //
        val.push_back(volFieldValue);
        vol.push_back(mesh.cellVolumes()[cId]);
      }

      //
      // open file
      //
      std::string filename =
        _case->getDirectory(dtXmlParser::constReference().currentState()) +
        "/" + virtualClassName() + "_" + getLabel() + "_" + fieldHeader.name() +
        ".csv";
      std::fstream of;
      of.open(filename.c_str(), std::ios::out | std::ios::trunc);

      //
      // write header
      //
      of << "# 1  x" << std::endl
         << "# 2  y" << std::endl
         << "# 3  z" << std::endl
         << "# 4  value" << std::endl
         << "# 5  volume" << std::endl;

      //
      // write values
      //
      dt__forFromToIndex(0, coord.size(), ii)
      {
        of << logMe::dtFormat("%16.8e, %16.8e, %16.8e, %16.8e, %16.8e") %
                coord[ii].x() % coord[ii].y() % coord[ii].z() % val[ii] %
                vol[ii]
           << std::endl;
      }

      //
      // close file
      //
      of.close();
    } catch (::Foam::error &err)
    {
      dt__throw(
        apply(),
        << "Instance of ::Foam::error thrown." << std::endl
        << dt__eval(err.what()) << std::endl
        << dt__eval(err.message())
      );
    }
  }
}
} // namespace dtOO
