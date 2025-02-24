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

#include "gslMinFloatAttr.h"
#include "logMe/dtMacros.h"

#include <boost/assign.hpp>
#include <gsl/gsl_vector_double.h>
#include <logMe/logMe.h>
#include <progHelper.h>

#include <gsl/gsl_vector.h>
#include <gsl/gsl_multimin.h>

// proxy structure
struct gsl_proxy_t {
  dtOO::gslMinFloatAttr * gmd_p;
};
// proxy function
double gsl_proxy_gslMinFloatAttr(gsl_vector const * v, void * params) {
  dtOO::gslMinFloatAttr * const ob 
  = 
  static_cast< gsl_proxy_t * >(params)->gmd_p;
  dtOO::dtInt const & dim = ob->dimension();
  std::vector< dtOO::dtReal > xx(dim);
  dt__forFromToIndex(0, dim, i)  xx[i] = gsl_vector_get(v,i);
  return ob->ptrAttribute()->operator()(xx);
}

namespace dtOO {
  gslMinFloatAttr::gslMinFloatAttr(
    floatAtt * attribute,
    std::vector< dtReal > const & guess,
    std::vector< dtReal > const & step,
    dtReal const & precision,
    dtInt const & maxIterations
  ) 
  :
  _attribute(attribute),
  _dimension(guess.size()),
  _guess(guess),
  _step(step), 
  _precision(precision), 
  _maxIterations(maxIterations)
  {
    _converged = false;
  }
 
  gslMinFloatAttr::gslMinFloatAttr(
    floatAtt * attribute,
    dtPoint2 const & guess,
    dtPoint2 const & step,
    dtReal const & precision,
    dtInt const & maxIterations
  ) 
  :
  _attribute(attribute),
  _dimension(2),
  _guess( 
    ::boost::assign::list_of(guess.x())(guess.y())
  ),
  _step(
    ::boost::assign::list_of(step.x())(step.y())
  ),
  _precision(precision),
  _maxIterations(maxIterations),
  _result( 
    ::boost::assign::list_of
      (std::numeric_limits<dtReal>::infinity())
      (std::numeric_limits<dtReal>::infinity())
  )
  {
    _converged = false;
  }

  gslMinFloatAttr::gslMinFloatAttr(
    floatAtt * attribute,
    dtPoint3 const & guess,
    dtPoint3 const & step,
    dtReal const & precision,
    dtInt const & maxIterations
  ) 
  :
  _attribute(attribute),
  _dimension(3),
  _guess( 
    ::boost::assign::list_of(guess.x())(guess.y())(guess.z())
  ),
  _step(
    ::boost::assign::list_of(step.x())(step.y())(step.z())
  ),
  _precision(precision),
  _maxIterations(maxIterations),
  _result( 
    ::boost::assign::list_of
      (std::numeric_limits<dtReal>::infinity())
      (std::numeric_limits<dtReal>::infinity())
      (std::numeric_limits<dtReal>::infinity())
  )
  {
    _converged = false;
  }

  gslMinFloatAttr::gslMinFloatAttr(gslMinFloatAttr const & orig) 
  : 
  _dimension(orig._dimension),
  _guess(orig._guess), 
  _step(orig._step), 
  _precision(orig._precision), 
  _maxIterations(orig._maxIterations),
  _converged(orig._converged),
  _result(orig._result),
  _lastStatus(orig._lastStatus)
  {

  }

  std::vector< dtReal > const & gslMinFloatAttr::guess() const {
    return _guess;
  }

  std::vector< dtReal > const & gslMinFloatAttr::step() const {
    return _step;
  }

  dtReal const & gslMinFloatAttr::precision() const {
    return _precision;
  }

  dtInt const & gslMinFloatAttr::maxIterations() const {
    return _maxIterations;
  }

  bool const & gslMinFloatAttr::converged() const {
    return _converged;
  }

  void gslMinFloatAttr::converged( bool const converged) {
    _converged = converged;
  }

  std::vector< dtReal > const & gslMinFloatAttr::result() const {
    return _result;
  }

  void gslMinFloatAttr::result( std::vector< dtReal> const result ) {
    _result = result;
  }

  std::string const & gslMinFloatAttr::lastStatus() const {
    return _lastStatus;
  }

  dtInt const & gslMinFloatAttr::dimension() const {
    return _dimension;
  }

  void gslMinFloatAttr::lastStatus( std::string const & lastStatus ) {
    _lastStatus = lastStatus;
  }

  gslMinFloatAttr::~gslMinFloatAttr() {
  }

  gslMinFloatAttr * gslMinFloatAttr::clone( void ) const {
    return new gslMinFloatAttr(*this);
  }

  floatAtt const * const gslMinFloatAttr::ptrAttribute( void ) const {
    return _attribute.get();
  }
  bool gslMinFloatAttr::perform() {
    // create function structure
    gsl_multimin_function proxyF;
    // set proxy function
    proxyF.f = &gsl_proxy_gslMinFloatAttr; 
    // set proxy structure
    gsl_proxy_t proxyS = {this};
    // cast to void * and set to params
    proxyF.params = (void*) &proxyS;

    //dtInt dimension = _d

    dt__throwIf(_dimension!=_attribute->dimension(), perform());

    // set dimension
    proxyF.n = _dimension;

    //
    // create minimizer struct
    //
    gsl_multimin_fminimizer * minf = gsl_multimin_fminimizer_alloc(
      gsl_multimin_fminimizer_nmsimplex2, _dimension
    );

    //
    // set first guess and step size
    //
    gsl_vector * x = gsl_vector_alloc( _dimension );
    gsl_vector * ss = gsl_vector_alloc( _dimension );
    dt__forFromToIndex(0, _dimension, ii) {
      gsl_vector_set(x, ii, _guess[ii]);
      gsl_vector_set(ss, ii, _step[ii]);
    }

    //
    // iterate
    //
    int status = gsl_multimin_fminimizer_set(minf, &proxyF, x, ss);
    dt__throwIf(status, perform);
    dtInt iter = 0;
    _converged = false;
    do {
      iter = iter + 1;
      status = gsl_multimin_fminimizer_iterate(minf);
      if (status) {
        break;
      }

      if ( minf->fval < _precision ) {
        _converged = true;
        break;
      }
    }
    while (iter < _maxIterations);
   
    //
    // set result
    //
    if (_dimension == 1) {
      _result 
      = 
      ::boost::assign::list_of
        (gsl_vector_get(minf->x,0));
    }
    else if (_dimension == 2) {
      _result 
      = 
      ::boost::assign::list_of
        (gsl_vector_get(minf->x,0))
        (gsl_vector_get(minf->x,1));
    }
    else if (_dimension == 3) {
      _result 
      = 
      ::boost::assign::list_of
        (gsl_vector_get(minf->x,0))
        (gsl_vector_get(minf->x,1))
        (gsl_vector_get(minf->x,2));
    }
    else dt__throwUnexpected(perform());
    
    //
    // set last status
    //
    if (_converged) {
      _lastStatus 
      = 
      str(
        logMe::dtFormat("C %5.2e %3d (GSL: %3d)") 
        % minf->fval % iter % status
      );
    }
    else {
      _lastStatus 
      = 
      str(
        logMe::dtFormat("D %5.2e %3d (GSL: %3d)") 
        % minf->fval % iter % status
      );
    }

    //
    // free memory
    gsl_multimin_fminimizer_free(minf);
    gsl_vector_free(x);
    gsl_vector_free(ss);

    //
    // return status
    //
    return _converged;
  }

  dt__C_addCloneForpVH(gslMinFloatAttr);
}
