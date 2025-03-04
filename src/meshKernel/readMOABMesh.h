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

#ifndef READMOABMESH_H
#define READMOABMESH_H

#include <dtOOTypeDef.h>

#include "gmshBoundedVolume.h"
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>

#include <moab/EntityHandle.hpp>

namespace moab {
class Interface;
}

class MVertex;

namespace dtOO {
class constValue;
class analyticFunction;
class analyticGeometry;

class readMOABMesh : public gmshBoundedVolume {
public:
  dt__class(readMOABMesh, boundedVolume);
  dt__classSelfCreate(readMOABMesh);
  readMOABMesh();
  virtual ~readMOABMesh();
  virtual void init(
    ::QDomElement const &element,
    baseContainer *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG,
    lvH_boundedVolume const *const bV
  );
  virtual void makeGrid(void);

private:
  void convertToGmsh(void);
  static std::map<::moab::EntityHandle, ::MVertex *>
  createVertices(dtGmshModel *gm, ::moab::Interface const &mb);
  static std::map<std::string, dtGmshFace *> createFaces(
    dtGmshModel *gm,
    ::moab::Interface const &mb,
    std::map<::moab::EntityHandle, ::MVertex *> &mv_MOAB
  );
  static std::map<std::string, dtGmshRegion *> createRegions(
    dtGmshModel *gm,
    ::moab::Interface const &mb,
    std::map<::moab::EntityHandle, ::MVertex *> &mv_MOAB
  );

private:
  ptrHandling<::moab::Interface> _mb;
  std::string _fileName;
  std::map<::moab::EntityHandle, ::MVertex *> _mv_MOAB;
  std::map<std::string, dtGmshFace *> _ff_string;
  std::map<std::string, dtGmshRegion *> _rr_string;
  static bool _registrated;
};
} // namespace dtOO

#endif /* READCGNSMESH_H */
