#include <stdio.h>

#include <logMe/logMe.h>
#include <logMe/dtParMacros.h>
#include <xmlHeaven/dtXmlParser.h>
#include <interfaceHeaven/stringPrimitive.h>

#include <boost/program_options.hpp>
namespace po = boost::program_options;
#include <iostream>

using namespace dtOO;

int main( int ac, char* av[] ) {
  try {
    //
    // options
    //
    po::options_description desc("Allowed options");
    desc.add_options()
      ("help", "produce help message")
      ("xmlIn", po::value<std::string>(), "set input xml file (required)")
      ("xmlOut", po::value<std::string>(), "set output xml file (required)")
      (
        "log", 
        po::value<std::string>()->default_value("listState.log"), 
        "define logfile (optional)"
      )
    ;
    po::variables_map vm;        
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);  

    if (vm.count("help") || !vm.count("xmlIn") || !vm.count("xmlOut") ) {
      std::cout << desc << "\n";
      return 0;
    }

    //
    // create log files
    //
    logMe::initLog(vm["log"].as<std::string>()  );
    
    dt__infoNoClass(
      main(), 
      << "Call command:" << std::endl
      << std::vector<std::string>(av, av+ac)
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
    std::vector< std::string > state = parser.getStates();

    //
    // output labels
    //
    dt__forAllRefAuto(state, aState) std::cout << aState << std::endl;
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
