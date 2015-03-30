#ifndef LABELHANDLING_H
#define	LABELHANDLING_H

#include <string>
#include <logMe/dtMacros.h>

namespace dtOO {
  class labelHandling {
  public:
    dt__CLASSNAME(labelHandling);
    labelHandling();
    labelHandling(const labelHandling& orig);
    labelHandling(std::string const & label);
    virtual ~labelHandling();
    void setLabel(std::string const label);
    void setLabel(char const * const label);
    std::string getLabel( void ) const;
  private:
    std::string _label;
  };
}
#endif	/* LABELHANDLING_H */

