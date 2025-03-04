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

#ifndef GMSHBOUNDEDVOLUME_H
#define GMSHBOUNDEDVOLUME_H

#include <dtOOTypeDef.h>

#include "boundedVolume.h"
#include <interfaceHeaven/twoDArrayHandling.h>
#include <logMe/dtMacros.h>

class MVertex;

namespace dtOO {
class analyticGeometry;
class analyticFunction;
class constValue;
class baseContainer;
class dtGmshFace;
class dtGmshRegion;
class dtGmshModel;

class gmshBoundedVolume : public boundedVolume {
public:
  dt__class(gmshBoundedVolume, boundedVolume);
  dt__classSelfCreate(gmshBoundedVolume);
  gmshBoundedVolume();
  virtual ~gmshBoundedVolume();
  virtual void jInit(
    jsonPrimitive const &jE,
    baseContainer *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG,
    lvH_boundedVolume const *const bV
  );
  virtual void init(
    ::QDomElement const &element,
    baseContainer *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG,
    lvH_boundedVolume const *const bV
  );
  virtual vectorHandling<renderInterface *> getRender(void) const;
  virtual vectorHandling<renderInterface *> getExtRender(void) const;
  virtual std::vector<std::string> getMeshTags(void) const;
  virtual dtGmshFace *getFace(std::string const &tag) const;
  virtual dtGmshRegion *getRegion(std::string const &tag) const;
  virtual dtGmshModel *getModel(void) const;
  virtual void makePreGrid(void);
  virtual void makeGrid(void);

private:
  void updatePhysicals(void) const;
  mutable twoDArrayHandling<std::string> _physLabels;

protected:
  void updateBoundingBox(void);

protected:
  dtGmshModel *_gm;

private:
  static bool _registrated;
};
} // namespace dtOO
#endif /* GMSHBOUNDEDVOLUME_H */
