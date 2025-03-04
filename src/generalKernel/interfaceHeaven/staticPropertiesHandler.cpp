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

#include "staticPropertiesHandler.h"
#include "systemHandling.h"

#include <logMe/logMe.h>
#ifdef DTOO_HAS_OMP
#include <omp.h>
#endif
#ifdef DTOO_HAS_MPI
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/environment.hpp>
#endif

namespace dtOO {
dt__pH(staticPropertiesHandler) staticPropertiesHandler::_pH(NULL);

staticPropertiesHandler::staticPropertiesHandler()
  : optionHandling(), _initialized(false)
{
  dt__forceInfo(
    staticPropertiesHandler(), << "Create staticPropertiesHandler ..."
  );
  setOption("reparamOnFace_precision", "1.e-2");
  setOption("reparamInVolume_precision", "1.e-2");
  setOption("reparam_internalRestarts", "10");
  setOption("reparam_restarts", "5");
  setOption("reparam_restartIncreasePrecision", "1.e+1");
  setOption("reparam_internalRestartDecreasePrecision", "0.1");
  setOption("invY_precision", "1.e-2");
  setOption("xyz_resolution", "1.e-4");
  setOption("XYZ_resolution", "1.e-3");
  setOption("uvw_resolution", "1.e-2");
  setOption("point_render_diameter", "1.");
  setOption("vector_render_size", "1.");
  setOption("render_max_nPoints", "50");
  setOption("function_render_resolution_u", "10");
  setOption("function_render_resolution_v", "10");
  setOption("function_render_resolution_w", "10");
  setOption("geometry_render_resolution_u", "10");
  setOption("geometry_render_resolution_v", "10");
  setOption("geometry_render_resolution_w", "10");
  setOption("workingDirectory", systemHandling::currentDirectory());
  setOption("ompNumThreads", "1");
  setOption("map1dTo3d_deltaPer", "0.01");
  setOption("map2dTo3d_deltaPer", "0.01");
  setOption("map3dTo3d_deltaPer", "0.01");
  setOption("transfinite_correctionLoops", "10");
  setOption("isEqualExtendCheck", "false");
  setOption("logLevel", "0");
  if (std::getenv("DTOO_LOGLEVEL") != NULL)
  {
    setOption("logLevel", std::string(std::getenv("DTOO_LOGLEVEL")));
  }

  _gDebug = false;
}

staticPropertiesHandler *staticPropertiesHandler::getInstance(void)
{
  if (!_pH)
  {
    _pH.reset(new staticPropertiesHandler());
  }
  return _pH.get();
}

staticPropertiesHandler::~staticPropertiesHandler() {}

void staticPropertiesHandler::jInit(jsonPrimitive const &jE)
{
  if (_initialized)
    return;

  optionHandling oH;
  oH.jInit(jE);
  if (oH.hasOption("logLevel"))
  {
    setOption("logLevel", oH.getOption("logLevel"));
  }
  FILELog::ReportingLevel() = TLogLevel(getOptionInt("logLevel"));

  optionHandling::jInit(jE);
#ifdef DTOO_HAS_OMP
  omp_set_num_threads(getOptionInt("ompNumThreads"));
#endif
  _nRanks = 1;
  _thisRank = 0;
#ifdef DTOO_HAS_MPI
  if (::boost::mpi::environment::initialized())
  {
    _nRanks = ::boost::mpi::communicator().size();
    _thisRank = ::boost::mpi::communicator().rank();
  }
#endif

  dt__forceInfo(
    jInit(),
    << dt__eval(FILELog::ReportingLevel()) << std::endl
    << dt__eval(ompGetNumThreads()) << std::endl
    << dt__eval(ompGetThreadLimit()) << std::endl
    << dt__eval(ompGetMaxThreads()) << std::endl
    << dt__eval(mpiParallel()) << std::endl
    << dt__eval(thisRank()) << std::endl
    << dt__eval(nRanks()) << std::endl
    << dt__eval(_gDebug)
  );

  _initialized = true;
}

void staticPropertiesHandler::init(::QDomElement const *const wElement)
{
  if (!_initialized)
  {
    optionHandling::init(wElement);
    this->jInit(jsonPrimitive());
  }
}

bool staticPropertiesHandler::mpiParallel(void)
{
#ifdef DTOO_HAS_MPI
  return ::boost::mpi::environment::initialized();
#else
  return false;
#endif
}

void staticPropertiesHandler::mpiBarrier(void)
{
#ifdef DTOO_HAS_MPI
  ::boost::mpi::communicator().barrier();
#endif
}

dtInt staticPropertiesHandler::thisRank(void) const { return _thisRank; }

dtInt staticPropertiesHandler::nRanks(void) const { return _nRanks; }

bool staticPropertiesHandler::initialized(void) const { return _initialized; }

bool staticPropertiesHandler::gDebug(void) const { return _gDebug; }

void staticPropertiesHandler::setGDebug(void) { _gDebug = true; }

dtInt staticPropertiesHandler::ompGetMaxThreads(void)
{
  dtInt retInt = 1;
#ifdef DTOO_HAS_OMP
  retInt = omp_get_max_threads()
#endif
    return retInt;
}

dtInt staticPropertiesHandler::ompGetThreadNum(void)
{
  dtInt retInt = 0;
#ifdef DTOO_HAS_OMP
  retInt = omp_get_thread_num()
#endif
    return retInt;
}

bool staticPropertiesHandler::ompInParallel(void)
{
  bool retBool = false;
#ifdef DTOO_HAS_OMP
  retBool = omp_in_parallel()
#endif
    return retBool;
}

dtInt staticPropertiesHandler::ompGetNumThreads(void)
{
  dtInt retInt = 1;
#ifdef DTOO_HAS_OMP
  retInt = omp_get_num_threads()
#endif
    return retInt;
}

dtInt staticPropertiesHandler::ompGetThreadLimit(void)
{
  dtInt retInt = 1;
#ifdef DTOO_HAS_OMP
  retInt = omp_get_thread_limit()
#endif
    return retInt;
}

jsonPrimitive &staticPropertiesHandler::config(void) { return _config; }

jsonPrimitive const &staticPropertiesHandler::config(void) const
{
  return _config;
}

} // namespace dtOO
