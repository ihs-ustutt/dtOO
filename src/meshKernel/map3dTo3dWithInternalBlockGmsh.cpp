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

#include "map3dTo3dWithInternalBlockGmsh.h"

#include "boundedVolumeFactory.h"
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <logMe/logMe.h>
#include <meshEngine/dtGmshEdge.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshRegion.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <xmlHeaven/qtXmlBase.h>

namespace dtOO {
bool map3dTo3dWithInternalBlockGmsh::_registrated =
  boundedVolumeFactory::registrate(dt__tmpPtr(
    map3dTo3dWithInternalBlockGmsh, new map3dTo3dWithInternalBlockGmsh()
  ));

map3dTo3dWithInternalBlockGmsh::map3dTo3dWithInternalBlockGmsh()
  : gmshBoundedVolume()
{
}

map3dTo3dWithInternalBlockGmsh::~map3dTo3dWithInternalBlockGmsh() {}

void map3dTo3dWithInternalBlockGmsh::init(
  ::QDomElement const &element,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV
)
{
  //
  // init gmshBoundedVolume
  //
  gmshBoundedVolume::init(element, bC, cV, aF, aG, bV);

  //
  // get geometries
  //
  // 1) first element should be the outer channel
  // 2) next elements should be the internal blocks surrounding the
  //    internal surface
  //
  std::vector<::QDomElement> wElement =
    qtXmlPrimitive::getChildVector("analyticGeometry", element);
  dt__forAllIter(std::vector<::QDomElement>, wElement, it)
  {
    //
    // get analyticGeometry, cast and store in region vector
    //
    dt__pH(analyticGeometry) aG_t(dtXmlParserBase::createAnalyticGeometry(
      *it, (baseContainer *)bC, cV, aF, aG
    ));
    dt__ptrAss(
      map3dTo3d const *const mm3d, map3dTo3d::ConstDownCast(aG_t.get())
    );

    if (mm3d && !_m3d)
      _m3d.reset(mm3d->clone());
    else
      _internal.push_back(mm3d->clone());
  }

  //
  // set current model
  //
  ::GModel::setCurrent(_gm);

  //
  // add all faces of outer channel
  //
  std::vector<dtInt> rId;
  rId.push_back(1);
  _gm->addIfRegionToGmshModel(_m3d.get(), &(rId.back()));

  dtGmshRegion *const &volume = _gm->getDtGmshRegionByTag(1);
  dtGmshFace *const &hub = _gm->getDtGmshFaceByTag(1);
  dtGmshFace *const &shroud = _gm->getDtGmshFaceByTag(2);

  //
  // internal
  //
  dt__forAllIter(dt__pVH(map3dTo3d), _internal, it)
  {
    map3dTo3d const &theRegion = *it;
    rId.push_back(1);
    _gm->addIfRegionToGmshModel(&theRegion, &(rId.back()));
    //
    // create internal faces and edges
    //
    dt__pH(map1dTo3d)
      iE0(theRegion.segmentPercent(dtPoint3(0, 1, 0), dtPoint3(1, 1, 0)));
    dt__pH(map1dTo3d)
      iE1(theRegion.segmentPercent(dtPoint3(0, 1, 1), dtPoint3(1, 1, 1)));

    //
    // internal edges (boundary edges between channel and internal blocks)
    //
    int iE0Tag;
    int iE1Tag;
    _gm->addIfEdgeToGmshModel(iE0.get(), &iE0Tag);
    hub->addEdge(_gm->getDtGmshEdgeByTag(iE0Tag), 1);
    _gm->addIfEdgeToGmshModel(iE1.get(), &iE1Tag);
    shroud->addEdge(_gm->getDtGmshEdgeByTag(iE1Tag), 1);

    //
    // internal faces (boundary faces between channel and internal blocks)
    //
    dtInt iFTag;
    dt__pH(map2dTo3d) iF(theRegion.segmentPercent(
      dtPoint3(0, 1, 0), dtPoint3(1, 1, 0), dtPoint3(1, 1, 1), dtPoint3(0, 1, 1)
    ));
    _gm->addIfFaceToGmshModel(iF.get(), &iFTag);
    volume->addFace(_gm->getDtGmshFaceByTag(iFTag), 1);
  }

  //
  // correct transfinite surfaces and create region
  //
  std::list<::GFace *> ff = _gm->faces();
  dt__forAllIter(std::list<::GFace *>, ff, it)
  {
    dtGmshModel::cast2DtGmshFace(*it)->correctIfTransfinite();
  }
}
} // namespace dtOO
