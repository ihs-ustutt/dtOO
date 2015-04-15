#ifndef SIMCALLER_H
#define	SIMCALLER_H

#include <string.h>
#include <api/coSimLib.h>
#include <iostream>
#include <fstream>
#include <logMe/dtMacros.h>
#include <abstractModule.h>

using namespace covise;

namespace dtOO {

class simCaller : public covise::coSimLib {
    

private:
    virtual int compute(const char *);
    virtual int endIteration();
    
    void createUserMenu();
    void sendMesh();
    void sendBoundaryConditions();
        
    //params:
    covise::coIntScalarParam * _param_intParam;
    covise::coStringParam * _param_caseDir;
    //ports:
    covise::coInputPort * _p_unsGridInputP;
    covise::coInputPort * _p_bocoInputP;
    covise::coOutputPort * _p_pDataOutputP;
    covise::coOutputPort * _p_uDataOutputP;
    covise::coOutputPort * _p_testGridP;
//    covise::coOutputPort * _p_floatOutputPort;
    int counter;

public:
    //constructor:
    simCaller(int argc, char *argv[]);
    //destructor:
    virtual ~simCaller() {};
    
    dt__classOnlyName(simCaller);
    
};

}
#endif	/* SIMCALLER_H */

