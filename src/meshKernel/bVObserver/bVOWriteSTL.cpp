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

#include "bVOWriteSTL.h"
#include "interfaceHeaven/systemHandling.h"

#include "bVOInterfaceFactory.h"
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <constValueHeaven/constValue.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/MTriangle.h>
#include <logMe/dtParMacros.h>
#include <logMe/logMe.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshModel.h>
#include <xmlHeaven/qtXmlBase.h>

namespace dtOO {
bool bVOWriteSTL::_registrated =
  bVOInterfaceFactory::registrate(dt__tmpPtr(bVOWriteSTL, new bVOWriteSTL()));

bVOWriteSTL::bVOWriteSTL() {}

bVOWriteSTL::~bVOWriteSTL() {}

void bVOWriteSTL::init(
  ::QDomElement const &element,
  baseContainer const *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV,
  boundedVolume *attachTo
)
{
  //
  // init bVOInterface
  //
  bVOInterface::init(element, bC, cV, aF, aG, bV, attachTo);

  //		<bVObserver name="bVOWriteSTL"
  //		  filename="mesh.stl"
  //		/>
  _filename = qtXmlBase::getAttributeStr("filename", element);
}

void bVOWriteSTL::postUpdate(void)
{
  dt__onlyMaster
  {
    dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());

    //
    // set current model
    //
    ::GModel::setCurrent(gm);

    std::fstream of;
    of.open(_filename.c_str(), std::ios::out | std::ios::trunc);

    dtGmshModel::intGEntityVMap intGEntity;
    gm->getPhysicalGroups(2, intGEntity);

    dt__forFromToIter(
      dtGmshModel::intGEntityVMap::const_iterator,
      intGEntity.begin(),
      intGEntity.end(),
      it
    )
    {
      of << "solid " << gm->getPhysicalName(2, it->first) << std::endl;
      dt__forAllIndex(it->second, ii)
      {
        dtGmshFace *gf = dtGmshFace::SecureCast((it->second)[ii]);
        //
        // triangles
        //
        dt__forAllIndex(gf->triangles, jj)
        {
          SVector3 nn = gf->triangles[jj]->getFace(0).normal();

          std::vector<MVertex *> mv;
          gf->triangles[jj]->getVertices(mv);
          of << logMe::dtFormat("facet normal %g %g %g") % nn[0] % nn[1] % nn[2]
             << std::endl
             << "  outer loop" << std::endl;
          dt__forAllIndex(mv, kk)
          {
            of << logMe::dtFormat("    vertex %g %g %g") % mv[kk]->x() %
                    mv[kk]->y() % mv[kk]->z()
               << std::endl;
          }
          of << "  endloop" << std::endl;
          of << "endfacet" << std::endl;
        }
        //
        // quadrangles
        //
        dt__forAllIndex(gf->quadrangles, jj)
        {
          SVector3 nn = gf->quadrangles[jj]->getFace(0).normal();
          std::vector<MVertex *> mv;
          gf->quadrangles[jj]->getVertices(mv);
          of << logMe::dtFormat("facet normal %g %g %g") % nn[0] % nn[1] % nn[2]
             << std::endl
             << "  outer loop" << std::endl;
          dt__forFromToIndex(0, 3, kk)
          {
            of << logMe::dtFormat("    vertex %g %g %g") % mv[kk]->x() %
                    mv[kk]->y() % mv[kk]->z()
               << std::endl;
          }
          of << "  endloop" << std::endl;
          of << "endfacet" << std::endl;
          of << logMe::dtFormat("facet normal %g %g %g") % nn[0] % nn[1] % nn[2]
             << std::endl
             << "  outer loop" << std::endl;
          dtInt qid[3] = {0, 2, 3};
          dt__forFromToIndex(0, 3, kk)
          {
            of << logMe::dtFormat("    vertex %g %g %g") % mv[qid[kk]]->x() %
                    mv[qid[kk]]->y() % mv[qid[kk]]->z()
               << std::endl;
          }
          of << "  endloop" << std::endl;
          of << "endfacet" << std::endl;
        }
      }
      of << "endsolid " << gm->getPhysicalName(2, it->first) << std::endl;
    }

    of.close();
  }
}
} // namespace dtOO
