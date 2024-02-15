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
    if ( !Stream().is_open() ) return;
    Stream() << msg.c_str();
		Stream().flush();
  }

}
