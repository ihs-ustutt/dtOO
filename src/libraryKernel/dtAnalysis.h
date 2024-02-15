/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#ifndef DTANALYSIS_H
#define	DTANALYSIS_H

#include <dtOOTypeDef.h>

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
