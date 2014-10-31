#ifndef SFGENOPT_H
#define	SFGENOPT_H

#include <sFGen.h>

namespace dtOO {  
  class sFGenOpt : public sFGen {
  public:
    dt__CLASSNAME(sFGenOpt);
    sFGenOpt(int argc, char *argv[]);    
    virtual int compute(char const * str);    
    virtual ~sFGenOpt();
  private:
      covise::coOutputPort * _pOut_optiControlP;    
  };
}
#endif	/* SFGENOPT_H */

