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

#include "gslMinDist.h"

#include <boost/assign/list_of.hpp>
#include <limits>
#include <logMe/logMe.h>
#include <progHelper.h>

#include <analyticGeometryHeaven/analyticGeometry.h>
#include <logMe/logContainer.h>

#include <gsl/gsl_vector.h>
#include <gsl/gsl_multimin.h>

//
// analyticGeometry
//
// proxy structure
struct gsl_proxy_aG_t {
  dtOO::gslMinDist * gmd_p;
  dtOO::analyticGeometry const * aG_p;
};
// proxy function
double gsl_proxy_aG_gslMinDist(gsl_vector const * v, void * params) {
  dtOO::gslMinDist * const ob 
  = 
  static_cast< gsl_proxy_aG_t * >(params)->gmd_p;
  dtOO::analyticGeometry const * const aG 
  = 
  static_cast< gsl_proxy_aG_t * >(params)->aG_p;
  double xx[3];
  xx[0] = gsl_vector_get(v,0);
  if ( aG->dim() > 1 ) xx[1] = gsl_vector_get(v,1);
  if ( aG->dim() > 2 ) xx[2] = gsl_vector_get(v,2);
 
  dt__forFromToIndex(0, aG->dim(), ii) {
    if ( (xx[ii]<0.0) || (xx[ii]>1.0) ) {
      return std::numeric_limits< dtOO::dtReal >::max();
    }
  }
  return dtOO::dtLinearAlgebra::distance(
    aG->getPointPercent(xx),
    ob->p3()
  );
}

namespace dtOO {
  gslMinDist::gslMinDist(
    dtPoint3 const & p3, 
    dtPoint2 const & guess,
    dtPoint2 const & step,
    dtReal const & precision,
    dtInt const & maxIterations
  ) 
  : 
  refPointAlgo(p3, guess, step, precision, maxIterations) 
  {

  }

  gslMinDist::gslMinDist(
    dtPoint3 const & p3, 
    dtPoint3 const & guess,
    dtPoint3 const & step,
    dtReal const & precision,
    dtInt const & maxIterations
  ) 
  : 
  refPointAlgo(p3, guess, step, precision, maxIterations) 
  {

  }
 
  gslMinDist::gslMinDist(gslMinDist const & orig) 
  : 
  refPointAlgo(orig) 
  {

  }

  gslMinDist * gslMinDist::clone( void ) const {
    return new gslMinDist(*this);
  }

  int minimze(
    dtReal const & precision,
    dtInt const & dimension,
    int const & maxIterations,
    std::vector<dtReal> guess,
    std::vector<dtReal> step,
    gsl_multimin_function * proxyF, 
    bool & converged, 
    std::vector<dtReal> & result,
    std::string & lastStatus
  ) {
    // set dimension
    proxyF->n = dimension;

    //
    // create minimizer struct
    //
    gsl_multimin_fminimizer * minf = gsl_multimin_fminimizer_alloc(
      gsl_multimin_fminimizer_nmsimplex2, dimension
    );

    //
    // set first guess and step size
    //
    gsl_vector * x = gsl_vector_alloc( dimension );
    gsl_vector * ss = gsl_vector_alloc( dimension );
    dt__forFromToIndex(0, dimension, ii) {
      gsl_vector_set(x, ii, guess[ii]);
      gsl_vector_set(ss, ii, step[ii]);
    }

    //
    // iterate
    //
    int status = gsl_multimin_fminimizer_set(minf, proxyF, x, ss);
    dtInt iter = 0;
    if (!status) {
      converged = false;
      do {
        iter = iter + 1;
        status = gsl_multimin_fminimizer_iterate(minf);
        if (status) {
          break;
        }

        if ( minf->fval < precision ) {
          converged = true;
          break;
        }
      }
      while (iter < maxIterations);
    }
   
    //
    // set result
    //
    if (dimension == 1) {
      result 
      = 
      ::boost::assign::list_of
        (gsl_vector_get(minf->x,0));
    }
    else if (dimension == 2) {
      result 
      = 
      ::boost::assign::list_of
        (gsl_vector_get(minf->x,0))
        (gsl_vector_get(minf->x,1));
    }
   
    else if (dimension == 3) {
      result 
      = 
      ::boost::assign::list_of
        (gsl_vector_get(minf->x,0))
        (gsl_vector_get(minf->x,1))
        (gsl_vector_get(minf->x,2));
    }
    else {
      status = 99;
    }
    
    //
    // set last status
    //
    if (converged) {
      lastStatus 
      = 
      str(
        logMe::dtFormat("C %5.2e %3d") 
        % minf->fval % iter
      );
    }
    else {
      lastStatus 
      = 
      str(
        logMe::dtFormat("D %5.2e %3d") 
        % minf->fval % iter
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
    return status;
  }

  void gslMinDist::visit( analyticGeometry const * const aG ) {
    // create function structure
    gsl_multimin_function proxyF;
    // set proxy function
    proxyF.f = &gsl_proxy_aG_gslMinDist; 
    // set proxy structure
    gsl_proxy_aG_t proxyS = {this, aG};
    // cast to void * and set to params
    proxyF.params = (void*) &proxyS;

    //
    // declare iteration variables
    //
    bool converged = false;
    std::vector<dtReal> result;
    std::string lastStatus;

    //
    // start minimization and report warning on failure
    //
    dt__warnIf(
      minimze(
        precision(), dimension(), 100, guess(), step(),
        &proxyF, converged, result, lastStatus
      ),
      visit()
    );

    //
    // save result
    //
    this->converged(converged);
    this->result(result);
    this->lastStatus(lastStatus);
  }

  gslMinDist::~gslMinDist() {
  }

  dt__C_addCloneForpVH(gslMinDist);
}
