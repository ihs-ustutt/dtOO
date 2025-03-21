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

#ifndef analyticGeometry_H
#define analyticGeometry_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/labeledVectorHandling.h>
#include <interfaceHeaven/renderInterface.h>
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>
#include <mainConceptFwd.h>
#include <string>
#include <vector>

namespace dtOO {
class analyticGeometry : public labelHandling, public renderInterface {
public:
  dt__class(analyticGeometry, labelHandling);
  analyticGeometry();
  virtual ~analyticGeometry();
  analyticGeometry(analyticGeometry const &orig);
  dtPoint3 getPointPercent(dtReal const *const uvw) const;
  ::std::vector<dtVector3> firstDerPercent(dtReal const *const uvw) const;
  //
  // overload
  //
  virtual analyticGeometry *clone(void) const = 0;
  virtual analyticGeometry *create(void) const = 0;
  virtual dtInt dim(void) const = 0;
  virtual bool isClosed(dtInt const &dir) const = 0;
  virtual dtReal getMin(dtInt const &dir) const = 0;
  virtual dtReal getMax(dtInt const &dir) const = 0;
  virtual dtPoint3 getPoint(dtReal const *const uvw) const = 0;
  virtual ::std::vector<dtVector3> firstDer(dtReal const *const uvw) const = 0;
  //
  // optional overload
  //
  virtual std::string dumpToString(void) const;
  virtual bool isTransformed(void) const;
  virtual bool isCompound(void) const;
  virtual vectorHandling<analyticGeometry const *> compoundInternal(void) const;
  std::pair<dtPoint3, dtPoint3> boundingBox(void) const;
  dtReal boundingBoxValue(void) const;
  void updateBoundingBox(void) const;
  dtReal characteristicLength(void) const;
  std::vector<dtPoint3> cornerPoints(void) const;
  //
  //
  //
  dtPoint3 getPoint(std::vector<dtReal> const &uvw) const;
  static bool inXYZTolerance(
    dtPoint3 const &p0,
    dtPoint3 const &p1,
    dtReal *const dist,
    bool output,
    dtReal inc
  );
  static bool inXYZTolerance(dtPoint3 const &p0, dtPoint3 const &p1);
  static bool inXYZTolerance(dtReal const dist, dtReal inc = 1.);
  bool degenerated(void) const;
  static bool inUVWTolerance(dtPoint3 const &p0, dtPoint3 const &p1);
  static bool inUVWTolerance(dtPoint2 const &p0, dtPoint2 const &p1);
  static bool inUVWTolerance(dtReal const &p0, dtReal const &p1);
  //
  //
  //
  bool equal(const analyticGeometry &other) const;
  bool equal(analyticGeometry const *const other) const;
  //
  //
  //
  void dump(void) const;
  void setRenderResolution(int const &dir, dtInt const &value) const;
  dtInt getRenderResolution(int const &dir) const;

private:
  mutable dtInt _resU;
  mutable dtInt _resV;
  mutable dtInt _resW;
  mutable dtReal _characteristicLength;
  mutable std::pair<dtPoint3, dtPoint3> _boundingBox;
  mutable dtReal _boundingBoxValue;
};
dt__H_addCloneForpVH(analyticGeometry);
} // namespace dtOO
#endif /* analyticGeometry_H */
