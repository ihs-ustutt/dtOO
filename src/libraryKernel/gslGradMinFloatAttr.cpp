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

#include "gslGradMinFloatAttr.h"
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
static double gsl_proxy_f_gslGradMinFloatAttr(gsl_vector const *v, void *params)
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
static void gsl_proxy_df_gslGradMinFloatAttr(
  const gsl_vector *v, void *params, gsl_vector *g
)
{
  dtOO::floatAtt *const ob = static_cast<gsl_proxy_t *>(params)->att_p;
  dtOO::dtInt const &dim = ob->dimension();
  std::vector<dtOO::dtReal> xx(dim);
  dt__forFromToIndex(0, dim, i) xx[i] = gsl_vector_get(v, i);
  std::vector<dtOO::dtReal> grad(ob->dimension(), GSL_NAN);
  if (!ob->outOfRange(xx))
  {
    grad = ob->grad(xx);
  }
  dt__forFromToIndex(0, dim, i) gsl_vector_set(g, i, grad[i]);
}

static void gsl_proxy_fdf_gslGradMinFloatAttr(
  const gsl_vector *v, void *params, double *f, gsl_vector *g
)
{
  *f = gsl_proxy_f_gslGradMinFloatAttr(v, params);
  gsl_proxy_df_gslGradMinFloatAttr(v, params, g);

  if (*f == GSL_NAN)
  {
    dt__forFromToIndex(0, g->size, i) gsl_vector_set(g, i, GSL_NAN);
  }

  dt__forFromToIndex(0, g->size, i)
  {
    if (gsl_vector_get(g, i) == GSL_NAN)
    {
      *f = GSL_NAN;
    }
  }
}

static void gsl_proxy_errorhandler(
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

gslGradMinFloatAttr::~gslGradMinFloatAttr() {}

gslGradMinFloatAttr *gslGradMinFloatAttr::clone(void) const
{
  return new gslGradMinFloatAttr(*this);
}

bool gslGradMinFloatAttr::perform()
{
  dt__throwIf(!ptrAttribute()->hasGrad(), perform());
  // set error handler
  gsl_set_error_handler(&gsl_proxy_errorhandler);
  // create function structure
  gsl_multimin_function_fdf proxyF;
  // set proxy function
  proxyF.f = &gsl_proxy_f_gslGradMinFloatAttr;
  proxyF.df = &gsl_proxy_df_gslGradMinFloatAttr;
  proxyF.fdf = &gsl_proxy_fdf_gslGradMinFloatAttr;
  // set proxy structure
  gsl_proxy_t proxyS = {ptrAttribute()};
  // cast to void * and set to params
  proxyF.params = (void *)&proxyS;

  // strategy and gslGradMinFloatAttr must have the same dimension
  dt__throwIf(dimension() != ptrAttribute()->dimension(), perform());
  dt__throwIf(dimension() == 0, perform());

  // set dimension
  proxyF.n = dimension();

  //
  // create minimizer struct
  //
  gsl_multimin_fdfminimizer *minf = gsl_multimin_fdfminimizer_alloc(
    gsl_multimin_fdfminimizer_conjugate_fr, dimension()
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
  dtInt status;
  dtReal gF = std::numeric_limits<dtReal>::max();
  logContainer<gslGradMinFloatAttr> logC(logDEBUG, "perform()");
  logC() << "Using: " << ptrAttribute()->virtualClassName() << std::endl;
  dt__forAllIndex(guess(), jj)
  {
    try
    {
      // output
      logC() << logMe::dtFormat("%3d : [ ") % jj;
      dt__forAllIndex(guess()[jj], kk)
      {
        logC() << logMe::dtFormat("%5.2d ") % guess()[jj][kk];
      }
      logC() << "]";

      dt__forFromToIndex(0, dimension(), ii)
      {
        gsl_vector_set(xx, ii, guess()[jj][ii]);

        // correct initial guess
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

      dt__throwIf(
        gsl_multimin_fdfminimizer_set(
          minf, &proxyF, xx, step()[0], precision()
        ),
        perform()
      );
      // iterate
      iter = 0;
      do
      {
        iter = iter + 1;
        status = gsl_multimin_fdfminimizer_iterate(minf);

        logC() << logMe::dtFormat("\n  %3d (status = %2d) : x = ") % iter %
                    status;
        gsl_vector *x = gsl_multimin_fdfminimizer_x(minf);
        dt__forFromToIndex(0, dimension(), hh)
        {
          logC() << logMe::dtFormat("%+5.2e ") % gsl_vector_get(x, hh);
        }
        gsl_vector *grad = gsl_multimin_fdfminimizer_gradient(minf);
        logC() << " / grad = ";
        dt__forFromToIndex(0, dimension(), hh)
        {
          logC() << logMe::dtFormat("%+5.2e ") % gsl_vector_get(grad, hh);
        }
        logC() << logMe::dtFormat(" -> f = %+5.2e ( dx = ") % minf->f;
        gsl_vector *dx = gsl_multimin_fdfminimizer_dx(minf);
        dt__forFromToIndex(0, dimension(), hh)
        {
          logC() << logMe::dtFormat("%+5.2e ") % gsl_vector_get(dx, hh);
        }
        logC() << ")";

        // if status is anything else than zero, break the loop and continue
        // with another guess
        if (status)
        {
          logC() << logMe::dtFormat("\n  GSL: %s") %
                      std::string(gsl_strerror(status));
          break;
        }

        if (minf->f < precision())
        {
          converged(true);
          break;
        }
      } while (iter < maxIterations());

      logC() << logMe::dtFormat("\n  %3d -> %+5.2e (%+5.2e) = %d") % iter %
                  minf->f % precision() % converged()
             << std::endl;

      if (minf->f < gF)
      {
        gF = minf->f;
        std::vector<dtReal> tRes = result();
        dt__forAllIndex(tRes, kk) tRes[kk] = gsl_vector_get(minf->x, kk);
        result(tRes);
        logC() << logMe::dtFormat("-> Global minimum update -> %+5.2d\n") % gF;
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
    logC() << logMe::dtFormat("%+5.2d ") % result()[kk];
  }
  logC() << logMe::dtFormat(") = %+5.2d") % gF;
  if (!converged())
    logC() << " -> F";
  logC() << std::endl;

  //
  // free memory
  //
  gsl_vector_free(xx);
  gsl_vector_free(ss);
  gsl_multimin_fdfminimizer_free(minf);
  gsl_set_error_handler(NULL);

  //
  // return status
  //
  return converged();
}

dt__C_addCloneForpVH(gslGradMinFloatAttr);
} // namespace dtOO
