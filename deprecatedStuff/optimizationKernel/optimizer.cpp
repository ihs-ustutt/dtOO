#include "optimizer.h"
#include "interfaceHeaven/optimizationCyclingDecorator.h"
#include "analyticFunctionHeaven/pointCloud.h"
#include "interfaceHeaven/labelHandling.h"
#include "analyticFunctionHeaven/scaPiecewiseLinear.h"
#include <sys/wait.h>

#include <logMe/logMe.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticFunctionHeaven/constValue.h>
#include <analyticFunctionHeaven/scaFunction.h>
#include <analyticFunctionHeaven/scaPiecewiseLinear.h>
#include <boundedVolume.h>
#include <progHelper.h>
#include <interfaceHeaven/optimizationCyclingDecorator.h>

namespace dtOO { 
  void c_optimizer_callback_(void) {
    //
    // call cycle routine
    //
    optimizer().cycle();
  }
      
  DTCLASSLOGMETHODI(optimizer,
          << dt__eval(thisRef._optiCyclingDecorator) << std::endl
          << dt__eval(thisRef._cValP) << std::endl
          << dt__eval(thisRef._optiHandling) );

  optimizationCyclingDecorator * optimizer::_optiCyclingDecorator = NULL;
  std::vector< constValue *> * optimizer::_cValP = new std::vector< constValue *>();
  optimizationHandling * optimizer::_optiHandling = NULL;
  
  optimizer::optimizer() {
  }

  optimizer::optimizer(const optimizer& orig) {
  }

  optimizer::~optimizer() {
  }
  
  void optimizer::optimizeMe( void ) {
    
    //
    // fork process
    //
    int pid = fork();
    if (pid == 0) {
      //
      // child
      //
      FILELog::ReportingLevel() = logERROR;
      FILE* pFile = fopen("optim.log", "w");
      Output2FILE::Stream() = pFile;

      //
      // write steuerfile
      //
      writeOptiStf();

      //
      // start optimizer
      //
      f_optim_1_3_();

      exit(0);
    }
    else if (pid > 0) { 
      //
      // parent
      //
    }
    else {   
      //
      // error
      //
      dt__THROW(optimizeMe(),
             << "Forking not possible " << dt__eval(pid) );
    }
  }
  
  void optimizer::cycle( void ) {
    //
    // check for kill file
    //
    ifstream ifile("kill_optimizer");
    if ( ifile ) {
      remove("kill_optimizer");
      //
      // kill process
      //
      exit(0);
    }
    readOptiParamAndWriteRes();
    
    dt__info(cycle(),
            << "Optimization cycle ... ");
  }
  
  void optimizer::init( 
                    QDomElement const & element,
                    vectorHandling< constValue * > * const cValP,
                    vectorHandling< analyticGeometry * > * const aGeoP,
                    vectorHandling< boundedVolume * > * const bVolP,
                    optimizationCycling * firstOptiCycling
  ) {
    //
    // clear me
    //
    _optiCyclingDecorator = NULL;;
    _cValP->clear();
    _optiHandling = NULL;
    
    //
    // set options
    //
    QDomElement option = getChild("option", element);
    while ( !option.isNull() ) {
      std::string optionName = getAttributeStr("name", option);
      std::string optionValue = getAttributeStr("value", option);
      optionHandling::setOption(optionName, optionValue);
      option = option.nextSiblingElement("option");
    }
    
    //
    // set label of optimizer
    //
    labelHandling::setName( getAttributeStr("label", element ) );
    
    //
    // set scaFuncName
    //
    _scaFuncName = "f_"+getName()+"Designs";
    
    //
    // set constValues
    //
    QDomElement wElement = getChild("constValue", element);
    while ( !wElement.isNull() ) {
      //
      // get label of constValue
      //
      std::string label = getAttributeStr("label", wElement);
      //
      // get constValue
      //      
      constValue * cV = cValP->get(label);
      //
      // store pointer in vector
      //
      _cValP->push_back(cV);
      //
      // go to next element
      //
      wElement = getNextSibling("constValue", wElement);
    }

    //
    // set optimizationCycling
    //
    wElement = getChild("optimizationCycling", element);
    //
    // get label of optimizationHandling
    //
    std::string label = getAttributeStr("label", wElement);
    //
    // get attribute
    //
    std::string attribute = getAttributeStr("attribute", wElement);  
    if (attribute == "boundedVolume") {
      _optiCyclingDecorator = dynamic_cast< optimizationCyclingDecorator * >( bVolP->get(label) );
      if (!_optiCyclingDecorator) {
        dt__THROW(init(),
                << dt__eval(_optiCyclingDecorator) << std::endl
                << dt__eval(label) << " has not correct type.");
      }
    }
    else {
      dt__THROW(init(),
              << "optimizationCyclingDecorator is not optimizeable." << std::endl
              << dt__eval(label) << std::endl
              << dt__eval(attribute) );
    }
    
    //
    // set optimizationHandling
    //
    wElement = getChild("optimizationHandling", element);
    //
    // get label of optimizationHandling
    //
    label = getAttributeStr("label", wElement);
    //
    // get attribute
    //
    attribute = getAttributeStr("attribute", wElement);  
    if (attribute == "boundedVolume") {
      _optiHandling = dynamic_cast< optimizationHandling * >( bVolP->get(label) );
      if (!_optiHandling) {
        dt__THROW(init(),
                << dt__eval(_optiHandling) << std::endl
                << dt__eval(label) << " has not correct type.");
      }
    }
    else {
      dt__THROW(init(),
              << "optimizationHandling is not optimizeable." << std::endl
              << dt__eval(label) << std::endl
              << dt__eval(attribute) );
    }
    
    //
    // set decorator
    //
    _optiCyclingDecorator->enqueDecorator( firstOptiCycling );
    
    dump();
  }
  
  void optimizer::dump(void) {
    DTBUFFERINIT();
    DTBUFFER( << dt__eval(_optiCyclingDecorator) << std::endl);
    DTBUFFER( << dt__eval(_optiHandling) << std::endl);
    for (int ii=0;ii<_cValP->size();ii++) {
      DTBUFFER( << "_cValP[" << ii << "] = " << _cValP->at(ii) << dt__eval(_cValP->at(ii)->getName()) << std::endl);
    }
    dt__debug_BUFFER(dump());
  }
  
  void optimizer::writeOptiStf(void) {
    //
    // create optiStf file
    //
    std::ofstream optiStfFile;
    optiStfFile.open( "opti.stf", std::ofstream::out | std::ofstream::trunc );
    
    //
    // number parameters
    //
    optiStfFile << _cValP->size() << std::endl;
    
    dt__FORALL(*_cValP, ii,
            optiStfFile << _cValP->at(ii)->getMin() << ", " 
                        << _cValP->at(ii)->getMax() << std::endl; 
    );
    
    //
    // options
    //
    optiStfFile << getOptionInt("start_generation") << std::endl;
    optiStfFile << getOptionInt("ind_per_generation") << std::endl;
    optiStfFile << getOptionInt("sur_per_generation") << std::endl;
    optiStfFile << getOptionInt("max_generation") << std::endl;
    optiStfFile << getOptionFloat("convergence_radius") << std::endl;
    
    //
    // restart
    //
    int restart = 0;
    optiStfFile << restart << std::endl;
    
    //
    // close file
    //
    optiStfFile.close();
    
    //5               // Anzahl der Paramter
    //0.5,0.65        // Grenzen (unten, oben) Paramter 1
    //0.6,0.75        // Grenzen (unten, oben) Paramter 2
    //0.3,0.5         // Grenzen (unten, oben) Paramter 3
    //0.4,0.6         // Grenzen (unten, oben) Paramter 4
    //0.5,0.7         // Grenzen (unten, oben) Paramter 5
    //50              // Beeinflusst die Generierung der Startgeneration
    //24              // Individuen pro Generation
    //8               // Anzahl der Überlebenden pro Generation
    //500             // max. Anzahl berechneter Generationen
    //0.0001          // Konvergenzkriterium
    //1               // Neustart der Optimierung bei Abbruch?
    //                //  0 -> nein  : normaler Optimierungslauf
    //                //  1 -> ja    : einlesen der bisherigen Daten und dann weitermachen
    
  }
  
  void optimizer::readOptiParamAndWriteRes(void) {
    //
    // create optiStf file
    //
    std::fstream optiParamFile;
    optiParamFile.open( "opti_para_set.dat", std::fstream::in );    
    std::ofstream optiResFile;
    optiResFile.open( "opti_erg.dat", std::ofstream::out | std::ofstream::trunc );
    int nRuns;
    optiParamFile >> nRuns;
    
    int nParams;
    float paraVal;
    for (int ii=0;ii<nRuns;ii++) {
      float resultValue;            
      //
      //create design
      //
      try {
        optiParamFile >> nParams;
        for (int jj=0;jj<nParams;jj++) {
          optiParamFile >> paraVal;
          _cValP->at(jj)->setValue(paraVal);
        }
        //
        // execute optimizationCycling
        //
        _optiCyclingDecorator->doCycle();

        resultValue = _optiHandling->characterizeMe();
      }
      catch (eGeneral & eGenRef) {
        //
        // design fails
        //
        resultValue = _optiHandling->characterizeFailedDesign();
        
        //
        // clean up
        //
        eGenRef.buryMe();
        
        //
        // output
        //
        DTFORCEWF(readOptiParamAndWriteRes(),
                << "Design " << ii << " fails." << std::endl
                << "Discard this design.");
        
      }
      //
      // clear eGeneral
      //
      eGeneral().clear();
      
      //
      // write result value
      //
      optiResFile << resultValue << std::endl;
    }
    
    optiParamFile.close();
    optiResFile.close();
  }

  void optimizer::readOptiParamAndWriteResPara(void) {
    //
    // create optiStf file
    //
    std::fstream optiParamFile;
    optiParamFile.open( "opti_para_set.dat", std::fstream::in );    
    int nRuns;
    optiParamFile >> nRuns;
    
    std::vector< std::vector<float> > valVec; 
    std::vector< float > resVec; 

    int nParams;
    float paraVal;
    for (int ii=0;ii<nRuns;ii++) {
      valVec.push_back( std::vector<float>() );
      resVec.push_back(0.);
      optiParamFile >> nParams;
      for (int jj=0;jj<nParams;jj++) {
        optiParamFile >> paraVal;
        //_cValP->at(jj)->setValue(paraVal);
        valVec.back().push_back(paraVal);
      }
    }
    optiParamFile.close();

    int status = 0;
    int pid, wpid;
    
    for (int ii=0;ii<valVec.size();ii++) {
      pid = fork();
      if (pid == 0) {
        //
        // child
        //
        for (int jj=0;jj<valVec.at(ii).size();jj++) {
          _cValP->at(jj)->setValue( valVec.at(ii).at(jj) );
        }
        //
        // execute optimizationCycling
        //
        labelHandling * labelH = dynamic_cast< labelHandling * >(_optiHandling);
        if ( labelH == NULL ) {
          dt__THROW(readOptiParamAndWriteResPara(),
                  << dt__eval(labelH) );
        }
        labelH->setName(labelH->getName()+intToString(ii));
        _optiCyclingDecorator->doCycle();
        std::ofstream optiResParFile;
        std::string optiResPar = "res_"+intToString(ii);
        optiResParFile.open( optiResPar.c_str(), std::ofstream::out | std::ofstream::trunc );      
        optiResParFile << _optiHandling->characterizeMe();
        optiResParFile.close();
        
        exit(0);
      }
      else if (pid > 0) {
        //
        // parent
        //
      }
      else {
        //
        // error
        //
        dt__THROW(optimizeMe(),
               << "Forking not possible " << dt__eval(pid) );        
      }
    }

//    //
//    // i did my work, i can die
//    //
//    if (pid == 0) {
//      exit(0);    
//    }
    while (pid = waitpid(-1, NULL, 0)) {
       if (errno == ECHILD) {
          break;
       }
    }
    
    

    std::ofstream optiResFile;
    optiResFile.open( "opti_erg.dat", std::ofstream::out | std::ofstream::trunc );
    
    for (int ii=0;ii<valVec.size();ii++) {
//      optiResFile << resVec[ii] << std::endl;
      std::fstream optiResParFile;
      std::string optiResPar = "res_"+intToString(ii);
      optiResParFile.open( optiResPar.c_str(), std::ofstream::in );      
      float val;
      optiResParFile >> val;
      optiResFile << val << std::endl; //optiResParFile << _optiHandling->characterizeMe();
      optiResParFile.close();      
    }
    
//    float resultValue = _optiHandling->characterizeMe();
//    optiResFile << resultValue << std::endl;
      

    optiResFile.close();
  }
  
  void optimizer::updateToBestDesign(void) {
    //
    // read opti_aktuell.dat file
    //
    std::fstream protFile;
    protFile.open( "opti_aktuell.dat", std::fstream::in );
    
    //
    // get generation number
    //
    int generationNumber;
    protFile >> generationNumber;

    //
    // number of parameters
    //
    int nRed;
    protFile >> nRed;

    //
    // design number
    //    
    int designNumber;
    protFile >> designNumber;
    
    //
    // result value
    //
    float res;
    protFile >> res;
    
    //
    // set current best design
    //
    for(int ii=0;ii<_cValP->size();ii++) {
      float val;
      protFile >> val;
      _cValP->at(ii)->setValue(val);
    }
    
    //
    // close file
    //
    protFile.close();
    
  }

  std::vector< dtPoint2 > optimizer::getDesignsParameter(int const pIndex) {
    //
    // read opti_aktuell.dat file
    //
    std::fstream protFile;
    protFile.open( "Protokoll", std::fstream::in );
    int nIndPGen = getOptionInt("ind_per_generation");
    int nGen = getOptionInt("max_generation");
    int nParams = _cValP->size();
    std::vector< dtPoint2 > tmpPoint;
    
//    try {
    if ( protFile ) {
      for (int jj=0;jj<nGen;jj++) {
        protFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (!protFile ) break;
        for (int ii=0;ii<nIndPGen;ii++) {
          for (int kk=0;kk<(nParams+1);kk++) {
            float yyVal;
            protFile >> yyVal;          
            if (!protFile ) break;
            if ( (ii==0) && (kk==pIndex) ){ 
              tmpPoint.push_back( dtPoint2(jj, yyVal) );
            }
          }
          protFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');          
        }
      }
    }
    
    //
    // close file
    //
    protFile.close();
    
    return tmpPoint;
  }
  
  std::vector< dtPoint2 > optimizer::getDesignsResult(void) {
    return getDesignsParameter(0);
//    //
//    // read opti_aktuell.dat file
//    //
//    std::fstream protFile;
//    protFile.open( "Protokoll", std::fstream::in );
//    int nIndPGen = getOptionInt("ind_per_generation");
//    int nGen = getOptionInt("max_generation");
//    
//    std::vector< dtPoint2 > tmpPoint;
//    
////    try {
//    if ( protFile ) {
//      for (int jj=0;jj<nGen;jj++) {
//        protFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//        if (!protFile ) break;
//        for (int ii=0;ii<nIndPGen;ii++) {
//          float yyVal;
//          protFile >> yyVal;          
//          if (!protFile ) break;
//          if (ii==0){ 
//            tmpPoint.push_back( dtPoint2(jj, yyVal) );
//          }
//          protFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');          
//        }
//      }
//    }
//    
//    //
//    // close file
//    //
//    protFile.close();
//    
//    return tmpPoint;
  }
  
  void optimizer::registrateScaFunctions(vectorHandling< scaFunction * > * sFunP) {
    
    for (int ii=0;ii<(_cValP->size()+1);ii++) {
      std::string name = _scaFuncName+"_"+intToString(ii);
      //
      // check if label is already in vector
      //
      int pos;
      if ( sFunP->has(name) ) {
        pos =  sFunP->getPosition(name);
        delete sFunP->at(pos);
      }
      else {
        scaFunction * dummy;
        sFunP->push_back( dummy );
        pos = sFunP->size()-1;
      }

      sFunP->at(pos) = new scaPiecewiseLinear( getDesignsParameter(ii) );
      sFunP->at(pos)->setName( name );
    }
  }
}
