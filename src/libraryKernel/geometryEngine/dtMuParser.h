#ifndef dtMuParser_H
#define	dtMuParser_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <muParser.h>

namespace dtOO {
  class dtMuParser : public ::mu::Parser {
  public:
    dt__classOnlyName(dtMuParser);    
    dtMuParser();
    dtMuParser(const dtMuParser& orig);
    virtual ~dtMuParser();
  private:

  };
}
#endif	/* dtMuParser_H */

