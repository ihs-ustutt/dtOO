#ifndef EGENERAL_H
#define	EGENERAL_H

#include <logMe/dtMacros.h>
#include <string>
#include <vector>
#include <sstream>
#include <exception>

namespace dtOO {
  class eGeneral : public std::exception {
  public:
    dt__CLASSNAME(eGeneral);
    eGeneral() _GLIBCXX_USE_NOEXCEPT;
    eGeneral( std::ostream & msg) _GLIBCXX_USE_NOEXCEPT;
    virtual ~eGeneral() _GLIBCXX_USE_NOEXCEPT;
    void clear(void);
    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT;
  private:
    std::string _exceptionMsg;
  };
}
#endif	/* EGENERAL_H */

