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
#include <dtPlugin.h>
#include <interfaceHeaven/stringPrimitive.h>

#include <boost/program_options.hpp>
namespace po = boost::program_options;
#include <iostream>

#include <readline/readline.h>
#include <readline/history.h>

using namespace dtOO;

#define dt__commandIf( str, helpStr, name, description ) \
  helpStr << name << " : " << std::endl \
  << "    " << description << std::endl; \
  if ( str == name )

std::string parseCommand(
  std::string aRule,
  dtXmlParser & parser,
  baseContainer & bC,
  vectorHandling< constValue * > & cV,
  vectorHandling< analyticFunction* > & aF,
  vectorHandling< analyticGeometry * > & aG,
  vectorHandling< boundedVolume * > & bV,
  vectorHandling< dtCase * > & dtC,
  vectorHandling< dtPlugin * > & dtP
) {
  std::stringstream help;  
  help << "Commands:" << std::endl;
  std::string addRule = "";
  if ( stringPrimitive::stringContains("(", aRule) ) {
    addRule
    = 
    stringPrimitive::getStringBetweenAndRemove("(", ")", &aRule);
  }
  dt__commandIf( aRule, help, "parse", "parse input xml" ) {
    parser.parse();
    return std::string("");    
  }      
  dt__commandIf( aRule, help, "load", "parse output xml" ) {
    parser.load();
    return std::string("");    
  }
  dt__commandIf( aRule, help, "getStates", "get state labels" ) {
    std::vector< std::string > state = parser.getStates();
		std::stringstream ss;
		dt__forAllRefAuto(state, aState) ss << aState << " ";
    ss << std::endl;
		return ss.str();    
  }  
  dt__commandIf( aRule, help, "createConstValue", "create constValues" ) {
    parser.createConstValue(&cV);
    return std::string("");    
  }           
  dt__commandIf( aRule, help, "loadStateToConst", "load a state" ) { 
    parser.loadStateToConst(addRule, cV);
    return std::string("");    
  }      
  dt__commandIf( aRule, help, "write", "write current state" ) {
    parser.write(addRule, cV);
    return std::string("");    
  }      
  dt__commandIf( aRule, help, "writeUpdate", "write update of a state" ) {
    parser.writeUpdate(addRule, cV);
    return std::string("");    
  }
  dt__commandIf( aRule, help, "remove", "remove a state" ) {
    parser.remove(addRule);
    return std::string("");    
  }     
  dt__commandIf( 
    aRule, help, "createAnalyticFunction", "create analyticFunctions" 
  ) {
    parser.createAnalyticFunction(&bC, &cV, &aF);
    return std::string("");    
  }            
  dt__commandIf( 
    aRule, help, "createAnalyticGeometry", "create analyticGeometries"
  ) {
    parser.createAnalyticGeometry(&bC, &cV, &aF, &aG);
    return std::string("");    
  }            
  dt__commandIf( 
    aRule, help, "createBoundedVolume", "create boundedVolumes" 
  ) {
    parser.createBoundedVolume(&bC, &cV, &aF, &aG, &bV);
    return std::string("");    
  }            
  dt__commandIf( aRule, help, "createCase", "create cases" ) {
    parser.createCase(&bC, &cV, &aF, &aG, &bV, &dtC);              
    return std::string("");    
  }            
  dt__commandIf( aRule, help, "createPlugin", "create plugins" ) {
    parser.createPlugin(&bC, &cV, &aF, &aG, &bV, &dtP);
    return std::string("");    
  }
  dt__commandIf( 
    aRule, help, "destroyAndCreate", 
    "destroy constValues if empty and recreate all"
    " analyticFunctions, analyticGeometries, boundedVolumes, cases"
    " and plugins" 
  ) {
    parser.destroyAndCreate(bC, cV, aF, aG, bV, dtC, dtP);
    return std::string("");
  }        
  dt__commandIf( aRule, help, "applyPlugin", "apply a plugin") {      
    dtP.get(addRule)->apply();
    return std::string("");
  }     
  
  
  help 
  << std::endl
  << "Command " << aRule << " not found." 
  << std::endl;

  return help.str();
}

int main( int ac, char* av[] ) {
  try {
    //
    // options
    //
    po::options_description desc("Allowed options");
    desc.add_options()
      ("help", "produce help message")
      ("xmlIn,i", po::value<std::string>(), "set input xml file (required)")
      ("xmlOut,o", po::value<std::string>(), "set output xml file (required)")
      (
        "log", 
        po::value<std::string>()->default_value("cubanLinxShell.log"), 
        "define logfile (optional)"
      )
    ;
    po::variables_map vm;        
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);  

    if (
      vm.count("help") || !vm.count("xmlIn") || !vm.count("xmlOut") 
    ) {
      std::cout << desc << "\n";
      return 0;
    }

    std::cout 
    << "#" << std::endl
    << "# Starting cubanLinxShell" << std::endl
    << "#" << std::endl
    << "# log: " << vm["log"].as<std::string>() << std::endl
    << "# xmlIn: " << vm["xmlIn"].as<std::string>() << std::endl
    << "# xmlOut: " << vm["xmlOut"].as<std::string>() << std::endl
    << "#" << std::endl;      
    //
    // create log files
    //
    FILELog::ReportingLevel() = logDEBUG;
    Output2FILE::Stream().open( 
      vm["log"].as<std::string>(), 
      std::ofstream::out | std::ofstream::trunc 
    );	
    
    dtXmlParser parser(
      vm["xmlIn"].as<std::string>(), vm["xmlOut"].as<std::string>()
    );
    
    baseContainer bC;
    vectorHandling< analyticGeometry * > aG; 
    vectorHandling< constValue * > cV; 
    vectorHandling< analyticFunction * > aF; 
    vectorHandling< boundedVolume * > bV; 
    vectorHandling< dtCase * > dtC; 
    vectorHandling< dtPlugin * > dtP;      
    
    while (true) {
      //
      // get command
      //
      char * line = readline("cubanLinxShell ::> ");
      
      if (!line) break;
      
      //
      // make history
      //
      if (*line) add_history(line);
      
      //
      // make command string
      //
      std::string command(line);
      if ( command.empty() ) continue;
      
      //
      // free line
      //
      free(line);      
      
      if ( command == "exit" || std::cin.eof() ) {
        break;
      }
      else {
        try {
          std::cout 
          << 
          parseCommand(command, parser, bC, cV, aF, aG, bV, dtC, dtP);
        }
        catch (eGeneral & eGenRef) {
          std::cout << eGenRef.what() << std::endl;
        }
      }
    }

    std::cout << "# Free memory" << std::endl;    
    //
    // destroy objects
    //
    cV.destroy();
    aF.destroy();
    aG.destroy();
    bV.destroy();
    std::cout << "# Goodbye" << std::endl;    
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
    std::cerr << "exception caught: " << e.what() << '\n';
  }
	
	return 0;
}
