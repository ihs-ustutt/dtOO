/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include <stdio.h>

#include <logMe/logMe.h>
#include <logMe/dtParMacros.h>
#include <xmlHeaven/dtXmlParser.h>
#include <interfaceHeaven/stringPrimitive.h>

#include <boost/program_options.hpp>
namespace po = boost::program_options;
#include <iostream>

#include "dtArg.h"

using namespace dtOO;

int main( dtInt ac, char* av[] ) {
  try {
    //
    // options
    //
    dtArg vm("listState", ac, av);
    
    //
    // create machine
    //
    vm.setMachine();
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
    // create parser and parse
    //
    dtXmlParser::init(
      vm["xmlIn"].as<std::string>(), vm["xmlOut"].as<std::string>()
    );
    dtXmlParser & parser = dtXmlParser::reference();
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
