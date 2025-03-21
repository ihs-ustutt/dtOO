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

#ifndef DTOMEDGEFIELD_H
#define DTOMEDGEFIELD_H

#include <dtOOTypeDef.h>

#include "dtOMField.h"
#include "dtOMMesh.h"
#include <logMe/dtMacros.h>

namespace dtOO {
template <typename T> class dtOMEdgeField : public dtOMField {
public:
  dt__class(dtOMEdgeField, dtOMField);
  dtOMEdgeField(std::string const &label, dtOMMesh const &om, T const &init);
  virtual ~dtOMEdgeField();
  T &operator[](omEdgeH const &eH);
  T const &at(omEdgeH const &eH) const;
  void execute(T (*executeMe)(T const &member));
  dtLongUnsInt size(void) const;
  virtual void update(void);

private:
  std::map<omEdgeH, T> _field;
  T _init;
};

template <typename T>
dtOMEdgeField<T>::dtOMEdgeField(
  std::string const &label, dtOMMesh const &om, T const &init
)
  : dtOMField(label, om), _init(init)
{
  dt__forFromToIter(
    omConstEdgeI, refMesh().edges_begin(), refMesh().edges_end(), eIt
  ) _field[*eIt] = init;
}

template <typename T> dtOMEdgeField<T>::~dtOMEdgeField() {}

template <typename T> T &dtOMEdgeField<T>::operator[](omEdgeH const &eH)
{
  return _field[eH];
}

template <typename T> T const &dtOMEdgeField<T>::at(omEdgeH const &eH) const
{
  return _field.at(eH);
}

template <typename T>
void dtOMEdgeField<T>::execute(T (*executeMe)(T const &member))
{
  dt__forFromToIter(
    omConstEdgeI, refMesh().edges_begin(), refMesh().edges_end(), eIt
  ) _field[*eIt] = (*executeMe)(_field[*eIt]);
}

template <typename T> dtLongUnsInt dtOMEdgeField<T>::size(void) const
{
  return _field.size();
}

template <typename T> void dtOMEdgeField<T>::update(void)
{
  dt__forFromToIter(
    omConstEdgeI, refMesh().edges_begin(), refMesh().edges_end(), eIt
  )
  {
    typename std::map<omEdgeH, T>::iterator it = _field.find(*eIt);
    if (it == _field.end())
    {
      _field[*eIt] = _init;
    }
  }
}
} // namespace dtOO
#endif /* DTOMEDGEFIELD_H */
