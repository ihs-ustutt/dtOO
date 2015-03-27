#ifndef UNSTRUCTURED3DMESH_H
#define	UNSTRUCTURED3DMESH_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/renderInterface.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class unstructured3dMesh : public renderInterface {
  public:
    dt__CLASSSTD(unstructured3dMesh, renderInterface);    
    unstructured3dMesh();
    virtual ~unstructured3dMesh();
    void addPoints( vectorHandling< dtPoint3 > const & pp );
    void addPoint( dtPoint3 const & pp );
    void addElement( vectorHandling< int > const & el );
    vectorHandling< dtPoint3 > const & refP3( void ) const;    
    vectorHandling< vectorHandling< int > > const & refEl( void ) const;
    int getNHex( void ) const;
    int getNTet( void ) const;
    int getNPyr( void ) const;
    int getNPri( void ) const;
  private:
    vectorHandling< dtPoint3 > _pp;
    vectorHandling< vectorHandling< int > > _el;
    int _nTet;
    int _nHex;
    int _nPyr;
    int _nPri;
  };
}
#endif	/* UNSTRUCTURED3DMESH_H */

