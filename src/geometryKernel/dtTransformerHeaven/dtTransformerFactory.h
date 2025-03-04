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

#ifndef DTTRANSFORMERFACTORY_H
#define DTTRANSFORMERFACTORY_H

#include <dtOOTypeDef.h>

#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>
#include <string>

namespace dtOO {
class dtTransformer;

class dtTransformerFactory {
public:
  dt__classOnlyName(dtTransformerFactory);
  virtual ~dtTransformerFactory();
  static bool registrate(dtTransformer const *const);
  static dtTransformer *create(char const *const str);
  static dtTransformer *create(std::string const str);
  static dtTransformer *createFromPlugin(
    std::string const &className,
    std::string const &pluginName,
    std::string const &pluginDriver
  );

private:
  dtTransformerFactory();

private:
  static dt__pVH(dtTransformer) _transformer;
};
} // namespace dtOO

#endif /* DTTRANSFORMERFACTORY_H */
