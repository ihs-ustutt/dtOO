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

#include "biThicknessIncreasing.h"
#include "dtTransformerFactory.h"
#include "thicknessIncreasing.h"
#include <algorithm>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/scaOneD.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <analyticFunctionHeaven/vec2dOneD.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

namespace dtOO {
bool biThicknessIncreasing::_registrated = dtTransformerFactory::registrate(
  dt__tmpPtr(biThicknessIncreasing, new biThicknessIncreasing())
);

biThicknessIncreasing::biThicknessIncreasing() : dtTransformer()
{
  _tD[0] = NULL;
  _tD[1] = NULL;
  _para = NULL;
  _nPointsOne = 0;
  _splineOrder = 0;
}

biThicknessIncreasing::~biThicknessIncreasing() {}

dtTransformer *biThicknessIncreasing::clone(void) const
{
  dt__throw(clone(), << "Not yet implemented.");
}

dtTransformer *biThicknessIncreasing::create(void) const
{
  return new biThicknessIncreasing();
}

lvH_analyticFunction
biThicknessIncreasing::apply(lvH_analyticFunction const *const sFunP) const
{
  lvH_analyticFunction transSFun;
  for (int ii = 0; ii < sFunP->size(); ii++)
  {
    std::vector<dtPoint2> p2;
    std::vector<dtPoint2> p2Inv;
    //
    // cast pointer and check if it is a analyticFunction
    //
    dt__ptrAss(
      vec2dOneD const *const theF, vec2dOneD::ConstDownCast(sFunP->at(ii))
    );

    std::vector<dtReal> itVal;
    std::vector<std::string> header;
    header.push_back("Y_x");
    header.push_back("Y_y");
    header.push_back("N_x");
    header.push_back("N_y");
    header.push_back("tt");
    header.push_back("ttInv");
    header.push_back("L_i/L");
    header.push_back("p2_x");
    header.push_back("p2_y");
    header.push_back("p2Inv_x");
    header.push_back("p2Inv_y");

    //
    // thickness increasing
    //
    dtReal cLength = theF->length();
    dt__info(apply(), << dt__eval(cLength));
    dtReal xMin = _para->xMin(0);
    dtReal xMax = _para->xMax(0);
    //
    // points
    //
    for (int jj = 0; jj < _nPointsOne; jj++)
    {
      dt__toFloat(dtReal jjF, jj);
      dt__toFloat(dtReal nPointsOneF, _nPointsOne);
      dtReal paraOne =
        _para->YFloat(jjF * ((xMax - xMin) / (nPointsOneF - 1.)));
      dtPoint2 YY = theF->YdtPoint2Percent(paraOne);
      dtVector2 NN = theF->unitNdtVector2Percent(paraOne);
      dtReal curLength = theF->length(theF->x_percent(paraOne));
      dtReal tt = _tD[0]->YFloat(curLength / cLength);
      dtReal ttInv = _tD[1]->YFloat(curLength / cLength);
      p2.push_back(YY + tt * NN);
      p2Inv.push_back(YY - ttInv * NN);
      itVal.push_back(YY.x());
      itVal.push_back(YY.y());
      itVal.push_back(NN.x());
      itVal.push_back(NN.y());
      itVal.push_back(tt);
      itVal.push_back(ttInv);
      itVal.push_back(curLength / cLength);
      itVal.push_back(p2[jj].x());
      itVal.push_back(p2[jj].y());
      itVal.push_back(p2Inv[jj].x());
      itVal.push_back(p2Inv[jj].y());
    }
    dt__debug(apply(), << logMe::vecToTable(header, itVal));
    //
    // reverse orientation of resulting splineCurve
    //
    if (_reverse)
    {
      std::reverse(p2.begin(), p2.end());
      std::reverse(p2Inv.begin(), p2Inv.end());
    }
    //
    // create new function
    //
    std::reverse(p2Inv.begin(), p2Inv.end());

    //
    // remove
    //
    dtReal uvRes =
      staticPropertiesHandler::getInstance()->getOptionFloat("uvw_resolution");
    if (dtLinearAlgebra::distance(p2.back(), p2Inv.front()) < uvRes)
    {
      dt__info(
        apply(),
        << dt__point2d(p2.back()) << std::endl
        << dt__point2d(p2Inv.front()) << std::endl
        << dt__eval(dtLinearAlgebra::distance(p2.back(), p2Inv.front()))
        << std::endl
        << dt__eval(uvRes) << std::endl
        << "Removing duplicate point."
      );
      p2.pop_back();
    }
    std::vector<dtPoint2> p2All(p2.size() + p2Inv.size());
    int counter = 0;
    dt__forAllIndex(p2, ii)
    {
      p2All[counter] = p2[ii];
      counter++;
    }
    dt__forAllIndex(p2Inv, ii)
    {
      p2All[counter] = p2Inv[ii];
      counter++;
    }
    if (dtLinearAlgebra::distance(p2All.front(), p2Inv.back()) < uvRes)
    {
      dt__info(
        apply(),
        << dt__point2d(p2All.back()) << std::endl
        << dt__point2d(p2Inv.back()) << std::endl
        << dt__eval(dtLinearAlgebra::distance(p2All.front(), p2Inv.back()))
        << std::endl
        << dt__eval(uvRes) << std::endl
        << "Closing spline."
      );
      p2All.push_back(p2All.front());
    }
    else
    {
      dt__info(
        apply(),
        << dt__point2d(p2All.back()) << std::endl
        << dt__point2d(p2Inv.back()) << std::endl
        << dt__eval(dtLinearAlgebra::distance(p2All.front(), p2Inv.back()))
        << std::endl
        << dt__eval(uvRes) << std::endl
        << "Open spline."
      );
    }

    ptrHandling<dtCurve2d> dtC2d(
      bSplineCurve2d_pointConstructOCC(p2All, _splineOrder).result()
    );

    //
    // create scaCurve2dOneD
    //
    transSFun.push_back(new vec2dCurve2dOneD(dtC2d.get()));
    transSFun.back()->setLabel(sFunP->at(ii)->getLabel());
  }
  return transSFun;
}

void biThicknessIncreasing::init(
  ::QDomElement const *tE,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG
)
{
  dtTransformer::init(tE, bC, cV, aF, aG);

  handleInt("order", dtXmlParserBase::getAttributeInt("order", *tE));
  handleBool("reverse", dtXmlParserBase::getAttributeBool("reverse", *tE));

  //
  // get functions
  //
  handleAnalyticFunction(
    "function_label",
    aF->get(dtXmlParserBase::getAttributeStr("function_label", *tE))
  );
  handleAnalyticFunction(
    "function_label_inverted",
    aF->get(dtXmlParserBase::getAttributeStr("function_label_inverted", *tE))
  );
  handleAnalyticFunction(
    "parameter_one_percent_function",
    aF->get(
      dtXmlParserBase::getAttributeStr("parameter_one_percent_function", *tE)
    )
  );

  handleInt(
    "number_points_one",
    dtXmlParserBase::getAttributeIntMuParse("number_points_one", *tE, cV, aF)
  );
}

bool biThicknessIncreasing::isNecessary(void) const { return true; }

void biThicknessIncreasing::handleInt(std::string const name, dtInt const value)
{
  if (name == "order")
  {
    _splineOrder = value;
    return;
  }
  else if (name == "number_points_one")
  {
    _nPointsOne = value;
    return;
  }
  dtTransformer::handleInt(name, value);
}

void biThicknessIncreasing::handleBool(std::string const name, bool const value)
{
  if (name == "reverse")
  {
    _reverse = value;
    return;
  }
  dtTransformer::handleBool(name, value);
}

void biThicknessIncreasing::handleAnalyticFunction(
  std::string const name, analyticFunction const *value
)
{
  if (name == "function_label")
  {
    dt__ptrAss(_tD[0], scaOneD::ConstDownCast(value));
    return;
  }
  if (name == "function_label_inverted")
  {
    dt__ptrAss(_tD[1], scaOneD::ConstDownCast(value));
    return;
  }
  else if (name == "parameter_one_percent_function")
  {
    dt__ptrAss(_para, scaOneD::ConstDownCast(value));
    return;
  }
  dtTransformer::handleAnalyticFunction(name, value);
}
} // namespace dtOO
