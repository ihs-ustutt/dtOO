#ifndef COVOLUME_H
#define	COVOLUME_H

#include "boundedVolume.h"
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>
#include "blockMeshDictWriter.h"

namespace covise {
  class coDoSet;
}

namespace dtOO {
  class analyticSurface;
  class scaFunction;
  
  class coVolume : public boundedVolume,
                  public blockMeshDictWriter {
  public:
    DTCLASSNAMEMETHOD(coVolume);
    DTCLASSLOGMETHOD(coVolume);
    coVolume();
    coVolume(const coVolume& orig);
    virtual ~coVolume();
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
    static const int _nGridVal = 97;
    std::vector< float > _val;
    vectorHandling< constValue * > _valCValP;
    static const int _nGridInt = 7;
    std::vector< int > _int;
    vectorHandling< constValue * > _intCValP;
    enum dtSurfName {
      dtNorth = 0, dtWest, dtSouth, dtEast, dtFront, dtBack, dtInternal 
    } _surfacePos;
    enum dtValName {
      dtNode0u = 0, 
      dtNode0v, 
      dtNode1u, 
      dtNode1v,
      dtNode2u, 
      dtNode2v,
      dtNode3u, 
      dtNode3v,
      dtNode4u, 
      dtNode4v,
      dtNode5u, 
      dtNode5v,
      dtNode6u, 
      dtNode6v,
      dtNode7u, 
      dtNode7v,
      dtNode8u, 
      dtNode8v,
      dtNode9u, 
      dtNode9v,
      dtNode10u, 
      dtNode10v,
      dtNode11u, 
      dtNode11v,
      dtNode12u, 
      dtNode12v,
      dtNode13u, 
      dtNode13v,
      dtNode14u, 
      dtNode14v,
      dtNode15u, 
      dtNode15v,
      dtNode16u, 
      dtNode16v,
      dtNode17u, 
      dtNode17v,
      dtNode18u, 
      dtNode18v,
      dtNode19u, 
      dtNode19v,
      dtNode20u, 
      dtNode20v,
      dtNode21u, 
      dtNode21v,
      dtNode22u, 
      dtNode22v,
      dtNode23u, 
      dtNode23v,
      dtNode24u, 
      dtNode24v,
      dtNode25u, 
      dtNode25v,
      dtNode26u, 
      dtNode26v,
      dtNode27u, 
      dtNode27v,
      dtNode28u, 
      dtNode28v,
      dtNode29u, 
      dtNode29v,
      dtNode30u, 
      dtNode30v,
      dtNode31u, 
      dtNode31v,
      dtNode32u, 
      dtNode32v,
      dtNode33u, 
      dtNode33v,
      dtNode34u, 
      dtNode34v,
      dtNode35u, 
      dtNode35v,
      dtNode36u, 
      dtNode36v,
      dtNode37u, 
      dtNode37v,
      dtNode38u, 
      dtNode38v,
      dtNode39u, 
      dtNode39v,
      dtNode40u, 
      dtNode40v,
      dtNode41u,       
      dtNode41v,
      dtNode42u, 
      dtNode42v,
      dtNode43u, 
      dtNode43v,
      dtNode44u, 
      dtNode44v,      
      dtNode45u, 
      dtNode45v,
      dtA, 
      dtB, 
      dtC,
      dtD,
      dtE
    } _valPos;    
    enum dtIntName {
      dtAlpha = 0, dtBeta, dtGamma, dtDelta, dtEpsilon, dtPhi, dtPsi
    } _intPos;      
    vectorHandling< int > _markedEdgeFrom;
    vectorHandling< int > _markedEdgeTo;
    vectorHandling< scaFunction * > _markedEdgeScaFunction;    
  };
}
#endif	/* COVOLUME_H */

