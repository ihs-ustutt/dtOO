#include "openFOAM.h"

#include <logMe/logMe.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <functionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <interfaceHeaven/systemHandling.h>

#ifdef _NUMERIC_H_
  #undef sign
#endif

// avoid issues with pre-processor defines
#undef FALSE
#undef TRUE
#undef OFF
#undef ON
#undef NO
#undef YES
#undef NO_1
#undef YES_1
#undef FALSE_1
#undef TRUE_1
#undef NONE
#undef PLACEHOLDER
#undef INVALID

//#include <HashTable.H>
#include <argList.H>
#include <Time.H>
#include <IOdictionary.H>
#include <OStringStream.H>
#include <OFstream.H>
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
		
		DTINFOWF(init(), << "I'm an openFOAM case!!!!!!");
		
		int argc = 3;
		char ** argv = new char*[3];
		argv[0] = "dieter";
		argv[1] = "-case";
		argv[2] = "myOpenFOAMCase";
		Foam::argList args(argc, (char**&) argv, false, false);

		
		
		
		Foam::dictionary timeDict( 
		  Foam::fileName(args.rootPath()+"/"+args.caseName()+"/timeFile") 
		);
		timeDict.add("deltaT", Foam::scalar(1.));
    timeDict.add("writeFrequency", Foam::scalar(1.));				

		
		Foam::Time runTime(timeDict, args.rootPath(), args.caseName());

		
		Foam::dictionary myDict("myDict");
		Foam::dictionary subDict("subDict");

		Foam::IOobject ioObject("IOobject", runTime);
		
		
		//Foam::word word("wordValue");
//		Foam::scalar scalar(.01);
		
		subDict.add(Foam::word("word"), Foam::word("wordValue"));
		subDict.add(Foam::word("scalar"), Foam::scalar(.01));
		

		myDict.add(Foam::word("foamFile"), subDict);
		myDict.add(Foam::word("timeFile"), timeDict);

    Foam::IOdictionary ioDict(ioObject, myDict);
		ioDict.add(Foam::word("tester"), Foam::scalar(3.));

    Foam::OStringStream os;
		Foam::OFstream of(
		  Foam::fileName(args.rootPath()+"/"+args.caseName()+"/dictFile")
		);
		
		ioDict.writeData( of );

	}
		
  void openFOAM::apply(void) {  
  }
}


