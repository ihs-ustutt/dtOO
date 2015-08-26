#ifndef SYSTEMHANDLING_H
#define	SYSTEMHANDLING_H

#include <logMe/dtMacros.h>
#include <string>

namespace dtOO {
  class systemHandling {
  public:
    dt__classOnlyName(systemHandling);
    virtual ~systemHandling();
    static void command(std::string const & cmd);
    static void commandAndWait(std::string const & cmd);
    static bool createDirectory( std::string const & dirPath );
    static bool fileExists(std::string const & filename);
    static bool directoryExists(std::string const & dirname);
    static void deleteFile( std::string const & filename );
    static void deleteDirectory( std::string const & dirname );    
    static void copyDirectory( std::string const & from, std::string const & to);
    static std::string currentDirectory( void );
  private:
    systemHandling();    
  };
}

#endif	/* SYSTEMHANDLING_H */

