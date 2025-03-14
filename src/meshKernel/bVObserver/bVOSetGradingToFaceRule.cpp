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

#include "bVOSetGradingToFaceRule.h"

#include "bVOInterfaceFactory.h"
#include <boundedVolume.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <math.h>
#include <meshEngine/dtGmshEdge.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshRegion.h>
#include <progHelper.h>
#include <vector>
#include <xmlHeaven/dtXmlParserBase.h>

namespace dtOO {
bool bVOSetGradingToFaceRule::_registrated = bVOInterfaceFactory::registrate(
  dt__tmpPtr(bVOSetGradingToFaceRule, new bVOSetGradingToFaceRule())
);

bVOSetGradingToFaceRule::bVOSetGradingToFaceRule() {}

bVOSetGradingToFaceRule::~bVOSetGradingToFaceRule() {}

void bVOSetGradingToFaceRule::init(
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

  //    <bVObserver
  //      name="bVOSetGradingToFaceRule"
  //  		rule="
  //        {:aS_skinCorpus_dtIn*::3::-1:}
  //			"
  //     />

  dt__info(init(), << dtXmlParserBase::convertToString(element));
  jsonPrimitive jE;
  jE.append<std::vector<std::string>>(
    "_rule", dtXmlParserBase::getAttributeStrVector("rule", element)
  );
  bVOInterface::jInit(jE, bC, cV, aF, aG, bV, attachTo);
}

void bVOSetGradingToFaceRule::preUpdate(void)
{
  dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());

  //
  // set current model
  //
  ::GModel::setCurrent(gm);

  logContainer<bVOSetGradingToFaceRule> logC(logINFO, "preUpdate");
  dt__forAllRefAuto(config().lookup<std::vector<std::string>>("_rule"), aRule)
  {
    //
    // create components
    //
    std::vector<std::string> comp =
      dtXmlParserBase::convertToStringVector(":", ":", aRule);
    dt__throwIf(comp.size() != 4, preUpdate());

    //
    // assign components
    //
    std::string const &fStr0 = comp[0];
    std::string const &fStr1 = comp[1];
    dtReal const &fType = stringPrimitive::stringToFloat(comp[2]);
    dtReal const &fGrading = stringPrimitive::stringToFloat(comp[3]);

    //
    // output
    //
    logC() << aRule << std::endl
           << ">" << fStr0 << "< -> >" << fStr1 << "< : " << fType << " / "
           << fGrading << std::endl;

    //
    // loop all edges
    //
    dt__forAllRefAuto(gm->dtEdges(), anEdge)
    {
      bool breakEdge = false;
      //
      // fStr0 / beginVertex -> endVertex / fStr1
      //
      dt__forAllRefAuto(anEdge->getBeginVertex()->faces(), aFaceStart)
      {
        if (breakEdge)
          break;
        if (gm->matchWildCardPhysical(fStr0, aFaceStart))
        {
          dt__forAllRefAuto(anEdge->getEndVertex()->faces(), aFaceEnd)
          {
            if (gm->matchWildCardPhysical(fStr1, aFaceEnd))
            {
              logC() << "  edge[ tag = " << anEdge->tag() << " ] : ( "
                     << gm->getPhysicalString(aFaceStart) << " ) -> ( "
                     << gm->getPhysicalString(aFaceEnd) << " )" << std::endl;
              anEdge->setGrading(fGrading, fType);
              breakEdge = true;
              break;
            }
          }
        }
      }

      //
      // fStr1 / beginVertex -> endVertex / fStr0
      //
      dt__forAllRefAuto(anEdge->getBeginVertex()->faces(), aFaceStart)
      {
        if (breakEdge)
          break;
        if (gm->matchWildCardPhysical(fStr1, aFaceStart))
        {
          dt__forAllRefAuto(anEdge->getEndVertex()->faces(), aFaceEnd)
          {
            if (gm->matchWildCardPhysical(fStr0, aFaceEnd))
            {

              logC() << "  reverse edge[ tag = " << anEdge->tag() << " ] : ( "
                     << gm->getPhysicalString(aFaceStart) << " ) -> ( "
                     << gm->getPhysicalString(aFaceEnd) << " )" << std::endl;
              anEdge->setGrading(-fGrading, fType);
              breakEdge = true;
              break;
            }
          }
        }
      }
    }
  }
}
} // namespace dtOO
