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
#include "attributionHeaven/floatAtt.h"
#include "logMe/dtMacros.h"

#include <boost/assign.hpp>
#include <boost/assign/list_of.hpp>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_vector_double.h>
#include <limits>
#include <logMe/logContainer.h>
#include <logMe/logMe.h>
#include <progHelper.h>

#include <gsl/gsl_multimin.h>
#include <gsl/gsl_vector.h>

// proxy structure
struct gsl_proxy_t {
  dtOO::floatAtt *att_p;
};
// proxy function
double gsl_proxy_gslMinFloatAttr(gsl_vector const *v, void *params)
{
  dtOO::floatAtt *const ob = static_cast<gsl_proxy_t *>(params)->att_p;
  dtOO::dtInt const &dim = ob->dimension();
  std::vector<dtOO::dtReal> xx(dim);
  dt__forFromToIndex(0, dim, i) xx[i] = gsl_vector_get(v, i);
  return ob->rangeCheckAndCall(xx);
}

void gsl_proxy_errorhandler(
  const char *reason, const char *file, int line, int gsl_errno
)
{
  dt__throwNoClass(
    gsl_proxy_errorhandler(),
    << "reason = " << std::string(reason) << std::endl
    << "file = " << std::string(file) << std::endl
    << "line = " << line << std::endl
    << std::string(gsl_strerror(gsl_errno))
  );
}

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

gslMinFloatAttr::gslMinFloatAttr(
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

gslMinFloatAttr::gslMinFloatAttr(
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

gslMinFloatAttr::gslMinFloatAttr(
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

gslMinFloatAttr::gslMinFloatAttr(
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

gslMinFloatAttr::gslMinFloatAttr(
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

gslMinFloatAttr::gslMinFloatAttr(
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

gslMinFloatAttr::gslMinFloatAttr(gslMinFloatAttr const &orig)
  : _dimension(orig._dimension), _guess(orig._guess), _step(orig._step),
    _precision(orig._precision), _maxIterations(orig._maxIterations),
    _converged(orig._converged), _result(orig._result),
    _lastStatus(orig._lastStatus)
{
}

dtReal const &gslMinFloatAttr::precision() const { return _precision; }

bool const &gslMinFloatAttr::converged() const { return _converged; }

void gslMinFloatAttr::converged(bool const converged)
{
  _converged = converged;
}

std::vector<dtReal> const &gslMinFloatAttr::result() const { return _result; }

void gslMinFloatAttr::result(std::vector<dtReal> const result)
{
  _result = result;
}

std::string const &gslMinFloatAttr::lastStatus() const { return _lastStatus; }

dtInt const &gslMinFloatAttr::dimension() const { return _dimension; }

void gslMinFloatAttr::lastStatus(std::string const &lastStatus)
{
  _lastStatus = lastStatus;
}

gslMinFloatAttr::~gslMinFloatAttr() {}

gslMinFloatAttr *gslMinFloatAttr::clone(void) const
{
  return new gslMinFloatAttr(*this);
}

floatAtt const *const gslMinFloatAttr::ptrAttribute(void) const
{
  return _attribute.get();
}

bool gslMinFloatAttr::perform()
{
  // set error handler
  gsl_set_error_handler(&gsl_proxy_errorhandler);
  // create function structure
  gsl_multimin_function proxyF;
  // set proxy function
  proxyF.f = &gsl_proxy_gslMinFloatAttr;
  // set proxy structure
  gsl_proxy_t proxyS = {this->_attribute.get()};
  // cast to void * and set to params
  proxyF.params = (void *)&proxyS;

  // strategy and gslMinFloatAttr must have the same dimension
  dt__throwIf(_dimension != _attribute->dimension(), perform());
  dt__throwIf(_dimension == 0, perform());

  // set dimension
  proxyF.n = _dimension;

  //
  // create minimizer struct
  //
  gsl_multimin_fminimizer *minf = gsl_multimin_fminimizer_alloc(
    gsl_multimin_fminimizer_nmsimplex2rand, _dimension
  );

  //
  // set step size
  //
  gsl_vector *ss = gsl_vector_alloc(_dimension);
  dt__forFromToIndex(0, _dimension, ii) gsl_vector_set(ss, ii, _step[ii]);

  //
  // allocate first guess
  //
  gsl_vector *xx = gsl_vector_alloc(_dimension);

  dtInt iter = 0;
  _converged = false;
  dtInt status;
  dtReal gF = std::numeric_limits<dtReal>::max();
  logContainer<gslMinFloatAttr> logC(logDEBUG, "perform()");
  logC() << "Using: " << _attribute->virtualClassName() << std::endl;
  dt__forAllIndex(_guess, jj)
  {
    dt__forFromToIndex(0, _dimension, ii)
    {
      gsl_vector_set(xx, ii, _guess[jj][ii]);
    }
    // iterate
    dt__throwIf(gsl_multimin_fminimizer_set(minf, &proxyF, xx, ss), perform());
    iter = 0;
    do
    {
      iter = iter + 1;
      status = gsl_multimin_fminimizer_iterate(minf);

      if (status)
        break;

      // logC() << logMe::dtFormat("  %3d : ") % iter;
      // dt__forFromToIndex(0, _dimension, hh) {
      //   logC()
      //     << logMe::dtFormat("%5.2e ")
      //       % gsl_vector_get(gsl_multimin_fminimizer_x(minf), hh);
      // }
      // logC()
      //   << logMe::dtFormat(" -> %5.2e (%5.2e)\n")
      //     % minf->fval % gsl_multimin_fminimizer_size(minf);

      if (minf->fval < _precision)
      {
        _converged = true;
        break;
      }
      // dtReal const & msize = gsl_multimin_fminimizer_size(minf);
      // if (msize < 1.0e-09) {
      //   break;
      // }
    } while (iter < _maxIterations);

    if (minf->fval < gF)
    {
      gF = minf->fval;
      dt__forAllIndex(_result, kk) _result[kk] = gsl_vector_get(minf->x, kk);
      logC() << logMe::dtFormat("-> Global minimum update -> %5.2d\n") % gF;
    }

    logC() << logMe::dtFormat("%3d : [ ") % jj;
    dt__forAllIndex(_guess[jj], kk)
    {
      logC() << logMe::dtFormat("%5.2d ") % _guess[jj][kk];
    }

    logC() << logMe::dtFormat("] %3d -> %5.2e (%5.2e) = %d") % iter %
                minf->fval % _precision % _converged
           << std::endl;

    if (_converged)
      break;
  }
  logC() << logMe::dtFormat("=> min f( ");
  dt__forAllIndex(_result, kk)
  {
    logC() << logMe::dtFormat("%5.2d ") % _result[kk];
  }
  logC() << logMe::dtFormat(") = %5.2d") % gF;
  if (!_converged)
    logC() << " -> F";
  logC() << std::endl;

  //
  // set last status
  //
  if (_converged)
  {
    _lastStatus = str(
      logMe::dtFormat("C %5.2e %3d (GSL: %3d)") % minf->fval % iter % status
    );
  }
  else
  {
    _lastStatus = str(
      logMe::dtFormat("D %5.2e %3d (GSL: %3d)") % minf->fval % iter % status
    );
  }

  //
  // free memory
  //
  gsl_vector_free(xx);
  gsl_vector_free(ss);
  gsl_multimin_fminimizer_free(minf);
  gsl_set_error_handler(NULL);

  //
  // return status
  //
  return _converged;
}

dt__C_addCloneForpVH(gslMinFloatAttr);
} // namespace dtOO
