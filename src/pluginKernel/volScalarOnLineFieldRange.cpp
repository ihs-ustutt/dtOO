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

#include "volScalarOnLineFieldRange.h"

#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/aGBuilder/dtPoint3_map1dTo3dPoint.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <baseContainerHeaven/baseContainer.h>
#include <boundedVolume.h>
#include <constValueHeaven/constValue.h>
#include <dtCase.h>
#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParser.h>

#include <interfaceHeaven/staticPropertiesHandler.h>
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
bool volScalarOnLineFieldRange::_registrated = dtPluginFactory::registrate(
  dt__tmpPtr(volScalarOnLineFieldRange, new volScalarOnLineFieldRange())
);

volScalarOnLineFieldRange::volScalarOnLineFieldRange() {}

volScalarOnLineFieldRange::~volScalarOnLineFieldRange() {}

void volScalarOnLineFieldRange::init(
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
  //		name="volScalarOnLineFieldRange"
  //		label="volScalarOnLineFieldRange"
  //    field="URel"
  //    numPoints="10"
  //	>
  //    <case label="myCase"/>
  //    <analyticGeometry label="myAnalyticGeometry"
  //  </plugin>
  dt__throwIf(
    !dtXmlParser::hasChild("case", element) &&
      !dtXmlParser::hasChild("analyticGeometry", element) &&
      !dtXmlParser::hasAttribute("field", element) &&
      !dtXmlParser::hasAttribute("numPoints", element),
    init()
  );

  //
  // get parser and case
  //
  _case = dC->get(dtXmlParser::getAttributeStr(
    "label", dtXmlParser::getChild("case", element)
  ));
  _parser = dtXmlParser::ptr();

  //
  // get analyticGeometries
  //
  dt__forAllRefAuto(
    dtXmlParser::getChildVector("analyticGeometry", element), anEl
  )
  {
    _aG.push_back(aG->get(dtXmlParser::getAttributeStr("label", anEl)));
  }

  //
  // get field label
  //
  _field = dtXmlParser::getAttributeStr("field", element);

  //
  // get number of points
  //
  _nP = dtXmlParser::getAttributeIntMuParse("numPoints", element, cV, aF);
}

void volScalarOnLineFieldRange::apply(void)
{
  //
  // get directory
  //
  std::string wDir = _case->getDirectory(_parser->currentState());

  dt__onlyMaster
  {
    dt__throwIf(!systemHandling::directoryExists(wDir), apply());

    //
    // enable exception throwing
    //
    ::Foam::FatalError.throwExceptions();
    ::Foam::FatalIOError.throwExceptions();

    //
    // argList
    //
    ::Foam::argList::noParallel();
    dtInt argc = 3;
    std::vector<std::string> argvStr(3);
    argvStr[0] = getLabel();
    argvStr[1] = std::string("-case");
    argvStr[2] = wDir;
    char **argv = new char *[3];
    argv[0] = const_cast<char *>(argvStr[0].c_str());
    argv[1] = const_cast<char *>(argvStr[1].c_str());
    argv[2] = const_cast<char *>(argvStr[2].c_str());

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
      dt__throwIf(!fieldHeader.headerOk(), apply());
      dt__throwIf(fieldHeader.headerClassName() != "volScalarField", apply());

      //
      // update field
      //
      mesh.readUpdate();

      //
      // read desired field
      //
      ::Foam::volScalarField volField(fieldHeader, mesh);

      dt__forAllRefAuto(_aG, anAG)
      {
        //
        // only one dimensional mappings
        //
        dt__ptrAss(map1dTo3d const *m1d, map1dTo3d::ConstDownCast(anAG));

        //
        // create vector and value grid
        //
        std::vector<dtPoint3> grid =
          dtPoint3_map1dTo3dPoint(m1d, _nP + 1)
            .result(); // dtLinearAlgebra::unitGrid(_nP);
        vectorHandling<dtReal> value(grid.size());

//
// get values
//
#pragma omp parallel
        {
#pragma omp for
          dt__forAllIndex(grid, ii)
          {
            dtPoint3 const &xyz = grid[ii];
            ::Foam::vector probePoint(xyz.x(), xyz.y(), xyz.z());
            ::Foam::label cId = mesh.findNearestCell(probePoint);
            if (cId == -1)
            {
              dt__warning(apply(), << "Ignore point at " dt__point3d(xyz));
              continue;
            }
            ::Foam::scalar ofValue = volField[cId];
            value[ii] = ofValue;
          }
        }

        //
        // open file
        //
        std::string filename = _case->getDirectory(_parser->currentState()) +
                               "/" + virtualClassName() + "_" + getLabel() +
                               "_" + m1d->getLabel() + "_" +
                               fieldHeader.name() + ".csv";
        std::fstream of;
        of.open(filename.c_str(), std::ios::out | std::ios::trunc);

        //
        // calculate length
        //
        vectorHandling<dtReal> ll(grid.size(), 0);
        dt__forFromToIndex(1, ll.size(), ii)
        {
          ll[ii] =
            ll[ii - 1] + dtLinearAlgebra::length(grid[ii] - grid[ii - 1]);
        }

        //
        // write header
        //
        of << "# 1  x" << std::endl
           << "# 2  y" << std::endl
           << "# 3  z" << std::endl
           << "# 4  l" << std::endl
           << "# 5  value" << std::endl;

        //
        // write values
        //
        dt__forFromToIndex(0, value.size(), ii)
        {
          of << logMe::dtFormat("%16.8e, %16.8e, %16.8e, %16.8e, %16.8e") %
                  grid[ii].x() % grid[ii].y() % grid[ii].z() % ll[ii] %
                  value[ii]
             << std::endl;
        }
        of.close();
      }
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
