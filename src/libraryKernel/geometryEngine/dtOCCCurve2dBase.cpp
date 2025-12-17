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

#include "dtOCCCurve2dBase.h"

#include <Geom2d_Curve.hxx>
#include <logMe/logMe.h>

namespace dtOO {
struct dtOCCCurve2dBase::OCCHanWrap {
  Handle(Geom2d_Curve) occHandle;
};

dtOCCCurve2dBase::dtOCCCurve2dBase() : _hanWrap(new OCCHanWrap) {}

dtOCCCurve2dBase::dtOCCCurve2dBase(Handle(Geom2d_Curve) orig)
  : _hanWrap(new OCCHanWrap())
{
  _hanWrap->occHandle = orig;
}

void dtOCCCurve2dBase::setOCC(Handle(Geom2d_Curve) occCurve)
{
  dt__throwIf(occCurve.IsNull(), setOCC());

  _hanWrap->occHandle = occCurve;
}

Handle(Geom2d_Curve) const dtOCCCurve2dBase::getOCC(void) const
{
  dt__throwIf(_hanWrap->occHandle.IsNull(), getOCC());

  return _hanWrap->occHandle;
}

std::string dtOCCCurve2dBase::dumpToString(void) const
{
  std::stringstream ss;

  Handle(Geom2d_Curve) const &cH = getOCC();
  Handle(Standard_Type) const &stH = cH->DynamicType();

  ss << dt__dumpToString(
    << dt__eval(stH->Name()) << std::endl
    << dt__eval(_hanWrap->occHandle.get())
  );

  return ss.str();
}

dtOCCCurve2dBase::~dtOCCCurve2dBase() {}
} // namespace dtOO
