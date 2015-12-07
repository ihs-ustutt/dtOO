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

#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>

using namespace dtOO;

#define dt__commandIf( str, helpStr, name, description ) \
  helpStr << name << " : " << std::endl \
  << "    " << description << std::endl; \
  if ( str == name )

//
// dtXmlParser commands
//
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
  std::vector< std::string > addRule;
  if ( stringPrimitive::stringContains("(", aRule) ) {
    addRule
    = 
    stringPrimitive::convertToCSVStringVector(
      stringPrimitive::getStringBetweenAndRemove("(", ")", &aRule)
    );
    dt__infoNoClass(parseCommand(), << "addRule = " << addRule);
  }
  dt__commandIf( aRule, help, "parse", "parse input xml" ) {
    parser.parse();
    return std::string("");    
  }      
  dt__commandIf( aRule, help, "load", "parse output xml" ) {
    parser.load();
    return std::string("");    
  }
  dt__commandIf( 
    aRule, help, "info", "show quick info of current environment" 
  ) {
    std::stringstream ss;
    ss 
    << "parser " << parser.currentState() << std::endl
    << "cV " << cV.size() << std::endl
    << "aF " << aF.size() << std::endl
    << "aG " << aG.size() << std::endl
    << "bV " << bV.size() << std::endl      
    << "dtC " << dtC.size() << std::endl      
    << "dtP " << dtP.size() << std::endl;
    
    return ss.str();
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
    parser.loadStateToConst(addRule[0], cV);
    return std::string("");    
  }      
  dt__commandIf( aRule, help, "write", "write current state" ) {
    parser.write(addRule[0], cV);
    return std::string("");    
  }      
  dt__commandIf( aRule, help, "writeUpdate", "write update of a state" ) {
    parser.writeUpdate(addRule[0], cV);
    return std::string("");    
  }
  dt__commandIf( aRule, help, "remove", "remove a state" ) {
    parser.remove(addRule[0]);
    return std::string("");    
  } 
  dt__commandIf( aRule, help, "extract", "extract a state" ) {
    parser.extract(addRule[0], cV, addRule[1]);
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
    parser.createPlugin(&bC, &cV, &aF, &aG, &bV, &dtC, &dtP);
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
    dtP.get(addRule[0])->apply();
    return std::string("");
  }
  dt__commandIf( 
    aRule, help, "replaceDependencies", "replace all dependencies in string" 
  ) {
    std::stringstream in;
    dt__forFromToIndex(0, addRule.size()-1, ii) {
      in << addRule[ii] << ",";
    }
    in << addRule.back();
    
    std::stringstream ss;
    ss 
    << parser.replaceDependencies(in.str(), &bC, &cV, &aF, &aG) << std::endl;
		return ss.str();    
  }    
  dt__commandIf( aRule, help, "help", "This help") {
    help << std::endl;
    return help.str();
  }
  
  return std::string( "Unknown command: " + aRule + "\n");
}

//
// constValue commands
//
std::string parseCommand(
  std::string aRule, dtXmlParser & parser, vectorHandling< constValue * > & cV
) {
  std::stringstream help;  
  help << "Commands:" << std::endl;
  std::vector< std::string > addRule;
  if ( stringPrimitive::stringContains("(", aRule) ) {
    addRule
    = 
    stringPrimitive::convertToCSVStringVector(
      stringPrimitive::getStringBetweenAndRemove("(", ")", &aRule)
    );
    dt__infoNoClass(parseCommand(), << "addRule = " << addRule);
  }
  dt__commandIf( aRule, help, "set", "set a constValue" ) {
    std::stringstream ss;
    ss << "cV[ " << addRule[0] << " ] = " << cV.get(addRule[0])->getValue();
    cV.get(addRule[0])->setValue( qtXmlBase::muParseString( addRule[1] ) );
		ss << " -> " << cV.get(addRule[0])->getValue() << std::endl;
		return ss.str();        
  }      
  dt__commandIf( 
    aRule, help, "info", "show info of cV" 
  ) {
    std::stringstream ss;
    int cc = 0;
    dt__forAllRefAuto(cV, aCV) {
      ss 
      << logMe::dtFormat(
        "%4i | cV[ %32s ] = %16.8e ( %16.8e -- %16.8e ) %5.2f %%"
        ) 
        % cc 
        % aCV->getLabel() % aCV->getValue() % aCV->getMin() % aCV->getMax()
        % ((aCV->getValue() - aCV->getMin()) / (aCV->getMax() - aCV->getMin()))
      << std::endl;
      cc++;
    }
    
    return ss.str();
  }       
  dt__commandIf( aRule, help, "help", "This help") {
    help << std::endl;
    return help.str();
  }
  
  return std::string( "Unknown command: " + aRule + "\n");
}

//
// constValue commands
//
std::string parseCommand(
  std::string aRule, 
  dtXmlParser & parser, 
  vectorHandling< analyticGeometry * > & aG
) {
  std::stringstream help;  
  help << "Commands:" << std::endl;
  std::vector< std::string > addRule;
  if ( stringPrimitive::stringContains("(", aRule) ) {
    addRule
    = 
    stringPrimitive::convertToCSVStringVector(
      stringPrimitive::getStringBetweenAndRemove("(", ")", &aRule)
    );
    dt__infoNoClass(parseCommand(), << "addRule = " << addRule);
  }
  dt__commandIf( aRule, help, "getPointPercent", "get a point" ) {
    std::stringstream ss;
    dtPoint3 thePoint;
    //
    // map1dTo3d
    //
    if (addRule.size() == 2) {
      dt__throwIfNoClass( 
        !map1dTo3d::ConstDownCast( aG.get(addRule[0]) ), parseCommand() 
      );
      thePoint 
      = 
      map1dTo3d::ConstSecureCast(aG.get(addRule[0]))->getPointPercent( 
        qtXmlBase::muParseString(addRule[1]) 
      );
    }
    //
    // map1dTo3d
    //
    else if (addRule.size() == 3) {
      dt__throwIfNoClass( 
        !map2dTo3d::ConstDownCast( aG.get(addRule[0]) ), parseCommand() 
      );
      thePoint 
      = 
      map2dTo3d::ConstSecureCast(aG.get(addRule[0]))->getPointPercent( 
        qtXmlBase::muParseString(addRule[1]),
        qtXmlBase::muParseString(addRule[2])
      );
    }
    //
    // map1dTo3d
    //
    else if (addRule.size() == 4) {
      dt__throwIfNoClass( 
        !map3dTo3d::ConstDownCast( aG.get(addRule[0]) ), parseCommand() 
      );
      thePoint 
      = 
      map3dTo3d::ConstSecureCast(aG.get(addRule[0]))->getPointPercent( 
        qtXmlBase::muParseString(addRule[1]),
        qtXmlBase::muParseString(addRule[2]),
        qtXmlBase::muParseString(addRule[3]) 
      );
    }    
    ss 
    << logMe::dtFormat("aG[ %32s ] = ( %16.8e %16.8e %16.8e )")
      % aG.get(addRule[0])->getLabel() 
      % thePoint.x() % thePoint.y() % thePoint.z()
    << std::endl;
    
    return ss.str();
  }      
  dt__commandIf( 
    aRule, help, "info", "show info of aG" 
  ) {
    std::stringstream ss;
    dt__forAllRefAuto(aG, anAG) {
      ss 
      << logMe::dtFormat("aG[ %32s ] = %32s") 
        % anAG->getLabel() % anAG->virtualClassName()
      << std::endl;
    }
    
    return ss.str();
  }       
  dt__commandIf( aRule, help, "help", "This help") {
    help << std::endl;
    return help.str();
  }
  
  return std::string( "Unknown command: " + aRule + "\n");  
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
        po::value<std::string>()->default_value("built4CubanLinx.log"), 
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
    << "# Starting built4CubanLinx" << std::endl
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
    
    std::cout 
    << "# define standard variables" << std::endl
    << "#   baseContainer bC" << std::endl
    << "#   vectorHandling< analyticGeometry * > aG" << std::endl
    << "#   vectorHandling< constValue * > cV" << std::endl 
    << "#   vectorHandling< analyticFunction * > aF" << std::endl
    << "#   vectorHandling< boundedVolume * > bV" << std::endl
    << "#   vectorHandling< dtCase * > dtC" << std::endl
    << "#   vectorHandling< dtPlugin * > dtP" << std::endl;
    
    while (true) {
      //
      // get command
      //
      char * line = readline("built4CubanLinx ::> ");
      
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
      
      if ( command == "exit" || std::cin.eof() ) break;
      else {
        try {
          if ( stringPrimitive::stringContains("->", command) ) {
            std::string onClass 
            = 
            stringPrimitive::getStringBetweenAndRemove("", ">", &command);
            onClass = stringPrimitive::replaceStringInString("-", "", onClass);
            
            if (onClass == "cV") {
              std::cout 
              << 
              parseCommand(command, parser, cV);              
            }
            else if (onClass == "aG") {
              std::cout 
              << 
              parseCommand(command, parser, aG);              
            }
            else dt__throwNoClass(main(), << onClass << " not defined.");
          }
          else {
            std::cout 
            << 
            parseCommand(command, parser, bC, cV, aF, aG, bV, dtC, dtP);
          }
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
