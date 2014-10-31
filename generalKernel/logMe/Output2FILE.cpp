#include "Output2FILE.h"
#include "logBase.h"
#include <string>

namespace dtOO {
	Output2FILE::Output2FILE() {
		
	}
	
  std::ofstream& Output2FILE::Stream() {
    static std::ofstream file;//("logfile.log");// = std::cout;
    return file;
  }

  void Output2FILE::Output(const std::string& msg, TLogLevel level) { 
    OutputFile(msg, level);
  }

  void Output2FILE::OutputFile(const std::string& msg, TLogLevel level) {   
    if ( !Stream().is_open() )
    return;
    Stream() << msg.c_str();
  }

}
