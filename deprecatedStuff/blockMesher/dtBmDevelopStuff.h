#ifndef DTBMDEVELOPSTUFF_H
#define	DTBMDEVELOPSTUFF_H

#include <logMe/dtMacros.h>

#include <geometryEngine/dtSurface.h>

namespace dtOO {

class dtBmDevelopStuff {
    DTCLASSNAMEMETHOD(dtBmDevelopStuff);
    DTCLASSLOGMETHOD(dtBmDevelopStuff);
public:
    dtBmDevelopStuff();
    dtBmDevelopStuff(const dtBmDevelopStuff& orig);
    virtual ~dtBmDevelopStuff();
    
    std::vector<dtSurface *> createBlocks();
    std::vector<dtCurve *> createSurf();
    
private:

};

}

#endif	/* DTBMDEVELOPSTUFF_H */

