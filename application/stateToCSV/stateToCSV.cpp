#include <stdio.h>

#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParser.h>
#include <interfaceHeaven/vectorHandling.h>
#include <constValueHeaven/constValue.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <interfaceHeaven/systemHandling.h>
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
    
    //
    // additional arguments
    //
    vm.description().add_options()
      (
        "prefix", 
        dtPO::value< std::string >()->default_value("stateToCSV"), 
        "define prefix of csv output file (optional)"
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
    // vectorHandling of constValues
    //
    vectorHandling< constValue * > cV; 
    
    //
    // create parser and parse
    //
    dtXmlParser parser(
      vm["xmlIn"].as<std::string>(), vm["xmlOut"].as<std::string>()
    );
    parser.parse();
    parser.load();
    
    //
    // create directory
    //
    systemHandling::createDirectory("stateToCSV");
    
    //
    // create constValues
    //
    parser.createConstValue(&cV);

    //
    // get state labels
    //
    std::vector< std::string > state = parser.getStates();

    //
    // create output file
    //
    std::fstream of;
    of.open(
      ("./stateToCSV/"+vm["prefix"].as<std::string>()+".csv").c_str(),
      std::ios::out | std::ios::trunc
    );    

    //
    // write header
    //
    of << "# state";
    dt__forAllRefAuto(cV, aCV) of << ", " << aCV->getLabel();
    of << std::endl;
    
    //
    // write all states
    //
    dt__forAllRefAuto(state, aState) {
      parser.loadStateToConst( aState, cV );
      
      of << aState;
      dt__forAllRefAuto(cV, aCV) of << ", " << aCV->getValue();
      of << std::endl;      
    }
    of.close();    
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
      << "std::exception caught: " << e.what() << std::endl;
  }
	
	return 0;
}
