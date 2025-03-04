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

#ifndef dtOMDynamicVertexField_H
#define dtOMDynamicVertexField_H

#include <dtOOTypeDef.h>

#include "dtOMVertexField.h"

namespace dtOO {
template <typename T> class dtOMDynamicVertexField : public dtOMVertexField<T> {
public:
  dt__class(dtOMDynamicVertexField, dtOMVertexField<T>);
  dtOMDynamicVertexField(std::string const &label, dtOMMesh &om, T const &init);
  virtual ~dtOMDynamicVertexField();

private:
  dtOMMesh &_om;
};

template <typename T>
dtOMDynamicVertexField<T>::dtOMDynamicVertexField(
  std::string const &label, dtOMMesh &om, T const &init
)
  : dtOMVertexField<T>(label, om, init), _om(om)
{
  _om.enqueueField(this);
}

template <typename T> dtOMDynamicVertexField<T>::~dtOMDynamicVertexField()
{
  _om.dequeueField(this);
}
} // namespace dtOO
#endif /* dtOMDynamicVertexField_H */
