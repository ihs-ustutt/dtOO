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

#ifndef OPTIONHANDLING_H
#define	OPTIONHANDLING_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "vectorHandling.h"
#include <string>
#include <vector>

#include <mainConceptFwd.h>
#include <jsonHeaven/jsonPrimitive.h>

class QDomElement;

namespace dtOO {
  typedef std::pair< std::vector< std::string >, dtReal > optionGroupElement;
  typedef std::pair< std::vector< std::string >, dtReal > optionGroupElementInt;
  typedef std::vector< optionGroupElement > optionGroup;
  typedef std::vector< optionGroupElementInt > optionGroupInt;
  
  class baseContainer;
  
  class optionHandling {
    public:    
      dt__classOnlyName(optionHandling);
      optionHandling();
      optionHandling(const optionHandling& orig);
      virtual ~optionHandling();
      virtual void init(::QDomElement const * const wElement);
      virtual void jInit( jsonPrimitive const & jE );
      virtual void init(
        ::QDomElement const & wElement,
        baseContainer const * const bC,      
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG
      );      
      void setOption(std::string const name, std::string const value);
      std::string getOption(
        std::string const name, std::string const val
      ) const;
      std::string getOption(std::string const name) const;
      dtReal getOptionFloat(std::string const name) const;
      dtInt getOptionInt(std::string const name) const;
      bool optionTrue(std::string const name) const;
      bool hasOption(std::string const name) const;
      optionGroup getOptionGroup( std::string const name ) const;
      optionGroupInt getOptionGroupInt( std::string const name ) const;
      bool debugTrue( void ) const;
    private:
      std::vector< std::string > _optionName;
      std::vector< std::string > _optionValue;
  };
}

#endif	/* OPTIONHANDLING_H */
