#ifndef READMOABMESH_H
#define	READMOABMESH_H

#include <logMe/dtMacros.h>
#include "boundedVolume.h"
#include <interfaceHeaven/ptrHandling.h>

namespace moab {
  class Interface;
}

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class readMOABMesh : public boundedVolume {
  public:
    dt__class(readMOABMesh, boundedVolume);
    readMOABMesh();
    virtual ~readMOABMesh();
    virtual void init( 
      ::QDomElement const & element,
      baseContainer const * const bC,      
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP,
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      vectorHandling< boundedVolume * > const * const depBVolP
    );    
    virtual void makeGrid(void);
    virtual void makePreGrid(void);
  	virtual vectorHandling< renderInterface * > getRender( void ) const;
  private:
    ptrHandling< moab::Interface > _mb;
    std::string _fileName;
  };
}

#endif	/* READCGNSMESH_H */

