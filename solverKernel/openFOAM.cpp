#include "openFOAM.h"

#include <logMe/logMe.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <interfaceHeaven/systemHandling.h>

#include <criticalHeaven/prepareOpenFOAM.h>
#include <argList.H>
#include <Time.H>
#include <IOdictionary.H>
//#include <OStringStream.H>
#include <OFstream.H>
#include <IFstream.H>
#include <IOstream.H>
#include <IOobject.H>

namespace dtOO {  
  openFOAM::openFOAM() { 
  }

  openFOAM::~openFOAM() {
  }

	void openFOAM::init( 
		QDomElement const & element,
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG,
		vectorHandling< boundedVolume * > const * const bV,
		vectorHandling< dtCase * > const * const dC
	) {
	  dtCase::init(element, bC, cV, aF, aG, bV, dC);
		
		dt__info(init(), << "I'm an openFOAM case!!!!!!");
		
//		systemHandling::createDirectory(getLabel());
//		systemHandling::createDirectory(getLabel()+"/system");
		Foam::argList::noParallel();
		
		int argc = 3;
		char ** argv = new char*[3];
		argv[0] = const_cast< char * >("antoineInTheBuilding");
		argv[1] = const_cast< char * >("-case");
		argv[2] = const_cast< char * >(getLabel().c_str());
//		Foam::argList args(argc, argv, false, false);

		//
		// delete and copy template case
		//				
		systemHandling::commandAndWait( "rm -rf "+getOption("workingDirectory")+"/"+getLabel() );
		systemHandling::commandAndWait(
		  "cp -r "+getOption("templateCase")+" "+getOption("workingDirectory")+"/"+getLabel()
		);

		//
		// create openFOAM rootCase and time
		//
#include "setRootCase.H"
#include "createTime.H"
		
		//
		// read dictionary
		//
		Foam::IOdictionary fieldDict(
      Foam::IOobject(
				"system/controlDict",
				runTime,
				Foam::IOobject::READ_IF_PRESENT,
				Foam::IOobject::AUTO_WRITE,
				false
			)
		);
		
		//
		// add entry
		//
		fieldDict.add("dieter", "herbert");		
		
		//
		// write modified dict
		//
		fieldDict.writeObject(
		  Foam::IOstream::ASCII, 
			Foam::IOstream::currentVersion,
			Foam::IOstream::compressionType::UNCOMPRESSED
		);
	}
		
  void openFOAM::apply(void) {  
  }
}


