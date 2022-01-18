#ifndef UNSTRUCTURED3DMESH_H
#define	UNSTRUCTURED3DMESH_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/renderInterface.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class unstructured3dMesh : public renderInterface {
    public:
      dt__class(unstructured3dMesh, renderInterface);    
      unstructured3dMesh();
      virtual ~unstructured3dMesh();
      void addPoints( vectorHandling< dtPoint3 > const & pp );
      void addPoint( dtPoint3 const & pp );
      void addElement( vectorHandling< dtInt > const & el );
      vectorHandling< dtPoint3 > const & refP3( void ) const;    
      vectorHandling< vectorHandling< dtInt > > const & refEl( void ) const;
      dtInt getNHex( void ) const;
      dtInt getNTet( void ) const;
      dtInt getNPyr( void ) const;
      dtInt getNPri( void ) const;
    private:
      vectorHandling< dtPoint3 > _pp;
      vectorHandling< vectorHandling< dtInt > > _el;
      dtInt _nTet;
      dtInt _nHex;
      dtInt _nPyr;
      dtInt _nPri;
  };
}
#endif	/* UNSTRUCTURED3DMESH_H */

