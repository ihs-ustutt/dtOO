#include <stdio.h>

#include <logMe/logMe.h>
#include <logMe/dtParMacros.h>
#include <xmlHeaven/dtXmlParser.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <baseContainerHeaven/baseContainer.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <interfaceHeaven/vectorHandling.h>
#include <analyticFunctionHeaven/scaFunction.h>
#include <constValueHeaven/constValue.h>
#include <boundedVolume.h>
#include <dtCase.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <dtArg.h>

#include <iostream>

#include <boost/mpi.hpp>

using namespace dtOO;

int main( int ac, char* av[] ) {
  ::boost::mpi::environment env;
  ::boost::mpi::communicator world;    
  
  try {
    //
    // options
    //
    dtArg vm("runCase", ac, av);
    
    //
    // create machine
    //
    vm.setMachine();
    
    //
    // additional arguments
    //
    vm.description().add_options()
      (
        "state", 
        dtPO::value<std::string>()->required(), 
        "define state to run (required)"
      )
      (
        "case", 
        dtPO::value<std::string>()->required(), 
        "define case to run (required)"
      )
    ;
    vm.update();

    //
    //
    //
    baseContainer bC;
    vectorHandling< analyticGeometry * > aG; 
    vectorHandling< constValue * > cV; 
    vectorHandling< analyticFunction * > aF; 
    vectorHandling< boundedVolume * > bV; 
    vectorHandling< dtCase * > dtC; 

    //
    // create log files
    //
    logMe::initLog(
      vm["log"].as<std::string>()+"_"+stringPrimitive::intToString(world.rank())  
    );
    
    dtXmlParser parser(
      vm["xmlIn"].as<std::string>(), vm["xmlOut"].as<std::string>()
    );

    parser.parse();
    parser.load();
    dt__infoNoClass(
      main(), 
      << "Call command:" << std::endl
      << vm.callCommand()
    );

    //
    // output states
    //
    dt__quickinfo(<< "parser.getStates() = " << parser.getStates());

    //
    // create objects and mesh
    //
    parser.createConstValue(&cV);
    if (vm["state"].as<std::string>() == "lastState") {
      parser.loadStateToConst(parser.getStates().back(), cV);      
    }
    else {
      parser.loadStateToConst(vm["state"].as<std::string>(), cV);
    }
    parser.createAnalyticFunction(&bC, &cV, &aF);
    parser.createAnalyticGeometry(&bC, &cV, &aF, &aG);
    parser.createBoundedVolume(&bC, &cV, &aF, &aG, &bV);
    parser.createCase(&bC, &cV, &aF, &aG, &bV, &dtC);
    
    //
    // run current case
    //
    dtC.get(vm["case"].as<std::string>())->runCurrentState();

    //
    // destroy objects
    //
    cV.destroy();
    aF.destroy();
    aG.destroy();
    bV.destroy();
  }
  //
  // catch dtOO exceptions
  //
  catch (eGeneral & eGenRef) {
    dt__catchNoClass(main(), eGenRef.what());
    env.abort(64);
  }
  //
  // catch std exceptions
  //
  catch (std::exception& e) {
    std::cerr << "exception caught: " << e.what() << '\n';

    env.abort(64);
  }
	
	return 0;
}
