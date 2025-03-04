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

#include "geomVolume_multipleBoundedVolumeAGXmlBuilder.h"

#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/multipleBoundedVolume.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <dtLinearAlgebra.h>
#include <logMe/logMe.h>
#include <xmlHeaven/aGXmlBuilderFactory.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
bool geomVolume_multipleBoundedVolumeAGXmlBuilder::_registrated =
  aGXmlBuilderFactory::registrate(dt__tmpPtr(
    geomVolume_multipleBoundedVolumeAGXmlBuilder,
    new geomVolume_multipleBoundedVolumeAGXmlBuilder()
  ));

geomVolume_multipleBoundedVolumeAGXmlBuilder ::
  geomVolume_multipleBoundedVolumeAGXmlBuilder()
{
}

geomVolume_multipleBoundedVolumeAGXmlBuilder ::
  ~geomVolume_multipleBoundedVolumeAGXmlBuilder()
{
}

void geomVolume_multipleBoundedVolumeAGXmlBuilder::buildPart(
  ::QDomElement const &toBuild,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_analyticGeometry *result
) const
{
  dt__throwIf(
    !dtXmlParserBase::hasChild("analyticGeometry", toBuild), buildPart()
  );

  dt__pH(analyticGeometry) m3d;
  dt__pVH(analyticGeometry) m2d;
  dt__forAllRefAuto(
    dtXmlParserBase::getChildVector("analyticGeometry", toBuild), anEl
  )
  {
    dt__pH(analyticGeometry)
      aG_t(dtXmlParserBase::createAnalyticGeometry(anEl, bC, cV, aF, aG));
    if (aG_t->dim() == 3)
    {
      m3d.reset(aG_t->clone());
    }
    else if (aG_t->dim() == 2)
    {
      m2d.push_back(aG_t->clone());
    }
    else
      dt__throwUnexpected(buildPart());
  }

  result->push_back(new multipleBoundedVolume(m3d.get(), m2d));
}
} // namespace dtOO
