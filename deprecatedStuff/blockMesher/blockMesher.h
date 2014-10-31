#ifndef blockMesher_H
#define blockMesher_H

#include <string.h>
#include <api/coModule.h>
#include <iostream>
#include <fstream>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/coDoSetHandling.h>
#include <abstractModule.h>

using namespace covise;

//class analyticGeometry;
//class splineCurve3d;
namespace dtOO {
    
    class blockMesher : public covise::coModule,
            public abstractModule {
   
	private:
		virtual int compute(const char *);

		//ports:
		covise::coInputPort * _p_inPort;
                covise::coInputPort * _pIn_sFunP;
		covise::coOutputPort * _p_outPortGrid;
                covise::coOutputPort * _p_outPortStructGrid;
                covise::coOutputPort * _p_outPortSurf;
            
                covise::coFileBrowserParam * _p_logFilebrowser;
                covise::coIntScalarParam * _p_nxIntParam;
                covise::coIntScalarParam * _p_nyIntParam;
                covise::coIntScalarParam * _p_nzIntParam;
                
                coDoSetHandling< scaFunction * > _sFun;
   

	public:
		//constructor:
		blockMesher(int argc, char *argv[]);
                
                DTCLASSNAMEMETHOD(blockMesher);
};
}
#endif
