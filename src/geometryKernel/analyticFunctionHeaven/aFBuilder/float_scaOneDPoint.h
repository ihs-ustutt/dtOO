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

#ifndef FLOAT_SCAONEDPOINT_H
#define FLOAT_SCAONEDPOINT_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <vector>

namespace dtOO {
class scaOneD;

//! Samples a scalar function uniformly over its normalized unit interval.
class float_scaOneDPoint {
public:
  /*!
   * @param s1d Scalar function to sample.
   * @param nPoints Number of uniformly spaced samples from zero through one.
   */
  float_scaOneDPoint(scaOneD const *const s1d, dtInt const &nPoints);
  virtual ~float_scaOneDPoint();
  //! Return the sampled scalar values.
  std::vector<dtReal> result(void);

private:
  std::vector<dtReal> _ff;
};
} // namespace dtOO
#endif /* FLOAT_SCAONEDPOINT_H */
