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

#ifndef dtMeshGRegionWithBoundaryLayer_H
#define dtMeshGRegionWithBoundaryLayer_H

#include <dtOOTypeDef.h>

#include "dtMesh3DOperator.h"
#include "dtOMDynamicFaceField.h"
#include "dtOMDynamicVertexField.h"
#include "dtOMEdgeField.h"
#include "dtOMMesh.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

class GEntity;
class MVertex;
class MElement;
class FieldManager;

namespace dtOO {
class dtGmshFace;
class dtGmshModel;
class dtGmshRegion;

class dtMeshGRegionWithBoundaryLayer : public dtMesh3DOperator {
public:
  dt__classOnlyName(dtMeshGRegionWithBoundaryLayer);
  dt__classSelfCreate(dtMeshGRegionWithBoundaryLayer);
  dtMeshGRegionWithBoundaryLayer();
  virtual ~dtMeshGRegionWithBoundaryLayer();
  virtual void jInit(
    jsonPrimitive const &jE,
    baseContainer const *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG,
    lvH_boundedVolume const *const bV,
    lvH_dtMeshOperator const *const mO
  );
  virtual void init(
    ::QDomElement const &element,
    baseContainer const *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG,
    lvH_boundedVolume const *const bV,
    lvH_dtMeshOperator const *const mO
  );
  virtual void operator()(dtGmshRegion *dtgr);

private:
  void createLayerVertices(void);
  void createBoundaryLayerElements(
    std::vector<::MVertex *> &vertex, std::vector<::MElement *> &element
  ) const;
  static bool
  isSlidable(dtOMVertexField<bool> const &canSlideF, omVertexH const &vH);
  omHalfedgeH slidableHalfedgeInFace(omFaceH const &fH) const;
  void createRingOfBuddies(MVertex const *const mv0);
  void createMovedMesh(void);
  void meshWithGmsh(
    dtOMMesh const &mesh,
    std::vector<::MVertex *> &vertex,
    std::vector<::MElement *> &element,
    ::FieldManager *fm
  ) const;
  void adjustThickness(void);
  void determinMinMaxAverageAtSliders(
    twoDArrayHandling<dtReal> &avSpacing,
    twoDArrayHandling<dtReal> &maxSpacing,
    twoDArrayHandling<dtReal> &minSpacing,
    vectorHandling<dtReal> &avT,
    vectorHandling<dtReal> &maxT,
    vectorHandling<dtReal> &minT
  ) const;

private:
  static dtInt _NORMAL;
  static dtInt _SLIDER;
  static dtInt _FIXER;
  std::vector<bool> _flipNormal;
  //
  // define two surface meshes with fixed, thickness
  // and slidable vertex field
  //
  dtOMMesh _omInit;
  dtOMMesh _omMoved;
  dtOMDynamicVertexField<bool> _fixedF;
  dtOMDynamicVertexField<bool> _slidableF;
  dtOMDynamicVertexField<dtReal> _tF;
  dtOMDynamicVertexField<dtVector3> _nF;
  dtOMDynamicVertexField<bool> _flipNormalF;
  dtOMDynamicVertexField<std::vector<::MVertex *>> _buddyF;
  dtOMDynamicVertexField<std::vector<dtReal>> _realSpacing;
  dtOMDynamicVertexField<dtInt> _faceIndex;
  dtOMDynamicFaceField<dtInt> _extrudeF;
  dtOMDynamicFaceField<dtInt> _typeF;
  dtOMDynamicFaceField<dtInt> _nSpacingStepsF;
  dtMesh3DOperator *_3D;

private:
  static bool _registrated;
};
} // namespace dtOO

#endif /* dtMeshGRegionWithBoundaryLayer_H */
