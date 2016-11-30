#ifndef dtMeshGRegionTetgen_H
#define	dtMeshGRegionTetgen_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMesh3DOperator.h"

class GRegion;
class tetgenio;
class MVertex;
class MTriangle;
//class splitQuadRecovery {
//  
//};

//void buildTetgenStructure(
//  GRegion * gr, tetgenio &in, 
//  std::vector< MVertex * > &numberedV, splitQuadRecovery &sqr
//);
//void TransferTetgenMesh(
//  GRegion * gr, tetgenio &in, tetgenio &out, std::vector< MVertex * > &numberedV
//);

namespace dtOO {
  class dtGmshRegion;
  
  class dtMeshGRegionTetgen : public dtMesh3DOperator {
    public:
      dt__classOnlyName(dtMeshGRegionTetgen);          
      dtMeshGRegionTetgen();
      virtual ~dtMeshGRegionTetgen();
      virtual void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG,
        bVPtrVec const * const bV,
        labeledVectorHandling< dtMeshOperator * > const * const mO
      );            
      virtual void operator()( dtGmshRegion * dtgr );
    private:
      void convertToTetgen(
        dtGmshRegion * gr, tetgenio * in,
        std::map< int, ::MVertex * > & MVertexInt,
        std::map< ::MVertex *, int > & intMVertex              
      );
      void convertFromTetgen(
        dtGmshRegion * gr, tetgenio * out,
        std::map< int, ::MVertex * > & MVertexInt,
        std::map< ::MVertex *, int > & intMVertex        
      );      
      static void enqueueMVertices(
        std::vector< ::MVertex * > & vv,
        int & nodeIndex,
        std::map< int, ::MVertex * > & MVertexInt,
        std::map< ::MVertex *, int > & intMVertex
      );
      static void enqueueMTriangles(
        std::vector< ::MTriangle * > & toEnqueue,
        std::vector< ::MTriangle * > & triangle
      );      
    private:
      std::string _tetOpt;
  };
}

#endif	/* dtMeshGRegionTetgen_H */

