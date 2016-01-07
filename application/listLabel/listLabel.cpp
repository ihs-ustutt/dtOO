#include <stdio.h>

#include <logMe/logMe.h>
#include <logMe/dtParMacros.h>
#include <xmlHeaven/dtXmlParser.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <dtArg.h>

#include <iostream>

using namespace dtOO;

int main( int ac, char* av[] ) {
  try {
    //
    // options
    //
    dtArg vm("createState", ac, av);
    
    //
    // create machine
    //
    vm.setMachine();
    
    vm.description().add_options()
      (
        "type", 
        dtPO::value<std::string>()->required(), 
        "define type to output labels (required)"
      )
    ;
    vm.update();
    
    //
    // create log files
    //
    logMe::initLog(vm["log"].as<std::string>()  );
    dt__infoNoClass(
      main(), 
      << "Call command:" << std::endl
      << vm.callCommand()
    );
    
    //
    // create parser and parse
    //
    dtXmlParser parser(
      vm["xmlIn"].as<std::string>(), vm["xmlOut"].as<std::string>()
    );
    parser.parse();
    parser.load();

    //
    // get labels
    //
    std::vector< std::string > label 
    = 
    parser.getLabels(vm["type"].as<std::string>());

    //
    // output labels
    //
    dt__forAllRefAuto(label, aLabel) std::cout << aLabel << std::endl;
  }
  //
  // catch dtOO exceptions
  //
  catch (eGeneral & eGenRef) {
    dt__catchNoClass(main(), eGenRef.what());
  }
  //
  // catch std exceptions
  //
  catch (std::exception& e) {
    dtOO::FILELog().Get(dtOO::logERROR) 
      << "exception caught: " << e.what() 
      << std::endl;
  }
	
	return 0;
}
