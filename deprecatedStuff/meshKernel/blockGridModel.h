#ifndef BLOCKGRIDGMODEL_H
#define	BLOCKGRIDGMODEL_H

#include <dtLinearAlgebra.h>
#include "cardinalDirGModel.h"
#include <logMe/dtMacros.h>

namespace covise {
  class coDoSet;
}

namespace dtOO {
  class dtGmshFace;
  class dtGmshEdge;
  class dtGmshVertex;  
  
  class blockGridModel : public cardinalDirGModel {
  public:
    dt__CLASSNAME(blockGridModel);
    blockGridModel(std::string name="");
    virtual ~blockGridModel();
    void addDtGmshVertex(int const vId);
    void snapVertex(int const vId, int const aSId);
    void snap3dVertex(int const vId, int const rId);
    void snapVertexPercent(int const vId, int const aSId, dtPoint2 const ppUV);
    void addDtGmshEdge(int const eId, int const from, int const to);
    void addDtGmshFace(int const fId, int const e0, int const e1, int const e2, int const e3);
    bool snapEdge(int const eID);
    bool forceSnapEdge(int const eID);
    covise::coDoSet * toCoDoUnstructuredGrid(char const * const str) const;
    covise::coDoSet * toCoDoSurfUnsGrid(char const * const str, int const boundFaceNum) const;
    covise::coDoSet * makeBoCo(char const * const str) const;
    covise::coDoSet * toCoDoSet(char const * const str) const;
  private:
    float _snapTol;
  };
}
#endif	/* BLOCKGRIDGMODEL_H */

