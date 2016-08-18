#include <stdio.h>

#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParser.h>
#include <interfaceHeaven/vectorHandling.h>
#include <constValueHeaven/constValue.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <dtArg.h>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim_all.hpp>
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

std::vector< std::vector< std::pair< constValue *, float > > > simpleCreate(
  vectorHandling< constValue * > & constValuePtrVec,
  std::vector< int > & nSamples  
) {
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
    dt__forFromToIndex(0, nSamples[jj], ii) {
      samples.push_back( std::vector< std::pair< constValue *, float > >(0) );      
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

  return samples;
}
std::vector< std::vector< std::pair< constValue *, float > > > csvCreate(
  vectorHandling< constValue * > & cV,
  vectorHandling< constValue * > & constValuePtrVec,
  std::string const & filename,
  bool sampleAroundState
) {
  dt__infoNoClass(csvCreate(), << "Create CSV.");
  std::vector< std::vector< std::pair< constValue *, float > > > samples;
  std::ifstream in( filename.c_str() );
  dt__throwIfNoClass(!in, csvCreate());
  
  std::string line;
  //
  // read first line of file
  //
  dt__throwIfNoClass( !getline(in, line), csvCreate() );
  ::boost::algorithm::trim_all(line);
  std::vector< std::string > parts;
  boost::split(
    parts, line, boost::is_any_of(","), boost::token_compress_on
  );
  dt__forAllRefAuto(parts, aConstValue) {
    constValuePtrVec.push_back( cV.get( aConstValue ) );
  }
  
  //
  // read remaining file
  //  
  while ( getline(in, line) ) {
    boost::algorithm::trim_all(line);
    std::vector< std::string > parts;
    boost::split(
      parts, line, boost::is_any_of(","), boost::token_compress_on
    );

    dt__throwIfNoClass( parts.size()!=constValuePtrVec.size(), main() );

    std::vector< std::pair< constValue *, float > > aSample;

    dt__forAllIndex(parts, ii) {
      if ( sampleAroundState ) {
        aSample.push_back(
          std::pair< constValue *, float >(
            constValuePtrVec[ii], 
            cV.get( constValuePtrVec[ii]->getLabel() )->getValue() * stringPrimitive::stringToFloat( parts[ii] )   
          )
        );
      }
      else {
        aSample.push_back(
          std::pair< constValue *, float >(
            constValuePtrVec[ii], 
            constValuePtrVec[ii]->getMin()
            +
            stringPrimitive::stringToFloat( parts[ii] ) 
            * 
            (constValuePtrVec[ii]->getMax()-constValuePtrVec[ii]->getMin())
          )
        );        
      }
    }
    samples.push_back(aSample);
  }
  in.close();   

  return samples;
}

int main( int ac, char* av[] ) {
  try {
    //
    // options
    //
    dtArg vm("createState", ac, av);
    
    //
    // create machine
    //
    vm.setMachine();
    
    //
    // additional arguments
    //
    vm.description().add_options()
      (
        "state", 
        dtPO::value< std::string >()->default_value(""), 
        "parse last state before creation? (optional)"
      )    
      (
        "constValue,c", 
        dtPO::value< std::vector< std::string > >(), 
        "define constValue to modify (required)"
      )
      (
        "nSamples,n", 
        dtPO::value< std::vector< int > >(), 
        "number of samples (optional)"
      )
      (
        "readCsv", 
        dtPO::value< std::string >(), 
        "read from csv file (optional)"
      )    
      (
        "prefix", 
        dtPO::value< std::string >()->default_value("pair"), 
        "define prefix of state label (optional)"
      )
      (
        "sampleAroundState", 
        dtPO::value< bool >()->default_value(false), 
        "put all samples around loaded state (optional)"
      )    
    ;
    vm.update();

    dt__throwIfNoClass( 
      ( vm.count("constValue")&&vm.count("nSamples")&&vm.count("readCsv") )
      ||
      ( !vm.count("constValue")&&!vm.count("nSamples")&&!vm.count("readCsv") )
      ||
      ( vm.count("constValue")&&!vm.count("nSamples")&&vm.count("readCsv") )
      ||
      ( !vm.count("constValue")&&vm.count("nSamples")&&vm.count("readCsv") ), 
      main() 
    );
    
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
    // create constValues
    //
    vectorHandling< constValue * > cV;     
    parser.createConstValue(&cV);

    //
    // load state
    //
    if ( vm["state"].as< std::string >() != "" ) {
      parser.loadStateToConst( vm["state"].as< std::string >(), cV );
    }
    
    //
    // create vector
    //
    vectorHandling< constValue * > constValuePtrVec;

    
    //
    // create samples
    //
    std::vector< std::vector< std::pair< constValue *, float > > > samples;
    if ( vm.count("nSamples") && !vm.count("readCsv") ) {
      dt__forAllRefAuto(
        vm["constValue"].as< std::vector< std::string > >(), aConstValue
      ) {
        constValuePtrVec.push_back( cV.get( aConstValue ) );
      }
    
      std::vector< int > nSamples = vm["nSamples"].as< std::vector< int > >();
      dt__throwIfNoClass( nSamples.size()!=constValuePtrVec.size(), main() );
      samples = simpleCreate( constValuePtrVec, nSamples);
    }
    else if ( !vm.count("nSamples") && vm.count("readCsv") ) {
      samples 
      = 
      csvCreate(
        cV, 
        constValuePtrVec, vm["readCsv"].as< std::string >(), 
        vm["sampleAroundState"].as< bool >() 
      );
    }
    else dt__throwUnexpectedNoClass(main());

    //
    // create state vector
    //
    std::vector< vectorHandling< constValue * > > stateToWrite;
    std::vector< std::string > stateLabel;
    std::string prefix = vm["prefix"].as< std::string >();
    int pairCounter = 0;
    dt__forAllRefAuto(samples, samplesVec) {
      //
      // label
      //
      stateLabel.push_back(
        prefix+"_"+stringPrimitive::intToString(pairCounter) 
      );      
      //
      // set desired values
      //
      dt__forAllRefAuto(samplesVec, samplesVecVec) {
        samplesVecVec.first->setValue( samplesVecVec.second );
      }

      //
      // constValue vector
      //
      stateToWrite.push_back( vectorHandling< constValue * >(0) );      
      dt__forAllRefAuto(cV, aCV) {
        stateToWrite.back().push_back( aCV->clone() );
      }
      
      pairCounter++;
    }
    
    //
    // write states
    //
    parser.write( stateLabel, stateToWrite );
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
