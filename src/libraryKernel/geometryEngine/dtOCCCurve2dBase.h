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

#ifndef dtOCCCurve2dBase_H
#define dtOCCCurve2dBase_H

#include <dtOOTypeDef.h>

#include <interfaceHeaven/ptrHandling.h>
// clang-format off
#include <dtLinearAlgebra.h>
#include <Geom2d_Curve.hxx>
// clang-format on
#include <logMe/dtMacros.h>

namespace dtOO {
class dtOCCCurve2dBase {
public:
  dt__classOnlyName(dtOCCCurve2dBase);
  dtOCCCurve2dBase();
  dtOCCCurve2dBase(Handle(Geom2d_Curve) orig);
  virtual ~dtOCCCurve2dBase();
  void setOCC(Handle(Geom2d_Curve) occCurve);
  Handle(Geom2d_Curve) const getOCC(void) const;
  std::string dumpToString(void) const;

private:
  class OCCHanWrap;
  dt__pH(OCCHanWrap) _hanWrap;
};
} // namespace dtOO
#endif /* dtOCCCurve2dBase_H */
