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

#ifndef calculationTypeHandling_H
#define calculationTypeHandling_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <vector>

#include <limits>
#include <math.h>

#define dt__SMALL +01.E-06

namespace dtOO {
template <typename R, typename I> class calculationTypeHandling {
public:
  dt__classOnlyName(calculationTypeHandling);
  virtual ~calculationTypeHandling() {}
  static R next(I a)
  {
    R b = a;
    if ((I)b > a)
    {
      return b;
    }
    R ret = std::nextafterf(b, std::numeric_limits<R>::infinity());
    return ret;
  }
  static R nextIfSmaller(I a)
  {
    R b = a;
    if (b >= a)
    {
      return b;
    }
    else
    {
      return next(a);
    }
  }
  static R prev(I a)
  {
    R b = a;
    if ((I)b < a)
    {
      return b;
    }

    R ret = nexttowardf(b, -std::numeric_limits<R>::infinity());
    return ret;
  }
  static R prevIfBigger(I a)
  {
    R b = a;
    if (b <= a)
    {
      return b;
    }
    else
    {
      return prev(a);
    }
  }
  //    static dtReal boundToRange(dtReal const value, dtReal const a, dtReal
  //    const b);
  static R boundToRange(R const value, R const a, R const b)
  {
    if (value > b)
    {
      return b;
    }
    else if (value < a)
    {
      return a;
    }

    return value;
  }
  static bool isAscending(std::vector<R> const &check)
  {
    for (int ii = 1; ii < check.size(); ii++)
    {
      if (check[ii - 1] > check[ii])
      {
        return false;
      }
    }
    return true;
  }
  static bool isSmall(R const &val)
  {
    if (fabs(val) < dt__SMALL)
    {
      return true;
    }
    return false;
  }

private:
  calculationTypeHandling() {}
};
typedef calculationTypeHandling<dtReal, double> floatHandling;
} // namespace dtOO
#endif /* calculationTypeHandling_H */
