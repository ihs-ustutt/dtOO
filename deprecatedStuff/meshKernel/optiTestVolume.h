#ifndef OPTITESTVOLUME_H
#define	OPTITESTVOLUME_H

#include "boundedVolume.h"
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>
#include <interfaceHeaven/optimizationHandling.h>
#include <interfaceHeaven/optimizationCyclingDecorator.h>

namespace dtOO {
  class constValue;
  class scaFunction;
  class analyticGeometry;
  
  class optiTestVolume : public boundedVolume,
                         public optimizationHandling,
                         public optimizationCyclingDecorator {
  public:
    DTCLASSNAMEMETHOD(optiTestVolume);
    DTCLASSLOGMETHOD(optiTestVolume);    
    optiTestVolume();
    optiTestVolume(const optiTestVolume& orig);
    virtual ~optiTestVolume();
    virtual void init(
                   ::QDomElement const & element,
                   vectorHandling< constValue * > const * const cValP,
                   vectorHandling< scaFunction * > const * const sFunP,
                   vectorHandling< analyticGeometry * > const * const depAGeoP 
                 );    
    virtual float characterizeMe(void);
    virtual float characterizeFailedDesign(void);
    virtual void doCycle( void );    
    virtual void writeGrid( void );
    covise::coDoSet * toCoDoSet(char const * const str) const;
    virtual covise::coDoSet * toCoDoUnstructuredGrid(char const * const str) const;
  private:
    std::vector< float > _val;
    vectorHandling< constValue * > _valCValP;
    static const int _nVal = 2;    
    enum dtValName {
      dtX = 0,
      dtY
    } _valPos;

  };
}
#endif	/* OPTITESTVOLUME_H */

