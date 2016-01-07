#ifndef SYSTEMHANDLING_H
#define	SYSTEMHANDLING_H

#include <logMe/dtMacros.h>
#include <string>
#include <boost/filesystem.hpp>

namespace dtOO {
  typedef ::boost::filesystem::path fpath;
  
  class systemHandling {
  public:
    dt__classOnlyName(systemHandling);
    virtual ~systemHandling();
    static void command(std::string const & cmd);
    static std::string commandAndWait(std::string const & cmd);
    static bool createDirectory( std::string const & dirPath );
    static bool fileExists(std::string const & filename);
    static bool directoryExists(std::string const & dirname);
    static void deleteFile( std::string const & filename );
    static void deleteDirectory( std::string const & dirname );    
    static void copyDirectory( std::string const & from, std::string const & to);
    static std::string currentDirectory( void );
    static void unsetEnv(std::string const & envName);
    static std::vector< fpath > directoryList( 
      std::string const & path 
    );
  private:
    systemHandling();    
  };
}

#endif	/* SYSTEMHANDLING_H */

