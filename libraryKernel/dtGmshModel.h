#ifndef DTGMSHMODEL_H
#define	DTGMSHMODEL_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <gmsh/GModel.h>

class GRegion;
class GFace;
class GEdge;
class GVertex;

namespace dtOO {
  class map1dTo3d;
  class map3dTo3d;
  class dtGmshRegion;
  class dtGmshFace;
  class dtGmshEdge;
  class dtGmshVertex;
  class boundedVolume;
  
  class dtGmshModel : public GModel {
  public:
    dt__CLASSNAME(dtGmshModel);      
    dtGmshModel(std::string name="");
    virtual ~dtGmshModel();
    virtual void attachToBoundedVolume( boundedVolume const * const bV);
    virtual boundedVolume const * getAttachedBoundedVolume( void );    
    dtGmshRegion * getDtGmshRegionByTag( int const tag ) const;
    dtGmshFace * getDtGmshFaceByTag( int const tag ) const;
    dtGmshEdge * getDtGmshEdgeByTag( int const tag ) const;  
    int getDtGmshEdgeTagByFromTo( int const from, int const to ) const;
    dtGmshEdge * getDtGmshEdgeByFromTo( int const from, int const to ) const;
    dtGmshVertex * getDtGmshVertexByTag( int const tag ) const;
    dtGmshRegion * cast2DtGmshRegion( GRegion * gr ) const;
    dtGmshFace * cast2DtGmshFace( GFace * gf ) const;
    dtGmshEdge * cast2DtGmshEdge( GEdge * ge ) const;  
    dtGmshVertex * cast2DtGmshVertex( GVertex * gv ) const;
    dtPoint3 cast2DtPoint3( GVertex * gv ) const;    
    void addVertexToGmshModel( dtPoint3 const & vertex, int const tag);
    void addVertexToGmshModel( dtPoint3 const * const vertex, int const tag);
    void addIfVertexToGmshModel( dtPoint3 const & vertex, int * const tag );
    void addEdgeToGmshModel( 
      map1dTo3d const * const edge, 
      int const tag, 
      int const from, 
      int const to
    );
  void addIfEdgeToGmshModel(
    map1dTo3d const * const edge, 
    int * const tag, 
    int const from, 
    int const to 
  );
//    void addVertexToGmshModel( dtPoint3 const & vertex, int const tag); 
    dtGmshRegion * addRegionToGmshModel( map3dTo3d const * const vol );
    void createEdge(int const geI, int const gfI, int const v0, int const v1);
    void createEmbeddedEdge(int const geI, int const gfI, int const v0, int const v1);    
    std::list< dtGmshEdge * > parallelEdges( dtGmshEdge const * const ge ) const;
    void meshEdgeTransfiniteFromTo(int const from, int const to, int const type, float const coeff, int const nEl);
    void meshVertex( int const tag = 0 );
    void meshEdge( int const tag );
    void meshFace( int const tag );
    void meshRegion( int const tag );
    int alreadyInModel( GVertex const * const gv ) const;
    int alreadyInModel( GEdge const * const ge ) const;
    int alreadyInModel( GFace const * const gf ) const;
  private:
    boundedVolume const * _bV;
  
  };
}
#endif	/* DTGMSHMODEL_H */

