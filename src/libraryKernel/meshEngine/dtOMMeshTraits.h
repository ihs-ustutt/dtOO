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

#ifndef DTOMMESHTRAITS_H
#define DTOMMESHTRAITS_H

#include <dtOOTypeDef.h>

#include <OpenMesh/Core/Mesh/Traits.hh>
#include <dtLinearAlgebra.h>

#include <gmsh/MElement.h>
#include <gmsh/MVertex.h>

namespace dtOO {
struct dtOMMeshTraits : public OpenMesh::DefaultTraits {
  VertexTraits
  {
  private:
    ::MVertex *_mv;

  public:
    VertexT() : _mv(NULL) {}
    void MVertex(::MVertex const *const mv)
    {
      _mv = const_cast<::MVertex *>(mv);
    }
    ::MVertex *MVertex(void) const { return _mv; }
  };

  HalfedgeTraits
  {
  private:
    bool _mark;

  public:
    HalfedgeT() : _mark(false) {}
    void mark(void) { _mark = true; }
    bool marked(void) const { return _mark; }
  };

  EdgeTraits
  {
  private:
    dtReal _dihedralAngle;

  public:
    EdgeT() : _dihedralAngle(0.) {}
    void dihedralAngle(dtReal const &dihedralAngle)
    {
      _dihedralAngle = dihedralAngle;
    }
    dtReal dihedralAngle(void) const { return _dihedralAngle; }
  };

  FaceTraits
  {
  private:
    ::MElement *_me;
    bool _inverted;
    bool _mark;

  public:
    FaceT() : _me(NULL), _inverted(false), _mark(false) {}
    void MElement(::MElement const *const me)
    {
      _me = const_cast<::MElement *>(me);
    }
    ::MElement *MElement(void) const { return _me; }
    void invert(void) { _inverted = true; }
    bool inverted(void) const { return _inverted; }
    void mark(void) { _mark = true; }
    bool marked(void) const { return _mark; }
    dtInt nVertices(void) const { return _me->getNumVertices(); }
  };
  //
  // standard attributes
  //
  VertexAttributes(OpenMesh::Attributes::Status);
  EdgeAttributes(OpenMesh::Attributes::Status);
  FaceAttributes(OpenMesh::Attributes::Normal | OpenMesh::Attributes::Status);
};
} // namespace dtOO

#endif /* DTOMMESHTRAITS_H */
