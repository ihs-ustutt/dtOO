#ifndef DTGMSHEDGE_H
#define	DTGMSHEDGE_H

#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <gmsh/GEdge.h>

class MElement;
class GVertex;
class GEntity;
class SBoundingBox3d;

namespace dtOO {
  class map1dTo3d;
  
  class dtGmshEdge : public ::GEdge {
  public:
    dt__class(dtGmshEdge, ::GEntity);
    dtGmshEdge(::GModel *m, int tag);
    dtGmshEdge(::GModel * model, int tag, ::GVertex *v1, ::GVertex *v2);
    virtual ~dtGmshEdge();
    virtual Range<double> parBounds(int i) const;
    virtual GPoint point(double p) const;
    virtual SVector3 firstDer(double par) const;
    void setMap1dTo3d( map1dTo3d const * const base );
    void setMap1dTo3dNoClone( map1dTo3d * base );
    map1dTo3d const * getMap1dTo3d( void ) const;
    void setBeginVertex( ::GVertex * gv );
    void setEndVertex( ::GVertex * gv );
    void meshTransfinite( int const type, float const coeff );
    void meshTransfiniteWNElements( 
      int const type, float const coeff, int const nElements 
    );
    void addGEntity( ::GEntity * const gEnt );
    void addVertex( ::GVertex * gv);
    void addElement( ::MElement * me );
    virtual ::SBoundingBox3d bounds( void ) const;
    static bool isEqual( ::GEdge const * const ge0, ::GEdge const * const ge1 );
  private:
    void setNElements( int const nE );    
  private:
    ptrHandling< map1dTo3d > _mm;
    bool _isSeamU;
    bool _isSeamV;    
  };
}
#endif	/* DTGMSHEDGE_H */
