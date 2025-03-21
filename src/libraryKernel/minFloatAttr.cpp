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

#include "minFloatAttr.h"
#include <attributionHeaven/floatAtt.h>

#include <boost/assign.hpp>
#include <boost/assign/list_of.hpp>
#include <limits>
#include <logMe/dtMacros.h>
#include <logMe/logContainer.h>
#include <logMe/logMe.h>
#include <progHelper.h>

namespace dtOO {
std::vector<std::vector<dtReal>>
convertDtPoint2(std::vector<dtPoint2> const &guess)
{
  std::vector<std::vector<dtReal>> ret;
  dt__forAllIndex(guess, i)
  {
    ret.push_back(::boost::assign::list_of(guess[i].x())(guess[i].y()));
  }
  return ret;
}

std::vector<std::vector<dtReal>>
convertDtPoint3(std::vector<dtPoint3> const &guess)
{
  std::vector<std::vector<dtReal>> ret;
  dt__forAllIndex(guess, i)
  {
    ret.push_back(
      ::boost::assign::list_of(guess[i].x())(guess[i].y())(guess[i].z())
    );
  }
  return ret;
}

minFloatAttr::minFloatAttr(
  dt__pH(floatAtt) const &attribute,
  std::vector<dtReal> const &guess,
  std::vector<dtReal> const &step,
  dtReal const &precision,
  dtInt const &maxIterations
)
  : _attribute(attribute), _dimension(guess.size()),
    _guess(::boost::assign::list_of(guess)), _step(step), _precision(precision),
    _maxIterations(maxIterations),
    _result(std::vector(guess.size(), std::numeric_limits<dtReal>::infinity()))
{
  _converged = false;
}

minFloatAttr::minFloatAttr(
  dt__pH(floatAtt) const &attribute,
  dtPoint2 const &guess,
  dtPoint2 const &step,
  dtReal const &precision,
  dtInt const &maxIterations
)
  : _attribute(attribute), _dimension(2),
    _guess(
      ::boost::assign::list_of(::boost::assign::list_of(guess.x())(guess.y()))
    ),
    _step(::boost::assign::list_of(step.x())(step.y())), _precision(precision),
    _maxIterations(maxIterations),
    _result(::boost::assign::list_of(std::numeric_limits<dtReal>::infinity())(
      std::numeric_limits<dtReal>::infinity()
    ))
{
  _converged = false;
}

minFloatAttr::minFloatAttr(
  dt__pH(floatAtt) const &attribute,
  std::vector<dtPoint2> const &guess,
  dtPoint2 const &step,
  dtReal const &precision,
  dtInt const &maxIterations
)
  : _attribute(attribute), _dimension(2), _guess(convertDtPoint2(guess)),
    _step(::boost::assign::list_of(step.x())(step.y())), _precision(precision),
    _maxIterations(maxIterations),
    _result(::boost::assign::list_of(std::numeric_limits<dtReal>::infinity())(
      std::numeric_limits<dtReal>::infinity()
    ))
{
  _converged = false;
}

minFloatAttr::minFloatAttr(
  dt__pH(floatAtt) const &attribute,
  dtPoint3 const &guess,
  dtPoint3 const &step,
  dtReal const &precision,
  dtInt const &maxIterations
)
  : _attribute(attribute), _dimension(3),
    _guess(::boost::assign::list_of(
      ::boost::assign::list_of(guess.x())(guess.y())(guess.z())
    )),
    _step(::boost::assign::list_of(step.x())(step.y())(step.z())),
    _precision(precision), _maxIterations(maxIterations),
    _result(::boost::assign::list_of(std::numeric_limits<dtReal>::
                                       infinity())(std::numeric_limits<
                                                   dtReal>::infinity())(
      std::numeric_limits<dtReal>::infinity()
    ))
{
  _converged = false;
}

minFloatAttr::minFloatAttr(
  dt__pH(floatAtt) const &attribute,
  std::vector<dtPoint3> const &guess,
  dtPoint3 const &step,
  dtReal const &precision,
  dtInt const &maxIterations
)
  : _attribute(attribute), _dimension(3), _guess(convertDtPoint3(guess)),
    _step(::boost::assign::list_of(step.x())(step.y())), _precision(precision),
    _maxIterations(maxIterations),
    _result(::boost::assign::list_of(std::numeric_limits<dtReal>::
                                       infinity())(std::numeric_limits<
                                                   dtReal>::infinity())(
      std::numeric_limits<dtReal>::infinity()
    ))
{
  _converged = false;
}

minFloatAttr::minFloatAttr(
  dt__pH(floatAtt) const &attribute,
  dtReal const &guess,
  dtReal const &step,
  dtReal const &precision,
  dtInt const &maxIterations
)
  : _attribute(attribute), _dimension(1),
    _guess(::boost::assign::list_of(::boost::assign::list_of(guess))),
    _step(::boost::assign::list_of(step)), _precision(precision),
    _maxIterations(maxIterations),
    _result(::boost::assign::list_of(std::numeric_limits<dtReal>::infinity()))
{
  _converged = false;
}

minFloatAttr::minFloatAttr(minFloatAttr const &orig)
  : _dimension(orig._dimension), _guess(orig._guess), _step(orig._step),
    _precision(orig._precision), _maxIterations(orig._maxIterations),
    _converged(orig._converged), _result(orig._result)
{
}

dtReal const &minFloatAttr::precision() const { return _precision; }

bool const &minFloatAttr::converged() const { return _converged; }

void minFloatAttr::converged(bool const converged) { _converged = converged; }

std::vector<dtReal> const &minFloatAttr::result() const { return _result; }

void minFloatAttr::result(std::vector<dtReal> const result)
{
  _result = result;
}

dtInt const &minFloatAttr::dimension() const { return _dimension; }

dtInt const &minFloatAttr::maxIterations() const { return _maxIterations; }

std::vector<dtReal> const &minFloatAttr::step() const { return _step; }

std::vector<std::vector<dtReal>> const &minFloatAttr::guess() const
{
  return _guess;
}

minFloatAttr::~minFloatAttr() {}

minFloatAttr *minFloatAttr::clone(void) const
{
  return new minFloatAttr(*this);
}

floatAtt const *const minFloatAttr::ptrAttribute(void) const
{
  return _attribute.get();
}

floatAtt *const minFloatAttr::ptrAttribute(void) { return _attribute.get(); }

dt__C_addCloneForpVH(minFloatAttr);
} // namespace dtOO
