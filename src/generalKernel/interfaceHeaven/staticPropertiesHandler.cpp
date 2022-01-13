#include "staticPropertiesHandler.h"
#include "systemHandling.h"

#include <logMe/logMe.h>
#include <omp.h>
#ifdef DTOO_HAS_MPI  
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/environment.hpp>
#endif

namespace dtOO {
  dt__pH(staticPropertiesHandler) staticPropertiesHandler::_pH(NULL);
  
  staticPropertiesHandler::staticPropertiesHandler() 
  : optionHandling(), _initialized(false) {
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
		setOption("root_printLevel", "0");
    setOption("root_maxIterations", "1000");
    setOption("root_maxFunctionCalls", "10000");
    setOption("workingDirectory", systemHandling::currentDirectory());
    setOption("reparamOnFace_minimizer", ":Minuit2:kMigrad:");
    setOption("reparamInVolume_minimizer", ":Minuit2:kMigrad:");
    setOption("ompNumThreads", "1");
    setOption("map1dTo3d_deltaPer", "0.01");    
    setOption("map2dTo3d_deltaPer", "0.01");    
    setOption("map3dTo3d_deltaPer", "0.01");   
    setOption("transfinite_correctionLoops", "10");   
    setOption("isEqualExtendCheck", "false");
    setOption("logLevel", "0");
    
    _gDebug = false;
  }

  staticPropertiesHandler * staticPropertiesHandler::getInstance( void ) {
    if (_pH.isNull()) {    
      _pH.reset( new staticPropertiesHandler() );
    }
    return _pH.get();
  }
  
  staticPropertiesHandler::~staticPropertiesHandler() {
  }
  
  void staticPropertiesHandler::init(::QDomElement const * const wElement) {
    if (_initialized) return;
    
    optionHandling::init(wElement);
    
    omp_set_num_threads( getOptionInt("ompNumThreads") );
    _nRanks = 1;
    _thisRank = 0;    
#ifdef DTOO_HAS_MPI    
    if ( ::boost::mpi::environment::initialized() ) {
      _nRanks = ::boost::mpi::communicator().size();
      _thisRank = ::boost::mpi::communicator().rank();
    }
#endif    
    FILELog::ReportingLevel() = TLogLevel(getOptionInt("logLevel"));
    
    dt__forceInfo(
      init(),
      << dt__eval(FILELog::ReportingLevel()) << std::endl
      << dt__eval(omp_get_num_threads()) << std::endl
      << dt__eval(omp_get_thread_limit()) << std::endl
      << dt__eval(omp_get_max_threads()) << std::endl
      << dt__eval(mpiParallel()) << std::endl
      << dt__eval(thisRank()) << std::endl
      << dt__eval(nRanks()) << std::endl
      << dt__eval(_gDebug)
    );
    
    _initialized = true;
  }
  
  bool staticPropertiesHandler::mpiParallel( void ) {
#ifdef DTOO_HAS_MPI        
    return ::boost::mpi::environment::initialized();
#else
    return false;
#endif
  }

  void staticPropertiesHandler::mpiBarrier( void ) {
#ifdef DTOO_HAS_MPI        
    ::boost::mpi::communicator().barrier();
#endif
  }
  
  int staticPropertiesHandler::thisRank( void ) const {
    return _thisRank;
  }

  int staticPropertiesHandler::nRanks( void ) const {
    return _nRanks;
  }  
  
  bool staticPropertiesHandler::initialized( void ) const {
    return _initialized;
  }
  
  bool staticPropertiesHandler::gDebug( void ) const {
    return _gDebug;
  }
  
  void staticPropertiesHandler::setGDebug( void ) {
    _gDebug = true;
  }
}
