#ifndef CVOLUME_H
#define	CVOLUME_H

#include "boundedVolume.h"
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>
#include "blockMeshDictWriter.h"

namespace covise {
  class coDoSet;
}

namespace dtOO {
  class analyticSurface;
  class cVolume : public boundedVolume,
                  public blockMeshDictWriter {
  public:
    DTCLASSNAMEMETHOD(cVolume);
    DTCLASSLOGMETHOD(cVolume);
    cVolume();
    cVolume(const cVolume& orig);
    virtual ~cVolume();
    virtual void init(
                   QDomElement const & element,
                   vectorHandling< constValue * > const * const cValP,
                   vectorHandling< scaFunction * > const * const sFunP,
                   vectorHandling< analyticGeometry * > const * const depAGeoP 
                 );
    void dump(void) const;
    virtual void writeGrid( void );
    virtual void writeCheapGrid( void );
    virtual covise::coDoSet * toCoDoSet(char const * const str) const;
    virtual covise::coDoSet * toCoDoUnstructuredGrid(char const * const str) const;    
  protected:
    bool parameterCheck(void);
  private:
    void autoAdjust(void);
    void pushGridToBlockMeshDictWriter(void);
  protected:
    std::string _caseDirectory;    
  private:
    vectorHandling< analyticSurface * > _surfP;
    static const int _nGridVal = 24;
    std::vector< float > _val;
    vectorHandling< constValue * > _valCValP;
    static const int _nGridInt = 7;
    std::vector< int > _int;
    vectorHandling< constValue * > _intCValP;
    enum dtSurfName {
      dtNorth = 0, dtWest, dtSouth, dtEast, dtFront, dtBack, dtInternal 
    } _surfacePos;
    enum dtValName {
      dtNode2 = 0, dtNode6, dtNode1, dtNode5, dtNode9, dtNode11, dtNode8,
      dtNode10, dtNode13, dtNode15, dtNode16, dtNode17, dtNode18u, dtNode18v,
      dtNode19u, dtNode19v, dtNode22, dtNode23,
      dtA, dtB, dtC, dtD, dtE, dtF 
    } _valPos;      
    enum dtIntName {
      dtAlpha = 0, dtBeta, dtGamma, dtDelta, dtEpsilon, dtPhi, dtPsi
    } _intPos;      
  };
}
#endif	/* CVOLUME_H */

