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

#include "scaMuParserThreeD.h"

#include <geometryEngine/dtMuParser.h>
#include <logMe/logMe.h>

#include "analyticFunctionTransformed.h"

namespace dtOO {
scaMuParserThreeD::scaMuParserThreeD() : scaThreeD() {}

scaMuParserThreeD::scaMuParserThreeD(scaMuParserThreeD const &orig)
  : scaThreeD(orig), _parser(new dtMuParser())
{
  dt__forFromToIndex(0, 3, ii)
  {
    _arg.push_back(new double(0.));
    _argStr[ii] = orig._argStr[ii];
  }
  _expressionStr = orig._expressionStr;

  try
  {
    //
    // create parser
    //
    dt__forFromToIndex(0, 3, ii)
    {
      _parser->DefineVar(_argStr[ii].c_str(), &(_arg[ii]));
    }
    _parser->SetExpr(_expressionStr.c_str());
  }
  //
  // error handling
  //
  catch (mu::Parser::exception_type &e)
  {
    dt__throw(scaMuParserThreeD(), << e.GetMsg());
  }
}

scaMuParserThreeD::scaMuParserThreeD(
  std::string const expression,
  std::string const argOne,
  std::string const argTwo,
  std::string const argThree
)
  : scaThreeD(), _parser(new dtMuParser())
{
  dt__forFromToIndex(0, 3, ii) _arg.push_back(new double(0.));
  _argStr[0] = argOne;
  _argStr[1] = argTwo;
  _argStr[2] = argThree;
  _expressionStr = expression;

  try
  {
    //
    // create parser
    //
    dt__forFromToIndex(0, 3, ii)
    {
      _parser->DefineVar(_argStr[ii].c_str(), &(_arg[ii]));
    }
    _parser->SetExpr(_expressionStr.c_str());
  }
  //
  // error handling
  //
  catch (mu::Parser::exception_type &e)
  {
    dt__throw(scaMuParserThreeD(), << e.GetMsg());
  }
}

scaMuParserThreeD::~scaMuParserThreeD() {}

dtReal scaMuParserThreeD::YFloat(
  dtReal const &x0, dtReal const &x1, dtReal const &x2
) const
{
  dtReal yy;

  try
  {
    const_cast<double &>(_arg[0]) = static_cast<double>(x0);
    const_cast<double &>(_arg[1]) = static_cast<double>(x1);
    const_cast<double &>(_arg[2]) = static_cast<double>(x2);

    int nDim;
    double *yyD = _parser->Eval(nDim);
    dt__throwIf(nDim != 1, Y());
    yy = static_cast<dtReal>(yyD[0]);
  } catch (mu::Parser::exception_type &e)
  {
    dt__throw(Y(), << e.GetMsg());
  }
  return yy;
}

bool scaMuParserThreeD::closed(dtInt const &dir) const
{
  dt__throwIf((dir != 0) && (dir != 1) && (dir != 2), closed());
  return false;
}

scaMuParserThreeD *scaMuParserThreeD::clone(void) const
{
  return new scaMuParserThreeD(*this);
}

scaMuParserThreeD *
scaMuParserThreeD::cloneTransformed(dtTransformer const *const dtT) const
{
  return new analyticFunctionTransformed<scaMuParserThreeD>(*this, dtT);
}

scaMuParserThreeD *scaMuParserThreeD::create(void) const
{
  return new scaMuParserThreeD();
}
} // namespace dtOO
