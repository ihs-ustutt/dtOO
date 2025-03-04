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

#include "dtMeshCustom2x3TransfiniteGFace.h"
#include "dtMeshGFace.h"
#include <progHelper.h>

#include <analyticGeometryHeaven/map2dTo3d.h>
#include <gmsh/Context.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/MTriangle.h>
#include <gmsh/meshGFace.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshModel.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <xmlHeaven/qtXmlPrimitive.h>

#include "dtMeshOperatorFactory.h"
#include <analyticFunctionHeaven/scaOneD.h>

namespace dtOO {
bool dtMeshCustom2x3TransfiniteGFace::_registrated =
  dtMeshOperatorFactory::registrate(dt__tmpPtr(
    dtMeshCustom2x3TransfiniteGFace, new dtMeshCustom2x3TransfiniteGFace()
  ));

dtMeshCustom2x3TransfiniteGFace::dtMeshCustom2x3TransfiniteGFace()
  : dtMeshTransfinite2DOperator()
{
}

dtMeshCustom2x3TransfiniteGFace::dtMeshCustom2x3TransfiniteGFace(
  const dtMeshCustom2x3TransfiniteGFace &orig
)
  : dtMeshTransfinite2DOperator(orig)
{
}

dtMeshCustom2x3TransfiniteGFace::~dtMeshCustom2x3TransfiniteGFace() {}

void dtMeshCustom2x3TransfiniteGFace::jInit(
  jsonPrimitive const &jE,
  baseContainer const *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV,
  lvH_dtMeshOperator const *const mO
)
{
  dtMesh2DOperator::jInit(jE, bC, cV, aF, aG, bV, mO);

  _alpha_1 = scaOneD::PointerDownCast(
    config().lookupClone<analyticFunction>("_alpha_1", aF)
  );
  _alpha_2 = scaOneD::PointerDownCast(
    config().lookupClone<analyticFunction>("_alpha_2", aF)
  );
  _beta_1 = scaOneD::PointerDownCast(
    config().lookupClone<analyticFunction>("_beta_1", aF)
  );
  _beta_2 = scaOneD::PointerDownCast(
    config().lookupClone<analyticFunction>("_beta_2", aF)
  );
  _beta_3 = scaOneD::PointerDownCast(
    config().lookupClone<analyticFunction>("_beta_3", aF)
  );
}

void dtMeshCustom2x3TransfiniteGFace::init(
  ::QDomElement const &element,
  baseContainer const *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV,
  lvH_dtMeshOperator const *const mO
)
{
  dtMesh2DOperator::init(element, bC, cV, aF, aG, bV, mO);

  jsonPrimitive jE;
  jE.append(
    "_alpha_1",
    analyticFunction::ConstDownCast(
      aF->get(qtXmlBase::getAttributeStr("alpha_1", element))
    )
  );
  jE.append(
    "_alpha_2",
    analyticFunction::ConstDownCast(
      aF->get(qtXmlBase::getAttributeStr("alpha_2", element))
    )
  );
  jE.append(
    "_beta_1",
    analyticFunction::ConstDownCast(
      aF->get(qtXmlBase::getAttributeStr("beta_1", element))
    )
  );
  jE.append(
    "_beta_2",
    analyticFunction::ConstDownCast(
      aF->get(qtXmlBase::getAttributeStr("beta_2", element))
    )
  );
  jE.append(
    "_beta_3",
    analyticFunction::ConstDownCast(
      aF->get(qtXmlBase::getAttributeStr("beta_3", element))
    )
  );
  dtMeshCustom2x3TransfiniteGFace::jInit(jE, bC, cV, aF, aG, bV, mO);
}

void dtMeshCustom2x3TransfiniteGFace::operator()(dtGmshFace *dtgf)
{

  dtgf->model()->setCurrentMeshEntity(dtgf);

  dt__throwIf(dtgf->meshAttributes.method != MESH_TRANSFINITE, operator());

  // destroy the mesh if it exists
  deMeshGFace()(dtgf);

  Msg::Info(
    "Meshing surface %d ( dtMeshCustom2x3TransfiniteGFace )", dtgf->tag()
  );

  //
  //    /*             k
  //        2L+H +------------+ L+H
  //             |            |
  //             |            |
  //           l |            | j
  //             |            |
  //       2L+2H +------------+
  //             0     i      L
  //    */

  std::vector<std::vector<MVertex *>> &tab(dtgf->transfinite_vertices);
  twoDArrayHandling<dtPoint2> pUV =
    dtMeshTransfinite2DOperator::computeEdgeLoops(dtgf);
  dtInt L = pUV.size(0) - 1;
  dtInt H = pUV.size(1) - 1;
  twoDArrayHandling<dtVector3> nXYZ(L + 1, H + 1, dtVector3(0, 0, 0));
  twoDArrayHandling<dtVector2> nUV(L + 1, H + 1, dtVector2(0, 0));
  twoDArrayHandling<dtVector2> dF(L + 1, H + 1, dtVector2(0, 0));
  twoDArrayHandling<dtVector2> ddF(L + 1, H + 1, dtVector2(0, 0));

  for (int i = 1; i < L; i++)
  {
    nXYZ[i][0] = dtLinearAlgebra::normalize(dtLinearAlgebra::crossProduct(
                   dtGmshModel::extractPosition(tab[i + 1][0]) -
                     dtGmshModel::extractPosition(tab[i][0]),
                   dtgf->getMap2dTo3d()->normal(pUV[i][0])
                 )) +
                 dtLinearAlgebra::normalize(dtLinearAlgebra::crossProduct(
                   dtGmshModel::extractPosition(tab[i][0]) -
                     dtGmshModel::extractPosition(tab[i - 1][0]),
                   dtgf->getMap2dTo3d()->normal(pUV[i][0])
                 ));
    nXYZ[i][H] = dtLinearAlgebra::normalize(dtLinearAlgebra::crossProduct(
                   dtGmshModel::extractPosition(tab[i + 1][H]) -
                     dtGmshModel::extractPosition(tab[i][H]),
                   dtgf->getMap2dTo3d()->normal(pUV[i][H])
                 )) +
                 dtLinearAlgebra::normalize(dtLinearAlgebra::crossProduct(
                   dtGmshModel::extractPosition(tab[i][H]) -
                     dtGmshModel::extractPosition(tab[i - 1][H]),
                   dtgf->getMap2dTo3d()->normal(pUV[i][H])
                 ));
  }
  for (int i = 1; i < H; i++)
  {
    nXYZ[L][i] = dtLinearAlgebra::normalize(dtLinearAlgebra::crossProduct(
                   dtGmshModel::extractPosition(tab[L][i + 1]) -
                     dtGmshModel::extractPosition(tab[L][i]),
                   dtgf->getMap2dTo3d()->normal(pUV[L][i])
                 )) +
                 dtLinearAlgebra::normalize(dtLinearAlgebra::crossProduct(
                   dtGmshModel::extractPosition(tab[L][i]) -
                     dtGmshModel::extractPosition(tab[L][i - 1]),
                   dtgf->getMap2dTo3d()->normal(pUV[L][i])
                 ));
    nXYZ[0][i] = dtLinearAlgebra::normalize(dtLinearAlgebra::crossProduct(
                   dtGmshModel::extractPosition(tab[0][i + 1]) -
                     dtGmshModel::extractPosition(tab[0][i]),
                   dtgf->getMap2dTo3d()->normal(pUV[0][i])
                 )) +
                 dtLinearAlgebra::normalize(dtLinearAlgebra::crossProduct(
                   dtGmshModel::extractPosition(tab[0][i]) -
                     dtGmshModel::extractPosition(tab[0][i - 1]),
                   dtgf->getMap2dTo3d()->normal(pUV[0][i])
                 ));
  }

  dtReal d_0_1 = (tab[0][0]->distance(tab[0][1]));
  dtReal d_0_2 = (tab[L][0]->distance(tab[L][1]));
  dtReal d_H_1 = (tab[0][H]->distance(tab[0][H - 1]));
  dtReal d_H_2 = (tab[L][H]->distance(tab[L][H - 1]));

  for (int i = 0; i < L + 1; i++)
  {
    dtReal iF = static_cast<dtReal>(i);
    nUV[i][0] = dtLinearAlgebra::toDtVector2(dtLinearAlgebra::solveMatrix(
      dtgf->getMap2dTo3d()->jacobi(pUV[i][0]),
      dtLinearAlgebra::createMatrixVector(
        -(d_0_1 + (iF / L) * (d_0_2 - d_0_1)) *
        dtLinearAlgebra::normalize(nXYZ[i][0])
      )
    ));
    // nUV[i][0] = dtVector2(1./L, nUV[i][0].y());
    nUV[i][H] = dtLinearAlgebra::toDtVector2(dtLinearAlgebra::solveMatrix(
      dtgf->getMap2dTo3d()->jacobi(pUV[i][H]),
      dtLinearAlgebra::createMatrixVector(
        -(d_H_1 + (iF / L) * (d_H_2 - d_H_1)) *
        dtLinearAlgebra::normalize(nXYZ[i][H])
      )
    ));
  }
  dtReal e_0_1 = (tab[1][0]->distance(tab[0][0]));
  dtReal e_0_2 = (tab[L][0]->distance(tab[L - 1][0]));
  dtReal e_L_1 = (tab[1][H]->distance(tab[0][H]));
  dtReal e_L_2 = (tab[L][H]->distance(tab[L - 1][H]));

  for (int i = 0; i < H + 1; i++)
  {
    dtReal iF = static_cast<dtReal>(i);
    nUV[0][i] = dtLinearAlgebra::toDtVector2(dtLinearAlgebra::solveMatrix(
      dtgf->getMap2dTo3d()->jacobi(pUV[0][i]),
      dtLinearAlgebra::createMatrixVector(
        (e_0_1 + (iF / H) * (e_0_2 - e_0_1)) *
        dtLinearAlgebra::normalize(nXYZ[0][i])
      )
    ));
    nUV[L][i] = dtLinearAlgebra::toDtVector2(dtLinearAlgebra::solveMatrix(
      dtgf->getMap2dTo3d()->jacobi(pUV[L][i]),
      dtLinearAlgebra::createMatrixVector(
        (e_L_1 + (iF / H) * (e_L_2 - e_L_1)) *
        dtLinearAlgebra::normalize(nXYZ[L][i])
      )
    ));
  }
  nUV[0][0] = .5 * (nUV[1][0] + nUV[0][1]);
  nUV[L][0] = .5 * (nUV[L][1] + nUV[L - 1][0]);
  nUV[L][H] = .5 * (nUV[L][H - 1] + nUV[L - 1][H]);
  nUV[0][H] = .5 * (nUV[1][H] + nUV[0][H - 1]);
  for (int i = 1; i < L; i++)
  {
    ddF[i][0] = (nUV[i + 1][0] - nUV[i - 1][0]) / (2. / L);
    ddF[i][H] = (nUV[i + 1][H] - nUV[i - 1][H]) / (2. / L);
  }
  for (int i = 1; i < H; i++)
  {
    ddF[0][i] = (nUV[0][i + 1] - nUV[0][i - 1]) / (2. / H);
    ddF[L][i] = (nUV[L][i + 1] - nUV[L][i - 1]) / (2. / H);
  }
  // create points using transfinite interpolation
  for (int i = 1; i < L; i++)
  {
    for (int j = 1; j < H; j++)
    {
      dtReal iF = static_cast<dtReal>(i);
      dtReal jF = static_cast<dtReal>(j);

      double psi = iF / (L);
      double eps = jF / (H);
      dtReal alpha_1 = _alpha_1->YFloat(psi);
      dtReal alpha_2 = _alpha_2->YFloat(psi);

      dtReal beta_1 = _beta_1->YFloat(eps);
      dtReal beta_2 = _beta_2->YFloat(eps);
      dtReal beta_3 = _beta_3->YFloat(eps);

      dtVector2 P_u = alpha_1 * dtLinearAlgebra::toDtVector2(pUV[0][j]) +
                      alpha_2 * dtLinearAlgebra::toDtVector2(pUV[L][j]);
      dtVector2 P_v = beta_1 * dtLinearAlgebra::toDtVector2(pUV[i][0]) +
                      beta_2 * nUV[i][0] +
                      beta_3 * dtLinearAlgebra::toDtVector2(pUV[i][H]);
      dtVector2 P_uP_v =
        alpha_1 * beta_1 * dtLinearAlgebra::toDtVector2(pUV[0][0]) +
        alpha_1 * beta_2 * nUV[0][0] +
        alpha_1 * beta_3 * dtLinearAlgebra::toDtVector2(pUV[0][H]) +
        alpha_2 * beta_1 * dtLinearAlgebra::toDtVector2(pUV[L][0]) +
        alpha_2 * beta_2 * nUV[L][0] +
        alpha_2 * beta_3 * dtLinearAlgebra::toDtVector2(pUV[L][H]);

      GPoint gp = dtgf->point(
        P_u.x() + P_v.x() - P_uP_v.x(), P_u.y() + P_v.y() - P_uP_v.y()
      );

      pUV[i][j] = dtPoint2(0, 0) + P_u + P_v - P_uP_v;
      MFaceVertex *newv = new MFaceVertex(
        gp.x(),
        gp.y(),
        gp.z(),
        dtgf,
        P_u.x() + P_v.x() - P_uP_v.x(),
        P_u.y() + P_v.y() - P_uP_v.y()
      );
      dtgf->mesh_vertices.push_back(newv);
      tab[i][j] = newv;
    }
  }

  dtMeshTransfinite2DOperator::createTransfiniteElements(dtgf);

  dtgf->meshStatistics.status = GFace::DONE;
}
} // namespace dtOO
