#ifndef POINTCONTAINER_H
#define	POINTCONTAINER_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <string>
#include <logMe/dtMacros.h>

namespace dtOO {
  class pointContainer {
    public:    
      dt__classOnlyName(pointContainer);
      pointContainer();
      pointContainer(const pointContainer& orig);
      virtual ~pointContainer();
      void add(dtPoint3 const point, std::string const string);
      dtPoint3 get(std::string const string ) const;
      dtPoint3 get(std::string const * const stringP) const;
      bool has(std::string const string) const;
      bool has(std::string const * const stringP) const;
      bool isEmpty( void ) const;
      std::vector< std::string > labels( void ) const;
    private:
      std::vector< std::pair <dtPoint3, std::string > > _pair;
  };
}
#endif	/* POINTCONTAINER_H */

