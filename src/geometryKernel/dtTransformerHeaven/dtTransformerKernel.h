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

#ifndef dtTransformerKernel_H
#define dtTransformerKernel_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <pluginEngine/pugg/Kernel.h>

namespace dtOO {
class dtTransformerKernel : public ::pugg::Kernel {
public:
  dt__classOnlyName(dtTransformerKernel);
  ~dtTransformerKernel();

private:
};
} // namespace dtOO

#endif /* dtTransformerKernel_H */
