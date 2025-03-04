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

#ifndef dtOCCGeomFillBoundaryBase_H
#define dtOCCGeomFillBoundaryBase_H

#include <dtOOTypeDef.h>

#include <GeomFill_Boundary.hxx>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
// class Handle_GeomFill_Boundary;

namespace dtOO {
class dtOCCGeomFillBoundaryBase {
public:
  dt__classOnlyName(dtOCCGeomFillBoundaryBase);
  dtOCCGeomFillBoundaryBase();
  dtOCCGeomFillBoundaryBase(Handle_GeomFill_Boundary &orig);
  virtual ~dtOCCGeomFillBoundaryBase();
  void setOCC(Handle_GeomFill_Boundary occCurve);
  Handle_GeomFill_Boundary const getOCC(void) const;
  std::string dumpToString(void) const;

private:
  class OCCHanWrap;
  dt__pH(OCCHanWrap) _hanWrap;
};
} // namespace dtOO
#endif /* dtOCCGeomFillBoundaryBase_H */
