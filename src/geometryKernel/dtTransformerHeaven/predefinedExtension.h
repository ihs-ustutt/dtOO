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

#ifndef PREDEFINEDEXTENSION_H
#define PREDEFINEDEXTENSION_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
class analyticFunction;

class predefinedExtension : public dtTransformer {
public:
  dt__classOnlyName(predefinedExtension);
  predefinedExtension();
  predefinedExtension(predefinedExtension const &orig);
  virtual ~predefinedExtension();
  virtual dtTransformer *clone(void) const;
  virtual dtTransformer *create(void) const;
  virtual bool isNecessary(void) const;
  virtual void init(
    ::QDomElement const *tE,
    baseContainer *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG
  );
  virtual lvH_analyticFunction apply(lvH_analyticFunction const *const sFunP
  ) const;

private:
  std::vector<dtPoint2> calculateExtPoints(
    dtPoint2 const &start,
    dtVector2 const &vvStart,
    dtPoint2 const &end,
    dtVector2 const &vvEnd
  ) const;

private:
  dtReal _yIn;
  dtReal _yOut;
  dtReal _alphaIn;
  dtReal _alphaOut;
  static bool _registrated;
};
} // namespace dtOO

#endif /* PREDEFINEDEXTENSION_H */
