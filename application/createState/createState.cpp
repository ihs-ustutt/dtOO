#include <stdio.h>

#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParser.h>
#include <interfaceHeaven/vectorHandling.h>
#include <constValueHeaven/constValue.h>
#include <interfaceHeaven/stringPrimitive.h>

#include <boost/program_options.hpp>
namespace po = boost::program_options;
#include <iostream>

using namespace dtOO;

//
// recursive combination creation
//
template< typename T > 
void combinations( 
  std::vector< std::vector< T > > array, 
  int i, 
  std::vector< T > accum, 
  std::vector< std::vector< T > > & comb
) {
  if (i == array.size()) // done, no more rows
  {
    comb.push_back(accum); // assuming comb is global
  }
  else
  {
    std::vector< T > row = array[i];
    for(int j = 0; j < row.size(); ++j)
    {
      std::vector< T > tmp(accum);
      tmp.push_back(row[j]);
      combinations(array, i+1, tmp, comb);
    }
  }
}

int main( int ac, char* av[] ) {
  try {
    //
    // options
    //
    po::options_description desc("Allowed options");
    desc.add_options()
      ("help", "produce help message")
      ("xmlIn", po::value<std::string>(), "set input xml file (required)")
      ("xmlOut", po::value<std::string>(), "set output xml file (required)")
      (
        "constValue,c", 
        po::value< std::vector< std::string > >(), 
        "define constValue to modify (required)"
      )
      (
        "nSamples,n", 
        po::value< std::vector< int > >(), 
        "number of samples (required)"
      )
      (
        "prefix", 
        po::value< std::string >()->default_value("pair"), 
        "define prefix of state label (optional)"
      )    
      ("log", po::value<std::string>(), "define logfile (required)")    
    ;
    po::variables_map vm;        
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);  

    if (
      vm.count("help")
      ||
      !vm.count("xmlIn") || !vm.count("xmlOut") 
      || 
      !vm.count("constValue") || !vm.count("nSamples")
      || 
      !vm.count("log")
    ) {
      std::cout << desc << std::endl;
      return 0;
    }
    
    //
    // create log files
    //
    FILELog::ReportingLevel() = logDEBUG;
    Output2FILE::Stream().open( 
      vm["log"].as<std::string>(), 
      std::ofstream::out | std::ofstream::trunc 
    );	

    //
    // vectorHandling of constValues
    //
    vectorHandling< constValue * > cV; 
    
    //
    // create parser and parse
    //
    dtXmlParser parser(
      vm["xmlIn"].as<std::string>(), vm["xmlOut"].as<std::string>()
    );
    parser.parse();
    parser.load();

    //
    // create constValues
    //
    parser.createConstValue(&cV);
    
    std::vector< std::string > constValueString 
    = 
    vm["constValue"].as< std::vector< std::string > >();
    std::vector< int > nSamples = vm["nSamples"].as< std::vector< int > >();
    if (nSamples.size()!=constValueString.size()) {
      dt__throwNoClass(main(), 
        <<  "nSamples = " << nSamples << std::endl
        << "constValueString = " << constValueString 
      );
    }
    
    //
    // create vector
    //
    vectorHandling< constValue * > constValuePtrVec;
    dt__forAllRefAuto(constValueString, aConstValue) {
      constValuePtrVec.push_back( cV.get( aConstValue ) );
    }
      
    //
    // create samples
    //
    std::vector< std::vector< std::pair< constValue *, float > > > samples;
    dt__forAllIndex(constValuePtrVec, jj) {
      //
      // get desired constValue
      //
      constValue * thisCV = constValuePtrVec[jj];
      
      //
      // convert int to float
      float nSamplesF = static_cast< float >(nSamples[jj]);
      float step = (thisCV->getMax() - thisCV->getMin()) / (nSamplesF-1.);
      
      //
      // store samples
      //
      samples.push_back( std::vector< std::pair< constValue *, float > >(0) );
      dt__forFromToIndex(0, nSamples[jj], ii) {
        float iiF = static_cast< float >(ii);
        
        samples.back().push_back(
          std::pair< constValue *, float >(
            thisCV, thisCV->getMin() + iiF * step 
          )
        );
      }
    }
    
    //
    // create combinations
    //
    std::vector< std::vector< std::pair< constValue *, float > > > out;
    std::vector< std::pair< constValue *, float > > accum;
    int counter = 0;
    combinations(samples, counter, accum, out);
    
    //
    // create states
    //
    std::string prefix = vm["prefix"].as< std::string >();
    int pairCounter = 0;
    dt__forAllRefAuto(out, outVec) {
      dt__forAllRefAuto(outVec, outVecVec) {
        outVecVec.first->setValue( outVecVec.second );
      } 
      parser.write(
        prefix+"_"+stringPrimitive::intToString(pairCounter), 
        constValuePtrVec
      );
      pairCounter++;
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
