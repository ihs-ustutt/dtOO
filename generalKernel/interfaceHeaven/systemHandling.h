#ifndef SYSTEMHANDLING_H
#define	SYSTEMHANDLING_H

#include <logMe/dtMacros.h>
#include <string>

namespace dtOO {
  class systemHandling {
  public:
    dt__CLASSNAME(systemHandling);
    systemHandling();
    systemHandling(const systemHandling& orig);
    virtual ~systemHandling();
    void command(std::string & cmd) const;
    bool createDirectory( std::string const & dirPath ) const;
    bool fileExists(std::string const filename) const;
    void deleteFile( std::string const filename ) const;
  private:
  };
}

#endif	/* SYSTEMHANDLING_H */

