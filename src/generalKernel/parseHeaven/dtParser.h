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

#ifndef dtParser_H
#define dtParser_H

#include <dtOOTypeDef.h>

#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>
#include <mainConceptFwd.h>
#include <string>

namespace dtOO {
class analyticGeometry;
class analyticFunction;
class constValue;
class dtTransformer;
class baseContainer;

//! Parser to handle internal dialect.
/*! The parser handles internal dialect. A string is parsed and all fragments
 *  with key signs are replaced.
 */
class dtParser {
public:
  dt__classOnlyName(dtParser);
  dtParser();
  dtParser(
    baseContainer const *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG,
    lvH_boundedVolume const *const bV,
    lvH_dtCase const *const dC
  );
  virtual ~dtParser();
  /*!
   * @param expression String to parse.
   *
   *   Parse an expression, e.g. by
   *
   *   > dtParser(bC, cV, aF, aG, bV, dC)["3.*#myCV#+@m1d(1.)@"]
   *
   *   All keywords that start with "`", "!", "|", "~", "#", "$" or "@"
   *   are replaced by the corresponding value. Mathematical calculations
   *   are not done within the operator[]. If also an mathematical
   *   evaluation is desired, the operator() has to be used. E.g. by a
   *   simple second call to
   *
   *   > dtParser()(dtParser(bC, cV, aF, aG, bV, dC)["3.*#myCV#+@m1d(1.)@"])
   *
   *   The the result of the operation is a std::vector< dtReal > giving
   *   each dimension that is separated by a "," in the expression.
   */
  std::string operator[](std::string const expression) const;
  std::vector<dtReal> operator()(std::string const expression) const;

private:
  static std::string replaceDependencies(
    std::string const expression, lvH_constValue const *const cV
  );
  static std::string replaceDependencies(
    std::string const expression,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF
  );
  static std::string replaceDependencies(
    std::string const expression,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG
  );
  static std::string replaceDependencies(
    std::string const expression,
    baseContainer const *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG
  );
  static dtReal muParseString(std::string const expression);
  static std::vector<dtReal> muParseCSString(std::string const expression);

private:
  baseContainer const *_bC;
  lvH_constValue const *_cV;
  lvH_analyticFunction const *_aF;
  lvH_analyticGeometry const *_aG;
  lvH_boundedVolume const *_bV;
  lvH_dtCase const *_dC;

protected:
  static std::string _CALCSIGN;
  static std::string _POINTSIGN;
  static std::string _VECTORSIGN;
  static std::string _DTTSIGN;
  static std::string _CVSIGN;
  static std::string _AFSIGN;
  static std::string _AGSIGN;
};
} // namespace dtOO
#endif /* dtParser_H */
