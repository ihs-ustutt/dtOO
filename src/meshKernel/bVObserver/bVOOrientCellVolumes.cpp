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

#include "bVOOrientCellVolumes.h"

#include "bVOInterfaceFactory.h"
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <constValueHeaven/constValue.h>
#include <gmsh/MElement.h>
#include <logMe/logMe.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshRegion.h>
#include <xmlHeaven/qtXmlBase.h>

namespace dtOO {
bool bVOOrientCellVolumes::_registrated = bVOInterfaceFactory::registrate(
  dt__tmpPtr(bVOOrientCellVolumes, new bVOOrientCellVolumes())
);

bVOOrientCellVolumes::bVOOrientCellVolumes() {}

bVOOrientCellVolumes::~bVOOrientCellVolumes() {}

void bVOOrientCellVolumes::init(
  ::QDomElement const &element,
  baseContainer const *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV,
  boundedVolume *attachTo
)
{
  //
  // init bVOInterface
  //
  bVOInterface::init(element, bC, cV, aF, aG, bV, attachTo);

  //		<bVObserver name="bVOOrientCellVolumes" positive="true"/>
  jsonPrimitive jE;
  jE.append<bool>(
    "_positive", qtXmlBase::getAttributeBool("positive", element)
  );
  bVOInterface::jInit(jE, bC, cV, aF, aG, bV, attachTo);
}

void bVOOrientCellVolumes::postUpdate(void)
{
  dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());

  //
  // set current model
  //
  ::GModel::setCurrent(gm);

  //
  // counter
  //
  dtInt gCount = 0;
  dtInt pCount = 0;
  dtInt nCount = 0;

  //
  // revert element if necessary
  //
  std::list<GRegion *> reg = gm->regions();
  dt__forAllConstIter(std::list<GRegion *>, reg, it)
  {
    if (config().lookup<bool>("_positive"))
    {
      dt__forFromToIndex(0, (*it)->getNumMeshElements(), ii)
      {
        gCount++;
        if ((*it)->getMeshElement(ii)->getVolume() < 0.)
        {
          nCount++;
          (*it)->getMeshElement(ii)->reverse();
        }
        else
        {
          pCount++;
        }
      }
    }
    else
    {
      dt__forFromToIndex(0, (*it)->getNumMeshElements(), ii)
      {
        gCount++;
        if ((*it)->getMeshElement(ii)->getVolume() > 0.)
        {
          pCount++;
          (*it)->getMeshElement(ii)->reverse();
        }
        else
        {
          nCount++;
        }
      }
    }
  }
  dt__info(
    postUpdate(),
    << "_positive = " << config().lookup<bool>("_positive") << std::endl
    << "Number of cells: " << gCount << std::endl
    << "Positive number of cells: " << pCount << std::endl
    << "Negative number of cells: " << nCount
  );
}
} // namespace dtOO
