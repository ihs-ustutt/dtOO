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

#include "dtMeshFreeGradingGEdge.h"

#include "dtGmshEdge.h"
#include "dtMeshOperatorFactory.h"
#include <analyticFunctionHeaven/aFBuilder/float_scaOneDPoint.h>
#include <analyticFunctionHeaven/analyticFunctionCompoundTrojanHorse.h>
#include <analyticFunctionHeaven/scaLinearOneD.h>
#include <analyticFunctionHeaven/scaMultiOneD.h>
#include <analyticFunctionHeaven/scaOneD.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <gmsh/GVertex.h>
#include <gmsh/GmshDefines.h>
#include <gmsh/MLine.h>
#include <gmsh/MVertex.h>
#include <interfaceHeaven/threadSafe.h>
#include <interfaceHeaven/timeHandling.h>
#include <logMe/dtMacros.h>
#include <progHelper.h>
#include <xmlHeaven/qtXmlBase.h>

namespace dtOO {
bool dtMeshFreeGradingGEdge::_registrated = dtMeshOperatorFactory::registrate(
  dt__tmpPtr(dtMeshFreeGradingGEdge, new dtMeshFreeGradingGEdge())
);

dtMeshFreeGradingGEdge::dtMeshFreeGradingGEdge(void) : dtMeshGEdge() {}

dtMeshFreeGradingGEdge::~dtMeshFreeGradingGEdge() {}

void dtMeshFreeGradingGEdge::jInit(
  jsonPrimitive const &jE,
  baseContainer const *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV,
  lvH_dtMeshOperator const *const mO
)
{
  dtMeshGEdge::jInit(jE, bC, cV, aF, aG, bV, mO);
  std::vector<dtInt> tTrans = jE.lookup<std::vector<dtInt>>("typeTransfinite");
  //
  // it is necessary to store here the raw pointers and not a clone of the
  // function, because the function is a compound that can be modified in
  // another observer
  //
  std::vector<analyticFunction const *> gFun =
    jE.lookupVecRaw<analyticFunction>("gradingFunctions", aF);
  dt__forAllIndex(gFun, ii)
  {
    _gradingInt[tTrans[ii]] = scaOneD::ConstDownCast(gFun[ii]);
    dt__info(
      jInit(),
      << "Map < " << tTrans[ii] << " > to "
      << _gradingInt[tTrans[ii]]->getLabel()
    );
  }
}

void dtMeshFreeGradingGEdge::init(
  ::QDomElement const &element,
  baseContainer const *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV,
  lvH_dtMeshOperator const *const mO
)
{
  dtMeshGEdge::init(element, bC, cV, aF, aG, bV, mO);

  jsonPrimitive jE;
  jE.append<std::vector<dtInt>>(
    "typeTransfinite",
    qtXmlBase::getAttributeIntVector("typeTransfinite", element)
  );

  std::vector<std::string> aFLabel =
    qtXmlBase::getAttributeStrVector("gradingLabel", element);

  lvH_analyticFunction gradFun;
  dt__forAllIndex(aFLabel, ii) { gradFun.push_back(aF->get(aFLabel[ii])); }
  jE.append<std::vector<analyticFunction *>>("gradingFunctions", gradFun);
  jE.append<dtReal>(
    "_tolerance",
    dtXmlParserBase::getAttributeFloatMuParse(
      "tolerance", element, cV, aF, 1.e-8
    )
  );
  jE.append<dtInt>(
    "_nSmoothSteps",
    dtXmlParserBase::getAttributeIntMuParse("nSmoothSteps", element, cV, aF, 20)
  );
  dtMeshFreeGradingGEdge::jInit(jE, bC, cV, aF, aG, bV, mO);
}

void dtMeshFreeGradingGEdge::operator()(dtGmshEdge *dtge)
{
  dtInt transType = dtge->meshAttributes.typeTransfinite;
  if (dtge->meshAttributes.method == MESH_TRANSFINITE &&
      _gradingInt.find(transType) != _gradingInt.end())
  {
    bool reverse = false;
    if (dtge->meshAttributes.coeffTransfinite < 0.)
      reverse = true;
    ::Msg::Info(
      "Meshing curve %d ( dtMeshFreeGradingGEdge transType=%d, reverse=%d)",
      dtge->tag(),
      transType,
      reverse
    );
    map1dTo3d const *const m1d = dtge->getMap1dTo3d();

    dtInt const nP = dtge->meshAttributes.nbPointsTransfinite;
    std::vector<dtReal> gg =
      float_scaOneDPoint(_gradingInt[transType], nP).result();
    if (_gradingInt[transType]->isCompound())
    {
      dt__info(
        operator()(),
        << "Grading type = " << transType << " is a analyticFunctionCompound."
      );
      analyticFunctionCompoundTrojanHorse<scaOneD> trojanHorse(
        _gradingInt[transType],
        _gradingInt[transType]->vecRef(),
        _gradingInt[transType]->mapRef()
      );

      if (trojanHorse.hasTag(dtge->tag()))
      {
        dt__info(
          operator()(),
          << "Grading type = " << transType
          << " contains a tagged compound of tag = " << dtge->tag() << "."
        );

        gg = float_scaOneDPoint(trojanHorse.componentFromTag(dtge->tag()), nP)
               .result();
      }
    }

    if (reverse)
    {
      dt__forAllRefAuto(gg, aGG) aGG = 1. - aGG;
      progHelper::reverse(gg);
    }

    logContainer<dtMeshFreeGradingGEdge> logC(logDEBUG, "operator()");

    std::vector<dtReal> uu(gg.size());
    uu[0] = m1d->getUMin();
    uu[gg.size() - 1] = m1d->getUMax();
    std::vector<dtPoint3> p3_u(gg.size());
    p3_u[0] = m1d->getPoint(uu[0]);
    p3_u[gg.size() - 1] = m1d->getPoint(uu[gg.size() - 1]);

    dt__forFromToIndex(1, nP, ii) uu[ii] = m1d->u_percent(gg[ii]);

    //      #pragma omp parallel
    //      {
    //        threadSafe< dt__pH(map1dTo3d) > wMap;
    //        wMap().reset( m1d->clone() );
    //        #pragma omp for
    //        dt__forFromToIndex(1, nP-1, ii) {
    //          uu[ii] = wMap()->u_percent( gg[ii] );
    //          p3_u[ii] = wMap()->getPoint(uu[ii]);
    //        }
    //      }

    dt__forFromToIndex(
      0, config().lookupDef<dtInt>("_nSmoothSteps", 20), smoothIt
    )
    {
      std::vector<dtReal> dL(gg.size(), 0.);
      dt__forFromToIndex(1, nP, ii)
      {
        p3_u[ii] = m1d->getPoint(uu[ii]);
        dL[ii] = dtLinearAlgebra::length(p3_u[ii] - p3_u[ii - 1]);
      }

      dtReal sumL = dtLinearAlgebra::sum(dL);
      std::vector<dtReal> ll(gg.size(), 0.);
      scaMultiOneD<scaLinearOneD> l_u;
      dt__forFromToIndex(1, nP, ii)
      {
        ll[ii] = ll[ii - 1] + dL[ii];
        l_u.add(scaLinearOneD(uu[ii - 1], uu[ii], ll[ii - 1], ll[ii]));
      }

      //        logC()
      //          << "sumL = " << sumL << std::endl
      //          << logMe::dtFormat("%16s %16s %16s %16s %16s %16s => %16s")
      //            % "u_n" % "l-l_0" % "dL" % "gg" % "(l-l_0)/sum(l)"
      //            % "gg-(l-l_0)/sum(l)" % "u_n+1"
      //          << std::endl;
      dtReal sumEps = 0;
      dtReal maxEps = std::numeric_limits<dtReal>::min();
      dt__forFromToIndex(1, nP - 1, ii)
      {
        //          logC()
        //            << logMe::dtFormat(
        //              "%16.10e %16.10e %16.10e %16.10e %16.10e %16.10e =>
        //              %16.10e"
        //            )
        //              % uu[ii] % ll[ii] % dL[ii] % gg[ii] % (ll[ii]/sumL)
        //              % (gg[ii]-(ll[ii]/sumL)) % l_u.invYFloat( gg[ii] * sumL
        //              )
        //            << std::endl;
        sumEps = sumEps + fabs(gg[ii] - (ll[ii] / sumL));
        maxEps = std::max(maxEps, std::fabs<dtReal>(gg[ii] - (ll[ii] / sumL)));
        uu[ii] = l_u.invYFloat(gg[ii] * sumL);
      }
      dtReal tol = config().lookupDef<dtReal>("_tolerance", 1.E-08);
      logC() << logMe::dtFormat(
                  "[ %3i ] sumEps = %12.5e maxEps = %12.5e (threshold = %12.5e)"
                ) %
                  smoothIt % sumEps % maxEps % tol
             << std::endl;

      if (maxEps < tol)
        break;
    }

    //
    // add mesh vertices
    //

    dt__forFromToIndex(1, nP - 1, ii)
    {
      dtPoint3 pp(m1d->getPoint(uu[ii]));

      dtge->mesh_vertices.push_back(
        new ::MEdgeVertex(pp.x(), pp.y(), pp.z(), dtge, uu[ii])
      );
    }

    //
    // add mesh elements
    //
    dtge->addLine(new ::MLine(
      dtge->getBeginVertex()->mesh_vertices[0], dtge->mesh_vertices.front()
    ));
    dt__forFromToIndex(1, dtge->mesh_vertices.size(), ii)
    {
      dtge->addLine(
        new ::MLine(dtge->mesh_vertices[ii - 1], dtge->mesh_vertices[ii])
      );
    }
    dtge->addLine(new ::MLine(
      dtge->mesh_vertices.back(), dtge->getEndVertex()->mesh_vertices[0]
    ));
    dtge->meshStatistics.status = ::GEntity::MeshGenerationStatus::DONE;
  }
  else
    dtMeshGEdge::operator()(dtge);
}
} // namespace dtOO
