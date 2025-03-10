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

#ifndef gslMinFloatAttr_H
#define gslMinFloatAttr_H

#include <dtOOTypeDef.h>

#include <attributionHeaven/floatAtt.h>
#include <logMe/dtMacros.h>

namespace dtOO {

class gslMinFloatAttr {
public:
  dt__classOnlyName(gslMinFloatAttr);
  gslMinFloatAttr(
    dt__pH(floatAtt) const &attribute,
    std::vector<dtReal> const &guess,
    std::vector<dtReal> const &step,
    dtReal const &precision,
    dtInt const &maxIterations = 100
  );
  gslMinFloatAttr(
    dt__pH(floatAtt) const &attribute,
    dtPoint2 const &guess,
    dtPoint2 const &step,
    dtReal const &precision,
    dtInt const &maxIterations = 100
  );
  gslMinFloatAttr(
    dt__pH(floatAtt) const &attribute,
    std::vector<dtPoint2> const &guess,
    dtPoint2 const &step,
    dtReal const &precision,
    dtInt const &maxIterations = 100
  );
  gslMinFloatAttr(
    dt__pH(floatAtt) const &attribute,
    dtPoint3 const &guess,
    dtPoint3 const &step,
    dtReal const &precision,
    dtInt const &maxIterations = 100
  );
  gslMinFloatAttr(
    dt__pH(floatAtt) const &attribute,
    std::vector<dtPoint3> const &guess,
    dtPoint3 const &step,
    dtReal const &precision,
    dtInt const &maxIterations = 100
  );
  gslMinFloatAttr(
    dt__pH(floatAtt) const &attribute,
    dtReal const &guess,
    dtReal const &step,
    dtReal const &precision,
    dtInt const &maxIterations = 100
  );
  gslMinFloatAttr(gslMinFloatAttr const &orig);
  virtual ~gslMinFloatAttr();
  virtual gslMinFloatAttr *clone(void) const;

  dtReal const &precision() const;
  bool const &converged() const;
  std::vector<dtReal> const &result() const;
  std::string const &lastStatus() const;
  dtInt const &dimension() const;
  //
  // set result
  //
  void converged(bool const converged);
  void result(std::vector<dtReal> const result);
  void lastStatus(std::string const &lastStatus);
  bool perform();
  floatAtt const *const ptrAttribute(void) const;

private:
  dtInt const _dimension;
  std::vector<std::vector<dtReal>> const _guess;
  std::vector<dtReal> const _step;
  dtReal const _precision;
  dtInt const _maxIterations;
  bool _converged;
  std::vector<dtReal> _result;
  std::string _lastStatus;
  dt__pH(floatAtt) _attribute;
};
dt__H_addCloneForpVH(gslMinFloatAttr);
} // namespace dtOO
#endif /* gslMinFloatAttr_H */
