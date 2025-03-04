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

#ifndef DTPOINT2_SURFACEEQUIDISTANTPOINT_H
#define DTPOINT2_SURFACEEQUIDISTANTPOINT_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/twoDArrayHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtSurface;

class dtPoint2_surfaceEquidistantPoint {
public:
  dt__classOnlyName(dtPoint2_surfaceEquidistantPoint);
  dtPoint2_surfaceEquidistantPoint();
  dtPoint2_surfaceEquidistantPoint(
    dtSurface const *const dtS, dtInt const &nU, dtInt const nV
  );
  virtual ~dtPoint2_surfaceEquidistantPoint();
  twoDArrayHandling<dtPoint2> result(void);

private:
  void writeTableToLog(void) const;

private:
  twoDArrayHandling<dtPoint2> _pUV;
  twoDArrayHandling<dtPoint3> _pXYZ;
  twoDArrayHandling<dtVector2> _dMS;
  std::vector<dtReal> _sumM;
  std::vector<dtReal> _sumS;
};
} // namespace dtOO
#endif /* DTPOINT2_SURFACEEQUIDISTANTPOINT_H */
