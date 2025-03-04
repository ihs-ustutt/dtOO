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

#ifndef VECTORHANDLING_H
#define VECTORHANDLING_H

#include <dtOOTypeDef.h>

#include "labelHandling.h"
#include "stringPrimitive.h"
#include <logMe/dtMacros.h>
#include <logMe/logContainer.h>
#include <logMe/logMe.h>
#include <string>
#include <vector>

namespace dtOO {
//----------------------------------------------------------------------------
// h
//----------------------------------------------------------------------------
template <typename T> class vectorHandling : public std::vector<T> {
public:
  typedef typename std::vector<T>::iterator iterator;

public:
  dt__classOnlyName(vectorHandling);
  vectorHandling();
  vectorHandling(const vectorHandling &orig0, const vectorHandling &orig1);
  vectorHandling(const std::vector<T> &orig);
  vectorHandling(int const dim);
  virtual ~vectorHandling();
  using std::vector<T>::push_back;
  void destroy(void);
};

//----------------------------------------------------------------------------
// cpp
//----------------------------------------------------------------------------
template <typename T> vectorHandling<T>::vectorHandling() {}

template <typename T>
vectorHandling<T>::vectorHandling(
  const vectorHandling &orig0, const vectorHandling &orig1
)
  : std::vector<T>(orig0.size() + orig1.size())
{
  dtInt counter = 0;
  dt__forAllIndex(orig0, ii)
  {
    this->at(counter) = orig0[ii];
    counter++;
  }
  dt__forAllIndex(orig1, ii)
  {
    this->at(counter) = orig1[ii];
    counter++;
  }
}

template <typename T>
vectorHandling<T>::vectorHandling(const std::vector<T> &orig)
  : std::vector<T>(orig.size())
{
  dt__forAllIndex(orig, ii) { this->at(ii) = orig[ii]; }
}

template <typename T>
vectorHandling<T>::vectorHandling(int const dim) : std::vector<T>(dim)
{
}

template <typename T> vectorHandling<T>::~vectorHandling() {}

template <typename T> void vectorHandling<T>::destroy(void)
{
  dt__forAllIndex(*this, ii) delete this->at(ii);
  this->clear();
}
} // namespace dtOO
#endif /* VECTORHANDLING_H */
