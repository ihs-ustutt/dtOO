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

#ifndef bVODumpModel_H
#define bVODumpModel_H

#include <dtOOTypeDef.h>

#include "bVOInterface.h"
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/twoDArrayHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class boundedVolume;
class dtGmshEdge;

class bVODumpModel : public bVOInterface {
public:
  dt__class(bVODumpModel, bVOInterface);
  dt__classSelfCreate(bVODumpModel);
  bVODumpModel();
  virtual ~bVODumpModel();
  virtual void preUpdate(void);
  virtual void postUpdate(void);
  using bVOInterface::postUpdate;

private:
  static bool _registrated;
};
} // namespace dtOO
#endif /* bVODumpModel_H */
