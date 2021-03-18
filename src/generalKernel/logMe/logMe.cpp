#include "logMe.h"

#include <dtOOVersion.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <cxxabi.h>
#include <string.h>

namespace dtOO {
	std::string logMe::initLog( std::string const & logFileName ) {
    //
    // close first
    //
		if ( Output2FILE::Stream().is_open() ) Output2FILE::Stream().close();
    
    //
    // set file
    //
		std::string nameStr = logFileName;
		Output2FILE::Stream().open( 
      nameStr, std::ofstream::out | std::ofstream::trunc 
    );
		if (!Output2FILE::Stream().good()) {
			nameStr = logFileName+NowDateAndTime()+".log";
			Output2FILE::Stream().open( 
        nameStr, std::ofstream::out | std::ofstream::trunc 
      );
			if ( !Output2FILE::Stream().good()) {
				dt__throw(compute, << "Cannot open log file " << dt__eval(nameStr) );
			}
    }
    
    dt__info(
      compute(), 
      << "**********************************************"
         "**********************************" << std::endl
      << "* " << std::endl
      << "* (d)esign (t)ool (O)bject-(O)riented" << std::endl                    
      << "* " << std::endl
      << "* version : " << dtOO::git::versionMajor
      << "." << dtOO::git::versionMinor
      << "." << dtOO::git::versionCommitNo
      << " ( " << dtOO::git::versionName << " )" << std::endl
      << "* compiled " << __DATE__ << " " << __TIME__ << std::endl            
      << "* " << std::endl
      << "**********************************************"
         "**********************************" << std::endl
      << " "
		);		
		
		return nameStr;
	}
  
  void logMe::closeLog( void ) {
		if (Output2FILE::Stream().is_open()) {
			Output2FILE::Stream().close();
		}
  }
	
  // Produces a stack backtrace with demangled function & method names.
  std::string logMe::Backtrace(void) {
		int skip = 1;
		void *callstack[128];
		const int nMaxFrames = sizeof(callstack) / sizeof(callstack[0]);
		char buf[1024];
		int nFrames = backtrace(callstack, nMaxFrames);
		char **symbols = backtrace_symbols(callstack, nFrames);

		std::ostringstream trace_buf;
		trace_buf 
		<< std::endl 
		<<  "Backtrace\n";
		for (int i = skip; i < nFrames; i++) {
			//printf("%s\n", symbols[i]);

			Dl_info info;
			if (dladdr(callstack[i], &info) && info.dli_sname) {
				char *demangled = NULL;
				int status = -1;
				if (info.dli_sname[0] == '_')
					demangled = abi::__cxa_demangle(info.dli_sname, NULL, 0, &status);
				snprintf(
					buf, sizeof(buf), "[ %-3d ] %*p %s + %zd\n",
					i, int(2 + sizeof(void*) * 2), callstack[i],
					status == 0 ? demangled :
					info.dli_sname == 0 ? symbols[i] : info.dli_sname,
					(char *)callstack[i] - (char *)info.dli_saddr
				);
				free(demangled);
			} else {
				snprintf(
					buf, sizeof(buf), "[ %-3d ] %*p %s\n",
					i, int(2 + sizeof(void*) * 2), callstack[i], symbols[i]
				);
			}
			trace_buf << buf;
		}
		free(symbols);
		if (nFrames == nMaxFrames) trace_buf << "[truncated]\n";
		trace_buf << std::endl;
		return trace_buf.str();
	}	

	/**
	 * 
   * @todo Make me template.
   */
  std::string logMe::floatMatrixToString( 
	  std::vector< std::vector< float > > const & mat 
	) {
    std::ostringstream os;
    
    int nU = mat.size();
    int nV = mat[0].size();
    os << boost::format("| %13s ") % "i   |   j>>";
    for (int jj=0; jj<nV; jj++) {
      os << boost::format("| %13i ") % jj;
    }
    os << std::endl;
    for (int ii=0; ii<nU; ii++) {
      os << boost::format("| %13i ") % ii;      
			for (int jj=0; jj<nV; jj++) {
        os << boost::format("| %+11.6e ") % mat[ii][jj];
			}
      os << std::endl;
		}
    os << std::endl;
    return os.str();
  }  
}