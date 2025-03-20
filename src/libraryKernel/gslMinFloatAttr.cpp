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
#include "exceptionHeaven/eGeneral.h"
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
  if (!ob->outOfRange(xx))
  {
    return ob->operator()(xx);
  }
  return GSL_NAN;
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

gslMinFloatAttr::~gslMinFloatAttr() {}

gslMinFloatAttr *gslMinFloatAttr::clone(void) const
{
  return new gslMinFloatAttr(*this);
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
  gsl_proxy_t proxyS = {ptrAttribute()};
  // cast to void * and set to params
  proxyF.params = (void *)&proxyS;

  // strategy and gslMinFloatAttr must have the same dimension
  dt__throwIf(dimension() != ptrAttribute()->dimension(), perform());
  dt__throwIf(dimension() == 0, perform());

  // set dimension
  proxyF.n = dimension();

  //
  // create minimizer struct
  //
  gsl_multimin_fminimizer *minf = gsl_multimin_fminimizer_alloc(
    gsl_multimin_fminimizer_nmsimplex2rand, dimension()
  );

  //
  // set step size
  //
  gsl_vector *ss = gsl_vector_alloc(dimension());
  dt__forFromToIndex(0, dimension(), ii) gsl_vector_set(ss, ii, step()[ii]);

  //
  // allocate first guess
  //
  gsl_vector *xx = gsl_vector_alloc(dimension());

  dtInt iter = 0;
  converged(false);
  ;
  dtInt status;
  dtReal gF = std::numeric_limits<dtReal>::max();
  logContainer<gslMinFloatAttr> logC(logDEBUG, "perform()");
  logC() << "Using: " << ptrAttribute()->virtualClassName() << std::endl;
  dt__forAllIndex(guess(), jj)
  {
    try
    {
      logC() << logMe::dtFormat("%3d : [ ") % jj;
      dt__forAllIndex(guess()[jj], kk)
      {
        logC() << logMe::dtFormat("%5.2d ") % guess()[jj][kk];
      }
      logC() << "] ";

      dt__forFromToIndex(0, dimension(), ii)
      {
        gsl_vector_set(xx, ii, guess()[jj][ii]);

        dtReal const up = guess()[jj][ii] + step()[ii];
        dtReal const low = guess()[jj][ii] - step()[ii];
        dt__warnIfWithMessageAndSolution(
          up > 1.0,
          gsl_vector_set(xx, ii, 1.0 - step()[ii]),
          perform(),
          << "Shift upper guess to " << 1.0 - step()[ii]
        );
        dt__warnIfWithMessageAndSolution(
          low < 0.0,
          gsl_vector_set(xx, ii, 0.0 + step()[ii]),
          perform(),
          << "Shift lower guess to " << 0.0 + step()[ii]
        );
      }
      // iterate
      dt__throwIf(
        gsl_multimin_fminimizer_set(minf, &proxyF, xx, ss), perform()
      );
      iter = 0;
      do
      {
        iter = iter + 1;
        status = gsl_multimin_fminimizer_iterate(minf);

        if (status)
          break;

        // logC() << logMe::dtFormat("  %3d : ") % iter;
        // dt__forFromToIndex(0, dimension(), hh) {
        //   logC()
        //     << logMe::dtFormat("%5.2e ")
        //       % gsl_vector_get(gsl_multimin_fminimizer_x(minf), hh);
        // }
        // logC()
        //   << logMe::dtFormat(" -> %5.2e (%5.2e)\n")
        //     % minf->fval % gsl_multimin_fminimizer_size(minf);

        if (minf->fval < precision())
        {
          converged(true);
          break;
        }
        // dtReal const & msize = gsl_multimin_fminimizer_size(minf);
        // if (msize < 1.0e-09) {
        //   break;
        // }
      } while (iter < maxIterations());

      logC() << logMe::dtFormat("%3d -> %5.2e (%5.2e) = %d") % iter %
                  minf->fval % precision() % converged()
             << std::endl;

      if (minf->fval < gF)
      {
        gF = minf->fval;
        std::vector<dtReal> tRes = result();
        dt__forAllIndex(tRes, kk) tRes[kk] = gsl_vector_get(minf->x, kk);
        result(tRes);
        logC() << logMe::dtFormat("-> Global minimum update -> %5.2d\n") % gF;
      }

      if (converged())
        break;
    } catch (eGeneral const &egen)
    {
      logC() << "-> Exception" << std::endl;
    }
  }
  logC() << logMe::dtFormat("=> min f( ");
  dt__forAllIndex(result(), kk)
  {
    logC() << logMe::dtFormat("%5.2d ") % result()[kk];
  }
  logC() << logMe::dtFormat(") = %5.2d") % gF;
  if (!converged())
    logC() << " -> F";
  logC() << std::endl;

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
  return converged();
}

dt__C_addCloneForpVH(gslMinFloatAttr);
} // namespace dtOO
