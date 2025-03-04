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

#ifndef feOpenFOAMCase_H
#define feOpenFOAMCase_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

#include "dtCase.h"

class MVertex;
class MElement;

namespace dtOO {
class constValue;
class analyticFunction;
class analyticGeometry;
class boundedVolume;
class baseContainer;
class dtGmshModel;
class dtXmlParser;
class feOpenFOAMSetupRule;

class feOpenFOAMCase : public dtCase {
public:
  dt__class(feOpenFOAMCase, dtCase);
  dt__classSelfCreate(feOpenFOAMCase);
  feOpenFOAMCase();
  virtual ~feOpenFOAMCase();
  virtual std::vector<std::string> factoryAlias(void) const;
  virtual void init(
    ::QDomElement const &element,
    baseContainer const *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG,
    lvH_boundedVolume const *const bV,
    lvH_dtCase const *const dC
  );
  virtual void runCurrentState(void);

private:
  void initMeshVectors(
    std::vector<::MVertex *> &allVerts,
    std::vector<std::pair<::MElement *, dtInt>> &allElems,
    std::map<int, std::string> &physicalNames
  );

private:
  lvH_constValue const *_cV;
  std::string _dictRule;
  std::map<std::vector<std::string>, dt__pH(feOpenFOAMSetupRule)> _setupRule;
  std::vector<std::vector<std::string>> _fieldRule;
  std::vector<boundedVolume *> _bV;
  std::string _runCommand;
  std::vector<std::string> _noWriteRule;
  static bool _registrated;
};
} // namespace dtOO
#endif /* feOpenFOAMCase_H */
