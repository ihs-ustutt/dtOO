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

#ifndef SCAMUPARSERONED_H
#define SCAMUPARSERONED_H

#include <dtOOTypeDef.h>

#include "scaOneD.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <vector>

namespace dtOO {
class dtTransformer;
class dtMuParser;
//! Scalar `muParser` function with one function argument.
/*! Binding to `muParser` library. The function is created via a `string`
 *  expression.
 */
class scaMuParserOneD : public scaOneD {
public:
  dt__class(scaMuParserOneD, analyticFunction);
  scaMuParserOneD();
  scaMuParserOneD(scaMuParserOneD const &orig);
  /*!
   * @param expression Function expression.
   * @param argument Argument.
   * @exception dtOO::eGeneral Catch `muParser` and throw exception.
   *
   *   Create a simple function, e.g by
   *
   *   > fun = scaMuParserOneD("5.*xx*xx+(2*xx)", "xx")
   *
   */
  scaMuParserOneD(std::string const expression, std::string const argument);
  scaMuParserOneD *clone(void) const;
  scaMuParserOneD *cloneTransformed(dtTransformer const *const dtT) const;
  scaMuParserOneD *create(void) const;
  virtual ~scaMuParserOneD();
  virtual dtReal YFloat(dtReal const &xx) const;
  void setArgumentRange(dtReal xMin, dtReal xMax);

private:
  dt__pH(dtMuParser) _parser;
  dt__pH(double) _argument;
  std::string _expressionStr;
  std::string _argumentStr;
};
} // namespace dtOO
#endif /* SCAMUPARSERONED_H */
