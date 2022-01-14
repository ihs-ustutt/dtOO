#ifndef DTGMSHVERTEX_H
#define	DTGMSHVERTEX_H

#include <dtOOTypeDef.h>

#include <gmsh/GVertex.h>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

class GPoint;
  
namespace dtOO { 
  class dtGmshEdge;
  
  class dtGmshVertex : public ::GVertex {
  public:
    dt__class(dtGmshVertex, ::GEntity); 
    dtGmshVertex(::GModel *m, int tag, double ms);
    dtGmshVertex(::GModel *m, int tag);
    virtual ~dtGmshVertex();
    virtual GPoint point() const;
    virtual double x() const;
    virtual double y() const;
    virtual double z() const;    
    virtual void setPosition(GPoint &p);
    virtual void setPosition(dtPoint3 const p);    
    virtual void setPosition(dtPoint3 const * const p);
    dtPoint3 cast2DtPoint3( void ) const;
    void addGEntity( ::GEntity * const gEnt );
    static bool isEqual( 
      ::GVertex const * const gv0, 
      ::GVertex const * const gv1 
    );
    virtual std::list< dtGmshEdge * > dtEdges( void ) const;
    std::string getPhysicalString( void ) const;    
  public:
    mutable ::GEntity::MeshGenerationStatus _status;
  private:
    dtPoint3 _dtP;
  };
}
#endif	/* DTGMSHVERTEX_H */

