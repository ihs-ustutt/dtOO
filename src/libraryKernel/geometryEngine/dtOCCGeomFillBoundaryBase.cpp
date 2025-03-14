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

#include "dtOCCGeomFillBoundaryBase.h"

#include <GeomFill_Boundary.hxx>
#include <logMe/logMe.h>

namespace dtOO {
struct dtOCCGeomFillBoundaryBase::OCCHanWrap {
  Handle_GeomFill_Boundary occHandle;
};

dtOCCGeomFillBoundaryBase::dtOCCGeomFillBoundaryBase()
  : _hanWrap(new OCCHanWrap)
{
}

dtOCCGeomFillBoundaryBase::dtOCCGeomFillBoundaryBase(
  Handle_GeomFill_Boundary &orig
)
  : _hanWrap(new OCCHanWrap())
{
  _hanWrap->occHandle = orig;
}

void dtOCCGeomFillBoundaryBase::setOCC(Handle_GeomFill_Boundary occCurve)
{
  dt__throwIf(occCurve.IsNull(), setOCC());

  _hanWrap->occHandle = occCurve;
}

Handle_GeomFill_Boundary const dtOCCGeomFillBoundaryBase::getOCC(void) const
{
  dt__throwIf(_hanWrap->occHandle.IsNull(), getOCC());

  return _hanWrap->occHandle;
}

std::string dtOCCGeomFillBoundaryBase::dumpToString(void) const
{
  std::stringstream ss;

  Handle(GeomFill_Boundary) const &bH = getOCC();
  Handle(Standard_Type) const &stH = bH->DynamicType();

  ss << dt__dumpToString(
    << dt__eval(stH->Name()) << std::endl
    << dt__eval(_hanWrap->occHandle.get())
  );

  return ss.str();
}

dtOCCGeomFillBoundaryBase::~dtOCCGeomFillBoundaryBase() {}
} // namespace dtOO
