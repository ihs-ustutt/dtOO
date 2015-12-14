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

#include <boost/program_options.hpp>
namespace po = boost::program_options;
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
    po::options_description desc("Allowed options");
    desc.add_options()
      ("help", "produce help message")
      ("xmlIn", po::value<std::string>(), "set input xml file (required)")
      ("xmlOut", po::value<std::string>(), "set output xml file (required)")
      ("state", po::value<std::string>(), "define state to run (required)")
      ("case", po::value<std::string>(), "define case to run (required)")
      (
        "log", 
        po::value<std::string>()->default_value("runCase.log"), 
        "define logfile (optional)"
      )
    ;
    po::variables_map vm;        
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);  

    if (
      vm.count("help")
      ||
      !vm.count("xmlIn") || !vm.count("xmlOut") 
      || 
      !vm.count("state") || !vm.count("case")
    ) {
      std::cout << desc << "\n";
      return 0;
    }

    //
    //
    //
    baseContainer bC;
    vectorHandling< analyticGeometry * > aG; 
    vectorHandling< constValue * > cV; 
    vectorHandling< analyticFunction * > aF; 
    vectorHandling< boundedVolume * > bV; 
    vectorHandling< dtCase * > dtC; 

    dtXmlParser parser(
      vm["xmlIn"].as<std::string>(), vm["xmlOut"].as<std::string>()
    );

    parser.parse();
    parser.load();

    //
    // create log files
    //
    logMe::initLog(
      vm["log"].as<std::string>()+"_"+stringPrimitive::intToString(world.rank())  
    );
    dt__infoNoClass(
      main(), 
      << "Call command:" << std::endl
      << std::vector<std::string>(av, av+ac)
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
