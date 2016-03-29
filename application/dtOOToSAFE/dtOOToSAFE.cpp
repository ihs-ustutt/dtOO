#include <stdio.h>

#include <logMe/logMe.h>
#include <logMe/dtParMacros.h>
#include <xmlHeaven/dtXmlParser.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <interfaceHeaven/systemHandling.h>
#include <baseContainerHeaven/baseContainer.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <interfaceHeaven/vectorHandling.h>
#include <analyticFunctionHeaven/scaFunction.h>
#include <constValueHeaven/constValue.h>
#include <boundedVolume.h>
#include <dtCase.h>
#include <resultValueHeaven/resultValue.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <dtArg.h>

#include <iostream>

using namespace dtOO;

int main( int ac, char* av[] ) {
  try {
    //
    // options
    //
    dtArg vm("dtOOToSAFE", ac, av);    
    
    //
    // set machine
    //
    vm.setMachine();

    //
    // additional arguments
    //    
    std::vector< std::string > def(0);
    vm.description().add_options()
      (
        "statePattern", 
        dtPO::value<std::string>()->required(), 
        "define state (required)"
      )
      (
        "x", 
        dtPO::value< std::vector< std::string > >()->required()->default_value(
          std::vector< std::string >(0), ""
        ), 
        "define x values (required)"
      )
      (
        "y", 
        dtPO::value< std::vector< std::string > >()->required()->default_value(
          std::vector< std::string >(0), ""    
        ), 
        "define y values (required)"
      )
      (
        "yWorst", 
        dtPO::value< std::vector< float > >()->required()->default_value(
          std::vector< float >(0), "0"    
        ),
        "define worst y values (required)"
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
    
    dtXmlParser parser(
      vm["xmlIn"].as<std::string>(), vm["xmlOut"].as<std::string>()
    );

    parser.parse();
    parser.load();

    //
    // create directory
    //
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
    std::vector< float > yWorst 
    = 
    vm["yWorst"].as< std::vector< float > >();    
    
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
    std::fstream stateOf("./dtOOToSAFE/state", std::ios::out | std::ios::trunc); 
    
    //
    // adjust yWorst vector
    //
    if (yWorst.size() == 1) {
      dt__infoNoClass(
        main(), 
        << "Adjust yWorst vector to size = " << yOf.size() << " and value = "
        << yWorst.front() << "."
      );
      yWorst.resize(yOf.size(), yWorst.front());
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
      
      stateOf << aState << std::endl;
      dt__forAllIndex(xLabel, ii) {
        *(xOf[ii]) << cV.get(xLabel[ii])->getValue() << std::endl;
      }
      if ( !result.empty() ) {
        dt__forAllIndex(yLabel, ii) {
          *(yOf[ii]) << result.get(yLabel[ii])->operator()() << std::endl;
        }
        result.destroy();
      }
      else {
        dt__forAllIndex(yLabel, ii) {
          *(yOf[ii]) << yWorst[ii] << std::endl;
        }   
      }
    }
    //
    // destroy objects
    //
    cV.destroy();
    aF.destroy();
    aG.destroy();
    bV.destroy();
    dtC.destroy();

    stateOf.close();
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
