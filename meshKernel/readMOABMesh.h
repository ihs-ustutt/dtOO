#ifndef READMOABMESH_H
#define	READMOABMESH_H

#include <logMe/dtMacros.h>
#include "boundedVolume.h"
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/ptrVectorHandling.h>

namespace moab {
  class Interface;
}

class MVertex;

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
    virtual vectorHandling< renderInterface * > getExtRender( void ) const;
    virtual std::vector< std::string > getMeshTags( void ) const;
	  virtual dtGmshFace * getFace( std::string const & tag ) const;
    virtual dtGmshRegion * getRegion( std::string const & tag ) const;
    virtual dtGmshModel * getModel( void ) const;
  private:
    void convertToGmsh( void );
    static std::map< int, ::MVertex * > createVertices( 
      moab::Interface const & mb
    );
    static std::map< std::string, dtGmshFace * > createFaces( 
      moab::Interface const & mb, std::map< int, ::MVertex * > & mv_MOAB 
    );    
  private:
    ptrHandling< moab::Interface > _mb;
    std::string _fileName;
    vectorHandling< dtGmshFace * > _ff;
    vectorHandling< dtGmshRegion * > _rr;
    vectorHandling< dtGmshModel * > _mm;
    std::map< int, ::MVertex * > _mv_MOAB;
    std::map< std::string, dtGmshFace * > _ff_string;
  };
}

#endif	/* READCGNSMESH_H */

