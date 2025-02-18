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

#include "dtAnalysis.h"
#include <interfaceHeaven/stringPrimitive.h>
#include <logMe/logMe.h>

#include <RConfigure.h>
#include <Math/GSLNLSMinimizer.h>
#include <Math/GSLMinimizer.h>
#include <Math/GSLSimAnMinimizer.h>
#include <Minuit2/Minuit2Minimizer.h>
#include <Math/Functor.h>
#include <Rtypes.h>

namespace dtOO {
	dtAnalysis::dtAnalysis() {
	}

	dtAnalysis::dtAnalysis(const dtAnalysis& orig) {
	}

	dtAnalysis::~dtAnalysis() {
	}

  dtMinimizer * dtAnalysis::createMinimizer( std::string minType ) {
    std::string mG 
    = 
    stringPrimitive::getStringBetweenAndRemove(":", ":", &minType);
    std::string mT
    = 
    stringPrimitive::getStringBetweenAndRemove(":", ":", &minType);
    
    if (mG == "Minuit2") {
      return new ::ROOT::Minuit2::Minuit2Minimizer(mT.c_str());      
    }
    else if (mG == "GSLMinimizer") {
      return new ::ROOT::Math::GSLMinimizer(mT.c_str());      
    }    
    else if (mG == "GSLNLSMinimizer") {
      return new ::ROOT::Math::GSLNLSMinimizer();      
    }        
    else if (mG == "GSLSimAnMinimizer") {
      return new ::ROOT::Math::GSLSimAnMinimizer();      
    }        
    else dt__throwUnexpected(createMinimizer());
  }  	
}
