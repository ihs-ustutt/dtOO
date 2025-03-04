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

#ifndef dtParMacros_H
#define dtParMacros_H

#include <dtOOTypeDef.h>

#include <interfaceHeaven/staticPropertiesHandler.h>

#define dt__onlyMaster                                                         \
  if (dtOO::staticPropertiesHandler::getInstance()->thisRank() == 0)

#endif /* dtParMacros_H */
