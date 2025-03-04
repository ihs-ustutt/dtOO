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

#ifndef QSHAPEMETRIC_H
#define QSHAPEMETRIC_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>

class MElement;
class MTetrahedron;
class MPyramid;

namespace dtOO {
class qShapeMetric {
public:
  dt__classOnlyName(qShapeMetric);
  qShapeMetric();
  virtual ~qShapeMetric();
  dtReal operator()(::MElement const *const me);

private:
  static dtReal calculatePyramid(::MPyramid *pyr);
  static dtReal calculateTetrahedron(::MTetrahedron *tet);
};
} // namespace dtOO
#endif /* QSHAPEMETRIC_H */
