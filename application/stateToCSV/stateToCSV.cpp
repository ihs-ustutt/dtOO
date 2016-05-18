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
    dtArg vm("stateToCSV", ac, av);
    
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
      (
        "x", 
        dtPO::value< std::vector< std::string > >()->required()->default_value(
          std::vector< std::string >(0), ""
        ), 
        "define x values"
      )  
      (
        "percent", 
        dtPO::value< bool >()->required()->default_value(false), 
        "normalize x values"
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
    dtXmlParser::init(
      vm["xmlIn"].as<std::string>(), vm["xmlOut"].as<std::string>()
    );
    dtXmlParser & parser = dtXmlParser::reference();
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

    std::vector< std::string > xLabel 
    = 
    vm["x"].as< std::vector< std::string > >();
    if (xLabel.empty()) {
      dt__forAllRefAuto(cV, aCV) {      
        xLabel.push_back( aCV->getLabel() );
      }
    }
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
//    dt__forAllRefAuto(cV, aCV) of << ", " << aCV->getLabel();
    dt__forAllIndex(xLabel, ii) {
       of << ", " << cV.get(xLabel[ii])->getLabel();
    }            
    of << std::endl;
    
    //
    // write all states
    //
    dt__forAllRefAuto(state, aState) {
      parser.loadStateToConst( aState, cV );
      
      of << aState;
//      dt__forAllRefAuto(cV, aCV) {
        //of << ", " << aCV->getValue();
      dt__forAllIndex(xLabel, ii) {
        if ( vm["percent"].as< bool >() ) {
          of 
            << ", " 
            << 
              ( cV.get(xLabel[ii])->getValue() - cV.get(xLabel[ii])->getMin() )
              /
              ( cV.get(xLabel[ii])->getMax() - cV.get(xLabel[ii])->getMin() );
        }
        else {
         of << ", " << cV.get(xLabel[ii])->getValue();
        }
      }        
//      }
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
