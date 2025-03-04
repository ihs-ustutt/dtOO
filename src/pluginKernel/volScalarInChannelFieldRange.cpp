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

#include "volScalarInChannelFieldRange.h"

#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/aGBuilder/dtPoint3_map2dTo3dPoint.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <baseContainerHeaven/baseContainer.h>
#include <boundedVolume.h>
#include <constValueHeaven/constValue.h>
#include <dtCase.h>
#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParser.h>

#include <interfaceHeaven/staticPropertiesHandler.h>
#include <interfaceHeaven/systemHandling.h>
#include <xmlHeaven/dtXmlParser.h>

#include <Time.H>
#include <argList.H>
#include <criticalHeaven/prepareOpenFOAM.h>
#include <interpolationCellPoint.H>
#include <linear.H>
#include <meshEngine/dtFoamLibrary.h>
#include <polyMesh.H>
#include <volFields.H>

#include "dtPluginFactory.h"
#include <logMe/dtParMacros.h>

namespace dtOO {
bool volScalarInChannelFieldRange::_registrated = dtPluginFactory::registrate(
  dt__tmpPtr(volScalarInChannelFieldRange, new volScalarInChannelFieldRange())
);

volScalarInChannelFieldRange::volScalarInChannelFieldRange() {}

volScalarInChannelFieldRange::~volScalarInChannelFieldRange() {}

void volScalarInChannelFieldRange::init(
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
  //		name="volScalarInChannelFieldRange"
  //		label="volScalarInChannelFieldRange"
  //    numPoints="{10}{10}"
  //    field="U"
  //    min="0.00"
  //    max="0.02"
  //    analyticGeometryLabel="myAnalyticGeometry"
  //	>
  //    <case label="myCase"/>
  //  </plugin>
  dt__throwIf(
    !dtXmlParser::hasChild("case", element) &&
      !dtXmlParser::hasAttribute("analyticGeometryLabel", element) &&
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
  dt__throwIf(cV->empty(), init());
  _parser = dtXmlParser::ptr();

  //
  // get analyticGeometry
  //
  _aG = aG->get(dtXmlParser::getAttributeStr("analyticGeometryLabel", element));

  //
  // get field label
  //
  _field = dtXmlParser::getAttributeStr("field", element);

  //
  // get number of points
  //
  _nP = dtXmlParser::getAttributeIntVectorMuParse("numPoints", element, cV, aF);
}

void volScalarInChannelFieldRange::apply(void)
{
  //
  // get directory
  //
  std::string wDir = _case->getDirectory(_parser->currentState());

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
      dt__throwIf(!fieldHeader.headerOk(), apply());
      dt__throwIf(fieldHeader.headerClassName() != "volScalarField", apply());

      //
      // read desired field
      //
      ::Foam::volScalarField volField(fieldHeader, mesh);

      //
      // read U header
      //
      ::Foam::IOobject UHeader(
        "U", runTime.timeName(), mesh, ::Foam::IOobject::MUST_READ
      );

      //
      // update field
      //
      mesh.readUpdate();
      //
      //        //
      //        // read UField
      //        //
      ::Foam::volVectorField UField(UHeader, mesh);

      //
      // interpolation
      //
      ::Foam::interpolationCellPoint<::Foam::vector> interU(UField);
      ::Foam::interpolationCellPoint<::Foam::scalar> interVolField(volField);

      //
      // only two dimensional mappings
      //
      dt__ptrAss(map2dTo3d const *m2d, map2dTo3d::ConstDownCast(_aG));

      //
      // create surface and value grid
      //
      twoDArrayHandling<dtPoint3> gridPre =
        dtPoint3_map2dTo3dPoint(m2d, _nP[0] + 1, _nP[1] + 1).result();

      //
      // create grid by averaging 4 points
      //
      twoDArrayHandling<dtPoint3> grid(_nP[0], _nP[1]);
      twoDArrayHandling<dtReal> A(_nP[0], _nP[1]);
      twoDArrayHandling<dtVector3> N(_nP[0], _nP[1]);
#pragma omp parallel
      {
#pragma omp for
        dt__forFromToIndex(0, _nP[0], ii)
        {
          dt__forFromToIndex(0, _nP[1], jj)
          {
            grid[ii][jj] = dtLinearAlgebra::toDtPoint3(
              (dtLinearAlgebra::toDtVector3(gridPre[ii][jj]) +
               dtLinearAlgebra::toDtVector3(gridPre[ii + 1][jj]) +
               dtLinearAlgebra::toDtVector3(gridPre[ii + 1][jj + 1]) +
               dtLinearAlgebra::toDtVector3(gridPre[ii][jj + 1])) /
              4.
            );
            N[ii][jj] =
              dtLinearAlgebra::normalize(dtLinearAlgebra::crossProduct(
                gridPre[ii + 1][jj] - gridPre[ii][jj],
                gridPre[ii][jj + 1] - gridPre[ii][jj]
              ));
            A[ii][jj] = dtLinearAlgebra::area(
              gridPre[ii][jj],
              gridPre[ii + 1][jj],
              gridPre[ii + 1][jj + 1],
              gridPre[ii][jj + 1]
            );
          }
        }
      }

      //
      // get values
      //
      twoDArrayHandling<dtReal> val(grid.size(0), grid.size(1));
      twoDArrayHandling<dtVector3> sf(grid.size(0), grid.size(1));
      twoDArrayHandling<dtReal> phi(grid.size(0), grid.size(1));

#pragma omp parallel
      {
#pragma omp for
        dt__forAllIndex(grid, ii)
        {
          dt__forAllIndex(grid[ii], jj)
          {
            //
            // get location
            //
            dtPoint3 const &xyz = grid[ii][jj];

            //
            // create FOAM vector and search cell that contains point
            //
            ::Foam::vector probePoint(xyz.x(), xyz.y(), xyz.z());
            ::Foam::label cId = mesh.findCell(probePoint);

            ::Foam::vector UValue;
            ::Foam::scalar volFieldValue;

            //
            // set not found cells area to zero
            //
            if (cId == -1)
            {
              A[ii][jj] = 0.;
              UValue = ::Foam::vector::zero;
              volFieldValue = ::Foam::scalar(0);
              dt__warning(apply(), << "Ignore point at " dt__point3d(xyz));
            }
            else
            {
              //
              // do interpolation
              //
              UValue = interU.interpolate(
                ::Foam::vector(xyz.x(), xyz.y(), xyz.z()), cId
              );
              volFieldValue = interVolField.interpolate(
                ::Foam::vector(xyz.x(), xyz.y(), xyz.z()), cId
              );
            }

            //
            // assign values
            //
            val[ii][jj] = volFieldValue;

            phi[ii][jj] =
              A[ii][jj] *
              dtLinearAlgebra::dotProduct(
                dtVector3(UValue.x(), UValue.y(), UValue.z()), N[ii][jj]
              );
            sf[ii][jj] = A[ii][jj] * N[ii][jj];
          }
        }
      }

      //
      // open file
      //
      std::string filename = _case->getDirectory(_parser->currentState()) +
                             "/" + virtualClassName() + "_" + getLabel() + "_" +
                             fieldHeader.name() + ".csv";
      std::fstream of;
      of.open(filename.c_str(), std::ios::out | std::ios::trunc);

      //
      // write header
      //
      of << "# 1  x" << std::endl
         << "# 2  y" << std::endl
         << "# 3  z" << std::endl
         << "# 4  value" << std::endl
         << "# 5  sfX " << std::endl
         << "# 6  sfY " << std::endl
         << "# 7  sfZ " << std::endl
         << "# 8  phi" << std::endl;

      //
      // write values
      //
      dt__forFromToIndex(0, grid.size(0), ii)
      {
        dt__forFromToIndex(0, grid.size(1), jj)
        {
          if (phi[ii][jj] == 0.)
            continue;

          of << logMe::dtFormat("%16.8e, %16.8e, %16.8e, "
                                "%16.8e, %16.8e, %16.8e, %16.8e, %16.8e") %
                  grid[ii][jj].x() % grid[ii][jj].y() % grid[ii][jj].z() %
                  val[ii][jj] % sf[ii][jj].x() % sf[ii][jj].y() %
                  sf[ii][jj].z() % phi[ii][jj]
             << std::endl;
        }
      }
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
