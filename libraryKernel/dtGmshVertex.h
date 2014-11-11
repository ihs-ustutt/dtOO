#ifndef DTGMSHVERTEX_H
#define	DTGMSHVERTEX_H

#include <gmsh/GVertex.h>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

class GPoint;
  
namespace dtOO {
  class map2dTo3d;
  class map3dTo3d;
  
  class dtGmshVertex : public GVertex {
  public:
    dt__CLASSNAME(dtGmshVertex); 
    dtGmshVertex(GModel *m, int tag, double ms);
    dtGmshVertex(GModel *m, int tag);
    virtual ~dtGmshVertex();
    virtual GPoint point() const;
    virtual double x() const;
    virtual double y() const;
    virtual double z() const;    
    virtual void setPosition(GPoint &p);
    virtual void setPosition(dtPoint3 const p);    
    virtual void setPosition(dtPoint3 const * const p);
    void snapToMap2dTo3d( map2dTo3d const * const mm);
    void snapToMap3dTo3d( map3dTo3d const * const mm);
    std::vector< map2dTo3d const * > const & getRefToSnapMap2dTo3d( void ) const;
    std::vector< map3dTo3d const * > const & getRefToSnapMap3dTo3d( void ) const;
    bool snappedTo(map2dTo3d const * const mm) const;
    dtPoint3 cast2DtPoint3( void ) const;
    virtual dtGmshVertex * clone( GModel * destModel ) const;
    static bool isEqual( GVertex const * const gv0, GVertex const * const gv1 );
  private:
    dtPoint3 * _CGALPoint;
    GPoint * _GPointPoint;
    std::vector< map2dTo3d const * > _m2d;
    std::vector< map3dTo3d const * > _m3d;
  };
}
#endif	/* DTGMSHVERTEX_H */

