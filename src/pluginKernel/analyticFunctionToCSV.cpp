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

#include "analyticFunctionToCSV.h"

#include "dtPluginFactory.h"
#include <analyticFunctionHeaven/aFBuilder/dtPoint3_vec3dTwoD.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec3dTwoD.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <baseContainerHeaven/baseContainer.h>
#include <boundedVolume.h>
#include <constValueHeaven/constValue.h>
#include <dtCase.h>
#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParser.h>

namespace dtOO {
bool analyticFunctionToCSV::_registrated = dtPluginFactory::registrate(
  dt__tmpPtr(analyticFunctionToCSV, new analyticFunctionToCSV())
);

analyticFunctionToCSV::analyticFunctionToCSV() {}

analyticFunctionToCSV::~analyticFunctionToCSV() {}

void analyticFunctionToCSV::init(
  ::QDomElement const &element,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV,
  lvH_dtCase const *const dC,
  lvH_dtPlugin const *const pL
)
{
  dtPlugin::init(element, bC, cV, aF, aG, bV, dC, pL);
  //	<plugin
  //		name="analyticFunctionToCSV"
  //		label="analyticFunctionToCSV"
  //    numPoints={u}{v}{w}
  //	>
  //  <analyticFunction label="fun1"/>
  //  <case label="myCase"/>
  //  </plugin>
  //
  // get case
  //
  _case = NULL;
  if (dtXmlParser::hasChild("case", element))
  {
    _case = dC->get(dtXmlParser::getAttributeStr(
      "label", dtXmlParser::getChild("case", element)
    ));
    _parser = dtXmlParser::ptr();
  }

  //
  // get number of points
  //
  _nP = dtXmlParser::getAttributeIntVectorMuParse("numPoints", element, cV, aF);

  //
  // get analyticFunctions
  //
  dt__forAllRefAuto(
    dtXmlParser::getChildVector("analyticFunction", element), anEl
  )
  {
    _aF.push_back(aF->get(dtXmlParser::getAttributeStr("label", anEl)));
  }
}

void analyticFunctionToCSV::apply(void)
{
  dt__forAllRefAuto(_aF, theF)
  {
    //
    // open file
    //
    std::string filename;
    if (_case)
    {
      filename = _case->getDirectory(_parser->currentState()) + "/" +
                 theF->getLabel() + ".csv";
    }
    else
    {
      filename = _parser->currentState() + "_" + theF->getLabel() + ".csv";
    }
    dt__info(apply(), << "Write to " << filename);
    std::fstream of;
    of.open(filename.c_str(), std::ios::out | std::ios::trunc);

    //
    // write header
    //
    of << logMe::dtFormat("# xDim %16i") % theF->xDim() << std::endl
       << logMe::dtFormat("# yDim %16i") % theF->yDim() << std::endl
       << logMe::dtFormat("# nP %16i %16i %16i") % _nP[0] % _nP[1] % _nP[2]
       << std::endl;

    //
    // (u, v) -> (x, y, z)
    //
    if (vec3dTwoD::ConstDownCast(theF))
    {
      twoDArrayHandling<dtPoint3> grid =
        dtPoint3_vec3dTwoD(vec3dTwoD::ConstSecureCast(theF), _nP[0], _nP[1])
          .result();
      dt__forFromToIndex(0, grid[0].size(), jj)
      {
        dt__forFromToIndex(0, grid.size(), ii)
        {
          dtPoint3 const &ref = grid[ii][jj];
          of << logMe::dtFormat("%16.8e %16.8e %16.8e") % ref.x() % ref.y() %
                  ref.z()
             << std::endl;
        }
        //
        // blockwise gnuplot selection
        //
        of << "#" << std::endl;
        of << "#" << std::endl;
      }
    }
    else
      dt__throwUnexpected(apply());
    //
    // close file
    //
    of.close();
  }
}
} // namespace dtOO
