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
  class map3dTo3d;
  class dtGmshRegion;
  class dtGmshFace;
  class dtGmshEdge;
  class dtGmshVertex;
  class unstructured3dMesh;
  class unstructured3dSurfaceMesh;
  
  class dtGmshModel : public GModel {
  public:
		typedef std::map< int, std::vector< GEntity * > > intGEntityVMap;
  public:
    dt__CLASSSTD(dtGmshModel, GModel);      
    dtGmshModel(std::string name="");
    virtual ~dtGmshModel();
    dtGmshRegion * getDtGmshRegionByTag( int const tag ) const;
    dtGmshFace * getDtGmshFaceByTag( int const tag ) const;
    dtGmshEdge * getDtGmshEdgeByTag( int const tag ) const;  
    int getDtGmshEdgeTagByFromTo( int const from, int const to ) const;
    dtGmshEdge * getDtGmshEdgeByFromTo( int const from, int const to ) const;
    dtGmshVertex * getDtGmshVertexByTag( int const tag ) const;
    static dtGmshRegion * cast2DtGmshRegion( GEntity * gr );
    static dtGmshFace * cast2DtGmshFace( GEntity * gf );
    static dtGmshEdge * cast2DtGmshEdge( GEntity * ge );  
    static dtGmshVertex * cast2DtGmshVertex( GEntity * gv );
    static dtPoint3 cast2DtPoint3( GVertex * gv );  
    static dtPoint3 cast2DtPoint3( MVertex * mv );
    void addIfVertexToGmshModel( dtPoint3 const & vertex, int * const tag );
    void addIfEdgeToGmshModel(
      map1dTo3d const * const edge, 
      int * const tag, 
      int const from, 
      int const to 
    );
    dtGmshRegion * addRegionToGmshModel( map3dTo3d const * const vol );
    void meshEdgeTransfiniteFromTo(int const from, int const to, int const type, float const coeff, int const nEl);
    void meshVertex( int const tag = 0 );
    void meshEdge( int const tag );
    void meshFace( int const tag );
    void meshRegion( int const tag );
    int alreadyInModel( GVertex const * const gv ) const;
    int alreadyInModel( GEdge const * const ge ) const;
    int alreadyInModel( GFace const * const gf ) const;
    unstructured3dMesh * toUnstructured3dMesh( void ) const;
    static unstructured3dMesh * toUnstructured3dMesh(
      std::vector< MVertex * > const & vertices, std::vector< MElement * > const & elements
    ); 
    static unstructured3dSurfaceMesh * toUnstructured3dSurfaceMesh( 
      std::vector< MVertex * > const & vertices, std::vector< MElement * > const & elements
    );
    void dtReadCGNS(const std::string & name);
    void clearModel( void );
  };
}
#endif	/* DTGMSHMODEL_H */

