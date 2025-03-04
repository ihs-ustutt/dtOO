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

#include "dtMeshOperatorFactory.h"

#include "dtMeshAndOrientGEdge.h"
#include "dtMeshAndOrientGFace.h"
#include "dtMeshCustom2x3TransfiniteGFace.h"
#include "dtMeshFreeGradingGEdge.h"
#include "dtMeshGEdge.h"
#include "dtMeshGFace.h"
#include "dtMeshGFaceWithTransfiniteLayer.h"
#include "dtMeshGRegion.h"
#include "dtMeshGRegionTetgen.h"
#include "dtMeshGRegionWithBoundaryLayer.h"
#include "dtMeshGRegionWithOneLayer.h"
#include "dtMeshGVertex.h"
#include "dtMeshLaplacianGFace.h"
#include "dtMeshOperator.h"
#include "dtMeshTransfiniteGFace.h"
#include "dtMeshTransfiniteRecombineSelectionGFace.h"
#include "dtOptimizeMeshGRegion.h"
#include <logMe/logMe.h>

namespace dtOO {
dt__pVH(dtMeshOperator) dtMeshOperatorFactory::_product;

dtMeshOperatorFactory::dtMeshOperatorFactory() {}

dtMeshOperatorFactory::~dtMeshOperatorFactory() {}

bool dtMeshOperatorFactory::registrate(dtMeshOperator const *const reg)
{
  dt__forAllRefAuto(_product, aProd)
  {
    if (aProd.virtualClassName() == reg->virtualClassName())
    {
      return false;
    }
  }
  _product.push_back(reg->create());

  return true;
}

dtMeshOperator *dtMeshOperatorFactory::create(char const *const str)
{
  return create(std::string(str));
}

dtMeshOperator *dtMeshOperatorFactory::create(std::string const str)
{
  dt__forAllRefAuto(_product, aProd)
  {
    //
    // check virtual class name
    //
    if (aProd.virtualClassName() == str)
    {
      return aProd.create();
    }

    //
    // check alias
    //
    dt__forAllRefAuto(aProd.factoryAlias(), anAlias)
    {
      if (anAlias == str)
        return aProd.create();
    }
  }

  std::vector<std::string> av;
  dt__forAllRefAuto(_product, aProd)
  {
    av.push_back(aProd.virtualClassName());
    dt__forAllRefAuto(aProd.factoryAlias(), anAlias)
    {
      av.push_back("  -> " + anAlias);
    }
  }
  dt__throw(
    create(),
    << str << " could not be created." << std::endl
    << "Implemented builder:" << std::endl
    << logMe::vecToString(av, 1) << std::endl
  );
}
} // namespace dtOO
