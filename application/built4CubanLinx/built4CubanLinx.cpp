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
#include <dtArg.h>

#include <iostream>

#include <linenoise/linenoise.h>

#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>

#include <interfaceHeaven/systemHandling.h>

#include <dtOOVersion.h>

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
  cVPtrVec & cV,
  aFPtrVec & aF,
  aGPtrVec & aG,
  bVPtrVec & bV,
  dCPtrVec & dtC,
  dPPtrVec & dtP
) {
  std::stringstream help;  
  help << "Commands:" << std::endl;
  std::vector< std::string > addRule;
  if ( stringPrimitive::stringContains("(", aRule) ) {
    addRule
    = 
    stringPrimitive::convertToCSVStringVector(
      stringPrimitive::getStringBetweenAndRemoveRespectOcc("(", ")", &aRule)
    );
    dt__infoNoClass(parseCommand(), << "addRule = " << addRule);
  }
  dt__commandIf( aRule, help, "parse", "parse input xml" ) {
    parser.parse();
    return std::string("");    
  }
  dt__commandIf( aRule, help, "reparse", "reparse input xml" ) {
    //
    // destroy existing stuff
    //
    cV.destroy();
    aF.destroy();
    aG.destroy();
    bV.destroy();
    dtC.destroy();
    dtP.destroy();
    
    //
    // reinit and parse again
    //
    parser.reinit();
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
    << "dtXmlParser -> state > " << parser.currentState() << " <" << std::endl
    << "constValue       [cV ] size = " << cV.size() << std::endl
    << "analyticFunction [aF ] size = " << aF.size() << std::endl
    << "analyticGeometry [aG ] size = " << aG.size() << std::endl
    << "boundedVolume    [bV ] size = " << bV.size() << std::endl      
    << "dtCase           [dtC] size = " << dtC.size() << std::endl      
    << "dtPlugin         [dtP] size = " << dtP.size() << std::endl;
    
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
    cV.destroy();
    parser.createConstValue(&cV);
    return std::string("");    
  }           
  dt__commandIf( aRule, help, "loadStateToConst", "load a state" ) {
    dt__throwIfNoClass(addRule.size()!=1, parseCommand());
    parser.loadStateToConst(addRule[0], cV);
    return std::string("");    
  }
  dt__commandIf( aRule, help, "diffStateTo", "diff current state to another state" ) {
    std::stringstream ss;
    dt__throwIfNoClass(addRule.size()!=1, parseCommand());
    //parser.loadStateToConst(addRule[0], cV);
    std::string currentState = parser.currentState();
    parser.loadStateToConst(addRule[0], cV);
    vectorHandling< float > cV_val(
      cV.size(), std::numeric_limits<float>::max()
    );
    dt__forAllIndex(cV, ii) {
      cV_val[ii] = cV[ii]->getValue();
    }
    ss //<< currentState << " <--> " << addRule[0] << std::endl;
      << logMe::dtFormat("%32s | %32s | %32s | %8s") 
           % "label" % currentState % addRule[0] % " "
      << std::endl
      << "---------------------------------|"
      << std::endl;
    parser.loadStateToConst(currentState, cV);
    dt__forAllIndex(cV, ii) {
      if ( cV_val[ii] != cV[ii]->getValue() ) {
        ss 
        << logMe::dtFormat(
          "%32s | %32.16e | %32.16e | %8.2f"
          ) 
          % cV[ii]->getLabel() % cV[ii]->getValue() 
          % cV_val[ii] % (cV[ii]->getValuePercent()*100.)
        << std::endl;
      }
    }
    
    return ss.str();
  }       
  dt__commandIf( aRule, help, "write", "write current state" ) {
    dt__throwIfNoClass(addRule.size()!=1, parseCommand());
    parser.write(addRule[0], cV);
    return std::string("");    
  }      
  dt__commandIf( aRule, help, "writeUpdate", "write update of a state" ) {
    dt__throwIfNoClass(addRule.size()!=1, parseCommand());
    parser.writeUpdate(addRule[0], cV);
    return std::string("");    
  }
  dt__commandIf( aRule, help, "remove", "remove a state" ) {
    dt__throwIfNoClass(addRule.size()!=1, parseCommand());
    parser.remove(addRule[0]);
    return std::string("");    
  } 
  dt__commandIf( aRule, help, "extract", "extract a state" ) {
    dt__throwIfNoClass(addRule.size()!=2, parseCommand());
    parser.extract(addRule[0], cV, addRule[1]);
    return std::string("");    
  }       
  dt__commandIf( aRule, help, "extractAll", "extract all states" ) {
    dt__throwIfNoClass(addRule.size()!=0, parseCommand());
    parser.extractAll(cV);
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
    dt__forAllRefAuto(dtP, aP) {
      if ( stringPrimitive::matchWildcard(addRule[0], aP->getLabel()) ) {
        aP->apply();
      }
    }  
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
  dt__commandIf( aRule, help, "gDebug", "Set global debug option") {
    staticPropertiesHandler::getInstance()->setGDebug();
    return "";
  }    
  dt__commandIf( aRule, help, "commandAndWait", "call system command" ) {
    dt__throwIfNoClass(addRule.size()!=1, parseCommand());
    std::stringstream ss;
    ss << systemHandling::commandAndWait(addRule[0]) << std::endl;
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
  std::string aRule, dtXmlParser & parser, cVPtrVec & cV
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
// analyticFunction commands
//
std::string parseCommand(
  std::string aRule, 
  dtXmlParser & parser, 
  aFPtrVec & aF
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
  dt__commandIf( 
    aRule, help, "info", "show info of aF" 
  ) {
    std::stringstream ss;
    dt__forAllRefAuto(aF, anAF) {
      ss 
      << logMe::dtFormat("aF[ %32s ] = %32s") 
        % anAF->getLabel() % anAF->virtualClassName()
      << std::endl;
    }
    
    return ss.str();
  }
  dt__commandIf( 
    aRule, help, "dump", "show info of aF" 
  ) {
    std::stringstream ss;
    dt__forAllRefAuto(aF, anAF) anAF->dump();
    
    return ss.str();
  }         
  dt__commandIf( aRule, help, "help", "This help") {
    help << std::endl;
    return help.str();
  }
  
  return std::string( "Unknown command: " + aRule + "\n");  
}

//
// analyticGeometry commands
//
std::string parseCommand(
  std::string aRule, 
  dtXmlParser & parser, 
  aGPtrVec & aG
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

std::string parseCommand(
  std::string aRule, 
  dtXmlParser & parser, 
  bVPtrVec & bV
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
  dt__commandIf( 
    aRule, help, "info", "show info of dtP" 
  ) {
    std::stringstream ss;
    dt__forAllRefAuto(bV, anBV) {
      ss 
      << logMe::dtFormat("bV[ %32s ] = %32s") 
        % anBV->getLabel() % anBV->virtualClassName()
      << std::endl;
    }
    
    return ss.str();
  }       
  dt__commandIf( aRule, help, "makePreGrid", "make pre grid") {
    dt__forAllRefAuto(bV, aBV) {
      if (
        stringPrimitive::matchWildcard( addRule[0], aBV->getLabel() )
      ) aBV->makePreGrid();
    }  
    return std::string("");
  }
  dt__commandIf( aRule, help, "makeGrid", "make grid") {
    dt__forAllRefAuto(bV, aBV) {
      if (
        stringPrimitive::matchWildcard( addRule[0], aBV->getLabel() )
      ) aBV->makeGrid();
    }  
    return std::string("");
  }  
  dt__commandIf( aRule, help, "getMeshTags", "get mesh tags") {
    std::stringstream ss;
    std::vector< std::string > tags = bV.get(addRule[0])->getMeshTags();
    dt__forAllRefAuto(tags, aTag) ss << aTag << std::endl;
    return ss.str();
  }
  dt__commandIf( aRule, help, "help", "This help") {
    help << std::endl;
    return help.str();
  }
  
  return std::string( "Unknown command: " + aRule + "\n");  
}

std::string parseCommand(
  std::string aRule, 
  dtXmlParser & parser, 
  dCPtrVec & dtC
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
  dt__commandIf( 
    aRule, help, "info", "show info of dtP" 
  ) {
    std::stringstream ss;
    dt__forAllRefAuto(dtC, anDtC) {
      ss 
      << logMe::dtFormat("dtC[ %32s ] = %32s") 
        % anDtC->getLabel() % anDtC->virtualClassName()
      << std::endl;
    }
    
    return ss.str();
  }
  dt__commandIf( 
    aRule, help, "runCurrentState", "show info of dtP" 
  ) {
    std::stringstream ss;
    dtC.get( addRule[0] )->runCurrentState();
    
    ss << "Done.";
  
    return ss.str();
  }    
  dt__commandIf( aRule, help, "help", "This help") {
    help << std::endl;
    return help.str();
  }
  
  return std::string( "Unknown command: " + aRule + "\n");  
}

std::string parseCommand(
  std::string aRule, 
  dtXmlParser & parser, 
  dPPtrVec & dtP
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
//  dt__commandIf( aRule, help, "info", "get a point" ) {
//    std::stringstream ss;    
//    return ss.str();
//  }      
  dt__commandIf( 
    aRule, help, "info", "show info of dtP" 
  ) {
    std::stringstream ss;
    dt__forAllRefAuto(dtP, anDtP) {
      ss 
      << logMe::dtFormat("dtP[ %32s ] = %32s") 
        % anDtP->getLabel() % anDtP->virtualClassName()
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
    dtArg vm("built4CubanLinx", ac, av);    
    
    //
    // set machine
    //
    vm.setMachine();
    
    //
    // update arguments
    //
    vm.update();

    //
    // output welcome header
    //
    std::cout 
      << "#" << std::endl
      << "# Starting built4CubanLinx" << std::endl
      << "#   dtOO version : " << git::version << std::endl
      << "#   log: " << vm["log"].as<std::string>() << std::endl
      << "#   xmlIn: " << vm["xmlIn"].as<std::string>() << std::endl
      << "#   xmlOut: " << vm["xmlOut"].as<std::string>() << std::endl
      << "#" << std::endl;      

    //
    // create log files
    //
    logMe::initLog(vm["log"].as<std::string>()  );    
    dt__infoNoClass(
      main(), 
      << "Call command:" << std::endl
      << vm.callCommand()
    );
    
    dtXmlParser::init(
      vm["xmlIn"].as<std::string>(), vm["xmlOut"].as<std::string>()
    );
    dtXmlParser & parser = dtXmlParser::reference();
    
    baseContainer bC;
    aGPtrVec aG; 
    cVPtrVec cV; 
    aFPtrVec aF; 
    bVPtrVec bV; 
    dCPtrVec dtC; 
    dPPtrVec dtP;      
    
    std::cout 
      << "# define standard variables" << std::endl
      << "#   baseContainer bC" << std::endl
      << "#   aGPtrVec aG" << std::endl
      << "#   cVPtrVec cV" << std::endl 
      << "#   aFPtrVec aF" << std::endl
      << "#   bVPtrVec bV" << std::endl
      << "#   dCPtrVec dtC" << std::endl
      << "#   dPPtrVec dtP" << std::endl;
    
    linenoiseHistorySetMaxLen(50);
    
    while (true) {
      //
      // get command
      //
      char * line = linenoise("built4CubanLinx::> ");
      
      if (line == NULL) break;
      
      //
      // make history
      //
      if (*line) linenoiseHistoryAdd(line);
      
      //
      // make command string
      //
      ::std::string command(line);
      if ( command.empty() ) continue;
      
      //
      // free line
      //
      free(line);      
      
      if ( command == "exit" ) break;
      
      else {
        try {
          if ( stringPrimitive::stringContains("->", command) ) {
            ::std::string onClass 
            = 
            stringPrimitive::getStringBetweenAndRemove("", ">", &command);
            onClass = stringPrimitive::replaceStringInString("-", "", onClass);
            
            if (onClass == "cV") {
              std::cout << parseCommand(command, parser, cV);
            }
            else if (onClass == "aF") {
              std::cout << parseCommand(command, parser, aF);
            }
            else if (onClass == "aG") {
              std::cout << parseCommand(command, parser, aG);
            }
            else if (onClass == "bV") {
              std::cout << parseCommand(command, parser, bV);
            }
            else if (onClass == "dtC") {
              std::cout << parseCommand(command, parser, dtC);
            }
            else if (onClass == "dtP") {
              std::cout << parseCommand(command, parser, dtP);
            }            
            else dt__throwNoClass(main(), << onClass << " not defined.");
          }
          else if ( command.find_first_of(".") == 0 ) {
            command.erase(0, 1);
            std::cout 
            << 
            parseCommand(
              "commandAndWait("+command+")", 
              parser, bC, cV, aF, aG, bV, dtC, dtP
            );
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
