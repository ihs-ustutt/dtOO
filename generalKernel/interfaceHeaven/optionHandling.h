#ifndef OPTIONHANDLING_H
#define	OPTIONHANDLING_H

#include <logMe/dtMacros.h>
#include "vectorHandling.h"
#include <string>
#include <vector>

class QDomElement;

namespace dtOO {
  typedef std::pair< std::vector< std::string >, float > optionGroupElement;
  typedef std::pair< std::vector< std::string >, float > optionGroupElementInt;
  typedef std::vector< optionGroupElement > optionGroup;
  typedef std::vector< optionGroupElementInt > optionGroupInt;
  
  class constValue;
  class analyticFunction;
  
  class optionHandling {
    public:    
      dt__CLASSNAME(optionHandling);
      optionHandling();
      optionHandling(const optionHandling& orig);
      virtual ~optionHandling();
      virtual void init(QDomElement const * const wElement);
      void setOption(std::string const name, std::string const value);
      std::string getOption(std::string const name, std::string const val) const;
      std::string getOption(std::string const name) const;
      float getOptionFloat(std::string const name) const;
      float getOptionFloat(
        std::string const name,
        vectorHandling< constValue * > const * const cV,
        vectorHandling< analyticFunction * > const * const aF					
      ) const;
      int getOptionInt(std::string const name) const;
      int getOptionInt(
        std::string const name,
        vectorHandling< constValue * > const * const cV,
        vectorHandling< analyticFunction * > const * const aF					
      ) const;
      bool optionTrue(std::string const name) const;
      bool hasOption(std::string const name) const;
      optionGroup getOptionGroup( std::string const name ) const;
      optionGroupInt getOptionGroupInt( std::string const name ) const;
    private:
      std::vector< std::string > _optionName;
      std::vector< std::string > _optionValue;
      optionGroupElement _emptyOptionGroupElement;
  };
}

#endif	/* OPTIONHANDLING_H */

