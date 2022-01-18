#ifndef UNSTRUCTURED3DSURFACEMESH_H
#define	UNSTRUCTURED3DSURFACEMESH_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/renderInterface.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class unstructured3dSurfaceMesh : public renderInterface {
    public:
      dt__class(unstructured3dSurfaceMesh, renderInterface);    
      unstructured3dSurfaceMesh();
      virtual ~unstructured3dSurfaceMesh();
      void addPoints( vectorHandling< dtPoint3 > const & pp );
      void addPoint( dtPoint3 const & pp );
      void addElement( vectorHandling< dtInt > const & el );
      vectorHandling< dtPoint3 > const & refP3( void ) const;    
      vectorHandling< vectorHandling< dtInt > > const & refEl( void ) const;
      dtInt getNQuads( void ) const;
      dtInt getNTris( void ) const;
    private:
      vectorHandling< dtPoint3 > _pp;
      vectorHandling< vectorHandling< dtInt > > _el;
      dtInt _nQuads;
      dtInt _nTris;
  };
}
#endif	/* UNSTRUCTURED3DSURFACEMESH_H */

