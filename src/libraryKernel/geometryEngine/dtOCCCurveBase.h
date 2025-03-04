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

#ifndef dtOCCCurveBase_H
#define dtOCCCurveBase_H

#include <dtOOTypeDef.h>

#include <Geom_Curve.hxx>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtOCCCurveBase {
public:
  dt__classOnlyName(dtOCCCurveBase);
  dtOCCCurveBase();
  dtOCCCurveBase(Handle_Geom_Curve orig);
  virtual ~dtOCCCurveBase();
  void setOCC(Handle_Geom_Curve occCurve);
  Handle_Geom_Curve const getOCC(void) const;
  std::string dumpToString(void) const;

private:
  class OCCHanWrap;
  dt__pH(OCCHanWrap) _hanWrap;
};
} // namespace dtOO
#endif /* dtOCCCurveBase_H */
