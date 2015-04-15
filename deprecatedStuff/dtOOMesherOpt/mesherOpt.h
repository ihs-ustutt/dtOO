#ifndef MESHEROPT_H
#define	MESHEROPT_H

#include <mesher.h>

namespace dtOO {  
  class mesherOpt : public mesher {
  public:
    dt__classOnlyName(mesherOpt);
    mesherOpt(int argc, char *argv[]);    
    virtual int compute(char const * str);    
    virtual ~mesherOpt();
  private:
      covise::coOutputPort * _pOut_optiControlP;
  };
}
#endif	/* MESHEROPT_H */

