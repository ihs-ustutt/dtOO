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

#ifndef UcylInChannel_H
#define UcylInChannel_H

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

class UcylInChannel : public dtPlugin {
public:
  dt__class(UcylInChannel, dtPlugin);
  dt__classSelfCreate(UcylInChannel);
  UcylInChannel();
  virtual ~UcylInChannel();
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
  vectorHandling<analyticGeometry const *> _aG;
  dtXmlParser const *_parser;
  dtCase const *_case;
  //    std::string _field;
  std::vector<dtInt> _nP;
  dtVector3 _axis;
  dtPoint3 _origin;
  static bool _registrated;
};
} // namespace dtOO

#endif /* UcylInChannel_H */
