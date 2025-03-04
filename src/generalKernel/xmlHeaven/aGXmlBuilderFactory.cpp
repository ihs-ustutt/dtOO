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

#include "aGXmlBuilderFactory.h"

#include "aGXmlBuilder.h"
#include <logMe/logMe.h>
#include <string.h>

namespace dtOO {
dt__pVH(aGXmlBuilder) aGXmlBuilderFactory::_builder;

aGXmlBuilderFactory::~aGXmlBuilderFactory() {}

bool aGXmlBuilderFactory::registrate(aGXmlBuilder const *const reg)
{
  dt__forAllRefAuto(_builder, aBuilder)
  {
    if (aBuilder.virtualClassName() == reg->virtualClassName())
    {
      return false;
    }
  }
  _builder.push_back(reg->create());

  return true;
}

aGXmlBuilder *aGXmlBuilderFactory::create(char const *const str)
{
  return create(std::string(str));
}

aGXmlBuilder *aGXmlBuilderFactory::create(std::string const str)
{
  dt__debug(create(), << "str = " << str);
  dt__forAllRefAuto(_builder, aBuilder)
  {
    //
    // check virtual class name
    //
    if (aBuilder.virtualClassName() == str)
    {
      return aBuilder.create();
    }

    //
    // check virtual class name without postFix
    //
    if (stringPrimitive::replaceStringInString(
          "AGXmlBuilder", "", aBuilder.virtualClassName()
        ) == str)
    {
      return aBuilder.create();
    }

    //
    // check alias
    //
    dt__forAllRefAuto(aBuilder.factoryAlias(), anAlias)
    {
      if (anAlias == str)
        return aBuilder.create();
    }
  }

  std::vector<std::string> av;
  dt__forAllRefAuto(_builder, aBuilder)
  {
    av.push_back(aBuilder.virtualClassName());
    dt__forAllRefAuto(aBuilder.factoryAlias(), anAlias)
    {
      av.push_back("  -> " + anAlias);
    }
  }
  dt__throw(
    create(),
    << str << " could not be created." << std::endl
    << "Implemented builder:" << std::endl
    << logMe::vecToString(av, 1) << std::endl
  );
}

aGXmlBuilderFactory::aGXmlBuilderFactory() {}
} // namespace dtOO
