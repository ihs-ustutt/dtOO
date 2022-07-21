#ifndef LABELHANDLING_H
#define	LABELHANDLING_H

#include <dtOOTypeDef.h>

#include <string>
#include <logMe/dtMacros.h>

namespace dtOO {
  class labelHandling {
  public:
    dt__classOnlyName(labelHandling);
    labelHandling();
    labelHandling(const labelHandling& orig);
    labelHandling(std::string const & label);
    virtual ~labelHandling();
    void setLabel(std::string const label);
    std::string getLabel( void ) const;
  private:
    std::string _label;
  };
}
#endif	/* LABELHANDLING_H */

