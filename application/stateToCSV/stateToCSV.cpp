#include <stdio.h>

#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParser.h>
#include <interfaceHeaven/vectorHandling.h>
#include <constValueHeaven/constValue.h>
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
        "prefix", 
        po::value< std::string >()->default_value("stateToCSV"), 
        "define prefix of csv output file (optional)"
      )
      ("log", po::value<std::string>(), "define logfile (required)")
    ;
    po::variables_map vm;        
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);

    if (
      vm.count("help")
      ||
      !vm.count("xmlIn") || !vm.count("xmlOut") 
      || 
      !vm.count("log")
    ) {
      std::cout << desc << std::endl;
      return 0;
    }
    
    //
    // create log files
    //
    FILELog::ReportingLevel() = logDEBUG;
    Output2FILE::Stream().open( 
      vm["log"].as<std::string>(), 
      std::ofstream::out | std::ofstream::trunc 
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
    // create constValues
    //
    parser.createConstValue(&cV);

    //
    // get state labels
    //
    std::vector< std::string > state= parser.getStates();

    //
    // create output file
    //
    std::fstream of;
    of.open(
      (vm["prefix"].as<std::string>()+".csv").c_str(),
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
