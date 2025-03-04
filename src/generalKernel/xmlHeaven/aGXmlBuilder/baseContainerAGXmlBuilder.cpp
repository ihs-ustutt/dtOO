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

#include "baseContainerAGXmlBuilder.h"

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>
#include <baseContainerHeaven/baseContainer.h>
#include <baseContainerHeaven/transformerContainer.h>
#include <dtLinearAlgebra.h>
#include <dtTransformerHeaven/dtTransformer.h>
#include <logMe/logMe.h>
#include <xmlHeaven/aGXmlBuilderFactory.h>

namespace dtOO {
bool baseContainerAGXmlBuilder::_registrated = aGXmlBuilderFactory::registrate(
  dt__tmpPtr(baseContainerAGXmlBuilder, new baseContainerAGXmlBuilder())
);

baseContainerAGXmlBuilder::baseContainerAGXmlBuilder() {}

baseContainerAGXmlBuilder::~baseContainerAGXmlBuilder() {}

void baseContainerAGXmlBuilder::buildPart(
  ::QDomElement const &toBuild,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_analyticGeometry *result
) const
{

  std::vector<::QDomElement> wElement =
    dtXmlParserBase::getChildVector(toBuild);

  // dt__FORALL(wElement, ii,
  for (int ii = 0; ii < wElement.size(); ii++)
  {
    if (dtXmlParserBase::is("Point_3", wElement[ii]))
    {
      vectorHandling<dtPoint3> workingPointP;
      dtXmlParserBase::createBasic(
        &wElement[ii], bC, cV, aF, aG, NULL, &workingPointP
      );
    }
    else if (dtXmlParserBase::is("Vector_3", wElement[ii]))
    {
      dtVector3 workingVectorP =
        dtXmlParserBase::createDtVector3(&wElement[ii], bC, cV, aF, aG);
    }
    else if (dtXmlParserBase::is("transformer", wElement[ii]))
    {
      dt__pH(dtTransformer)
        dtT(dtXmlParserBase::createTransformer(&wElement[ii], bC, cV, aF, aG));
    }
  }
}
} // namespace dtOO
