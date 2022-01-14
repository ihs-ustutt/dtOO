#ifndef OUTPUT2FILE_H
#define	OUTPUT2FILE_H

#include <dtOOTypeDef.h>

#include <fstream>
#include "logBase.h"

namespace dtOO {
  class Output2FILE {
  public:
    static std::ofstream& Stream();
    static void Output(const std::string& msg, TLogLevel level);
    static void OutputFile(const std::string& msg, TLogLevel level);
  private:
    Output2FILE();
  };
}
#endif	/* OUTPUT2FILE_H */

