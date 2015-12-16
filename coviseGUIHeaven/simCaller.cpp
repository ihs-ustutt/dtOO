#include "simCaller.h"
#include <do/coDoSet.h>
#include <do/coDoUnstructuredGrid.h>
#include <do/coDoIntArr.h>
#include <api/coSimLib.h>
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>

const int SUCCESS = 1;
const int FAILURE = 0;

namespace dtOO {
    simCaller::simCaller(int argc, char *argv[])
	:coSimLib(argc, argv, argv[0], "simCaller"){
        
        cout << "##############################" << endl;
        cout << "#####   SimCaller "            << endl;
        cout << "#####   PID =  " << getpid()   << endl;
        cout << "##############################" << endl;
        
        createUserMenu();
        
        _p_unsGridInputP = addInputPort("unsGrid", "UnstructuredGrid", "DomainGrid");
        _p_bocoInputP = addInputPort("boco", "UnstructuredGrid", "BoundaryConditions");
        _p_bocoInputP->setRequired(0);
        
        _p_pDataOutputP = addOutputPort("pData", "Float", "pressureDataOutput");
        _p_uDataOutputP = addOutputPort("uData", "Vec3", "velocityDataOutput");
        _p_testGridP = addOutputPort("testGrid", "UnstructuredGrid", "testingGrid");
//        _p_floatOutputPort = addOutputPort("floatParam", "Float", "Data Output");
        counter = 0;
    }
    
    int simCaller::compute(const char *) {
        
     try {
      //
      // init log file
      //
      std::string logFileName = std::string( coModule::getTitle() );
      abstractModule::initializeLogFile( "./"+logFileName+".log" );
         
  
  
      
//        setVerbose(3);
        sendInfo("I'm the SimCaller!");
//        sendInfo("integer-param is %i", _param_intParam->getValue());
//        setUserArg(0,_param_caseDir->getValue());
        
       if (counter == 0) {
            if (startSim()!=0) {
              dt__throw(compute(), << "Unable to start Simulation!");
            }
            sendMesh();
            sendBoundaryConditions();
        
        }
        
        if (counter == 1) {          
        int sDat = 4;
        int secondDat = 2;
        sendData(&sDat, sizeof(int));
        sendBS_Data(&secondDat, sizeof(int));
        int recDat = 0;
        recvData(&recDat, sizeof(int));
        sendInfo("recieved param is %i", recDat);
        sendData(&counter, sizeof(int));       
        
            dt__info(compute(),
                    <<dt__eval(counter) << std::endl
                    <<dt__eval(coSimLib::isConnected()) );  
                          
        }
        
        if (counter == 2) {
           dt__info(compute(),
                   <<dt__eval(counter) << std::endl
                   <<dt__eval(coSimLib::isConnected()) );   
          executeCommands(); //benötigt um coSend1Data aus Simulation auf Modul-Port zu legen. 
        }
        counter++;
        
     }
     catch (eGeneral & eGenRef) {
      dt__catch(compute(), eGenRef.what());
      abstractModule::closeLogFile();
      send_stop_pipeline();
      
      return FAILURE;
    }
        
        return SUCCESS;
    }
    
    int simCaller::endIteration(void) {
        return 1;
    }
    
    
    void simCaller::createUserMenu() {
        _param_intParam = addInt32Param("intParam", "communication value");
        _param_caseDir = addStringParam("CaseDirectory", "Absolut Path to OF-case-directory");
        
    }
    
    void simCaller::sendMesh() {
        
      const covise::coDoUnstructuredGrid * inGrid;
      
      
      if (_p_unsGridInputP->isConnected()) {
          
//        const covise::coDoUnstructuredGrid * inGrid = _p_unsGridInputP->getCurrentObject();
          dt__ptrAss(const covise::coDoSet * inSet, dynamic_cast<const covise::coDoSet *>(_p_unsGridInputP->getCurrentObject()) );
          for (int ii=0; ii<inSet->getNumElements(); ii++) {
              inGrid = dynamic_cast<const covise::coDoUnstructuredGrid *>(inSet->getElement(ii) );
              if (inGrid) {
                  break;
              }
          }
          if (!inGrid) {
              dt__throw(compute(), << "No Grid in Input-Set.");
          }
      }
      
      int USGNums[6];
      USGNums[3] = 5; //Number of Blades
      USGNums[4] = 2; //Number of parallel regions
      USGNums[5] = 0; // = Gate in oldOF-Module
      
      inGrid->getGridSize(USGNums+1, USGNums+2, USGNums);
      
      float *xCoord, *yCoord, *zCoord;
      int *elem, *conn, *typeList;
      inGrid->getAddresses(&elem, &conn, &xCoord, &yCoord, &zCoord);
      inGrid->getTypeList(&typeList);
      
      sendBS_Data(USGNums, 6*sizeof(int));

      sendBS_Data(xCoord, USGNums[0]*sizeof(float));
      sendBS_Data(yCoord, USGNums[0]*sizeof(float));
      sendBS_Data(zCoord, USGNums[0]*sizeof(float));

      sendBS_Data(elem, USGNums[1]*sizeof(int));
      sendBS_Data(typeList, USGNums[1]*sizeof(int));
		
      sendBS_Data(conn, USGNums[2]*sizeof(int));
      
      
       //-----DEBUGGING---------
      int numElem, numConn, numCoord;
      int hasType;
      int *inElemList, *inConnList, *inTypeList;
      float *inxc, *inyc, *inzc;
      inGrid->getGridSize(&numElem, &numConn, &numCoord);
      hasType = inGrid->hasTypeList();
      inGrid->getAddresses(&inElemList, &inConnList, &inxc, &inyc, &inzc);
      if(hasType) {
          inGrid->getTypeList(&inTypeList);
      }
      covise::coDoUnstructuredGrid * tmpTestGrid = new covise::coDoUnstructuredGrid(_p_testGridP->getObjName(), numElem, numConn, numCoord, hasType);//, hasType);
      int *outElemList, *outConnList, *outTypeList;
      float *outxc, *outyc, *outzc;
      tmpTestGrid->getAddresses(&outElemList, &outConnList, &outxc, &outyc, &outzc);
      if(hasType) {
          tmpTestGrid->getTypeList(&outTypeList);
      }
      for (int ii=0; ii<numElem; ii++) {
          outElemList[ii] = inElemList[ii];
      }
      if (hasType) {
          for (int ii=0; ii<numElem; ii++) {
              outTypeList[ii] = inTypeList[ii];
          }
      }
      for (int ii=0; ii<numConn; ii++) {
          outConnList[ii] = inConnList[ii];
      }
      for (int ii=0; ii<numCoord; ii++) {
          outxc[ii] = inxc[ii];
          outyc[ii] = inyc[ii];
          outzc[ii] = inzc[ii];
      }
      _p_testGridP->setCurrentObject(tmpTestGrid);
      
      //------END DEBUGGING------
        
    }
    
    void simCaller::sendBoundaryConditions() {
        
      const covise::coDoIntArr * tmpBocoArr;
      std::vector<const covise::coDoIntArr *> bocoVec;
      
      if (_p_bocoInputP->isConnected()) {
          dt__ptrAss(const covise::coDoSet * inSet, dynamic_cast<const covise::coDoSet *>(_p_bocoInputP->getCurrentObject()) );
          for (int ii=0; ii<inSet->getNumElements(); ii++) {
              tmpBocoArr = dynamic_cast<const covise::coDoIntArr *>(inSet->getElement(ii) );
              if (tmpBocoArr) {
                  bocoVec.push_back(tmpBocoArr);
              }
          }
          dt__info(sendBoundaryConditions(),
                    <<dt__eval(bocoVec.size()) );
      }
      
      const covise::coDoIntArr * wallArray = bocoVec[4];
      int sizeColumnWall = wallArray->getDimension(0);
      int numColumnsWall = wallArray->getDimension(1);
      int *wall;
      wallArray->getAddress(&wall);
      
      int dimension[2];
      dimension[0] = bocoVec[0]->getDimension(0);
      dimension[1] = bocoVec[0]->getDimension(1) + bocoVec[1]->getDimension(1) + bocoVec[2]->getDimension(1) + bocoVec[3]->getDimension(1);
      const covise::coDoIntArr * balanceArray = new covise::coDoIntArr("balance", 2, dimension );
      int *balance;
      balanceArray->getAddress(&balance);
      int *inlet;
      bocoVec[0]->getAddress(&inlet);
      int runPara = 0;
      for (int ii=0; ii<( bocoVec[0]->getDimension(0)*bocoVec[0]->getDimension(1) ); ii++) {
          balance[runPara] = inlet[ii];
          runPara++;
      }
      for (int ii=0; ii<( bocoVec[1]->getDimension(0)*bocoVec[1]->getDimension(1) ); ii++) {
          balance[runPara] = inlet[ii];
          runPara++;
      }
      for (int ii=0; ii<( bocoVec[2]->getDimension(0)*bocoVec[2]->getDimension(1) ); ii++) {
          balance[runPara] = inlet[ii];
          runPara++;
      }
      for (int ii=0; ii<( bocoVec[3]->getDimension(0)*bocoVec[3]->getDimension(1) ); ii++) {
          balance[runPara] = inlet[ii];
          runPara++;
      }
      int sizeColumnBalance = balanceArray->getDimension(0);
      int numColumnsBalance = balanceArray->getDimension(1);
      
      int bocoNums[4] = {sizeColumnWall, numColumnsWall, sizeColumnBalance, numColumnsBalance};
      sendBS_Data(bocoNums, 4*sizeof(int));
      
      sendBS_Data(wall, bocoNums[0]*bocoNums[1]*sizeof(int));
      
      sendBS_Data(balance, bocoNums[2]*bocoNums[3]*sizeof(int));
      
        
    }
    
}
MODULE_MAIN(designTool, dtOO::simCaller)

