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

#include "customGmsh.h"
#include "xmlHeaven/dtXmlParserBase.h"

#include "boundedVolumeFactory.h"
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <logMe/logMe.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshRegion.h>

namespace dtOO {
bool customGmsh::_registrated =
  boundedVolumeFactory::registrate(dt__tmpPtr(customGmsh, new customGmsh()));

customGmsh::customGmsh() : gmshBoundedVolume() {}

customGmsh::~customGmsh() {}

void customGmsh::init(
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

  dt__forAllRefAuto(qtXmlPrimitive::getChildVector(element), anEl)
  {
    //
    // analyticGeometry - faces
    //
    if (qtXmlPrimitive::is("analyticGeometry", anEl))
    {
      handleAnalyticGeometry(anEl, bC, cV, aF, aG, bV);
    }
    //
    // customRegions
    //
    else if (qtXmlPrimitive::is("customRegion", anEl))
    {
      handleCustomRegion(anEl, bC, cV, aF, aG, bV);
    }
  }
}

dtInt customGmsh::handleCustomFace(
  ::QDomElement const &element,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV
)
{
  vectorHandling<map1dTo3d *> out_edges;
  vectorHandling<map1dTo3d *> in_edges;

  dt__forAllRefAuto(
    qtXmlPrimitive::getChildVector(
      "analyticGeometry", qtXmlPrimitive::getChild("edgeLoop", element)
    ),
    aEdgeLabel
  )
  {
    out_edges.push_back(map1dTo3d::MustDownCast(
      dtXmlParserBase::createAnalyticGeometry(aEdgeLabel, bC, cV, aF, aG)
    ));
  }

  dt__pH(map2dTo3d) theFace;
  theFace.reset(map2dTo3d::MustDownCast(dtXmlParserBase::createAnalyticGeometry(
    qtXmlPrimitive::getChild(
      "analyticGeometry", qtXmlPrimitive::getChild("face", element)
    ),
    bC,
    cV,
    aF,
    aG
  )));

  //
  // handle internal edges
  //
  if (qtXmlPrimitive::hasChild("internalEdgeLoop", element))
  {
    dt__forAllRefAuto(
      qtXmlPrimitive::getChildVector("internalEdgeLoop", element), iEdgeElement
    )
    {
      dt__forAllRefAuto(
        qtXmlPrimitive::getChildVector("analyticGeometry", iEdgeElement),
        aEdgeLabel
      )
      {
        in_edges.push_back(map1dTo3d::MustDownCast(
          dtXmlParserBase::createAnalyticGeometry(aEdgeLabel, bC, cV, aF, aG)
        ));
      }
    }
  }
  dtInt fTag = handleCustomFace(theFace.get(), out_edges, in_edges);

  out_edges.destroy();
  in_edges.destroy();

  return fTag;
}

dtInt customGmsh::handleCustomFace(
  map2dTo3d const *const face,
  vectorHandling<map1dTo3d *> const &oedges,
  vectorHandling<map1dTo3d *> const &iedges
)
{
  //
  // add outter edges
  //
  std::list<::GEdge *> edges;
  std::vector<dtInt> ori;
  dt__forAllRefAuto(oedges, oedge)
  {
    dtInt tag = _gm->addIfEdgeToGmshModel(oedge);
    if (tag < 0)
    {
      ori.push_back(-1);
      tag = -tag;
    }
    else
    {
      ori.push_back(1);
    }
    edges.push_back(_gm->getEdgeByTag(tag));
  }
  //
  // add internal edges
  //
  std::vector<::GEdge *> iedgesL;
  dt__forAllRefAuto(iedges, iedge)
  {
    iedgesL.push_back(_gm->getEdgeByTag(_gm->addIfEdgeToGmshModel(iedge)));
  }
  //
  // add face
  //
  dtInt ftag;
  _gm->addIfFaceToGmshModel(face, &ftag, edges, ori);

  //
  // create edge loop of internal edges
  //
  ::GEdgeLoop el(iedgesL);
  for (::GEdgeLoop::citer it = el.begin(); it != el.end(); ++it)
  {
    _gm->getDtGmshFaceByTag(ftag)->addEdge(it->getEdge(), it->getSign());
  }

  return ftag;
}

dtInt customGmsh::handleCustomRegion(
  ::QDomElement const &element,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV
)
{
  std::list<::GFace *> faces;
  dt__forAllRefAuto(qtXmlPrimitive::getChildVector(element), anEl)
  {
    if (qtXmlPrimitive::is("analyticGeometry", anEl))
    {
      faces.push_back(
        _gm->getFaceByTag(handleAnalyticGeometry(anEl, bC, cV, aF, aG, bV))
      );
    }
    else if (qtXmlPrimitive::is("customFace", anEl))
    {
      faces.push_back(
        _gm->getFaceByTag(handleCustomFace(anEl, bC, cV, aF, aG, bV))
      );
    }
    else
      dt__throwUnexpected(handleCustomRegion());
  }
  //
  // make a region
  //
  dtInt tag = 0;
  _gm->addIfRegionToGmshModel(
    NULL, &tag, faces, std::vector<dtInt>(faces.size(), 1)
  );

  return tag;
}

dtInt customGmsh::handleAnalyticGeometry(
  ::QDomElement const &element,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV
)
{
  dt__pH(analyticGeometry) thisAG;
  thisAG.reset(dtXmlParserBase::createAnalyticGeometry(element, bC, cV, aF, aG)
  );
  dtInt thisId = 0;
  _gm->addIfToGmshModel(thisAG.get(), &thisId);
  return thisId;
}
} // namespace dtOO
