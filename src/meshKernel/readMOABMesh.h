#ifndef READMOABMESH_H
#define	READMOABMESH_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "gmshBoundedVolume.h"
#include <interfaceHeaven/ptrHandling.h>

#include <moab/EntityHandle.hpp>

namespace moab {
  class Interface;
}

class MVertex;

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class readMOABMesh : public gmshBoundedVolume {
  public:
    dt__class(readMOABMesh, boundedVolume);
    dt__classSelfCreate(readMOABMesh);
    readMOABMesh();
    virtual ~readMOABMesh();
    virtual void init( 
      ::QDomElement const & element,
      baseContainer * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF,
      aGPtrVec const * const aG,
      bVPtrVec const * const bV
    );    
    virtual void makeGrid(void); 
  private:
    void convertToGmsh( void );
    static std::map< ::moab::EntityHandle, ::MVertex * > createVertices( 
      dtGmshModel * gm, ::moab::Interface const & mb
    );
    static std::map< std::string, dtGmshFace * > createFaces( 
      dtGmshModel * gm, 
      ::moab::Interface const & mb, 
      std::map< ::moab::EntityHandle, ::MVertex * > & mv_MOAB 
    );    
    static std::map< std::string, dtGmshRegion * > createRegions(
      dtGmshModel * gm, 
      ::moab::Interface const & mb, 
      std::map< ::moab::EntityHandle, ::MVertex * > & mv_MOAB 
    );  
  private:
    ptrHandling< ::moab::Interface > _mb;
    std::string _fileName;
    std::map< ::moab::EntityHandle, ::MVertex * > _mv_MOAB;
    std::map< std::string, dtGmshFace * > _ff_string;
    std::map< std::string, dtGmshRegion * > _rr_string;
  };
}

#endif	/* READCGNSMESH_H */

