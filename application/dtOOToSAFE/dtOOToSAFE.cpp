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
#include <resultValueHeaven/resultValue.h>
#include <interfaceHeaven/stringPrimitive.h>

#include <boost/program_options.hpp>
namespace po = boost::program_options;
#include <iostream>

#include "interfaceHeaven/systemHandling.h"

//#include <boost/mpi.hpp>

using namespace dtOO;

int main( int ac, char* av[] ) {
//  ::boost::mpi::environment env;
//  ::boost::mpi::communicator world;    
  
  try {
    //
    // options
    //
    po::options_description desc("Allowed options");
    desc.add_options()
      ("help", "produce help message")
      ("xmlIn", po::value<std::string>(), "set input xml file (required)")
      ("xmlOut", po::value<std::string>(), "set output xml file (required)")
      ("statePattern", po::value<std::string>(), "define state (required)")
      ("log", po::value<std::string>(), "define logfile (required)")
      ("x", po::value< std::vector< std::string > >(), "define x values (required)")
      ("y", po::value< std::vector< std::string > >(), "define y values (required)")
    ;
    po::variables_map vm;        
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);  

    if (
      vm.count("help")
      ||
      !vm.count("xmlIn") || !vm.count("xmlOut") 
      || 
      !vm.count("statePattern") || !vm.count("log")
      || 
      !vm.count("x") || !vm.count("y")      
    ) {
      std::cout << desc << "\n";
      return 0;
    }

    //
    // create log file
    //
    FILELog::ReportingLevel() = logDEBUG;
    Output2FILE::Stream().open( 
      vm["log"].as<std::string>(), std::ofstream::out | std::ofstream::trunc 
    );					
    
    dtXmlParser parser(
      vm["xmlIn"].as<std::string>(), vm["xmlOut"].as<std::string>()
    );

    parser.parse();
    parser.load();

    systemHandling::createDirectory("dtOOToSAFE");
    
    //
    // create state vector
    //
    std::vector< std::string > state = parser.getStates();
    std::vector< std::string > stateToWrite;
        
    dt__forAllRefAuto(state, aState) {
      if ( 
        stringPrimitive::stringContains( 
          vm["statePattern"].as<std::string>(), aState
        )
      ) {
        stateToWrite.push_back( aState );
      }
    }
    dt__infoNoClass(main(), << "Writing " << stateToWrite.size() << " states.");
    
    //
    // create x vector (constValue)
    //
    std::vector< std::string > xLabel 
    = 
    vm["x"].as< std::vector< std::string > >();
    std::vector< std::string > yLabel 
    = 
    vm["y"].as< std::vector< std::string > >();
    
    dt__infoNoClass(main(), << "xLabel = " << xLabel);
    dt__infoNoClass(main(), << "yLabel = " << yLabel);
    std::vector< std::fstream * > xOf;
    dt__forAllRefAuto(xLabel, aLabel) {
      xOf.push_back( 
        new std::fstream( 
          ("./dtOOToSAFE/x_"+aLabel).c_str(), 
          std::ios::out | std::ios::trunc 
        ) 
      );
    }
    std::vector< std::fstream * > yOf;
    dt__forAllRefAuto(yLabel, aLabel) {
      yOf.push_back( 
        new std::fstream( 
          ("./dtOOToSAFE/y_"+aLabel).c_str(), 
          std::ios::out | std::ios::trunc 
        ) 
      );
    }    
//    dt__forAllRefAuto(vm["x"].as< std::vector< std::string > >(), aCV) {
//      
//    }
    
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
    // create objects
    //
    parser.createConstValue(&cV);
    parser.createAnalyticFunction(&bC, &cV, &aF);
    parser.createAnalyticGeometry(&bC, &cV, &aF, &aG);
    parser.createBoundedVolume(&bC, &cV, &aF, &aG, &bV);
    parser.createCase(&bC, &cV, &aF, &aG, &bV, &dtC);
    
    dtC.back()->update();

    dt__forAllRefAuto(stateToWrite, aState) {
      parser.loadStateToConst(aState, cV);
      vectorHandling< resultValue * > result
      =
      dtC.back()->result( aState );
      
      dt__forAllIndex(xLabel, ii) {
        *(xOf[ii]) << cV.get(xLabel[ii])->getValue() << std::endl;
      }
      dt__forAllIndex(yLabel, ii) {
        *(yOf[ii]) << result.get(yLabel[ii])->operator()() << std::endl;
      }
      result.destroy();
    }
    //
    // destroy objects
    //
    cV.destroy();
    aF.destroy();
    aG.destroy();
    bV.destroy();
    dtC.destroy();

    dt__forAllRefAuto(xOf, aFile) {
      aFile->close();
      delete aFile;
    }
    dt__forAllRefAuto(yOf, aFile) {
      aFile->close();
      delete aFile;
    }
    
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
