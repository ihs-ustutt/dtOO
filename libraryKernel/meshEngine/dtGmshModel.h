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
    dtGmshRegion * getDtGmshRegionByTag( int const tag ) const;
    dtGmshFace * getDtGmshFaceByTag( int const tag ) const;
    dtGmshFace * getDtGmshFaceByPhysical( std::string const & physical ) const;
    dtGmshEdge * getDtGmshEdgeByTag( int const tag ) const;  
    int getDtGmshEdgeTagByFromTo( int const from, int const to ) const;
    dtGmshEdge * getDtGmshEdgeByFromTo( int const from, int const to ) const;
    dtGmshVertex * getDtGmshVertexByTag( int const tag ) const;
    static dtGmshRegion * cast2DtGmshRegion( ::GEntity * gr );
    static dtGmshFace * cast2DtGmshFace( ::GEntity * gf );
    static std::list< dtGmshFace * > cast2DtGmshFace( std::list< ::GFace * > faces );
    static dtGmshEdge * cast2DtGmshEdge( ::GEntity * ge );  
    static std::list< dtGmshEdge * > cast2DtGmshEdge( std::list< ::GEdge * > edges );
    static dtGmshVertex * cast2DtGmshVertex( ::GEntity * gv );
    static dtPoint3 cast2DtPoint3( ::GVertex * gv );  
    static dtPoint3 cast2DtPoint3( ::MVertex * mv );
    static void setPosition( ::MVertex * mv, dtPoint3 const & pp );
    void addIfVertexToGmshModel( dtPoint3 const & vertex, int * const tag );
    void addIfEdgeToGmshModel(
      map1dTo3d const * const edge, 
      int * const tag, 
      int const from, 
      int const to 
    );
    void addIfEdgeToGmshModel(map1dTo3d const * const edge, int * const tag);
    void addIfFaceToGmshModel(
      map2dTo3d const * const face, int * const tag,
      std::list< ::GEdge * > const & edges, std::vector< int > const & ori
    );
    void addIfFaceToGmshModel( 
      map2dTo3d const * const face, int * const tag,
      int const & eId0, int const & eId1, int const & eId2, int const & eId3
    );  
    void addIfFaceToGmshModel(map2dTo3d const * const face, int * const tag);    
    dtGmshRegion * addRegionToGmshModel( map3dTo3d const * const vol );
    void meshEdgeTransfiniteFromTo(int const from, int const to, int const type, float const coeff, int const nEl);
    void meshVertex( int const tag = 0 );
    void meshEdge( int const tag );
    void meshFace( int const tag );
    void meshRegion( int const tag );
    void meshRegion( void );    
    int alreadyInModel( ::GVertex const * const gv ) const;
    int alreadyInModel( ::GEdge const * const ge ) const;
    int alreadyInModel( ::GFace const * const gf ) const;
    unstructured3dMesh * toUnstructured3dMesh( void ) const;
    static unstructured3dMesh * toUnstructured3dMesh(
      std::vector< ::MVertex const * > const & vertices, 
      std::vector< ::MElement const * > const & elements
    ); 
    static unstructured3dMesh * toUnstructured3dMesh(
      std::vector< ::MElement const * > const & elements
    );     
    static renderInterface * toAdequateSurfaceRenderInterface( 
        std::vector< ::MElement const * > const & elements
	  );
    static renderInterface * toAdequateSurfaceRenderInterface( 
      std::vector< ::MVertex const * > const & vertices, 
      std::vector< ::MElement const * > const & elements
    );
    void dtReadCGNS(const std::string & name);
    void clearModel( void );
    int getMaxVertexTag( void );
    int getMaxEdgeTag( void );
    int getMaxFaceTag( void );
    int getMaxRegionTag( void );    
    std::list< ::GVertex * > vertices( void ) const;    
    std::list< ::GEdge * > edges( void ) const;    
    std::list< ::GFace * > faces( void ) const;
    std::list< ::GRegion * > regions( void ) const;
    void meshPhysical(int const & dim);
	  int getMeshVerticesForPhysicalGroup(
      int const & dim, 
      int const & num, 
      std::vector< ::MVertex const * > & vertices
    );    
    void tagPhysical(::GEntity * const ge, std::string const & pName);
  private:
    static unstructured3dSurfaceMesh * toUnstructured3dSurfaceMesh( 
      std::vector< ::MVertex const * > const & vertices, 
      std::vector< ::MElement const * > const & elements
    );    
    static discrete3dPoints * toDiscrete3dPoints( 
      std::vector< ::MVertex const * > const & vertices
    );    
  };
}
#endif	/* DTGMSHMODEL_H */

