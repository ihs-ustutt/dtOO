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

#include "dtMeshTransfiniteRecombineSelectionGFace.h"

#include "dtGmshFace.h"
#include "dtMeshOperatorFactory.h"
#include <gmsh/MQuadrangle.h>
#include <gmsh/MTriangle.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <xmlHeaven/qtXmlPrimitive.h>

namespace dtOO {
bool dtMeshTransfiniteRecombineSelectionGFace::_registrated =
  dtMeshOperatorFactory::registrate(dt__tmpPtr(
    dtMeshTransfiniteRecombineSelectionGFace,
    new dtMeshTransfiniteRecombineSelectionGFace()
  ));

dtMeshTransfiniteRecombineSelectionGFace ::
  dtMeshTransfiniteRecombineSelectionGFace()
  : dtMeshGFace()
{
}

dtMeshTransfiniteRecombineSelectionGFace ::
  dtMeshTransfiniteRecombineSelectionGFace(
    const dtMeshTransfiniteRecombineSelectionGFace &orig
  )
  : dtMeshGFace(orig)
{
}

void dtMeshTransfiniteRecombineSelectionGFace::init(
  ::QDomElement const &element,
  baseContainer const *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV,
  lvH_dtMeshOperator const *const mO
)
{
  dtMeshGFace::init(element, bC, cV, aF, aG, bV, mO);

  //
  // transfinite arrangement
  //
  _transfiniteArrangement = dtXmlParserBase::getAttributeIntMuParse(
    "transfiniteArrangement", element, cV, aF
  );

  std::vector<std::string> recombineRule =
    qtXmlPrimitive::getAttributeStrVector("recombineRule", element);

  //
  // parse recombine rule and create map
  // set "empty" to -1
  //
  dt__forAllIndex(recombineRule, ii)
  {
    recombineRule[ii] =
      dtXmlParserBase::replaceDependencies(recombineRule[ii], cV, aF, aG);
    if (qtXmlBase::stringContains("empty", recombineRule[ii]))
    {
      recombineRule[ii] =
        qtXmlBase::replaceStringInString("empty", "-1", recombineRule[ii]);
    }
    std::vector<dtInt> const aRule =
      dtXmlParserBase::muParseCSStringInt(recombineRule[ii]);

    _recombineRule[std::pair<int, dtInt>(aRule[0], aRule[1])] = true;
  }
}

dtMeshTransfiniteRecombineSelectionGFace ::
  ~dtMeshTransfiniteRecombineSelectionGFace()
{
}

void dtMeshTransfiniteRecombineSelectionGFace::operator()(dtGmshFace *dtgf)
{
  dt__throwIf(dtgf->meshAttributes.method != MESH_TRANSFINITE, operator());

  dtMeshGFace::operator()(dtgf);

  std::vector<::MTriangle *> &tri(dtgf->triangles);
  std::vector<::MQuadrangle *> &quad(dtgf->quadrangles);
  std::vector<std::vector<MVertex *>> &transVert(dtgf->transfinite_vertices);

  dt__info(
    operator(),
    << "Destroy " << tri.size() << " triangles" << std::endl
    << "Destroy " << quad.size() << " quadrangles"
  );

  dt__forAllIter(std::vector<::MTriangle *>, tri, it) delete (*it);
  tri.clear();
  dt__forAllIter(std::vector<::MQuadrangle *>, quad, it) delete (*it);
  quad.clear();

  for (int i = 0; i < (transVert.size() - 1); i++)
  {
    for (int j = 0; j < (transVert[i].size() - 1); j++)
    {
      std::vector<::MVertex *> v(4, NULL);
      v[0] = transVert[i][j];
      v[1] = transVert[i + 1][j];
      v[2] = transVert[i + 1][j + 1];
      v[3] = transVert[i][j + 1];

      //
      // check if there is a rule for this element
      //
      bool createQuad = (_recombineRule.find(std::pair<int, dtInt>(-1, -1)) !=
                         _recombineRule.end()) ||
                        (_recombineRule.find(std::pair<int, dtInt>(-1, j)) !=
                         _recombineRule.end()) ||
                        (_recombineRule.find(std::pair<int, dtInt>(i, -1)) !=
                         _recombineRule.end()) ||
                        (_recombineRule.find(std::pair<int, dtInt>(i, j)) !=
                         _recombineRule.end());

      if (createQuad)
      {
        quad.push_back(new ::MQuadrangle(v[0], v[1], v[2], v[3]));
      }
      else
      {
        if (_transfiniteArrangement == 1 ||
            (_transfiniteArrangement == 2 &&
             ((i % 2 == 0 && j % 2 == 1) || (i % 2 == 1 && j % 2 == 0))) ||
            (_transfiniteArrangement == -2 &&
             ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1))))
        {
          tri.push_back(new MTriangle(v[0], v[1], v[2]));
          tri.push_back(new MTriangle(v[2], v[3], v[0]));
        }
        else
        {
          tri.push_back(new MTriangle(v[0], v[1], v[3]));
          tri.push_back(new MTriangle(v[3], v[1], v[2]));
        }

        //          tri.push_back(new MTriangle(v[0], v[1], v[2]));
        //          tri.push_back(new MTriangle(v[2], v[3], v[0]));
      }
    }
  }

  //
  // mark as unstructured mesh
  //
  dtgf->meshAttributes.method = MESH_UNSTRUCTURED;
}
} // namespace dtOO
