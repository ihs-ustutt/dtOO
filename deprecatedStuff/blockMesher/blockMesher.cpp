#include "blockMesher.h"
#include "analyticGeometryHeaven/splineCurve3d.h"
#include "geometryEngine/dtCurve.h"
#include "analyticGeometryHeaven/analyticSurface.h"
#include <do/coDoUnstructuredGrid.h>
#include <do/coDoStructuredGrid.h>

//---------------------------------
#include <do/coDoSet.h>
//#include <do/coDoData.h>
//------------------------------------------------------------------------------
#include <logMe/logMe.h>
//------------------------------------------------------------------------------
//#include <geometryHeaven/geometry.h>
//------------------------------------------------------------------------------
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryBuilder/analyticGeometryBuilder.h>
#include <analyticGeometryBuilder/analyticGeometryBuilderFactory.h>
#include <analyticGeometryBuilder/analyticGeometryDirector.h>
//------------------------------------------------------------------------------
//#include <analyticFunctionHeaven/scaFunction.h>
//#include <analyticFunctionHeaven/constValue.h>
//------------------------------------------------------------------------------
#include <baseContainer/pointContainer.h>
//#include <baseContainer/vectorContainer.h>
//------------------------------------------------------------------------------
//#include <dtXmlParserDecorator/dtXmlParser.h>
//#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
//#include <dtXmlParserDecorator/dtXmlParserDecoratorFactory.h>
//--------------------------------
#include <vector>
#include <time.h>

#include "dtBmDevelopStuff.h"
#include "dtBmGrid.h"


/*#include <logMe/logMe.h>
#include "../geometryKernel/analyticGeometryHeaven/analyticGeometry.h"
#include "../geometryKernel/analyticGeometryHeaven/splineCurve3d.h"
//#include "../geometryKernel/analyticGeometryBuilder/analyticGeometryBuilderFactory.h"
//#include "../geometryKernel/dtLinearAlgebra.h"
*/
#include <geometryEngine/dtSurface.h>


#define FALSE 0
#define TRUE 1

const int SUCCESS = 1;
const int FAILURE = 0;

namespace dtOO {
    
    DTCLASSLOGMETHODI(blockMesher,
          << "No member attributes.");

blockMesher::blockMesher(int argc, char *argv[])
	:coModule(argc, argv, "blockMesher"){

	//ports:
	_p_inPort = addInputPort("inPort", "Points", "Block Input");
	_p_inPort->setRequired(0);
        _pIn_sFunP = addInputPort("sFunP", "USR_ScaFun", "sFunPDescription");
        _pIn_sFunP->setRequired(0);
	_p_outPortGrid = addOutputPort("outPortUns", "UnstructuredGrid", "Grid Output");
        _p_outPortStructGrid = addOutputPort("outPortStruct", "StructuredGrid", "StructGrid Output");
        _p_outPortSurf = addOutputPort("outPortSurf", "coDoSet", "Surf Output");
        
  
        
        //parameters:
//        _p_logFilebrowser = addFileBrowserParam("logFilebrowser", "logFilebrowserDescrition");
//        _p_logFilebrowser->setValue(".", "*.log/*");
        _p_nxIntParam = addInt32Param("nxIntParam" , "quantity knots xDirection");
        _p_nxIntParam->setValue(2);
        _p_nxIntParam->enable();
        _p_nyIntParam = addInt32Param("nyIntParam" , "quantity knots yDirection");
        _p_nyIntParam->setValue(2);
        _p_nyIntParam->enable();
        _p_nzIntParam = addInt32Param("nzIntParam" , "quantity knots zDirection");
        _p_nzIntParam->setValue(2);
        _p_nzIntParam->enable();
        
	}


int blockMesher::compute(const char * port) {
  //
  // init logMe
  //
//  FILE* pFile = fopen(_p_logFilebrowser->getValue(), "w");
//  Output2FILE::Stream() = pFile;
//
//	dt__info(compute(),
//          << "I'm the blockMesher");
  
	//read inPort:
/*	const coDistributedObject *inObj = p_inPort->getCurrentObject();


	//typeCheck InputPort
	if (!inObj)
	{
		sendInfo("No object at port '%s'",
				p_inPort->getName());
		return FAILURE;
	}
	else {
		sendInfo("object exists at port '%s' ",
				p_inPort->getName());
		return SUCCESS;
	}

	const coDoPoints *inPoints = dynamic_cast<const coDoPoints *>(inObj);
	if (!inPoints)
	{
		sendError("Received illegal type at port '%s' ",
				p_inPort->getName());
		return FAILURE;
	}
*/
       try { 
       //
      // init log file
      //
      std::string logFileName = std::string( coModule::getTitle() );
      abstractModule::initializeLogFile( "./"+logFileName+".log" );
      
      const covise::coDistributedObject * inObj;
      covise::coDoSet const * set;
      //
      // recreate scaFunctions
      //
      if ( _pIn_sFunP->isConnected() ) {
        inObj = _pIn_sFunP->getCurrentObject();
        set = dynamic_cast< covise::coDoSet const * >(inObj);
        _sFun.destroy();
        abstractModule::recreateScaFunctions(set, _sFun);
      }
        
         


  time_t tStart;
  time_t tEnd;
  
  tStart = time(NULL);
 
  
  if ( _p_nxIntParam->getValue() < 2 ) {
      _p_nxIntParam->setValue(2);
      dt__warning(compute(), "nx smaller 2, set to 2");
  }
  int nx = _p_nxIntParam->getValue();
  
  if ( _p_nyIntParam->getValue() < 2 ) {
      _p_nyIntParam->setValue(2);
      dt__warning(compute(), "ny smaller 2, set to 2");
  }
  int ny = _p_nyIntParam->getValue();
  
  if ( _p_nzIntParam->getValue() < 2 ) {
      _p_nzIntParam->setValue(2);
      dt__warning(compute(), "nz smaller 2, set to 2");
  }
  int nz = _p_nzIntParam->getValue();
  

  dtBmDevelopStuff dummyBlocks;  
  std::vector<dtSurface *> blockSurf;
  std::vector<dtSurface *> blockSurfI;
  std::vector<dtSurface *> blockSurfII;
  std::vector<dtSurface *> blockSurfIII;
  std::vector<dtSurface *> blockSurfIV;
  blockSurf = dummyBlocks.createBlocks();
  for (int ii=0; ii<6; ii++) 
  {
      blockSurfI.push_back(blockSurf.at(ii));
  }
  for (int ii=6; ii<12; ii++) 
  {
      blockSurfII.push_back(blockSurf.at(ii));
  }
  for (int ii=12; ii<18; ii++)
  {
      blockSurfIII.push_back(blockSurf.at(ii));
  }
  for (int ii=18; ii<blockSurf.size(); ii++)
  {
      blockSurfIV.push_back(blockSurf.at(ii));
  }
  dtBmGrid * dummyGridI = new dtBmGrid("firstGrid", blockSurfI, nx, ny, nz, &_sFun );
  dtBmGrid * dummyGridII = new dtBmGrid("secondGrid",blockSurfII, nx, ny, nz, &_sFun );
  dtBmGrid * dummyGridIII = new dtBmGrid("thirdGrid",blockSurfIII, nx, ny, nz, &_sFun );
  dtBmGrid * dummyGridIV = new dtBmGrid("fourthGrid",blockSurfIV, (nx+2), (ny-1), nz, &_sFun );
  
//  dtBmGrid * dummyCoupleGrid;
//  dtBmGrid * dummyStructCoupleGrid;
//  dtBmGrid * dummyMidCoupleGrid;
  
      
  int cm = 1;
  int coupleIndex = 3;
  //gridPoints = dummyGrid->createPoints( blockSurf, xRes, yRes, zRes );
  
//  coDoUnstructuredGrid * outGrid = dummyGrid->toCoDoUns(_p_outPortGrid->getObjName());
  
//  coDoStructuredGrid * outStructGrid = dummyGridII->toCoDoStruct(_p_outPortStructGrid->getObjName());
  coDoStructuredGrid * firstStructGrid = dummyGridI->toCoDoStruct("firstGrid");
  coDoStructuredGrid * secondStructGrid = dummyGridII->toCoDoStruct("secondGrid");
  coDoStructuredGrid * thirdStructGrid = dummyGridIII->toCoDoStruct("thirdGrid");
  coDoStructuredGrid * fourthStructGrid = dummyGridIV->toCoDoStruct("fourthGrid");
  coDoStructuredGrid * midStructGrid = dtBmGrid().coupleStructBlocks("midGrid", firstStructGrid, secondStructGrid, cm);
  coDoStructuredGrid * outStructGrid = dtBmGrid().coupleStructBlocks(_p_outPortStructGrid->getObjName(),
                                                                                 firstStructGrid, secondStructGrid, cm);
  coDoUnstructuredGrid * outGrid = dtBmGrid().unsCoupleStructBlocks(_p_outPortGrid->getObjName(),
                                                                          midStructGrid, fourthStructGrid,
                                                                          coupleIndex);

  
      
        tEnd = time(NULL);
        dt__info(time(), 
                   << "Duration Gridgeneration: " << dt__eval(tEnd-tStart) << " s" << std::endl
                );
        
        
        dtBmDevelopStuff dummySurf;
        std::vector<dtCurve *> mySurf;
        mySurf = dummySurf.createSurf();
        dtBmGrid * dummySurfGrid = new dtBmGrid(mySurf, nx, ny);
        coDoSet * outSurfGrid = dummySurfGrid->toCoDoUns2D(_p_outPortSurf->getObjName());
        delete dummySurfGrid;
  
 
        
// output:
	_p_outPortGrid->setCurrentObject( outGrid );
        _p_outPortStructGrid->setCurrentObject( outStructGrid );
        _p_outPortSurf->setCurrentObject( outSurfGrid );
        

        delete midStructGrid;
        delete firstStructGrid;
        delete secondStructGrid;
        delete thirdStructGrid;
        delete fourthStructGrid;
        
        delete dummyGridI;
        delete dummyGridII;
        delete dummyGridIII;
        delete dummyGridIV;
}
    catch (eGeneral & eGenRef) {
      dt__catch(compute(), eGenRef.what());
      send_stop_pipeline();
      return FAILURE;
    }
        
        return SUCCESS;
}
}
MODULE_MAIN(designTool, dtOO::blockMesher)

        


