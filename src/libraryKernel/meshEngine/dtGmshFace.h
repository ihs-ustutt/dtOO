#ifndef DTGMSHFACE_H
#define	DTGMSHFACE_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <gmsh/GFace.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/twoDArrayHandling.h>

class GModel;
class GEdge;
class MVertex;

namespace dtOO {
  class map2dTo3d;
  class dtOMMesh;
  class dtGmshModel;
  class dtGmshVertex;
  class dtGmshEdge;
  class dtGmshRegion;
    
  class dtGmshFace : public ::GFace {
  public:
    dt__class(dtGmshFace, ::GEntity);
    dtGmshFace(::GModel *m, dtInt tag);
    dtGmshFace(
      ::GModel *m, dtInt tag, 
      const std::list< ::GEdge * > &edges, const std::vector< dtInt > & ori
    );
    dtGmshFace(::GModel *m, dtInt tag, const std::list< ::GEdge * > &edges);
    virtual ~dtGmshFace();
    dtGmshModel const & refDtGmshModel( void ) const;    
    dtGmshModel & refDtGmshModel( void );    
    virtual ::GEntity::GeomType geomType( void ) const; 
    void setGeomType( ::GEntity::GeomType const & gT );    
    virtual Range<double> parBounds(int i) const; 
    virtual GPoint point(double par1, double par2) const; 
    virtual Pair<SVector3,SVector3> firstDer(const SPoint2 &param) const;
    virtual void secondDer(
      const SPoint2 &param, SVector3 & dudu, SVector3 & dvdv, SVector3 & dudv
    ) const;
    using GFace::normal;
    dtVector3 normal( dtPoint2 const & uv ) const;
    SPoint2 reparamOnFace( dtPoint3 const ppXYZ ) const;
    SPoint2 reparamOnFace(::GVertex const * gv) const;
    virtual SPoint2 parFromPoint(const SPoint3 &, bool onSurface) const; 
    virtual GPoint closestPoint(
      const SPoint3 &queryPoint, const double initialGuess[2]
    ) const;    
    virtual void setMap2dTo3d( map2dTo3d const * const base );
    virtual map2dTo3d const * getMap2dTo3d( void ) const;
    void addEdge( ::GEdge * edge, dtInt const ori );
    dtInt edgeOrientation( ::GEdge * edge ) const;
    void addEdgeLoop( std::list< ::GEdge * > edgeL );
    bool isClosed( dtInt const dim ) const;
    void meshTransfinite( void );
    void meshRecombine( void );
    void meshWNElements( 
      dtInt const & nElementsU, dtInt const & nElementsV 
    );
    void meshWNElements(
      dtInt const & nElements0, dtInt const & nElements1, 
      dtInt const & nElements2, dtInt const & nElements3 
    );
    std::vector< dtInt > estimateTransfiniteNElements( 
      dtReal const & uWidth, dtReal const & vWidth 
    ) const;
    void correctIfTransfinite( void );
    void meshUnstructured( void ); 
    bool isEqual( ::GFace const * const gf ) const;
    static bool isEqual( ::GFace const * const gf0, ::GFace const * const gf1 );
    std::vector< const ::MVertex * > getMeshVertices( void ) const;
    void getMeshVerticesAndElements(
      std::vector< ::MVertex const * > * const mv, 
      std::vector< ::MElement const * > * const me
    ) const;
    dtOMMesh * getOMMesh( void ) const;
    void addElement( ::MElement * me );
    void addGEntity( ::GEntity * const gEnt );
    std::string getPhysicalString( void ) const;
    void setGrading( 
      std::vector< dtReal > const & grading, std::vector< dtReal > & type
    );
    std::list< dtGmshVertex * > dtVertices( void ) const;   
    std::list< dtGmshEdge * > dtEdges( void ) const;     
    std::list< dtGmshRegion * > dtRegions( void ) const;     
  private:
    static bool sortPredicate(::MVertex const * d1, ::MVertex const * d2);
  private:
    dt__pH(map2dTo3d) _mm;
    ::GEntity::GeomType _geomType;    
  };
}

#endif	/* DTGMSHFACE_H */

