#include <stdio.h>

#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParser.h>
#include <baseContainerHeaven/baseContainer.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <interfaceHeaven/vectorHandling.h>
#include <analyticFunctionHeaven/scaFunction.h>
#include <constValueHeaven/constValue.h>
#include <boundedVolume.h>
#include <dtCase.h>

#include <boost/program_options.hpp>
namespace po = boost::program_options;
#include <iostream>

#include <boost/mpi.hpp>

using namespace dtOO;

int main( int ac, char* av[] ) {  
  ::boost::mpi::environment env;
  ::boost::mpi::communicator world;    
  
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help", "produce help message")
    ("xmlIn", po::value<std::string>(), "set input xml file (required)")
    ("xmlOut", po::value<std::string>(), "set output xml file (required)")
    ("state", po::value<std::string>(), "define state to run (required)")
    ("log", po::value<std::string>(), "define logfile (required)")
    ("parallel", po::value<bool>(), "parallel?")
  ;


  po::variables_map vm;        
  po::store(po::parse_command_line(ac, av, desc), vm);
  po::notify(vm);  
  
  if (
    vm.count("help")
    ||
    !vm.count("xmlIn") || !vm.count("xmlOut") 
    || 
    !vm.count("state") || !vm.count("log")
  ) {
    std::cout << desc << "\n";
    return 0;
  }
  
  bool parallelRun = false;
  if (vm.count("parallel")) {
    parallelRun = true;
  }  
  
  
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
	FILELog::ReportingLevel() = logINFO;
  Output2FILE::Stream().open( 
    vm["log"].as<std::string>(), std::ofstream::out | std::ofstream::trunc 
  );					
	
	dt__quickinfo(<< "parser.getStates() = " << parser.getStates());
  
  parser.createConstValue(&cV);
  parser.loadStateToConst(vm["state"].as<std::string>(), cV);
  parser.createAnalyticFunction(&bC, &cV, &aF);
  parser.createAnalyticGeometry(&bC, &cV, &aF, &aG);
  parser.createBoundedVolume(&bC, &cV, &aF, &aG, &bV);
  parser.createCase(&bC, &cV, &aF, &aG, &bV, &dtC);
  dtC.back()->runCurrentState();

  cV.destroy();
	aF.destroy();
	aG.destroy();
  bV.destroy();
	
	return 0;
}
