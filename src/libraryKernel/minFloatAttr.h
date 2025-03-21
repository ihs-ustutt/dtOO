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

#ifndef minFloatAttr_H
#define minFloatAttr_H

#include <dtOOTypeDef.h>

#include <attributionHeaven/floatAtt.h>
#include <logMe/dtMacros.h>

namespace dtOO {

class minFloatAttr {
public:
  dt__classOnlyName(minFloatAttr);
  minFloatAttr(
    dt__pH(floatAtt) const &attribute,
    std::vector<dtReal> const &guess,
    std::vector<dtReal> const &step,
    dtReal const &precision,
    dtInt const &maxIterations = 100
  );
  minFloatAttr(
    dt__pH(floatAtt) const &attribute,
    dtPoint2 const &guess,
    dtPoint2 const &step,
    dtReal const &precision,
    dtInt const &maxIterations = 100
  );
  minFloatAttr(
    dt__pH(floatAtt) const &attribute,
    std::vector<dtPoint2> const &guess,
    dtPoint2 const &step,
    dtReal const &precision,
    dtInt const &maxIterations = 100
  );
  minFloatAttr(
    dt__pH(floatAtt) const &attribute,
    dtPoint3 const &guess,
    dtPoint3 const &step,
    dtReal const &precision,
    dtInt const &maxIterations = 100
  );
  minFloatAttr(
    dt__pH(floatAtt) const &attribute,
    std::vector<dtPoint3> const &guess,
    dtPoint3 const &step,
    dtReal const &precision,
    dtInt const &maxIterations = 100
  );
  minFloatAttr(
    dt__pH(floatAtt) const &attribute,
    dtReal const &guess,
    dtReal const &step,
    dtReal const &precision,
    dtInt const &maxIterations = 100
  );
  minFloatAttr(minFloatAttr const &orig);
  virtual ~minFloatAttr();
  virtual minFloatAttr *clone(void) const;

  dtReal const &precision() const;
  bool const &converged() const;
  std::vector<dtReal> const &result() const;
  // std::string const &lastStatus() const;
  dtInt const &dimension() const;
  dtInt const &maxIterations() const;
  std::vector<dtReal> const &step() const;
  std::vector<std::vector<dtReal>> const &guess() const;
  //
  // set result
  //
  void converged(bool const converged);
  void result(std::vector<dtReal> const result);
  // void lastStatus(std::string const &lastStatus);
  virtual bool perform() { return false; };
  floatAtt const *const ptrAttribute(void) const;
  floatAtt *const ptrAttribute(void);

private:
  dtInt const _dimension;
  std::vector<std::vector<dtReal>> const _guess;
  std::vector<dtReal> const _step;
  dtReal const _precision;
  dtInt const _maxIterations;
  bool _converged;
  std::vector<dtReal> _result;
  //  std::string _lastStatus;
  dt__pH(floatAtt) _attribute;
};
dt__H_addCloneForpVH(minFloatAttr);
} // namespace dtOO
#endif /* minFloatAttr_H */
