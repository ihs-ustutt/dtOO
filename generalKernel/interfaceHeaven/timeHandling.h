#ifndef TIMEHANDLING_H
#define	TIMEHANDLING_H

#include <logMe/dtMacros.h>

namespace boost {
  namespace timer {
    class auto_cpu_timer;
  }
}

namespace dtOO {
  class timeHandling {
  public:
    dt__classOnlyName(timeHandling);    
    timeHandling(std::string const & keyword);
    virtual ~timeHandling();
  private:
    std::string _keyword;
    std::ostringstream _stream;
    ::boost::timer::auto_cpu_timer * _t;    
  };
}
#endif	/* TIMEHANDLING_H */