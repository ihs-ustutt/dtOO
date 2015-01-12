#ifndef SYSTEMHANDLING_H
#define	SYSTEMHANDLING_H

#include <logMe/dtMacros.h>
#include <string>

namespace dtOO {
  class systemHandling {
  public:
    dt__CLASSNAME(systemHandling);
    virtual ~systemHandling();
    static void command(std::string & cmd);
    static void commandAndWait(std::string & cmd);
    static bool createDirectory( std::string const & dirPath );
    static bool fileExists(std::string const filename);
    static void deleteFile( std::string const filename );
  private:
    systemHandling();    
  };
}

#endif	/* SYSTEMHANDLING_H */

