#ifndef DTARG_H
#define	DTARG_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <boost/program_options.hpp>
namespace dtPO = boost::program_options;
typedef dtPO::options_description dtArgDesc;
typedef dtPO::variables_map dtArgMap;

namespace dtOO {
  class dtArg : public dtArgMap {
  public:
    dt__classOnlyName(dtArg);    
    dtArg( std::string const & appName, dtInt argC, char const * const argV[] );
    virtual ~dtArg();
    void setMachine( void );
    dtArgDesc & description( void );
    void update( void );
    std::string callCommand(void);
  private:
    dtArgDesc _desc;
    dtInt _argC;
    char const * const * _argV;
  };
}

#endif	/* DTARG_H */

