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

#ifndef volVectorInChannelFieldRange_H
#define volVectorInChannelFieldRange_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

#include "dtPlugin.h"

namespace dtOO {
class dtXmlParser;
class constValue;
class analyticFunction;
class analyticGeometry;
class boundedVolume;
class baseContainer;
class dtCase;

class volVectorInChannelFieldRange : public dtPlugin {
public:
  dt__class(volVectorInChannelFieldRange, dtPlugin);
  dt__classSelfCreate(volVectorInChannelFieldRange);
  volVectorInChannelFieldRange();
  virtual ~volVectorInChannelFieldRange();
  virtual void init(
    ::QDomElement const &element,
    baseContainer *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG,
    lvH_boundedVolume const *const bV,
    lvH_dtCase const *const dC,
    lvH_dtPlugin const *const pL
  );
  virtual void apply(void);

private:
  analyticGeometry const *_aG;
  dtXmlParser const *_parser;
  dtCase const *_case;
  std::string _field;
  dtReal _min;
  dtReal _max;
  std::vector<dtInt> _nP;
  bool _noRange;
  static bool _registrated;
};
} // namespace dtOO

#endif /* volVectorInChannelFieldRange_H */
