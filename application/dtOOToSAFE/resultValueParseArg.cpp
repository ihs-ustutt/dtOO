#include <stdio.h>

#include <logMe/logMe.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <dtArg.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include <iostream>

using namespace dtOO;

int main( int ac, char* av[] ) {
  try {
    //
    // options
    //
    dtArg vm("resultValueParseArg", ac, av);
    vm.description().add_options()
      (
        "resultValue", dtPO::value<std::string>()->required(), 
        "set resultValue file (required)"
      )
    ;
    vm.update();

    //
    // create log files
    //
    logMe::initLog(vm["log"].as<std::string>()  );
    
    //
    // write call command to log
    //
    dt__infoNoClass(
      main(), 
      << "Call command:" << std::endl
      << vm.callCommand()
    );    
    
    //
    // open file
    //
    std::ifstream in( vm["resultValue"].as<std::string>().c_str() );    
    dt__throwIfNoClass(!in, main());
    
    //
    // write every second element to std::cout
    //
    std::cout << "Possible x Arguments: " << std::endl;
    
    //
    // read file
    //
    std::string line;
    while (getline(in, line)) {
      boost::algorithm::trim_all(line);
      std::vector< std::string > parts;
      boost::split(
        parts, line, boost::is_any_of(","), boost::token_compress_on
      );

      std::cout << "--y=" << parts[1] << " ";
    }
    in.close();   

    std::cout << std::endl;
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
	
  logMe::closeLog();
  
	return 0;
}
