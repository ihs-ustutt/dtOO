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

#ifndef dtMeshGRegionTetgen_H
#define	dtMeshGRegionTetgen_H

#include <dtOOTypeDef.h>

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
      dt__classSelfCreate(dtMeshGRegionTetgen);
      dtMeshGRegionTetgen();
      virtual ~dtMeshGRegionTetgen();
      virtual void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        lvH_dtMeshOperator const * const mO
      );            
      virtual void operator()( dtGmshRegion * dtgr );
    private:
      void convertToTetgen(
        dtGmshRegion * gr, tetgenio * in,
        std::map< int, ::MVertex * > & MVertexInt,
        std::map< ::MVertex *, dtInt > & intMVertex              
      );
      void convertFromTetgen(
        dtGmshRegion * gr, tetgenio * out,
        std::map< int, ::MVertex * > & MVertexInt,
        std::map< ::MVertex *, dtInt > & intMVertex        
      );      
      static void enqueueMVertices(
        std::vector< ::MVertex * > & vv,
        dtInt & nodeIndex,
        std::map< int, ::MVertex * > & MVertexInt,
        std::map< ::MVertex *, dtInt > & intMVertex
      );
      static void enqueueMTriangles(
        std::vector< ::MTriangle * > & toEnqueue,
        std::vector< ::MTriangle * > & triangle
      );      
    private:
      std::string _tetOpt;
    private:
      static bool _registrated;
  };
}

#endif	/* dtMeshGRegionTetgen_H */
