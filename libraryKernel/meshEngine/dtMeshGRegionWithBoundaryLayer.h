#ifndef dtMeshGRegionWithBoundaryLayer_H
#define	dtMeshGRegionWithBoundaryLayer_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtOMMesh.h"
#include "dtOMDynamicVertexField.h"
#include "dtOMEdgeField.h"
#include "dtOMDynamicFaceField.h"
#include "dtMesh3DOperator.h"

class GEntity;
class MVertex;
class MElement;

namespace dtOO {
  class dtGmshFace;
  class dtGmshModel;
  class dtGmshRegion;
  
  class dtMeshGRegionWithBoundaryLayer : public dtMesh3DOperator {
    public:
      dt__classOnlyName(dtMeshGRegionWithBoundaryLayer);          
      dtMeshGRegionWithBoundaryLayer();
      virtual ~dtMeshGRegionWithBoundaryLayer();
      virtual void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        vectorHandling< constValue * > const * const cV,
        vectorHandling< analyticFunction * > const * const aF,
        vectorHandling< analyticGeometry * > const * const aG,
        vectorHandling< boundedVolume * > const * const bV,
        vectorHandling< dtMeshOperator * > const * const mO
      );            
      virtual void operator()( dtGmshRegion * dtgr );
    private:
      void createLayerVertices(
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
      omHalfedgeH slidableHalfedgeInFace( omFaceH const & fH ) const;
      void createRingOfBuddies( MVertex const * const mv0 );
      void createMovedMesh( void );
      void meshWithGmsh(
        dtOMMesh const & mesh, 
        std::vector< ::MVertex * > & vertex, 
        std::vector< ::MElement * > & element
      ) const;
    private:
      static int _NORMAL;
      static int _SLIDER;
      static int _FIXER;        
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
      dtOMDynamicFaceField< int > _typeF;
      dtOMDynamicVertexField< std::vector< ::MVertex * > > _buddyF;
      std::vector< std::string > _faceLabel;
      std::vector< int > _faceOrientation;
      std::vector< std::string > _fixedFaceLabel;
      std::vector< int > _fixedFaceOrientation;
      std::vector< std::string > _slidableFaceLabel;
      std::vector< int > _slidableFaceOrientation;
      dtMesh3DOperator * _3D;
  };
}

#endif	/* dtMeshGRegionWithBoundaryLayer_H */

