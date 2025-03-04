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

#include "vec3dMuParserThreeD.h"

#include <geometryEngine/dtMuParser.h>
#include <logMe/logMe.h>

#include "analyticFunctionTransformed.h"

namespace dtOO {
vec3dMuParserThreeD::vec3dMuParserThreeD() : vec3dThreeD() {}

vec3dMuParserThreeD::vec3dMuParserThreeD(vec3dMuParserThreeD const &orig)
  : vec3dThreeD(orig), _parser(new dtMuParser())
{

  for (int ii = 0; ii < 3; ii++)
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
    for (int ii = 0; ii < 3; ii++)
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
    dt__throw(vec3dMuParserThreeD(), << e.GetMsg());
  }
}

vec3dMuParserThreeD::vec3dMuParserThreeD(
  std::string const expression,
  std::string const argOne,
  std::string const argTwo,
  std::string const argThree
)
  : vec3dThreeD(), _parser(new dtMuParser())
{

  for (int ii = 0; ii < 3; ii++)
  {
    _arg.push_back(new double(0.));
  }
  _argStr[0] = argOne;
  _argStr[1] = argTwo;
  _argStr[2] = argThree;
  _expressionStr = expression;

  try
  {
    //
    // create parser
    //
    for (int ii = 0; ii < 3; ii++)
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
    dt__throw(vec3dMuParserThreeD(), << e.GetMsg());
  }
}

vec3dMuParserThreeD::~vec3dMuParserThreeD() {}

aFY vec3dMuParserThreeD::Y(aFX const &xx) const
{
  dt__throwIf(xx.size() != 3, Y());
  aFY yy(3, 0.);

  try
  {
    for (int ii = 0; ii < 3; ii++)
    {
      const_cast<double &>(_arg[ii]) = static_cast<double>(xx[ii]);
    }
    int nDim;
    double *yyD = _parser->Eval(nDim);
    dt__throwIf(nDim != 3, Y());
    for (int ii = 0; ii < 3; ii++)
    {
      yy[ii] = static_cast<dtReal>(yyD[ii]);
    }
  } catch (mu::Parser::exception_type &e)
  {
    dt__throw(Y(), << e.GetMsg());
  }
  return yy;
}

bool vec3dMuParserThreeD::closed(dtInt const &dir) const
{
  dt__throwIf((dir != 0) && (dir != 1) && (dir != 2), closed);
  return false;
}

vec3dMuParserThreeD *vec3dMuParserThreeD::clone(void) const
{
  return new vec3dMuParserThreeD(*this);
}

vec3dMuParserThreeD *
vec3dMuParserThreeD::cloneTransformed(dtTransformer const *const dtT) const
{
  return new analyticFunctionTransformed<vec3dMuParserThreeD>(*this, dtT);
}

vec3dMuParserThreeD *vec3dMuParserThreeD::create(void) const
{
  return new vec3dMuParserThreeD();
}
} // namespace dtOO
