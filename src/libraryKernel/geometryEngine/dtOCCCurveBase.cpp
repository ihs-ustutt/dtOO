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

#include "dtOCCCurveBase.h"

#include <Geom_Curve.hxx>
#include <logMe/logMe.h>

namespace dtOO {
struct dtOCCCurveBase::OCCHanWrap {
  Handle_Geom_Curve occHandle;
};

dtOCCCurveBase::dtOCCCurveBase() : _hanWrap(new OCCHanWrap) {}

dtOCCCurveBase::dtOCCCurveBase(Handle_Geom_Curve orig)
  : _hanWrap(new OCCHanWrap())
{
  _hanWrap->occHandle = orig;
}

void dtOCCCurveBase::setOCC(Handle_Geom_Curve occCurve)
{
  dt__throwIf(occCurve.IsNull(), setOCC());

  _hanWrap->occHandle = occCurve;
}

Handle_Geom_Curve const dtOCCCurveBase::getOCC(void) const
{
  dt__throwIf(_hanWrap->occHandle.IsNull(), getOCC());

  return _hanWrap->occHandle;
}

std::string dtOCCCurveBase::dumpToString(void) const
{
  std::stringstream ss;

  Handle(Geom_Curve) const &cH = getOCC();
  Handle(Standard_Type) const &stH = cH->DynamicType();

  ss << dt__dumpToString(
    << dt__eval(stH->Name()) << std::endl
    << dt__eval(_hanWrap->occHandle.get())
  );

  return ss.str();
}

dtOCCCurveBase::~dtOCCCurveBase() {}
} // namespace dtOO
