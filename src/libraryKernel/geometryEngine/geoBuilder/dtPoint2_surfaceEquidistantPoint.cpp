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

#include "dtPoint2_surfaceEquidistantPoint.h"
#include <interfaceHeaven/vectorHandling.h>

#include <geometryEngine/dtSurface.h>

namespace dtOO {
dtPoint2_surfaceEquidistantPoint::dtPoint2_surfaceEquidistantPoint() {}

dtPoint2_surfaceEquidistantPoint::dtPoint2_surfaceEquidistantPoint(
  dtSurface const *const dtS, dtInt const &nU, dtInt const nV
)
{
  _pUV.resize(nU, nV);
  _pXYZ.resize(nU, nV);
  _dMS.resize(nU - 1, nV - 1);
  _sumM.resize(nV - 1, 0);
  _sumS.resize(nU - 1, 0);

  dtReal minU = dtS->minU();
  dtReal minV = dtS->minV();
  dtReal maxU = dtS->maxU();
  dtReal maxV = dtS->maxV();

  for (int ii = 0; ii < nU; ii++)
  {
    for (int jj = 0; jj < nV; jj++)
    {
      _pUV[ii][jj] = dtPoint2(
        minU + ii * (maxU - minU) / (static_cast<dtReal>(nU - 1)),
        minV + jj * (maxV - minV) / (static_cast<dtReal>(nV - 1))
      );
      _pXYZ[ii][jj] = dtS->getPoint3d(_pUV[ii][jj]);
    }
  }

  for (int ii = 0; ii < nU - 1; ii++)
  {
    for (int jj = 0; jj < nV - 1; jj++)
    {
      dtVector3 MM = _pXYZ[ii + 1][jj] - _pXYZ[ii][jj];
      dtVector3 SS = _pXYZ[ii][jj + 1] - _pXYZ[ii][jj];
      _dMS[ii][jj] =
        dtVector2(sqrt(MM.squared_length()), sqrt(SS.squared_length()));
      _sumM[jj] = _sumM[jj] + _dMS[ii][jj].x();
      _sumS[ii] = _sumS[ii] + _dMS[ii][jj].y();
    }
  }

  writeTableToLog();
}

dtPoint2_surfaceEquidistantPoint::~dtPoint2_surfaceEquidistantPoint() {}

twoDArrayHandling<dtPoint2> dtPoint2_surfaceEquidistantPoint::result(void)
{
  return _pUV;
}

void dtPoint2_surfaceEquidistantPoint::writeTableToLog(void) const
{
  twoDArrayHandling<dtReal> mat(_dMS.size(0), _dMS.size(1));
  for (int ii = 0; ii < _dMS.size(0); ii++)
  {
    for (int jj = 0; jj < _dMS.size(1); jj++)
    {
      mat[ii][jj] = _dMS[ii][jj].x();
    }
  }
  dt__info(writeTableToLog(), << logMe::floatMatrixToString(mat));

  for (int ii = 0; ii < _dMS.size(0); ii++)
  {
    for (int jj = 0; jj < _dMS.size(1); jj++)
    {
      mat[ii][jj] = _dMS[ii][jj].y();
    }
  }
  dt__info(writeTableToLog(), << logMe::floatMatrixToString(mat));

  dt__info(writeTableToLog(), << logMe::vecToString(_sumM));
  dt__info(writeTableToLog(), << logMe::vecToString(_sumS));
}
} // namespace dtOO
