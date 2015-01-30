#ifndef UNSTRUCTURED3DSURFACEMESH_H
#define	UNSTRUCTURED3DSURFACEMESH_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/renderInterface.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class unstructured3dSurfaceMesh : public renderInterface {
  public:
    dt__CLASSSTD(unstructured3dSurfaceMesh, renderInterface);    
    unstructured3dSurfaceMesh();
    virtual ~unstructured3dSurfaceMesh();
    void addPoints( vectorHandling< dtPoint3 > const & pp );
    void addPoint( dtPoint3 const & pp );
    void addElement( vectorHandling< int > const & el );
    vectorHandling< dtPoint3 > const & refP3( void ) const;    
    vectorHandling< vectorHandling< int > > const & refEl( void ) const;
    int getNQuads( void ) const;
  private:
    vectorHandling< dtPoint3 > _pp;
    vectorHandling< vectorHandling< int > > _el;
    int _nQuads;
  };
}
#endif	/* UNSTRUCTURED3DSURFACEMESH_H */

