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

#ifndef OFFSET_H
#define OFFSET_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
class offset : public dtTransformer {
public:
  dt__classOnlyName(offset);
  offset();
  offset(offset const &orig);
  virtual ~offset();
  virtual dtTransformer *clone(void) const;
  virtual dtTransformer *create(void) const;
  virtual bool isNecessary(void) const;
  void init(
    ::QDomElement const *tE,
    baseContainer *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG
  );
  virtual lvH_analyticGeometry apply(lvH_analyticGeometry const *const aGeoVecP
  ) const;
  virtual lvH_analyticFunction apply(lvH_analyticFunction const *const toTrans
  ) const;

private:
  dtReal _paraOneOffsetPercent;
  dtReal _paraTwoOffsetPercent;
  bool _xyPercent;
  dtReal _xOffset;
  dtReal _yOffset;
  dtReal _normalOffset;
  dtInt _nPieces;
  static bool _registrated;
};
} // namespace dtOO

#endif /* OFFSET_H */
