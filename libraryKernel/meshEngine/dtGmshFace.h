#ifndef DTGMSHFACE_H
#define	DTGMSHFACE_H

#include <dtLinearAlgebra.h>
#include <gmsh/GFace.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/twoDArrayHandling.h>

class GModel;
class GEdge;
class MVertex;

namespace dtOO {
  class map2dTo3d;
    
  class dtGmshFace : public GFace {
  public:
    dt__CLASSSTD(dtGmshFace, GEntity);
    dtGmshFace(GModel *m, int tag);
    dtGmshFace(GModel *m, int tag, const std::list<GEdge*> &edges, const std::vector< int > & ori);
    dtGmshFace(GModel *m, int tag, const std::list<GEdge*> &edges);
    virtual ~dtGmshFace();
    virtual Range<double> parBounds(int i) const; 
    virtual GPoint point(double par1, double par2) const; 
    virtual Pair<SVector3,SVector3> firstDer(const SPoint2 &param) const;
    virtual void secondDer(const SPoint2 &param, 
                           SVector3 *dudu, SVector3 *dvdv, SVector3 *dudv) const;
    SPoint2 reparamOnFace( dtPoint3 const ppXYZ ) const;
    SPoint2 reparamOnFace(GVertex const * gv) const;
    dtPoint2 reparamOnFace2d( dtPoint3 const ppXYZ ) const;
    virtual SPoint2 parFromPoint(const SPoint3 &, bool onSurface) const;    
    virtual std::list<GEdge*> edges( void ) const;
    virtual void setMap2dTo3d( map2dTo3d const * const base );
    virtual map2dTo3d const * getMap2dTo3d( void ) const;
    void addEdge( GEdge * edge, int const ori );
    void addEdgeLoop( std::list< GEdge * > edgeL );
    bool isClosed( int const dim ) const;
    void meshTransfinite( void );
    virtual void updateFace( void );
    virtual void makeSuitable( void );
    bool isEqual( GFace const * const gf ) const;
    static bool isEqual( GFace const * const gf0, GFace const * const gf1 );
    twoDArrayHandling< MVertex * > reconstructEdgesFromSurfaceMesh( void ) const;
    void getMeshVerticesAndElements(
      std::vector< MVertex const * > * const mv, 
      std::vector< MElement const * > * const me
    ) const;
  private:
    static bool sortPredicate(MVertex const * d1, MVertex const * d2);
  private:
    dt__pH(map2dTo3d) _mm;
  };
}

#endif	/* DTGMSHFACE_H */
