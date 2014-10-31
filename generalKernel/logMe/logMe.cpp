#include "logMe.h"

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <cxxabi.h>

namespace dtOO {
  // This function produces a stack backtrace with demangled function & method names.
  std::string Backtrace(void) {
		int skip = 1;
		void *callstack[128];
		const int nMaxFrames = sizeof(callstack) / sizeof(callstack[0]);
		char buf[1024];
		int nFrames = backtrace(callstack, nMaxFrames);
		char **symbols = backtrace_symbols(callstack, nFrames);

		std::ostringstream trace_buf;
		trace_buf << LOGDEL << "\n+---------------------+\n|\n";
		trace_buf <<  "+-- Backtrace\n";
		for (int i = skip; i < nFrames; i++) {
			//printf("%s\n", symbols[i]);

			Dl_info info;
			if (dladdr(callstack[i], &info) && info.dli_sname) {
				char *demangled = NULL;
				int status = -1;
				if (info.dli_sname[0] == '_')
					demangled = abi::__cxa_demangle(info.dli_sname, NULL, 0, &status);
				snprintf(
					buf, sizeof(buf), "%-3d %*p %s + %zd\n",
					i, int(2 + sizeof(void*) * 2), callstack[i],
					status == 0 ? demangled :
					info.dli_sname == 0 ? symbols[i] : info.dli_sname,
					(char *)callstack[i] - (char *)info.dli_saddr
				);
				free(demangled);
			} else {
				snprintf(
					buf, sizeof(buf), "%-3d %*p %s\n",
					i, int(2 + sizeof(void*) * 2), callstack[i], symbols[i]
				);
			}
			trace_buf << buf;
		}
		free(symbols);
		if (nFrames == nMaxFrames) trace_buf << "[truncated]\n";
		trace_buf <<  "+-- End Backtrace \n|\n+---------------------+";
		trace_buf << LOGDEL;
		return trace_buf.str();
	}	
	
  std::string floatVecToString( std::vector< float > const & vec, int const grouping ) {
    std::ostringstream os;
    os << "[" << LOGDEL;
    int ii = 0;
    while( ii < vec.size() ) {
      for (int jj=0;jj<grouping;jj++) {
        os << vec[ii] << " ";
        ii++;
        if (ii == vec.size()) break;
      }
      os << LOGDEL;
    }
    os << "]";
    return os.str();
  }
  
  std::string floatVecToTable( std::vector<std::string> const & header, std::vector< float > const & vec ) {
    std::ostringstream os;
    for (int ii=0; ii<header.size(); ii++) {
     os << boost::format("| %13s ") % header[ii];
    }
    os << LOGDEL;
    int ii = 0;
    int grouping = header.size();
    while( ii < vec.size() ) {
      for (int jj=0;jj<grouping;jj++) {
        os << boost::format("| %+11.6e ") % vec[ii];
        ii++;
        if (ii == vec.size()) break;
      }
      os << LOGDEL;
    }
    return os.str();
  } 

  std::string floatVecToTable( 
	  std::vector<std::string> const & addInfo, 
		std::vector<std::string> const & header, 
		std::vector< float > const & vec 
	) {
    std::ostringstream os;
    for (int ii=0; ii<addInfo.size(); ii++) {		
		  os << addInfo[ii] << LOGDEL;
		}
    for (int ii=0; ii<header.size(); ii++) {
     os << boost::format("| %13s ") % header[ii];
    }
    os << LOGDEL;
    int ii = 0;
    int grouping = header.size();
    while( ii < vec.size() ) {
      for (int jj=0;jj<grouping;jj++) {
        os << boost::format("| %+11.6e ") % vec[ii];
        ii++;
        if (ii == vec.size()) break;
      }
      os << LOGDEL;
    }
    return os.str();
  } 	

  std::string floatMatrixToString( std::vector< std::vector< float > > const & mat ) {
    std::ostringstream os;
    
    int nU = mat.size();
    int nV = mat[0].size();
    os << boost::format("| %13s ") % "i   |   j>>";
    for (int jj=0; jj<nV; jj++) {
      os << boost::format("| %13i ") % jj;
    }
    os << LOGDEL;
    for (int ii=0; ii<nU; ii++) {
      os << boost::format("| %13i ") % ii;      
			for (int jj=0; jj<nV; jj++) {
        os << boost::format("| %+11.6e ") % mat[ii][jj];
			}
      os << LOGDEL;
		}
    os << LOGDEL;
    return os.str();
  }  
}