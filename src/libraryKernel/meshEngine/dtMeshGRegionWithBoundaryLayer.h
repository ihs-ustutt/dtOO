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
class FieldManager;

void setRand(double r[6]);
int intersect_line_triangle(
  double X[3], double Y[3], double Z[3] ,
  double P[3], double N[3], const double eps_prec
);

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
      void determinMinMaxAverageAtSliders(void);
      void meshNormalsPointOutOfTheRegion( dtGmshRegion * dtgr );
    private:
      static int _NORMAL;
      static int _SLIDER;
      static int _FIXER;        
      int _nNormalSmoothingSteps;
      int _nGrowingSmoothingSteps;
      float _maxGrowingRatePerStep;
      float _maxDihedralAngle;
      int _nSpacingSteps;
      bool _flipNormal;
      //
      // define two surface meshes with fixed, thickness 
      // and slidable vertex field
      //      
      dtOMMesh _omInit;
      dtOMMesh _omMoved;      
      dtOMDynamicVertexField< bool > _fixedF;
      dtOMDynamicVertexField< bool > _slidableF;
      dtOMDynamicVertexField< float > _tF;
      dtOMDynamicVertexField< dtVector3 > _nF;
      dtOMDynamicFaceField< int > _extrudeF;
      dtOMDynamicFaceField< int > _typeF;
      dtOMDynamicVertexField< std::vector< ::MVertex * > > _buddyF;
      dtOMDynamicVertexField< std::vector< float > > _realSpacing;
      std::vector< std::string > _faceLabel;
      std::vector< std::string > _fixedFaceLabel;
      std::vector< std::string > _slidableFaceLabel;
      dtMesh3DOperator * _3D;
      std::vector< float > _avSpacing;
      std::vector< float > _maxSpacing;
      std::vector< float > _minSpacing;
      float _avT;
      float _maxT;
      float _minT;
  };
}

#endif	/* dtMeshGRegionWithBoundaryLayer_H */

