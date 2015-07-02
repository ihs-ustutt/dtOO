#ifndef dtGmshMeshBoundaryLayer_H
#define	dtGmshMeshBoundaryLayer_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtOMMesh.h"
#include "dtOMDynamicVertexField.h"
#include "dtOMEdgeField.h"
#include "dtOMDynamicFaceField.h"

class GEntity;
class MVertex;
class MElement;

namespace dtOO {
  class dtGmshFace;
  class dtGmshModel;
  
  class dtGmshMeshBoundaryLayer {
    public:
      dt__classOnlyName(dtGmshMeshBoundaryLayer);      
      dtGmshMeshBoundaryLayer( 
        std::list< dtGmshFace const * > const & face, 
        std::vector< int > const & ori,
        std::list< dtGmshFace const * > const & fface, 
        std::vector< int > const & fori,
        std::list< dtGmshFace const * > const & mface, 
        std::vector< int > const & mori,
        float const & thickness, std::vector< float > const & spacing,
        float const & maxDihedralAngle,
        int const nSmoothingSteps, int const nShrinkingSteps 
      );
      virtual ~dtGmshMeshBoundaryLayer();
      void operator()( 
        dtGmshModel * const wModel,
        std::vector< ::MVertex * > & vertex, 
        std::vector< ::MElement * > & element
      );    
    private:
      void determineThickness(
		    dtOMVertexField< dtVector3 > const & nF
      );
      void modifyGEntities(dtGmshModel * wModel);
      void dihedralAngleIntersection(
        dtOMEdgeField< float > const & dAF, 
        dtOMVertexField< dtVector3 > const & nF
      );
      void createBoundaryLayerElements(
	      std::vector< ::MVertex * > & vertex, 
        std::vector< ::MElement * > & element        
      ) const;
      static bool isSlidable( 
        dtOMVertexField< bool > const & canSlideF,
        omVertexH const & vH
      );      
    private:
      float _thickness;
      int _nSmoothingSteps;
      int _nShrinkingSteps;
      float _maxDihedralAngle;
      std::vector< float > _spacing;
      //
      // define two surface meshes with fixed, thickness 
      // and slidable vertex field
      //      
		  dtOMMesh _omInit;
		  dtOMMesh _omMoved;      
		  dtOMDynamicVertexField< bool > _fixedF;
      dtOMDynamicVertexField< bool > _slidableF;
	    dtOMDynamicVertexField< float > _tF;
      dtOMDynamicFaceField< int > _extrudeF;
      dtOMDynamicFaceField< std::string > _physicalName;
  };
}

#endif	/* dtGmshMeshBoundaryLayer_H */

