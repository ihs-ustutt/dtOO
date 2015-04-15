#ifndef CODOSETHANDLING_H
#define	CODOSETHANDLING_H

#include <logMe/dtMacros.h>
#include <progHelper.h>
#include <interfaceHeaven/vectorHandling.h>

namespace covise {
  class coDoSet;  
  class coDistributedObject;
}

namespace dtOO {
  class renderInterface;
  class discrete2dPoints;
  class discrete3dPoints;
  class solid2dLine;
  class solid3dLine;
  class solid3dSurface;
  class unstructured3dMesh;
  class unstructured3dSurfaceMesh;
  class discrete3dVector;
  
  class coDoSetHandling : public vectorHandling< renderInterface * > {
    public:
      dt__classOnlyName(coDoSetHandling);
      coDoSetHandling();
      virtual ~coDoSetHandling();
      covise::coDoSet * render2d( coDoSetHandling const & vec, char const * str) const;
      covise::coDoSet * render3d( coDoSetHandling::const_iterator const & it, char const * str ) const;
      covise::coDoSet * render3d(
        coDoSetHandling::const_iterator const & first, 
        coDoSetHandling::const_iterator const & last, 
        char const * str
      ) const;
      covise::coDoSet * render3d( coDoSetHandling const & vec, char const * str) const;
      static covise::coDoSet * createCoDoSet( covise::coDistributedObject * const cdo, char const * str );
      static covise::coDoSet * toCoDoSet( covise::coDoSet * setOne, covise::coDoSet * setTwo, char const * str );
      static covise::coDoSet * toCoDoSet( renderInterface const * const rI, char const * str );
    private:
	    static covise::coDoSet * renderElement2d( renderInterface const * const rI, char const * const str);
      static covise::coDoSet * renderElement3d( renderInterface const * const rI, char const * const str);
      static covise::coDoSet * discrete2dPointsToCoDoSet( discrete2dPoints const * const rI, char const * str );
      static covise::coDoSet * discrete3dPointsToCoDoSet( discrete3dPoints const * const rI, char const * str );
      static covise::coDoSet * solid2dLineToCoDoSet( solid2dLine const * const rI, char const * str );      
      static covise::coDoSet * solid3dLineToCoDoSet( solid3dLine const * const rI, char const * str );
      static covise::coDoSet * solid3dSurfaceToCoDoSet( solid3dSurface const * const rI, char const * str );
      static covise::coDoSet * unstructured3dMeshToCoDoSet( unstructured3dMesh const * const rI, char const * str );
      static covise::coDoSet * unstructured3dSurfaceMeshToCoDoSet( unstructured3dSurfaceMesh const * const rI, char const * str );
      static covise::coDoSet * discrete3dVectorToCoDoSet( discrete3dVector const * const rI, char const * str );
  };
}
#endif	/* CODOSETHANDLING_H */