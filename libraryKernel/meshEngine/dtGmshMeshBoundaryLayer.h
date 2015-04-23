#ifndef dtGmshMeshBoundaryLayer_H
#define	dtGmshMeshBoundaryLayer_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtOMMesh.h"
#include "dtOMVertexField.h"
#include "dtOMEdgeField.h"

class GEntity;
class MVertex;
class MElement;

namespace dtOO {
  class dtGmshFace;
  class dtGmshModel;
  
  class dtGmshMeshBoundaryLayer {
    public:
      dt__classOnlyName(dtGmshMeshBoundaryLayer);      
      dtGmshMeshBoundaryLayer();
      dtGmshMeshBoundaryLayer( 
        float const & thickness, std::vector< float > const & spacing,
        float const & maxDihedralAngle,
        int const nSmoothingSteps, int const nShrinkingSteps 
      );
      virtual ~dtGmshMeshBoundaryLayer();
      void operator()( 
        dtGmshModel * const wModel,
        std::list< dtGmshFace const * > const & face, 
        std::vector< int > const & ori,
        std::list< dtGmshFace const * > const & fface, 
        std::vector< int > const & fori,
        std::vector< ::MVertex * > & vertex, std::vector< ::MElement * > & element
      );    
    private:
      void determineThickness(
        dtOMMesh const & omInit,
        dtOMVertexField< dtVector3 > const & nF, dtOMVertexField< bool > const & fixedF, 
        dtOMMesh & omMoved, dtOMVertexField< float > & tF
      ) const;
      void modifyGEntities(
        dtOMMesh & omInit,      
        dtOMVertexField< bool > const & fixedF,
        dtGmshModel * wModel
      ) const;
      void dihedralAngleIntersection(
        dtOMMesh const & omInit, dtOMMesh & omMoved,
        dtOMEdgeField< float > const & dAF,        
        dtOMVertexField< bool > const & fixedF, 
        dtOMVertexField< dtVector3 > const & nF, dtOMVertexField< float > & tF
      ) const;
      void createBoundaryLayerElements(
        dtOMMesh const & omInit, dtOMMesh const & omMoved, 
        dtOMVertexField< bool > const & fixedF,
	      std::vector< ::MVertex * > & vertex, std::vector< ::MElement * > & element        
      ) const;
    private:
      float _thickness;
      int _nSmoothingSteps;
      int _nShrinkingSteps;
      float _maxDihedralAngle;
      std::vector< float > _spacing;
  };
}

#endif	/* dtGmshMeshBoundaryLayer_H */

