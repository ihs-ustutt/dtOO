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

#include "analyticGeometryToCSV.h"

#include "dtPluginFactory.h"
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/aGBuilder/dtPoint3_map1dTo3dPoint.h>
#include <analyticGeometryHeaven/aGBuilder/dtPoint3_map2dTo3dPoint.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <baseContainerHeaven/baseContainer.h>
#include <boundedVolume.h>
#include <constValueHeaven/constValue.h>
#include <dtCase.h>
#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParser.h>

namespace dtOO {
bool analyticGeometryToCSV::_registrated = dtPluginFactory::registrate(
  dt__tmpPtr(analyticGeometryToCSV, new analyticGeometryToCSV())
);

analyticGeometryToCSV::analyticGeometryToCSV() {}

analyticGeometryToCSV::~analyticGeometryToCSV() {}

void analyticGeometryToCSV::init(
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
  //		name="analyticGeometryToCSV"
  //		label="analyticGeometryToCSV"
  //    numPoints={u}{v}{w}
  //	>
  //    <analyticGeometry label="fun1"/>
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
    dtXmlParser::getChildVector("analyticGeometry", element), anEl
  )
  {
    _aG.push_back(aG->get(dtXmlParser::getAttributeStr("label", anEl)));
  }
}

void analyticGeometryToCSV::apply(void)
{
  dt__forAllRefAuto(_aG, theG)
  {
    //
    // open file
    //
    std::string filename;
    if (_case)
    {
      filename = _case->getDirectory(_parser->currentState()) + "/" +
                 theG->getLabel() + ".csv";
    }
    else
    {
      filename = _parser->currentState() + "_" + theG->getLabel() + ".csv";
    }

    dt__info(apply(), << "Write to " << filename);
    std::fstream of;
    of.open(filename.c_str(), std::ios::out | std::ios::trunc);

    //
    // write header
    //
    of << logMe::dtFormat("# dim %16i") % theG->dim() << std::endl
       << logMe::dtFormat("# nP %16i %16i %16i") % _nP[0] % _nP[1] % _nP[2]
       << std::endl;

    //
    // ( u ) -> (x, y, z)
    //
    if (map1dTo3d::ConstDownCast(theG))
    {
      std::vector<dtPoint3> grid =
        dtPoint3_map1dTo3dPoint(map1dTo3d::ConstSecureCast(theG), _nP[0])
          .result();
      dt__forFromToIndex(0, grid.size(), ii)
      {
        dtPoint3 const &ref = grid[ii];
        of << logMe::dtFormat("%16.8e %16.8e %16.8e") % ref.x() % ref.y() %
                ref.z()
           << std::endl;
      }
    }
    //
    // ( u, v ) -> (x, y, z)
    //
    else if (map2dTo3d::ConstDownCast(theG))
    {
      twoDArrayHandling<dtPoint3> grid =
        dtPoint3_map2dTo3dPoint(
          map2dTo3d::ConstSecureCast(theG), _nP[0], _nP[1]
        )
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
