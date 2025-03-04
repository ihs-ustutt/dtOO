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

#ifndef DTGMSHVERTEX_H
#define DTGMSHVERTEX_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <gmsh/GVertex.h>
#include <logMe/dtMacros.h>

class GPoint;

namespace dtOO {
class dtGmshEdge;

class dtGmshVertex : public ::GVertex {
public:
  dt__class(dtGmshVertex, ::GEntity);
  dtGmshVertex(::GModel *m, dtInt tag, double ms);
  dtGmshVertex(::GModel *m, dtInt tag);
  virtual ~dtGmshVertex();
  virtual GPoint point() const;
  virtual double x() const;
  virtual double y() const;
  virtual double z() const;
  virtual void setPosition(GPoint &p);
  virtual void setPosition(dtPoint3 const p);
  virtual void setPosition(dtPoint3 const *const p);
  dtPoint3 cast2DtPoint3(void) const;
  void addGEntity(::GEntity *const gEnt);
  static bool isEqual(::GVertex const *const gv0, ::GVertex const *const gv1);
  virtual std::list<dtGmshEdge *> dtEdges(void) const;
  std::string getPhysicalString(void) const;

public:
  mutable ::GEntity::MeshGenerationStatus _status;

private:
  dtPoint3 _dtP;
};
} // namespace dtOO
#endif /* DTGMSHVERTEX_H */
