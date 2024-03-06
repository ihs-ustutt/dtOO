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

#ifndef BOUNDEDVOLUMEFACTORY_H
#define	BOUNDEDVOLUMEFACTORY_H

#include <dtOOTypeDef.h>

#include <string>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class boundedVolume;
  
  class boundedVolumeFactory {
    public:
      dt__classOnlyName(boundedVolumeFactory);
      virtual ~boundedVolumeFactory();
      static bool registrate( boundedVolume const * const );
      static boundedVolume * create(char const * const str);
      static boundedVolume * create( std::string const str );
    private:
      boundedVolumeFactory();
    private:
      static dt__pVH(boundedVolume) _product;
  };
}

#endif	/* BOUNDEDVOLUMEFACTORY_H */
