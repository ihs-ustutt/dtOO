#ifndef TIMEHANDLING_H
#define	TIMEHANDLING_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>

namespace boost {
  namespace timer {
    class cpu_timer;
  }
}

namespace dtOO {
  class timeHandling {
  public:
    dt__classOnlyName(timeHandling);    
    timeHandling(std::string const & keyword);
    virtual ~timeHandling();
    void output( void );
  private:
    std::string _keyword;
    dt__pH(::boost::timer::cpu_timer) _t;
    int _step;
  };
}
#endif	/* TIMEHANDLING_H */