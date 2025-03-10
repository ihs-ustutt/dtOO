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

#ifndef constrainedFloatParam_H
#define constrainedFloatParam_H

#include <dtOOTypeDef.h>

#include "constValue.h"
#include <logMe/dtMacros.h>

namespace dtOO {
class constrainedFloatParam : public constValue {
public:
  dt__class(constrainedFloatParam, constValue);
  constrainedFloatParam(std::string const &label, std::string const &valueStr);
  constrainedFloatParam(
    std::string const &label,
    std::string const &valueStr,
    dtReal const &min,
    dtReal const &max
  );
  constrainedFloatParam(constrainedFloatParam const &orig);
  virtual ~constrainedFloatParam();
  virtual constrainedFloatParam *clone(void) const;
  virtual void dump(void) const;
  virtual dtReal getValue(void) const;
  virtual std::string getValueStr(void) const;
  virtual void setValue(dtReal const toSet);
  virtual void writeToElement(::QDomDocument &doc, ::QDomElement &element);
  virtual bool loadable(void) const;
  virtual void resolveConstraint(lvH_constValue const *const cVArr);

private:
  std::string _valueStr;
  lvH_constValue const *_cVArr;
};
} // namespace dtOO
#endif /* constrainedFloatParam_H */
