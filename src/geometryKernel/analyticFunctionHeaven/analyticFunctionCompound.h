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

#ifndef analyticFunctionCompound_H
#define analyticFunctionCompound_H

#include <dtOOTypeDef.h>

#include "analyticFunction.h"
#include <interfaceHeaven/renderInterface.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
template <typename funT> class analyticFunctionCompound : public funT {
public:
  dt__class(analyticFunctionCompound, analyticFunction);
  analyticFunctionCompound();
  analyticFunctionCompound(analyticFunctionCompound const &orig);
  analyticFunctionCompound(funT const &orig);
  analyticFunctionCompound(
    funT const &orig,
    vectorHandling<analyticFunction *> const vec,
    std::map<int, dtInt> const pos_tag
  );
  virtual ~analyticFunctionCompound();
  virtual analyticFunctionCompound *clone(void) const;
  virtual funT *weakClone(void) const;
  virtual analyticFunctionCompound *create(void) const;
  void trojanHorse(
    vectorHandling<analyticFunction *> const &vec,
    std::map<int, dtInt> const &pos_tag
  );
  funT const &component(dtInt const &pos) const;
  funT &addComponent(funT const &toAdd);
  dtInt nComponents(void) const;
  funT const &componentFromTag(dtInt const &tag) const;
  bool hasTag(dtInt const &tag) const;
  virtual vectorHandling<renderInterface *> getRender(void) const;
  virtual bool isCompound(void) const;
  virtual vectorHandling<analyticFunction *> const &vecRef(void) const;
  virtual std::map<int, dtInt> const &mapRef(void) const;
  bool isTrojan(void) const;

private:
  vectorHandling<analyticFunction *> _vec;
  std::map<int, dtInt> _pos_tag;
  bool _trojan;
};

template <typename funT>
analyticFunctionCompound<funT>::analyticFunctionCompound() : funT()
{
  _trojan = false;
}

template <typename funT>
analyticFunctionCompound<funT>::analyticFunctionCompound(
  analyticFunctionCompound const &orig
)
  : funT(orig)
{
  dt__forAllRefAuto(orig._vec, aFun) _vec.push_back(aFun->clone());
  dt__forAllRefAuto(orig._pos_tag, aPair)
  {
    _pos_tag[aPair.first] = aPair.second;
  }
  _trojan = orig._trojan;
}

template <typename funT>
analyticFunctionCompound<funT>::analyticFunctionCompound(funT const &orig)
  : funT(orig)
{
  _trojan = false;
}

template <typename funT>
analyticFunctionCompound<funT>::analyticFunctionCompound(
  funT const &orig,
  vectorHandling<analyticFunction *> const vec,
  std::map<int, dtInt> const pos_tag
)
  : funT(orig)
{
  dt__forAllRefAuto(vec, aFun) _vec.push_back(aFun->clone());
  dt__forAllRefAuto(pos_tag, aPair) { _pos_tag[aPair.first] = aPair.second; }
  _trojan = false;
}

template <typename funT>
analyticFunctionCompound<funT>::~analyticFunctionCompound()
{
  if (!_trojan)
    _vec.destroy();
}

template <typename funT>
analyticFunctionCompound<funT> *analyticFunctionCompound<funT>::clone(void
) const
{
  return new analyticFunctionCompound<funT>(*this);
}

template <typename funT>
funT *analyticFunctionCompound<funT>::weakClone(void) const
{
  return new funT(*this);
}

template <typename funT>
analyticFunctionCompound<funT> *analyticFunctionCompound<funT>::create(void
) const
{
  return new analyticFunctionCompound<funT>();
}

template <typename funT>
void analyticFunctionCompound<funT>::trojanHorse(
  vectorHandling<analyticFunction *> const &vec,
  std::map<int, dtInt> const &pos_tag
)
{
  dt__throwIf(!_vec.empty() || !_pos_tag.empty(), trojanHorse());
  dt__forAllRefAuto(vec, aVec) { _vec.push_back(aVec); }
  dt__forAllRefAuto(pos_tag, aPos_tag)
  {
    _pos_tag[aPos_tag.first] = aPos_tag.second;
  }
  _trojan = true;
}

template <typename funT>
funT const &analyticFunctionCompound<funT>::component(dtInt const &pos) const
{
  return *(funT::ConstDownCast(_vec[pos]));
}

template <typename funT>
funT &analyticFunctionCompound<funT>::addComponent(funT const &toAdd)
{
  _vec.push_back(toAdd.clone());

  dtInt tag = 0;
  if (!_pos_tag.empty())
    tag = _pos_tag.rbegin()->first + 1;
  _pos_tag[tag] = _vec.size() - 1;

  return *funT::DownCast(_vec.back());
}

template <typename funT>
dtInt analyticFunctionCompound<funT>::nComponents(void) const
{
  return _vec.size();
}

template <typename funT>
funT const &analyticFunctionCompound<funT>::componentFromTag(dtInt const &tag
) const
{
  return component(_pos_tag.at(tag));
}

template <typename funT>
bool analyticFunctionCompound<funT>::hasTag(dtInt const &tag) const
{
  return (_pos_tag.find(tag) != _pos_tag.end());
}

template <typename funT>
vectorHandling<renderInterface *> analyticFunctionCompound<funT>::getRender(void
) const
{
  vectorHandling<renderInterface *> retVec;
  dt__forAllIndex(_vec, ii)
  {
    vectorHandling<renderInterface *> tVec = _vec[ii]->getRender();
    dt__forAllIndex(tVec, jj) retVec.push_back(tVec[jj]);
  }

  return retVec;
}

template <typename funT>
bool analyticFunctionCompound<funT>::isCompound(void) const
{
  return true;
}

template <typename funT>
vectorHandling<analyticFunction *> const &
analyticFunctionCompound<funT>::vecRef(void) const
{
  return _vec;
}

template <typename funT>
std::map<int, dtInt> const &analyticFunctionCompound<funT>::mapRef(void) const
{
  return _pos_tag;
}

template <typename funT>
bool analyticFunctionCompound<funT>::isTrojan(void) const
{
  return _trojan;
}
} // namespace dtOO
#endif /* analyticFunctionCompound_H */
