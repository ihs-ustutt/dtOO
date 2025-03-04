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

#ifndef vec3dMuParserOneD_H
#define vec3dMuParserOneD_H

#include <dtOOTypeDef.h>

#include "vec3dOneD.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <vector>

namespace dtOO {
class dtTransformer;
class dtMuParser;

class vec3dMuParserOneD : public vec3dOneD {
public:
  dt__class(vec3dMuParserOneD, analyticFunction);
  vec3dMuParserOneD();
  vec3dMuParserOneD(vec3dMuParserOneD const &orig);
  vec3dMuParserOneD(std::string const expression, std::string const argOne);
  vec3dMuParserOneD *clone(void) const;
  virtual vec3dMuParserOneD *cloneTransformed(dtTransformer const *const dtT
  ) const;
  vec3dMuParserOneD *create(void) const;
  virtual ~vec3dMuParserOneD();
  virtual aFY Y(aFX const &xx) const;
  virtual bool closed(dtInt const &dir) const;

private:
  dt__pH(dtMuParser) _parser;
  dt__pVH(double) _arg;
  std::string _expressionStr;
  std::string _argStr[1];
};
} // namespace dtOO
#endif /* vec3dMuParserOneD_H */
