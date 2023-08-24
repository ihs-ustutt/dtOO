#ifndef DTGMSHMODEL_H
#define	DTGMSHMODEL_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <gmsh/GModel.h>

class GEntity;
class GRegion;
class GFace;
class GEdge;
class GVertex;
class FieldManager;

namespace dtOO {
  class analyticGeometry;
  class map1dTo3d;
  class map2dTo3d;
  class map3dTo3d;
  class dtGmshRegion;
  class dtGmshFace;
  class dtGmshEdge;
  class dtGmshVertex;
  class renderInterface;
  class unstructured3dMesh;
  class unstructured3dSurfaceMesh;
  class discrete3dPoints;
  
  class dtGmshModel : public ::GModel {
    public:
      typedef std::map< int, std::vector< ::GEntity * > > intGEntityVMap;
    public:
      dt__class(dtGmshModel, ::GModel);      
      dtGmshModel(std::string name="");
      dtGmshModel( std::string name, ::FieldManager * fm );
      virtual ~dtGmshModel();
      //
      // get GEntitys
      //   
      dtGmshRegion * getDtGmshRegionByTag( dtInt const tag ) const;
      dtGmshFace * getDtGmshFaceByTag( dtInt const tag ) const;
      dtGmshEdge * getDtGmshEdgeByPhysical( 
        std::string const & physical 
      ) const;
      dtGmshFace * getDtGmshFaceByPhysical( 
        std::string const & physical 
      ) const;
      std::list< dtGmshFace * > getDtGmshFaceListByPhysical( 
        std::string const & physical 
      ) const;      
      dtGmshRegion * getDtGmshRegionByPhysical(
        std::string const & physical
      ) const;
      std::list< dtGmshRegion * > getDtGmshRegionListByPhysical( 
        std::string const & physical 
      ) const;      
      dtGmshEdge * getDtGmshEdgeByTag( dtInt const tag ) const;
      dtInt getDtGmshEdgeTagByFromTo( dtInt const from, dtInt const to ) const;
      dtGmshEdge * getDtGmshEdgeByFromTo( dtInt const from, dtInt const to ) const;
      dtGmshVertex * getDtGmshVertexByTag( dtInt const tag ) const;
      ::GEntity * getGEntityByPhysical( std::string const & physical ) const;
      //
      // casts
      //
      static dtGmshRegion * cast2DtGmshRegion( ::GEntity * gr );
      static dtGmshFace * cast2DtGmshFace( ::GEntity * gf );
      static ::GModel * cast2GModel( dtGmshModel * gm );      
      static ::GEntity * cast2GEntity( dtGmshRegion * gr );      
      static ::GEntity * cast2GEntity( dtGmshFace * gf );      
      static ::GEntity * cast2GEntity( dtGmshEdge * ge );      
      static ::GEntity * cast2GEntity( dtGmshVertex * gv );      
      static std::list< dtGmshFace * > cast2DtGmshFace( 
        std::list< ::GFace * > faces 
      );
      static std::vector< dtGmshFace * > cast2DtGmshFace( 
        std::vector< ::GFace * > faces 
      );      
      static std::list< dtGmshFace * > cast2DtGmshFace( 
        std::list< ::GEntity * > faces 
      );
      
      static std::list< dtGmshRegion * > cast2DtGmshRegion( 
        std::list< ::GRegion * > regions 
      );    
      static std::list< dtGmshRegion * > cast2DtGmshRegion( 
        std::list< ::GEntity * > regions 
      );
      static dtGmshEdge * cast2DtGmshEdge( ::GEntity * ge );  
      static std::list< dtGmshEdge * > cast2DtGmshEdge( 
        std::list< ::GEdge * > edges 
      );
      static std::vector< dtGmshEdge * > cast2DtGmshEdge( 
        std::vector< ::GEdge * > edges 
      );      
      static dtGmshVertex * cast2DtGmshVertex( ::GEntity * gv );
      static std::list< dtGmshVertex * > cast2DtGmshVertex( 
        std::list< ::GVertex * > vertices
      );
      static std::vector< dtGmshVertex * > cast2DtGmshVertex( 
        std::vector< ::GVertex * > vertices
      );
      //
      // modify position of MVertex and GVertex
      //
      static dtPoint3 extractPosition( ::GVertex const * const gv );  
      static dtPoint3 extractPosition( ::MVertex const * const mv );
      static void setPosition( ::MVertex * mv, dtPoint3 const & pp );
      //
      // add vertex
      //
      void addIfVertexToGmshModel( dtPoint3 const & vertex, dtInt * const tag );
      //
      // add edge
      //    
      void addIfEdgeToGmshModel(map1dTo3d const * const edge, dtInt * const tag);
      void addIfEdgeToGmshModel(
        map1dTo3d const * const edge, 
        dtInt * const tag, 
        dtInt const from, 
        dtInt const to 
      );
      // SWIG
      dtInt addIfEdgeToGmshModel(map1dTo3d const * const edge);
      //
      // add face
      //
      void addIfFaceToGmshModel(map2dTo3d const * const face, dtInt * const tag);    
      void addIfFaceToGmshModel(
        map2dTo3d const * const face, dtInt * const tag,
        std::list< ::GEdge * > const & edges, std::vector< dtInt > const & ori
      );
      void addIfFaceToGmshModel( 
        map2dTo3d const * const face, dtInt * const tag,
        dtInt const & eId0, dtInt const & eId1, dtInt const & eId2, dtInt const & eId3
      );
      // SWIG
      dtInt addIfFaceToGmshModel(map2dTo3d const * const face);
      //
      //add region
      //
      void addIfRegionToGmshModel(
        map3dTo3d const * const region, dtInt * const tag
      );
      void addIfRegionToGmshModel(
        map3dTo3d const * const region, dtInt * const tag,
        std::list< ::GFace * > const & faces, std::vector< dtInt > const & ori   
      );    
      void addIfRegionToGmshModel( 
        map3dTo3d const * const region, dtInt * const tag,
        dtInt const & fId0, dtInt const & fId1, 
        dtInt const & fId2, dtInt const & fId3, 
        dtInt const & fId4, dtInt const & fId5
      );
      // SWIG
      dtInt addIfRegionToGmshModel(
        map3dTo3d const * const region, std::vector< int > const & faceIds
      );    
      dtInt addIfRegionToGmshModel(
        map3dTo3d const * const region
      );    

      //
      //add
      //
      void addIfToGmshModel(
        analyticGeometry const * const aG, dtInt * const tag
      );      
      //
      // add general entities
      //
      void add( ::GEntity * ge);    
      void meshEdgeTransfiniteFromTo(
        dtInt const from, dtInt const to, 
        dtInt const type, dtReal const coeff, dtInt const nEl
      );
      //
      // mesh entities
      //
      void prepareToMesh( void );
      //
      // visualization
      //
      unstructured3dMesh * toUnstructured3dMesh( void ) const;
      static unstructured3dMesh * toUnstructured3dMesh(
        std::vector< ::MElement const * > const & elements
      );     
      static renderInterface * toAdequateSurfaceRenderInterface( 
          std::vector< ::MElement const * > const & elements
      );
      void dtReadCGNS(const std::string & name);
      void clearModel( void );
      //
      // get max tags of entities
      //
      dtInt getMaxVertexTag( void ) const;
      dtInt getMaxEdgeTag( void ) const;
      dtInt getMaxFaceTag( void ) const;
      dtInt getMaxRegionTag( void ) const;
      //
      // get lists of geometrical entities
      //    
      std::list< ::GVertex * > vertices( void ) const;    
      std::list< ::GEdge * > edges( void ) const;    
      std::list< ::GFace * > faces( void ) const;
      std::list< ::GRegion * > regions( void ) const;
      std::list< dtGmshVertex * > dtVertices( void ) const;   
      std::list< dtGmshEdge * > dtEdges( void ) const; 
      std::list< dtGmshFace * > dtFaces( void ) const;
      std::list< dtGmshRegion * > dtRegions( void ) const;
      //
      // physical handling
      //
      void untagPhysical( ::GEntity * const ge );
      std::string getPhysicalString(::GEntity const * const ge ) const;
      std::vector< std::string > getPhysicalNames(
        dtInt const & dim, std::vector< dtInt > const & number
      ) const;
      void tagPhysical(::GEntity * const ge, std::string const & pName);
      dtInt getPhysicalNumber(const dtInt &dim, const std::string &name) const;
      void removeEmptyPhysicals( void );
      std::list< std::string > getFullPhysicalList( 
        ::GEntity const * const ent 
      ) const;
      std::list< ::GEntity * > getGEntityListByWildCardPhysical( 
        std::string wildStr
      ) const;      
      bool matchWildCardPhysical( 
        std::string wildStr, ::GEntity const * const ge 
      ) const;
      void setDebug( std::string const debug );
      //
      //
      //
      std::map< ::GEntity *, ::GEntity * > createTwin( 
        dtInt const dim0Tag, 
        dtInt const dim1Tag, 
        dtInt const dim2Tag, 
        dtInt const dim3Tag 
      ) const;      
    private:    
      static unstructured3dSurfaceMesh * toUnstructured3dSurfaceMesh( 
        std::vector< ::MVertex const * > const & vertices, 
        std::vector< ::MElement const * > const & elements
      );    
      static discrete3dPoints * toDiscrete3dPoints( 
        std::vector< ::MVertex const * > const & vertices
      );    
      static unstructured3dMesh * toUnstructured3dMesh(
        std::vector< ::MVertex const * > const & vertices, 
        std::vector< ::MElement const * > const & elements
      );     
      static renderInterface * toAdequateSurfaceRenderInterface( 
        std::vector< ::MVertex const * > const & vertices, 
        std::vector< ::MElement const * > const & elements
      );     
      //
      // check if geometrical entity is already in model
      //
      dtInt alreadyInModel( ::GVertex const * const gv ) const;
      dtInt alreadyInModel( ::GEdge const * const ge ) const;
      dtInt alreadyInModel( ::GFace const * const gf ) const;
      dtInt alreadyInModel( ::GRegion const * const gr ) const;
      //
      //
      //
      std::map< dtGmshVertex *, dtGmshVertex * > createVertexTwins( 
        dtInt startTag 
      ) const;
      std::map< dtGmshEdge *, dtGmshEdge * > createEdgeTwins( 
        dtInt startTag 
      ) const;
      std::map< dtGmshFace *, dtGmshFace * > createFaceTwins( 
        dtInt startTag
      ) const;
      std::map< dtGmshRegion *, dtGmshRegion * > createRegionTwins( 
        dtInt startTag 
      ) const;
      void updateAssociations( 
        std::map< dtGmshVertex *, dtGmshVertex * > & dim0, 
        std::map< dtGmshEdge *, dtGmshEdge * > & dim1,
        std::map< dtGmshFace *, dtGmshFace * > & dim2,
        std::map< dtGmshRegion *, dtGmshRegion * > & dim3
      ) const;    
    private:
      static std::map< std::string, dtInt > _facePositionStr;
      static std::map< std::string, dtInt > _edgePositionStr;
      static std::map< std::string, dtInt > _vertexPositionStr;
      static std::map< int, std::vector< std::string > > _positionStrFace;      
      static std::map< int, std::vector< std::string > > _positionStrEdge;      
      static std::map< int, std::vector< std::string > > _positionStrVertex;      
      std::string _debug;
      ::FieldManager * _fmTwin;
  };
}
#endif	/* DTGMSHMODEL_H */

