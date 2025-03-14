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

#include "dtMeshTransfiniteGFace.h"
#include "dtMeshGFace.h"
#include "progHelper.h"
#include <xmlHeaven/dtXmlParser.h>

#include <analyticGeometryHeaven/map2dTo3d.h>
#include <gmsh/GmshDefines.h>
#include <gmsh/meshGFace.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshModel.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <xmlHeaven/qtXmlPrimitive.h>

#include "dtMeshOperatorFactory.h"
#include <analyticFunctionHeaven/scaLinearOneD.h>
#include <analyticFunctionHeaven/scaMultiOneD.h>

/*
   s4 +-----c3-----+ s3
      |            |
      |            |
     c4            c2
      |            |
      |            |
   s1 +-----c1-----+ s2
*/

// f(u,v) = (1-u) c4(v) + u c2(v) + (1-v) c1(u) + v c3(u)
//          - [ (1-u)(1-v) s1 + u(1-v) s2 + uv s3 + (1-u)v s4 ]
#define TRAN_QUA(c1, c2, c3, c4, s1, s2, s3, s4, u, v)                         \
  (1. - u) * c4 + u *c2 + (1. - v) * c1 + v *c3 -                              \
    ((1. - u) * (1. - v) * s1 + u * (1. - v) * s2 + u * v * s3 +               \
     (1. - u) * v * s4)

namespace dtOO {
bool dtMeshTransfiniteGFace::_registrated = dtMeshOperatorFactory::registrate(
  dt__tmpPtr(dtMeshTransfiniteGFace, new dtMeshTransfiniteGFace())
);

dtMeshTransfiniteGFace::dtMeshTransfiniteGFace() : dtMeshTransfinite2DOperator()
{
}

dtMeshTransfiniteGFace::dtMeshTransfiniteGFace(
  const dtMeshTransfiniteGFace &orig
)
  : dtMeshTransfinite2DOperator(orig)
{
  _vCorrPos = orig._vCorrPos;
  _vCorrSteps = orig._vCorrSteps;
}

dtMeshTransfiniteGFace::~dtMeshTransfiniteGFace() {}

void dtMeshTransfiniteGFace::init(
  ::QDomElement const &element,
  baseContainer const *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV,
  lvH_dtMeshOperator const *const mO
)
{
  dtMeshTransfinite2DOperator::init(element, bC, cV, aF, aG, bV, mO);

  _vCorrPos = std::vector<dtReal>(0);
  _vCorrSteps = 0;

  if (dtXmlParser::hasAttribute("v_correction_position", element))
  {
    _vCorrPos = dtXmlParser::getAttributeFloatVectorMuParse(
      "v_correction_position", element, cV, aF
    );
  }
  if (dtXmlParser::hasAttribute("v_correction_steps", element))
  {
    _vCorrSteps = dtXmlParser::getAttributeIntMuParse(
      "v_correction_steps", element, cV, aF
    );
  }
}

void dtMeshTransfiniteGFace::operator()(dtGmshFace *dtgf)
{

  dtgf->model()->setCurrentMeshEntity(dtgf);

  if (dtgf->meshAttributes.method != MESH_TRANSFINITE)
  {
    dtMeshGFace()(dtgf);
    return;
  }

  // destroy the mesh if it exists
  deMeshGFace()(dtgf);
  Msg::Info(
    "Meshing surface %d / %s ( dtMeshTransfiniteGFace )",
    dtgf->tag(),
    dtgf->getPhysicalString().c_str()
  );

  //
  // reparam vertices
  //
  twoDArrayHandling<dtPoint2> pUV =
    dtMeshTransfinite2DOperator::computeEdgeLoops(dtgf);
  std::vector<std::vector<::MVertex *>> &tab = dtgf->transfinite_vertices;

  //
  // get array size and set boundaries
  //
  /*             k
      2L+H +------------+ L+H
           |            |
           |            |
         l |            | j
           |            |
     2L+2H +------------+
           0     i      L
  */
  dtInt L = pUV.size(0) - 1;
  dtInt H = pUV.size(1) - 1;
  double UC1 = pUV[0][0].x();
  double UC2 = pUV[L][0].x();
  double UC3 = pUV[L][H].x();
  double UC4 = pUV[0][H].x();
  double VC1 = pUV[0][0].y();
  double VC2 = pUV[L][0].y();
  double VC3 = pUV[L][H].y();
  double VC4 = pUV[0][H].y();

  //
  // boundary length
  //
  std::vector<double> lengths_i(L + 1, 0.);
  std::vector<double> lengths_j(H + 1, 0.);
  double L_i = 0.;
  double L_j = 0.;
  for (int i = 0; i < L; i++)
  {
    MVertex *v1 = tab[i][0];
    MVertex *v2 = tab[i + 1][0];
    L_i += v1->distance(v2);
    lengths_i[i + 1] = L_i;
  }
  for (int i = 0; i < H; i++)
  {
    MVertex *v1 = tab[0][i];
    MVertex *v2 = tab[0][i + 1];
    L_j += v1->distance(v2);
    lengths_j[i + 1] = L_j;
  }

  for (int i = 1; i < L; i++)
  {
    for (int j = 1; j < H; j++)
    {
      double u = lengths_i[i] / L_i;
      double v = lengths_j[j] / L_j;
      double Up = TRAN_QUA(
        pUV[i][0].x(),
        pUV[L][j].x(),
        pUV[i][H].x(),
        pUV[0][j].x(),
        UC1,
        UC2,
        UC3,
        UC4,
        u,
        v
      );
      double Vp = TRAN_QUA(
        pUV[i][0].y(),
        pUV[L][j].y(),
        pUV[i][H].y(),
        pUV[0][j].y(),
        VC1,
        VC2,
        VC3,
        VC4,
        u,
        v
      );
      pUV[i][j] = dtPoint2(Up, Vp);
    }
  }

  //
  // correction at constant v
  //
  dt__forAllRefAuto(_vCorrPos, constV)
  {
    dtInt pos = static_cast<dtInt>(constV * H);
    pUV = correctConstV(dtgf, pUV, pos, _vCorrSteps, lengths_i, L_i);
  }
  if (!_vCorrPos.empty())
  {
    pUV = linearInterpolateU(pUV, 0, static_cast<dtInt>(_vCorrPos.front() * H));

    dt__forFromToIndex(1, _vCorrPos.size(), ii)
    {
      pUV = linearInterpolateU(
        pUV,
        static_cast<dtInt>(_vCorrPos[ii - 1] * H),
        static_cast<dtInt>(_vCorrPos[ii] * H)
      );
    }

    pUV = linearInterpolateU(pUV, static_cast<dtInt>(_vCorrPos.back() * H), H);
  }

  for (int i = 1; i < L; i++)
  {
    for (int j = 1; j < H; j++)
    {
      GPoint gp = dtgf->point(pUV[i][j].x(), pUV[i][j].y());
      tab[i][j] = new MFaceVertex(
        gp.x(), gp.y(), gp.z(), dtgf, pUV[i][j].x(), pUV[i][j].y()
      );
      dtgf->mesh_vertices.push_back(tab[i][j]);
    }
  }

  dtMeshTransfinite2DOperator::createTransfiniteElements(dtgf);

  dtgf->meshStatistics.status = GFace::DONE;
}

twoDArrayHandling<dtPoint2> dtMeshTransfiniteGFace::correctConstV(
  dtGmshFace const *const dtgf,
  twoDArrayHandling<dtPoint2> pUV,
  dtInt const &pos,
  dtInt const &nSteps,
  std::vector<double> lengths_i,
  double const &L_i
) const
{
  dtInt L = pUV.size(0) - 1;
  dtInt H = pUV.size(1) - 1;

  logContainer<dtMeshTransfiniteGFace> logC(logDEBUG, "correctConstV()");

  std::vector<dtReal> uu(L + 1);
  std::vector<dtReal> vv(L + 1);
  std::vector<dtPoint3> p3_u(L + 1);

  dt__forFromToIndex(0, L + 1, ii)
  {
    uu[ii] = pUV[ii][pos].x();
    vv[ii] = pUV[ii][pos].y();
    p3_u[ii] = dtgf->getMap2dTo3d()->getPoint(uu[ii], vv[ii]);
  }

  //
  // sort in ascending order
  //
  bool mustInvert = false;
  if (uu[0] > uu[1])
  {
    mustInvert = true;
    progHelper::reverse(uu);
    progHelper::reverse(vv);
    progHelper::reverse(p3_u);
    dt__forAllIndex(lengths_i, anL)
    {
      lengths_i[anL] = lengths_i.back() - lengths_i[anL];
    }
    progHelper::reverse(lengths_i);
  }

  dt__forFromToIndex(0, _vCorrSteps, smoothIt)
  {
    std::vector<dtReal> dL(L + 1, 0.);
    dt__forFromToIndex(1, L + 1, ii)
    {
      p3_u[ii] = dtgf->getMap2dTo3d()->getPoint(uu[ii], vv[ii]);
      dL[ii] = dtLinearAlgebra::length(p3_u[ii] - p3_u[ii - 1]);
    }

    dtReal sumL = dtLinearAlgebra::sum(dL);
    std::vector<dtReal> ll(L + 1, 0.);
    scaMultiOneD<scaLinearOneD> l_u;
    dt__forFromToIndex(1, L, ii)
    {
      ll[ii] = ll[ii - 1] + dL[ii];
      l_u.add(scaLinearOneD(uu[ii - 1], uu[ii], ll[ii - 1], ll[ii]));
    }

    dtReal sumEps = 0;
    dtReal maxEps = std::numeric_limits<dtReal>::min();
    dt__forFromToIndex(1, L, ii)
    {
      sumEps = sumEps + fabs(lengths_i[ii] / L_i - (ll[ii] / sumL));
      maxEps = std::max(
        maxEps, std::fabs<dtReal>(lengths_i[ii] / L_i - (ll[ii] / sumL))
      );
      uu[ii] = l_u.invYFloat(lengths_i[ii] / L_i * sumL);
    }

    logC() << logMe::dtFormat("[ %3i ] sumEps = %12.5e maxEps = %12.5e") %
                smoothIt % sumEps % maxEps
           << std::endl;
  }

  //
  // revert
  //
  if (mustInvert)
  {
    progHelper::reverse(uu);
    progHelper::reverse(vv);
  }

  dt__forFromToIndex(1, L, ii) { pUV[ii][pos] = dtPoint2(uu[ii], vv[ii]); }

  return pUV;
}

twoDArrayHandling<dtPoint2> dtMeshTransfiniteGFace::linearInterpolateU(
  twoDArrayHandling<dtPoint2> pUV, dtInt const &vStart, dtInt const &vEnd
)
{
  dtReal theStep = 1. / static_cast<dtReal>(vEnd - vStart);
  dtReal cStep = 0.;
  dt__forFromToIndex(vStart + 1, vEnd, vI)
  {
    cStep = cStep + theStep;

    dt__forInnerIndex(pUV.fixJ(vI), ii)
    {
      pUV[ii][vI] = dtPoint2(
        pUV[ii][vStart].x() + cStep * (pUV[ii][vEnd].x() - pUV[ii][vStart].x()),
        pUV[ii][vI].y()
      );
    }
  }

  return pUV;
}

} // namespace dtOO
