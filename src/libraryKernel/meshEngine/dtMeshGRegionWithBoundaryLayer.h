#ifndef dtMeshGRegionWithBoundaryLayer_H
#define	dtMeshGRegionWithBoundaryLayer_H

#include <dtOOTypeDef.h>

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
class FieldManager;

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
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG,
        bVPtrVec const * const bV,
        labeledVectorHandling< dtMeshOperator * > const * const mO
      );            
      virtual void operator()( dtGmshRegion * dtgr );
    private:
      void createLayerVertices(void);
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
        std::vector< ::MElement * > & element,
        ::FieldManager * fm
      ) const;
      void adjustThickness( void );
      void determinMinMaxAverageAtSliders(
        twoDArrayHandling< dtReal > & avSpacing,
        twoDArrayHandling< dtReal > & maxSpacing,
        twoDArrayHandling< dtReal > & minSpacing,
        vectorHandling< dtReal > & avT,
        vectorHandling< dtReal > & maxT,
        vectorHandling< dtReal > & minT      
      );
    private:
      static int _NORMAL;
      static int _SLIDER;
      static int _FIXER;        
      int _nNormalSmoothingSteps;
      int _nGrowingSmoothingSteps;
      dtReal _maxGrowingRatePerStep;
      dtReal _maxDihedralAngle;
      std::vector< int > _nSpacingSteps;
      std::vector< bool > _flipNormal;
      //
      // define two surface meshes with fixed, thickness 
      // and slidable vertex field
      //      
      dtOMMesh _omInit;
      dtOMMesh _omMoved;      
      dtOMDynamicVertexField< bool > _fixedF;
      dtOMDynamicVertexField< bool > _slidableF;
      dtOMDynamicVertexField< dtReal > _tF;
      dtOMDynamicVertexField< dtVector3 > _nF;  
      dtOMDynamicVertexField< bool > _flipNormalF;
      dtOMDynamicVertexField< std::vector< ::MVertex * > > _buddyF;
      dtOMDynamicVertexField< std::vector< dtReal > > _realSpacing;
      dtOMDynamicVertexField< int > _faceIndex;
      dtOMDynamicFaceField< int > _extrudeF;
      dtOMDynamicFaceField< int > _typeF;
      dtOMDynamicFaceField< int > _nSpacingStepsF;         
      std::vector< std::string > _faceLabel;
      std::vector< std::string > _fixedFaceLabel;
      std::vector< std::string > _slidableFaceLabel;
      dtMesh3DOperator * _3D;
  };
}

#endif	/* dtMeshGRegionWithBoundaryLayer_H */

