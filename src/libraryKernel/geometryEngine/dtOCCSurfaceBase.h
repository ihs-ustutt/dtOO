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

#ifndef DTOCCSURFACEBASE_H
#define DTOCCSURFACEBASE_H

#include <dtOOTypeDef.h>

// clang-format off
#include <dtLinearAlgebra.h>
#include <Geom_Surface.hxx>
// clang-format on
#include <logMe/dtMacros.h>

namespace dtOO {
class dtOCCSurfaceBase {
public:
  dt__classOnlyName(dtOCCSurfaceBase);
  dtOCCSurfaceBase();
  dtOCCSurfaceBase(Handle(Geom_Surface) orig);
  virtual ~dtOCCSurfaceBase();
  void setOCC(Handle(Geom_Surface));
  Handle(Geom_Surface) const getOCC(void) const;
  std::string dumpToString(void) const;

private:
  class OCCHanWrap;
  dt__pH(OCCHanWrap) _hanWrap;
};
} // namespace dtOO
#endif /* DTOCCSURFACEBASE_H */
