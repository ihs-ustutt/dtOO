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

#ifndef VEC3DMUPARSERTWOD_H
#define VEC3DMUPARSERTWOD_H

#include <dtOOTypeDef.h>

#include "vec3dTwoD.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <vector>

namespace dtOO {
class dtTransformer;
class dtMuParser;

class vec3dMuParserTwoD : public vec3dTwoD {
public:
  dt__class(vec3dMuParserTwoD, analyticFunction);
  vec3dMuParserTwoD();
  vec3dMuParserTwoD(vec3dMuParserTwoD const &orig);
  vec3dMuParserTwoD(
    std::string const expression,
    std::string const argOne,
    std::string const argTwo
  );
  vec3dMuParserTwoD *clone(void) const;
  virtual vec3dMuParserTwoD *cloneTransformed(dtTransformer const *const dtT
  ) const;
  vec3dMuParserTwoD *create(void) const;
  virtual ~vec3dMuParserTwoD();
  virtual aFY Y(aFX const &xx) const;
  virtual bool closed(dtInt const &dir) const;

private:
  dt__pH(dtMuParser) _parser;
  dt__pVH(double) _arg;
  std::string _expressionStr;
  std::string _argStr[2];
};
} // namespace dtOO
#endif /* VEC3DMUPARSERTWOD_H */
