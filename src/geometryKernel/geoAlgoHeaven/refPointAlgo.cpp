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

#include "refPointAlgo.h"

#include <progHelper.h>
#include <vector>
#include <limits>
#include <boost/assign.hpp>

namespace dtOO {
  refPointAlgo::refPointAlgo( 
    dtPoint3 const & p3, 
    dtPoint2 const & guess,
    dtPoint2 const & step,
    dtReal const & precision,
    dtInt const & maxIterations
  ) 
  :
  _ref(
    ::boost::assign::list_of(p3.x())(p3.y())(p3.z())
  ),
  _dim(2),
  _guess( 
    ::boost::assign::list_of(guess.x())(guess.y()) 
  ),
  _step(
    ::boost::assign::list_of(step.x())(step.y()) ),
  _precision(precision),
  _maxIterations(maxIterations),
  _result( 
    ::boost::assign::list_of
      (std::numeric_limits<dtReal>::infinity())
      (std::numeric_limits<dtReal>::infinity())
  )
  {
    _lastStatus = std::string("none");
  }

  refPointAlgo::refPointAlgo( 
    dtPoint3 const & p3, 
    dtPoint3 const & guess,
    dtPoint3 const & step,
    dtReal const & precision,
    dtInt const & maxIterations
  ) 
  :
  _ref(
    ::boost::assign::list_of(p3.x())(p3.y())(p3.z())
  ),
  _dim(3),
  _guess( 
    ::boost::assign::list_of(guess.x())(guess.y())(guess.z()) 
  ),
  _step(
    ::boost::assign::list_of(step.x())(step.y())(step.z()) ),
  _precision(precision),
  _maxIterations(maxIterations),
  _result( 
    ::boost::assign::list_of
      (std::numeric_limits<dtReal>::infinity())
      (std::numeric_limits<dtReal>::infinity())
      (std::numeric_limits<dtReal>::infinity())
  )
  {
    _lastStatus = std::string("none");
  }


  refPointAlgo::refPointAlgo( refPointAlgo const & orig ) 
  :
  _ref(orig._ref),
  _dim(orig._dim),
  _guess( orig._guess ),
  _step( orig._step ),
  _precision(orig._precision),
  _maxIterations(orig._maxIterations)
  {
    _lastStatus = orig._lastStatus;
  }

  refPointAlgo::~refPointAlgo() {
  }

  dtInt const & refPointAlgo::dimension() const {
    return _dim;
  }

  dtPoint3 const refPointAlgo::p3(void) const {
    return dtPoint3(_ref[0], _ref[1], _ref[2]);
  }

  std::vector< dtReal > const & refPointAlgo::guess() const {
    return _guess;
  }

  std::vector< dtReal > const & refPointAlgo::step() const {
    return _step;
  }

  dtReal const & refPointAlgo::precision() const {
    return _precision;
  }

  dtInt const & refPointAlgo::maxIterations() const {
    return _maxIterations;
  }

  bool const & refPointAlgo::converged() const {
    return _converged;
  }

  void refPointAlgo::converged( bool const converged) {
    _converged = converged;
  }

  std::vector< dtReal > const & refPointAlgo::result() const {
    //dt__warnIf(!_converged, result());
    return _result;
  }

  void refPointAlgo::result( std::vector< dtReal> const result ) {
    _result = result;
  }

  std::string const & refPointAlgo::lastStatus() const {
    return _lastStatus;
  }

  void refPointAlgo::lastStatus( std::string const & lastStatus ) {
    _lastStatus = lastStatus;
  }

  dt__C_addCloneForpVH(refPointAlgo);
}
