#ifndef DTGMSHMODEL_H
#define	DTGMSHMODEL_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <gmsh/GModel.h>

class GEntity;
class GRegion;
class GFace;
class GEdge;
class GVertex;

namespace dtOO {
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
    virtual ~dtGmshModel();
    //
    // get GEntitys
    //
    dtGmshRegion * getDtGmshRegionByTag( int const tag ) const;
    dtGmshFace * getDtGmshFaceByTag( int const tag ) const;
    dtGmshFace * getDtGmshFaceByPhysical( std::string const & physical ) const;
    dtGmshRegion * getDtGmshRegionByPhysical(
      std::string const & physical
    ) const;
    dtGmshEdge * getDtGmshEdgeByTag( int const tag ) const;  
    int getDtGmshEdgeTagByFromTo( int const from, int const to ) const;
    dtGmshEdge * getDtGmshEdgeByFromTo( int const from, int const to ) const;
    dtGmshVertex * getDtGmshVertexByTag( int const tag ) const;
    //
    // casts
    //
    static dtGmshRegion * cast2DtGmshRegion( ::GEntity * gr );
    static dtGmshFace * cast2DtGmshFace( ::GEntity * gf );
    static std::list< dtGmshFace * > cast2DtGmshFace( 
      std::list< ::GFace * > faces 
    );
    static dtGmshEdge * cast2DtGmshEdge( ::GEntity * ge );  
    static std::list< dtGmshEdge * > cast2DtGmshEdge( 
      std::list< ::GEdge * > edges 
    );
    static dtGmshVertex * cast2DtGmshVertex( ::GEntity * gv );
    //
    // modify postion of MVertex and GVertex
    //
    static dtPoint3 extractPosition( ::GVertex const * const gv );  
    static dtPoint3 extractPosition( ::MVertex const * const mv );
    static void setPosition( ::MVertex * mv, dtPoint3 const & pp );
    //
    // add vertex
    //
    void addIfVertexToGmshModel( dtPoint3 const & vertex, int * const tag );
    //
    // add edge
    //    
    void addIfEdgeToGmshModel(map1dTo3d const * const edge, int * const tag);
    //
    // add face
    //
    void addIfFaceToGmshModel(map2dTo3d const * const face, int * const tag);    
    //
    //add region
    //
    void addIfRegionToGmshModel(
      map3dTo3d const * const region, int * const tag
    );    
    dtGmshRegion * addRegionToGmshModel( map3dTo3d const * const vol );
    void meshEdgeTransfiniteFromTo(
      int const from, int const to, 
      int const type, float const coeff, int const nEl
    );
    //
    // mesh entities
    //
    void meshVertex( int const tag = 0 );
    void meshEdge( int const tag );
    void meshFace( int const tag );
    void meshRegion( int const tag );
    void meshRegion( void );    
    void meshPhysical(int const & dim);      
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
    int getMaxVertexTag( void );
    int getMaxEdgeTag( void );
    int getMaxFaceTag( void );
    int getMaxRegionTag( void );
    //
    // get lists of geometrical entities
    //    
    std::list< ::GVertex * > vertices( void ) const;    
    std::list< ::GEdge * > edges( void ) const;    
    std::list< ::GFace * > faces( void ) const;
    std::list< ::GRegion * > regions( void ) const;
    //
    // physical handling
    //
    void untagPhysical( ::GEntity * const ge );
    std::string getPhysicalString(::GEntity * const ge ) const;
    void tagPhysical(::GEntity * const ge, std::string const & pName);
    //
    // guess entity to which an element belongs
    //
    static ::GEntity * guessOnWhat( ::MElement const * const me );
    void setDebug( std::string const debug );
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
    void addIfEdgeToGmshModel(
      map1dTo3d const * const edge, 
      int * const tag, 
      int const from, 
      int const to 
    );
    void addIfFaceToGmshModel(
      map2dTo3d const * const face, int * const tag,
      std::list< ::GEdge * > const & edges, std::vector< int > const & ori
    );
    void addIfFaceToGmshModel( 
      map2dTo3d const * const face, int * const tag,
      int const & eId0, int const & eId1, int const & eId2, int const & eId3
    );      
    void addIfRegionToGmshModel(
      map3dTo3d const * const region, int * const tag,
      std::list< ::GFace * > const & faces, std::vector< int > const & ori   
    );    
    void addIfRegionToGmshModel( 
      map3dTo3d const * const region, int * const tag,
      int const & fId0, int const & fId1, 
      int const & fId2, int const & fId3, 
      int const & fId4, int const & fId5
    );      
    //
    // check if geometrical entity is already in model
    //
    int alreadyInModel( ::GVertex const * const gv ) const;
    int alreadyInModel( ::GEdge const * const ge ) const;
    int alreadyInModel( ::GFace const * const gf ) const;
    int alreadyInModel( ::GRegion const * const gr ) const;     
  private:
    std::string _debug;
  };
}
#endif	/* DTGMSHMODEL_H */

