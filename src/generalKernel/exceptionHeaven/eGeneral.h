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

#ifndef EGENERAL_H
#define	EGENERAL_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <string>
#include <vector>
#include <sstream>
#include <exception>

namespace dtOO {
  class eGeneral : public std::exception {
  public:
    dt__classOnlyName( eGeneral );
    eGeneral() _GLIBCXX_USE_NOEXCEPT;
    eGeneral( std::ostream & msg) _GLIBCXX_USE_NOEXCEPT;
    virtual ~eGeneral() _GLIBCXX_USE_NOEXCEPT;
    void clear(void);
    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT;
  private:
    std::string _exceptionMsg;
  };
}
#endif	/* EGENERAL_H */
