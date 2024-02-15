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

#ifndef WRITESTEP_H
#define	WRITESTEP_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

#include "dtPlugin.h"

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  class baseContainer;
  class dtCase;
  
  class writeStep : public dtPlugin {
  public:
    dt__class(writeStep, dtPlugin);
    dt__classSelfCreate(writeStep);    
    writeStep();
    virtual ~writeStep();
    virtual void init( 
      ::QDomElement const & element,
      baseContainer * const bC,
      lvH_constValue const * const cV,
      lvH_analyticFunction const * const aF,
      lvH_analyticGeometry const * const aG,
      lvH_boundedVolume const * const bV,
      lvH_dtCase const * const dC,      
      lvH_dtPlugin const * const pL
    );    
    virtual void apply(void);
  private:
    lvH_analyticGeometry _aG;
    std::string _fn;
    dtInt _translator;
    static bool _registrated;
  };
}
#endif	/* WRITESTEP_H */
