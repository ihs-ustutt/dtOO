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

#include "dtPoint3Algo.h"

#include <progHelper.h>
#include <vector>
#include <limits>
#include <boost/assign.hpp>

namespace dtOO {
  dtPoint3Algo::dtPoint3Algo( 
    dtPoint3 const & p3, 
    dtPoint2 const & guess,
    dtPoint2 const & step,
    dtReal const & precision,
    dtInt const & maxIterations
  ) 
  :
  _p3(p3),
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

  dtPoint3Algo::dtPoint3Algo( 
    dtPoint3 const & p3, 
    dtPoint3 const & guess,
    dtPoint3 const & step,
    dtReal const & precision,
    dtInt const & maxIterations
  ) 
  :
  _p3(p3),
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


  dtPoint3Algo::dtPoint3Algo( dtPoint3Algo const & orig ) 
  :
  _p3(orig._p3),
  _dim(orig._dim),
  _guess( orig._guess ),
  _step( orig._step ),
  _precision(orig._precision),
  _maxIterations(orig._maxIterations)
  {
    _lastStatus = orig._lastStatus;
  }

  dtPoint3Algo::~dtPoint3Algo() {
  }

  dtInt const & dtPoint3Algo::dimension() const {
    return _dim;
  }

  dtPoint3 const & dtPoint3Algo::p3(void) const {
    return _p3;
  }

  std::vector< dtReal > const & dtPoint3Algo::guess() const {
    return _guess;
  }

  std::vector< dtReal > const & dtPoint3Algo::step() const {
    return _step;
  }

  dtReal const & dtPoint3Algo::precision() const {
    return _precision;
  }

  dtInt const & dtPoint3Algo::maxIterations() const {
    return _maxIterations;
  }

  bool const & dtPoint3Algo::converged() const {
    return _converged;
  }

  void dtPoint3Algo::converged( bool const converged) {
    _converged = converged;
  }

  std::vector< dtReal > const & dtPoint3Algo::result() const {
    //dt__warnIf(!_converged, result());
    return _result;
  }

  void dtPoint3Algo::result( std::vector< dtReal> const result ) {
    _result = result;
  }

  std::string const & dtPoint3Algo::lastStatus() const {
    return _lastStatus;
  }

  void dtPoint3Algo::lastStatus( std::string const & lastStatus ) {
    _lastStatus = lastStatus;
  }

  dt__C_addCloneForpVH(dtPoint3Algo);
}
