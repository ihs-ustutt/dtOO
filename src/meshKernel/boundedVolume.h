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

#ifndef boundedVolume_H
#define boundedVolume_H

#include <dtOOTypeDef.h>

#include <bVObserver/bVOSubject.h>
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/labeledVectorHandling.h>
#include <interfaceHeaven/optionHandling.h>
#include <interfaceHeaven/renderInterface.h>
#include <jsonHeaven/jsonPrimitive.h>
#include <logMe/dtMacros.h>
#include <mainConceptFwd.h>
#include <map>
#include <string>

namespace dtOO {
class analyticGeometry;
class analyticFunction;
class constValue;
class baseContainer;
class dtGmshFace;
class dtGmshRegion;
class dtGmshModel;

class boundedVolume : public bVOSubject,
                      public labelHandling,
                      public optionHandling,
                      public renderInterface {
public:
  dt__class(boundedVolume, labelHandling);
  boundedVolume();
  virtual ~boundedVolume();
  virtual boundedVolume *create(void) const = 0;
  virtual std::vector<std::string> factoryAlias(void) const;
  virtual void init(
    ::QDomElement const &element,
    baseContainer *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const sF,
    lvH_analyticGeometry const *const aF,
    lvH_boundedVolume const *const bV
  );
  virtual void jInit(
    jsonPrimitive const &jE,
    baseContainer *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG,
    lvH_boundedVolume const *const bV
  );
  virtual void makeGrid(void) = 0;
  virtual void makePreGrid(void) = 0;
  bool isMeshed(void) const;
  void setMeshed(void);
  bool isPreMeshed(void) const;
  void setPreMeshed(void);
  virtual std::vector<std::string> getMeshTags(void) const = 0;
  virtual dtGmshFace *getFace(std::string const &tag) const = 0;
  virtual dtGmshRegion *getRegion(std::string const &tag) const = 0;
  virtual dtGmshModel *getModel(void) const = 0;

protected:
  jsonPrimitive &config(void);
  jsonPrimitive const &config(void) const;

private:
  jsonPrimitive _config;
  bool _meshed;
  bool _preMeshed;
};
dt__I_addCloneForpVHNotImpl(boundedVolume);
} // namespace dtOO
#endif /* boundedVolume_H */
