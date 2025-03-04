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

#ifndef radialTranslatingMap2dTo3d_H
#define radialTranslatingMap2dTo3d_H

#include <dtOOTypeDef.h>

#include "map3dTo3d.h"
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class map2dTo3d;
class dtTransformer;

class radialTranslatingMap2dTo3d : public map3dTo3d {
public:
  dt__class(radialTranslatingMap2dTo3d, analyticGeometry);
  radialTranslatingMap2dTo3d();
  radialTranslatingMap2dTo3d(
    dtVector3 const &vv, dtReal const &distance, map2dTo3d const *const m2d
  );
  radialTranslatingMap2dTo3d(const radialTranslatingMap2dTo3d &orig);
  virtual ~radialTranslatingMap2dTo3d();
  virtual radialTranslatingMap2dTo3d *create(void) const;
  virtual radialTranslatingMap2dTo3d *clone(void) const;
  virtual radialTranslatingMap2dTo3d *
  cloneTransformed(dtTransformer const *const dtT) const;
  virtual bool isClosed(dtInt const &dir) const;
  virtual dtReal getMin(dtInt const &dir) const;
  virtual dtReal getMax(dtInt const &dir) const;
  virtual dtPoint3
  getPoint(dtReal const &uu, dtReal const &vv, dtReal const &ww) const;
  //	  virtual map2dTo3d * segmentConstU( dtReal const & uu ) const;
  //	  virtual map2dTo3d * segmentConstV( dtReal const & vv ) const;
  //  	virtual map2dTo3d * segmentConstW( dtReal const & ww ) const;
  virtual std::string dumpToString(void) const;
  //    map2dTo3d const & constRefMap2dTo3d( void ) const;
  //    dtVector3 const & rotationAxis( void ) const;
  //    dtPoint3 const & origin( void ) const;
private:
  //	  void correctOrigin( void );
private:
  ptrHandling<map2dTo3d> _m2d;
  dtVector3 _vv;
  dtReal _distance;
};
} // namespace dtOO
#endif /* radialTranslatingMap2dTo3d_H */
