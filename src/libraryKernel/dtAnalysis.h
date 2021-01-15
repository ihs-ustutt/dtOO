#ifndef DTANALYSIS_H
#define	DTANALYSIS_H

#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

#include <Math/GSLMinimizer.h>

namespace dtOO {
  typedef ROOT::Math::Minimizer dtMinimizer;
  
  class dtAnalysis {
    public:
      dt__classOnlyName(dtAnalysis);    
      dtAnalysis();
      dtAnalysis(const dtAnalysis& orig);
      virtual ~dtAnalysis();
      static dtMinimizer * createMinimizer( std::string minType );
    private:

  };
}
#endif	/* DTANALYSIS_H */

