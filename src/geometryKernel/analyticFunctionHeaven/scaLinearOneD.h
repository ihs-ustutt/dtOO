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

#ifndef scaLinearOneD_H
#define scaLinearOneD_H

#include <dtOOTypeDef.h>

#include "scaOneD.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <vector>

namespace dtOO {
class dtTransformer;

class scaLinearOneD : public scaOneD {
public:
  dt__class(scaLinearOneD, analyticFunction);
  scaLinearOneD();
  scaLinearOneD(scaLinearOneD const &orig);
  scaLinearOneD(
    dtReal const &x0, dtReal const &x1, dtReal const &y0, dtReal const &y1
  );
  scaLinearOneD *clone(void) const;
  scaLinearOneD *cloneTransformed(dtTransformer const *const dtT) const;
  scaLinearOneD *create(void) const;
  virtual ~scaLinearOneD();
  virtual dtReal YFloat(dtReal const &xx) const;
  virtual dtReal invYFloat(dtReal const &yy) const;

private:
  dtReal _m;
  dtReal _b;
};
} // namespace dtOO
#endif /* scaLinearOneD_H */
