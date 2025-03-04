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

#ifndef ANALYTICGEOMETRYCOMPOUND_H
#define ANALYTICGEOMETRYCOMPOUND_H

#include <dtOOTypeDef.h>

#include "analyticGeometry.h"
#include <interfaceHeaven/renderInterface.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
template <typename funT> class analyticGeometryCompound : public funT {
public:
  dt__class(analyticGeometryCompound, analyticGeometry);
  analyticGeometryCompound();
  analyticGeometryCompound(analyticGeometryCompound const &orig);
  virtual ~analyticGeometryCompound();
  virtual analyticGeometryCompound *clone(void) const;
  virtual analyticGeometryCompound *create(void) const;
  funT const &component(dtInt const &pos) const;
  funT &addComponent(funT const &toAdd);
  dtInt nComponents(void) const;
  virtual vectorHandling<renderInterface *> getRender(void) const;
  virtual bool isCompound(void) const;
  vectorHandling<analyticGeometry const *> compoundInternal(void) const;

private:
  vectorHandling<funT> _vec;
};

template <typename funT>
analyticGeometryCompound<funT>::analyticGeometryCompound() : funT()
{
}

template <typename funT>
analyticGeometryCompound<funT>::analyticGeometryCompound(
  analyticGeometryCompound const &orig
)
  : funT()
{
  for (int ii = 0; ii < orig._vec.size(); ii++)
  {
    _vec.push_back(funT(orig._vec[ii]));
  }
}

template <typename funT>
analyticGeometryCompound<funT>::~analyticGeometryCompound()
{
}

template <typename funT>
analyticGeometryCompound<funT> *analyticGeometryCompound<funT>::clone(void
) const
{
  return new analyticGeometryCompound<funT>(*this);
}

template <typename funT>
analyticGeometryCompound<funT> *analyticGeometryCompound<funT>::create(void
) const
{
  return new analyticGeometryCompound<funT>();
}

template <typename funT>
funT const &analyticGeometryCompound<funT>::component(dtInt const &pos) const
{
  return _vec[pos];
}

template <typename funT>
funT &analyticGeometryCompound<funT>::addComponent(funT const &toAdd)
{
  _vec.push_back(funT(toAdd));

  return _vec.back();
}

template <typename funT>
dtInt analyticGeometryCompound<funT>::nComponents(void) const
{
  return _vec.size();
}

template <typename funT>
vectorHandling<renderInterface *> analyticGeometryCompound<funT>::getRender(void
) const
{
  vectorHandling<renderInterface *> retVec;
  dt__forAllIndex(_vec, ii)
  {
    _vec[ii].setRenderResolution(0, this->getRenderResolution(0));
    _vec[ii].setRenderResolution(1, this->getRenderResolution(1));
    _vec[ii].setRenderResolution(2, this->getRenderResolution(2));
    vectorHandling<renderInterface *> tVec = _vec[ii].getRender();
    dt__forAllIndex(tVec, jj) retVec.push_back(tVec[jj]);
  }

  return retVec;
}

template <typename funT>
bool analyticGeometryCompound<funT>::isCompound(void) const
{
  return true;
}

template <typename funT>
vectorHandling<analyticGeometry const *>
analyticGeometryCompound<funT>::compoundInternal(void) const
{
  vectorHandling<analyticGeometry const *> aGV(_vec.size());
  dt__forAllIndex(_vec, ii)
  {
    aGV[ii] = analyticGeometry::ConstDownCast(&(_vec[ii]));
  }

  return aGV;
}
} // namespace dtOO
#endif /* ANALYTICGEOMETRYCOMPOUND_H */
