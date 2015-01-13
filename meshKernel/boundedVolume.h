#ifndef BOUNDEDVOLUME_H
#define	BOUNDEDVOLUME_H

#include <string>
#include <map>
#include <logMe/dtMacros.h>
#include "bVObserver/bVOSubject.h"
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/optionHandling.h>
#include <interfaceHeaven/vectorHandling.h>
#include <interfaceHeaven/renderInterface.h>
#include <dtLinearAlgebra.h>

namespace dtOO {
  class analyticGeometry;
  class map3dTo3d;
  class map2dTo3d;
  class map1dTo3d;
  class analyticFunction;
  class constValue;
  class baseContainer;
  
  class boundedVolume : public bVOSubject,
                        public labelHandling,
                        public optionHandling,
                        public renderInterface {
  public:
    dt__CLASSNAME(boundedVolume);
    boundedVolume();
    virtual ~boundedVolume();
    virtual void init( 
      QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const sF,
      vectorHandling< analyticGeometry * > const * const aF,
      vectorHandling< boundedVolume * > const * const bV
    );
    virtual void makeGrid(void) = 0;
    virtual void makePreGrid(void);
    void addId(std::string const typeStr, std::string const nameStr);
    int strToId(std::string const typeStr, std::string const nameStr) const;
    bool hasStrToId(std::string const typeStr, std::string const nameStr) const;
    int vStrToId(std::string const nameStr) const;
    int eStrToId(std::string const nameStr) const;
    int fStrToId(std::string const nameStr) const;
    int rStrToId(std::string const nameStr) const;
    map3dTo3d const * getPtrToMap3dTo3d( int const id ) const;
    map2dTo3d const * getPtrToMap2dTo3d( int const id ) const;
    dtPoint3 const * getPtrToVertex( int const id ) const;
    map1dTo3d const * getPtrToMap1dTo3d( int const id ) const;
    vectorHandling< map2dTo3d * > & getRefToMap2dTo3dHandling( void );
    vectorHandling< map2dTo3d * > const & getConstRefToMap2dTo3dHandling( void ) const;
    vectorHandling< map1dTo3d * > & getRefToMap1dTo3dHandling( void );
    vectorHandling< map3dTo3d * > & getRefToMap3dTo3dHandling( void );    
    vectorHandling< dtPoint3 * > & getRefToVertexHandling( void );
    bool isMeshed( void ) const;
    void setMeshed( void );
  private:
    std::map< std::string, int > _vId;
    std::map< std::string, int > _eId;
    std::map< std::string, int > _fId;
    std::map< std::string, int > _rId;
    vectorHandling< map3dTo3d * > _map3dTo3d;
    vectorHandling< map2dTo3d * > _map2dTo3d;
    vectorHandling< map1dTo3d * > _map1dTo3d;
    vectorHandling< dtPoint3 * > _vertexP;   
    bool _meshed;
  };
}
#endif	/* BOUNDEDVOLUME_H */

