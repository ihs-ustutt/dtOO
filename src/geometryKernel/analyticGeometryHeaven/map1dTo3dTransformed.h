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

#ifndef map1dTo3dTransformed_H
#define map1dTo3dTransformed_H

#include <dtOOTypeDef.h>

#include "analyticGeometry.h"
#include "map1dTo3d.h"
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <dtTransformerHeaven/dtTransformerInvThreeD.h>
#include <logMe/dtMacros.h>

namespace dtOO {
template <typename funT> class map1dTo3dTransformed : public funT {
  BOOST_STATIC_ASSERT((boost::is_base_of<map1dTo3d, funT>::value));

public:
  dt__class(map1dTo3dTransformed, analyticGeometry);
  map1dTo3dTransformed();
  map1dTo3dTransformed(map1dTo3dTransformed const &orig);
  map1dTo3dTransformed(funT const &orig);
  map1dTo3dTransformed(funT const &orig, dtTransformer const *const dtT);
  virtual ~map1dTo3dTransformed();
  virtual map1dTo3dTransformed *clone(void) const;
  virtual map1dTo3dTransformed *cloneTransformed(dtTransformer const *const dtT
  ) const;
  virtual map1dTo3dTransformed *create(void) const;
  virtual dtPoint3 getPoint(dtReal const &uu) const;
  //
  //
  //
  virtual dtVector3 firstDerU(dtReal const &uu) const;
  virtual bool isTransformed(void) const;

private:
  dt__pH(dtTransformerInvThreeD) _dtT;
};

template <typename funT>
map1dTo3dTransformed<funT>::map1dTo3dTransformed() : funT()
{
}

template <typename funT>
map1dTo3dTransformed<funT>::map1dTo3dTransformed(
  map1dTo3dTransformed const &orig
)
  : funT(orig)
{
  _dtT.reset(orig._dtT->clone());
}

template <typename funT>
map1dTo3dTransformed<funT>::map1dTo3dTransformed(funT const &orig) : funT(orig)
{
}

template <typename funT>
map1dTo3dTransformed<funT>::map1dTo3dTransformed(
  funT const &orig, dtTransformer const *const dtT
)
  : funT(orig)
{
  dtTransformerInvThreeD const *const dtsT =
    dynamic_cast<dtTransformerInvThreeD const *>(dtT);
  dt__throwIf(!dtsT, map1dTo3dTransformed());

  _dtT.reset(dtsT->clone());
}

template <typename funT> map1dTo3dTransformed<funT>::~map1dTo3dTransformed() {}

template <typename funT>
map1dTo3dTransformed<funT> *map1dTo3dTransformed<funT>::clone(void) const
{
  return new map1dTo3dTransformed<funT>(*this);
}

template <typename funT>
map1dTo3dTransformed<funT> *
map1dTo3dTransformed<funT>::cloneTransformed(dtTransformer const *const dtT
) const
{
  return new map1dTo3dTransformed<funT>(*this, dtT);
}

template <typename funT>
map1dTo3dTransformed<funT> *map1dTo3dTransformed<funT>::create(void) const
{
  return new map1dTo3dTransformed<funT>();
}

template <typename funT>
dtPoint3 map1dTo3dTransformed<funT>::getPoint(dtReal const &uu) const
{
  dtPoint3 pp = funT::getPoint(uu);

  return _dtT->apply(pp);
}

template <typename funT>
dtVector3 map1dTo3dTransformed<funT>::firstDerU(dtReal const &uu) const
{
  return _dtT->apply(funT::firstDerU(uu));
}

template <typename funT>
bool map1dTo3dTransformed<funT>::isTransformed(void) const
{
  return true;
}
} // namespace dtOO
#endif /* map1dTo3dTransformed_H */
