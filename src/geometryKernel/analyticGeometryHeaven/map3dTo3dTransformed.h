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

#ifndef map3dTo3dTransformed_H
#define map3dTo3dTransformed_H

#include <dtOOTypeDef.h>

#include "analyticGeometry.h"
#include "map3dTo3d.h"
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <dtTransformerHeaven/dtTransformerInvThreeD.h>
#include <logMe/dtMacros.h>

namespace dtOO {
template <typename funT> class map3dTo3dTransformed : public funT {
  BOOST_STATIC_ASSERT((boost::is_base_of<map3dTo3d, funT>::value));

public:
  dt__class(map3dTo3dTransformed, analyticGeometry);
  map3dTo3dTransformed();
  map3dTo3dTransformed(map3dTo3dTransformed const &orig);
  map3dTo3dTransformed(funT const &orig);
  map3dTo3dTransformed(funT const &orig, dtTransformer const *const dtT);
  virtual ~map3dTo3dTransformed();
  virtual map3dTo3dTransformed *clone(void) const;
  virtual map3dTo3dTransformed *cloneTransformed(dtTransformer const *const dtT
  ) const;
  virtual map3dTo3dTransformed *create(void) const;
  virtual dtPoint3
  getPoint(dtReal const &uu, dtReal const &vv, dtReal const &ww) const;
  virtual std::string dumpToString(void) const;
  //
  //
  //
  virtual std::vector<dtVector3>
  firstDer(dtReal const &uu, dtReal const &vv, dtReal const &ww) const;
  virtual dtPoint3 reparamInVolume(dtPoint3 const &ppXYZ) const;
  virtual bool isTransformed(void) const;

private:
  dt__pH(dtTransformerInvThreeD) _dtT;
};

template <typename funT>
map3dTo3dTransformed<funT>::map3dTo3dTransformed() : funT()
{
}

template <typename funT>
map3dTo3dTransformed<funT>::map3dTo3dTransformed(
  map3dTo3dTransformed const &orig
)
  : funT(orig)
{
  _dtT.reset(orig._dtT->clone());
}

template <typename funT>
map3dTo3dTransformed<funT>::map3dTo3dTransformed(funT const &orig) : funT(orig)
{
}

template <typename funT>
map3dTo3dTransformed<funT>::map3dTo3dTransformed(
  funT const &orig, dtTransformer const *const dtT
)
  : funT(orig)
{
  dtTransformerInvThreeD const *const dtsT =
    dynamic_cast<dtTransformerInvThreeD const *>(dtT);
  dt__throwIf(!dtsT, map3dTo3dTransformed());

  _dtT.reset(dtsT->clone());
}

template <typename funT> map3dTo3dTransformed<funT>::~map3dTo3dTransformed() {}

template <typename funT>
map3dTo3dTransformed<funT> *map3dTo3dTransformed<funT>::clone(void) const
{
  return new map3dTo3dTransformed<funT>(*this);
}

template <typename funT>
map3dTo3dTransformed<funT> *
map3dTo3dTransformed<funT>::cloneTransformed(dtTransformer const *const dtT
) const
{
  return new map3dTo3dTransformed<funT>(*this, dtT);
}

template <typename funT>
map3dTo3dTransformed<funT> *map3dTo3dTransformed<funT>::create(void) const
{
  return new map3dTo3dTransformed<funT>();
}

template <typename funT>
dtPoint3 map3dTo3dTransformed<funT>::getPoint(
  dtReal const &uu, dtReal const &vv, dtReal const &ww
) const
{
  return _dtT->apply(funT::getPoint(uu, vv, ww));
}

template <typename funT>
std::vector<dtVector3> map3dTo3dTransformed<funT>::firstDer(
  dtReal const &uu, dtReal const &vv, dtReal const &ww
) const
{
  return _dtT->apply(funT::firstDer(uu, vv, ww));
}

template <typename funT>
dtPoint3 map3dTo3dTransformed<funT>::reparamInVolume(dtPoint3 const &ppXYZ
) const
{
  return map3dTo3d::reparamInVolume(ppXYZ);
}

template <typename funT>
bool map3dTo3dTransformed<funT>::isTransformed(void) const
{
  return true;
}

template <typename funT>
std::string map3dTo3dTransformed<funT>::dumpToString(void) const
{
  std::stringstream ss;

  ss << dt__dumpToString(
    << "funT = " << funT::virtualClassName() << std::endl
    << funT::dumpToString() << std::endl
    << "_dtT = " << _dtT->virtualClassName()
  );

  return ss.str();
}
} // namespace dtOO
#endif /* map3dTo3dTransformed_H */
