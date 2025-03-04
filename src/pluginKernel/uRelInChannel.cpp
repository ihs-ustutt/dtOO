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

#include "uRelInChannel.h"

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
#include <polyMesh.H>
#include <volFields.H>

#include "dtPluginFactory.h"
#include <logMe/dtParMacros.h>
#include <meshEngine/dtFoamLibrary.h>

namespace dtOO {
bool uRelInChannel::_registrated =
  dtPluginFactory::registrate(dt__tmpPtr(uRelInChannel, new uRelInChannel()));

uRelInChannel::uRelInChannel() {}

uRelInChannel::~uRelInChannel() {}

void uRelInChannel::init(
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
  //		name="uRelInChannel"
  //		label="uRelInChannel"
  //    numPoints="{10}{10}"
  //	>
  //    <case label="myCase"/>
  //    <analyticGeometry label="myAnalyticGeometry"
  //  </plugin>
  dt__throwIf(
    !dtXmlParser::hasChild("case", element) &&
      !dtXmlParser::hasChild("analyticGeometry", element) &&
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
  // get number of points
  //
  _nP = dtXmlParser::getAttributeIntVectorMuParse("numPoints", element, cV, aF);
}

void uRelInChannel::apply(void)
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
      // disable floating point exception trapping
      dtFoamLibrary::unsetFpe();

      //
      // create rootCase
      //
      ::Foam::argList args(argc, argv);
      if (!args.checkRootCase())
      {
        ::Foam::FatalError.exit();
      }

      //
      // create time
      //
      ::Foam::Time runTime(
        ::Foam::Time::controlDictName,
        args.rootPath(),
        args.caseName(),
        "system",
        "constant",
        !args.optionFound("noFunctionObjects")
      );

      //
      // create mesh
      //
      ::Foam::fvMesh mesh(::Foam::IOobject(
        ::Foam::fvMesh::defaultRegion,
        runTime.timeName(),
        runTime,
        ::Foam::IOobject::MUST_READ
      ));

      //
      // lastTime
      //
      runTime.setTime(runTime.times().last(), runTime.times().size() - 1);

      dt__info(apply(), << dt__eval(runTime.timeName()));

      //
      // read header of Ucyl and U
      //
      ::Foam::IOobject uRelHeader(
        "uRel", runTime.timeName(), mesh, ::Foam::IOobject::MUST_READ
      );
      ::Foam::IOobject UHeader(
        "U", runTime.timeName(), mesh, ::Foam::IOobject::MUST_READ
      );

      //
      // only volVectorField
      //
      dt__throwIf(
        !uRelHeader.headerOk() && !UHeader.headerOk() &&
          uRelHeader.headerClassName() != "volVectorField" &&
          UHeader.headerClassName() != "volVectorField",
        apply()
      );

      //
      // update field
      //
      mesh.readUpdate();

      //
      // read U and Ucyl
      //
      ::Foam::volVectorField uRelField(uRelHeader, mesh);
      ::Foam::volVectorField UField(UHeader, mesh);

      dt__forAllRefAuto(_aG, anAG)
      {
        //
        // only two dimensional mappings
        //
        dt__ptrAss(map2dTo3d const *m2d, map2dTo3d::ConstDownCast(anAG));

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
        twoDArrayHandling<dtVector3> U(grid.size(0), grid.size(1));
        twoDArrayHandling<dtVector3> uRel(grid.size(0), grid.size(1));
        twoDArrayHandling<dtReal> U_n(grid.size(0), grid.size(1));
        twoDArrayHandling<dtReal> uRel_n(grid.size(0), grid.size(1));
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
              ::Foam::vector uRelValue;

              //
              // set not found cells area to zero
              //
              if (cId == -1)
              {
                A[ii][jj] = 0.;
                UValue = ::Foam::vector::zero;
                uRelValue = ::Foam::vector::zero;
              }
              else
              {
                UValue = UField[cId];
                uRelValue = uRelField[cId];
              }

              //
              // assign values
              //
              U[ii][jj] = dtVector3(UValue.x(), UValue.y(), UValue.z());
              uRel[ii][jj] =
                dtVector3(uRelValue.x(), uRelValue.y(), uRelValue.z());

              //
              // normal components
              //
              U_n[ii][jj] = dtLinearAlgebra::dotProduct(U[ii][jj], N[ii][jj]);
              uRel_n[ii][jj] =
                dtLinearAlgebra::dotProduct(uRel[ii][jj], N[ii][jj]);
            }
          }
        }
        //
        // open file
        //
        std::string filename = _case->getDirectory(_parser->currentState()) +
                               "/" + m2d->getLabel() + "_uRelInChannel.csv";
        std::fstream of;
        of.open(filename.c_str(), std::ios::out | std::ios::trunc);

        //
        // write header
        //
        of << "# 1  i" << std::endl
           << "# 2  j" << std::endl
           << "# 3  x" << std::endl
           << "# 4  y" << std::endl
           << "# 5  z" << std::endl
           << "# 6  U_x" << std::endl
           << "# 7  U_y" << std::endl
           << "# 8  U_z" << std::endl
           << "# 9  uRel_x" << std::endl
           << "# 10 uRel_y" << std::endl
           << "# 11 uRel_z" << std::endl
           << "# 12 U_n" << std::endl
           << "# 13 uRel_n" << std::endl
           << "# 14 A" << std::endl;

        //
        // write values
        //
        dt__forFromToIndex(0, grid.size(), ii)
        {
          dt__forFromToIndex(0, grid[ii].size(), jj)
          {
            of << logMe::dtFormat("%4i, %4i, "
                                  "%16.8e, %16.8e, %16.8e, "
                                  "%16.8e, %16.8e, %16.8e, "
                                  "%16.8e, %16.8e, %16.8e, "
                                  "%16.8e, %16.8e, "
                                  "%16.8e") %
                    ii % jj % grid[ii][jj].x() % grid[ii][jj].y() %
                    grid[ii][jj].z() % U[ii][jj].x() % U[ii][jj].y() %
                    U[ii][jj].z() % uRel[ii][jj].x() % uRel[ii][jj].y() %
                    uRel[ii][jj].z() % U_n[ii][jj] % uRel_n[ii][jj] % A[ii][jj]
               << std::endl;
          }
        }
        of.close();

        //
        // open file
        //
        filename = _case->getDirectory(_parser->currentState()) + "/" +
                   m2d->getLabel() + "_uRelInChannel_iAvg.csv";
        of.open(filename.c_str(), std::ios::out | std::ios::trunc);
        //
        // write second header
        //
        of << "# 1  j" << std::endl
           << "# 2  avg(x)" << std::endl
           << "# 3  avg(y)" << std::endl
           << "# 4  avg(z)" << std::endl
           << "# 5  avg(U_x)" << std::endl
           << "# 6  avg(U_y)" << std::endl
           << "# 7  avg(U_z)" << std::endl
           << "# 8  avg(uRel_x)" << std::endl
           << "# 9  avg(uRel_y)" << std::endl
           << "# 10 avg(uRel_z)" << std::endl
           << "# 11 avg(U_n)" << std::endl
           << "# 12 avg(uRel_n)" << std::endl
           << "# 13 sum(uRel_x*A*U_n)/sum(A*U_n)" << std::endl
           << "# 14 sum(uRel_y*A*U_n)/sum(A*U_n)" << std::endl
           << "# 15 sum(uRel_z*A*U_n)/sum(A*U_n)" << std::endl
           << "# 16 sum(U_n*A*U_n)/sum(A*U_n)" << std::endl;

        //
        // get values
        //
        dt__forFromToIndex(0, grid.size(1), jj)
        {
          dtVector3 avgXYZ(0, 0, 0);
          dtReal avgR = 0.;
          dtReal avgZ = 0.;
          dtVector3 avgU(0, 0, 0);
          dtVector3 avguRel(0, 0, 0);
          dtReal sumAU_n = 0.;
          dtVector3 sumuRelAU_n(0, 0, 0);
          dtReal sumU_nAU_n = 0.;
          dtReal avgU_n = 0.;
          dtReal avguRel_n = 0.;
          dtInt nSamples = grid.fixJ(jj).size();
          dt__forFromToIndex(0, nSamples, ii)
          {
            if (A[ii][jj] == 0.)
              continue;

            avgXYZ = avgXYZ + dtLinearAlgebra::toDtVector3(grid[ii][jj]);
            avgU = avgU + U[ii][jj];
            avguRel = avguRel + uRel[ii][jj];
            sumAU_n = sumAU_n + A[ii][jj] * U_n[ii][jj];
            sumuRelAU_n = sumuRelAU_n + uRel[ii][jj] * A[ii][jj] * U_n[ii][jj];
            sumU_nAU_n = sumU_nAU_n + U_n[ii][jj] * A[ii][jj] * U_n[ii][jj];
            avgU_n = avgU_n + U_n[ii][jj];
            avguRel_n = avguRel_n + uRel_n[ii][jj];
          }
          //
          // average value and position
          //
          avgXYZ = avgXYZ / nSamples;
          avgR = avgR / nSamples;
          avgZ = avgZ / nSamples;
          avgU = avgU / nSamples;
          avguRel = avguRel / nSamples;
          avgU_n = avgU_n / nSamples;
          avguRel_n = avguRel_n / nSamples;
          //
          // write values
          //
          of << logMe::dtFormat("%16i, "
                                "%16.8e, %16.8e, %16.8e, "
                                "%16.8e, %16.8e, %16.8e, "
                                "%16.8e, %16.8e, %16.8e, "
                                "%16.8e, %16.8e, "
                                "%16.8e, %16.8e, %16.8e, "
                                "%16.8e") %
                  jj % avgXYZ.x() % avgXYZ.y() % avgXYZ.z() % avgU.x() %
                  avgU.y() % avgU.z() % avguRel.x() % avguRel.y() %
                  avguRel.z() % avgU_n % avguRel_n %
                  (sumuRelAU_n.x() / sumAU_n) % (sumuRelAU_n.y() / sumAU_n) %
                  (sumuRelAU_n.z() / sumAU_n) % (sumU_nAU_n / sumAU_n)
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
