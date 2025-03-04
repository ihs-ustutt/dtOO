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

#ifndef dtPlugin_H
#define dtPlugin_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/labeledVectorHandling.h>
#include <interfaceHeaven/optionHandling.h>
#include <logMe/dtMacros.h>
#include <mainConceptFwd.h>
#include <xmlHeaven/dtXmlParserBase.h>

namespace pugg {
class Kernel;
}

namespace dtOO {
class constValue;
class analyticFunction;
class analyticGeometry;
class boundedVolume;
class dtCase;

class dtPlugin : public labelHandling, public optionHandling {
public:
  dt__class(dtPlugin, labelHandling);
  dtPlugin();
  virtual ~dtPlugin();
  virtual dtPlugin *create(void) const = 0;
  virtual void init(
    ::QDomElement const &element,
    baseContainer *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG,
    lvH_boundedVolume const *const bV,
    lvH_dtCase const *const dC,
    lvH_dtPlugin const *const pL
  );
  virtual void apply(void) = 0;
  virtual std::vector<std::string> factoryAlias(void) const;
  //
  // pugg stuff
  //
  static const dtInt version = 1;
  static const std::string server_name() { return "dtPluginServer"; }
  void setKernel(::pugg::Kernel *kernel);

private:
  dt__pH(::pugg::Kernel) _kernel;
};
dt__I_addCloneForpVHNotImpl(dtPlugin);
} // namespace dtOO
#endif /* dtPlugin_H */
