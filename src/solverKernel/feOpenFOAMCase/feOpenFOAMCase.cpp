#include "feOpenFOAMCase.h"

#include <logMe/logMe.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>

#include <xmlHeaven/dtXmlParser.h>
#include <interfaceHeaven/systemHandling.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <meshEngine/dtFoamLibrary.h>
#include <meshEngine/dtGmshModel.h>

#include <gmsh/MVertex.h>
#include <gmsh/MElement.h>

#include <criticalHeaven/prepareOpenFOAM.h>
#include <argList.H>
#include <Time.H>
#include <polyMesh.H>
#include <volFields.H>

#include "feOpenFOAMSetupRule.h"
#include <logMe/dtParMacros.h>
#include "dtCaseFactory.h"
#include <boost/assign.hpp>

namespace dtOO {
  bool feOpenFOAMCase::_registrated 
  =
  dtCaseFactory::registrate(
    dt__tmpPtr(feOpenFOAMCase, new feOpenFOAMCase())
  );

  feOpenFOAMCase::feOpenFOAMCase() {
  }

  feOpenFOAMCase::~feOpenFOAMCase() {
  }
  
  std::vector< std::string > feOpenFOAMCase::factoryAlias( void ) const {
    return ::boost::assign::list_of("OpenFOAMCase");
  }
    
  void feOpenFOAMCase::init( 
    ::QDomElement const & element,
    baseContainer const * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    lvH_dtCase const * const dC
  ) {
    dtCase::init(element, bC, cV, aF, aG, bV, dC);
    
    //
    // hold constValues
    //
    _cV = cV;

    //
    // get noWriteRules
    //
    _noWriteRule
    = 
    qtXmlPrimitive::getAttributeRareStrVector("noWriteRule", element);
      
    //
    // get dict rules
    //
    _dictRule
    = 
    dtXmlParser::replaceDependencies(
      dtXmlParser::getAttributeRareStr("dictRule", element), bC, cV, aF, aG
    );
    
    //
    // get setupRules
    //
    std::vector< std::string > tmpSetupRule 
    = 
    qtXmlPrimitive::getAttributeRareStrVector("setupRule", element);
    
    //
    // prepare setupRules
    //
    dt__forAllIndex(tmpSetupRule, ii) {
      //
      // replace dependencies
      //
      dt__forAllRefAuto(tmpSetupRule, aRule) {
        aRule = dtXmlParser::replaceDependencies(aRule, cV, aF, aG);
      }
      
      //
      // rule string vector
      //
      std::vector< std::string > ruleStrVec 
      =
      stringPrimitive::convertToStringVector(":", ":", tmpSetupRule[ii]);
      dt__throwIf( ruleStrVec.size()<3, init());
      
      //
      // create and init rule
      //
      feOpenFOAMSetupRule * rulePtr = feOpenFOAMSetupRule::create(ruleStrVec[0]);
      rulePtr->init(bC, cV, aF, aG, bV, dC);
      
      //
      // enqueue rule
      //
      _setupRule[ ruleStrVec ].reset( rulePtr );
      
      dt__info(
        init(), 
        << "feOpenFOAMSetupRule[ " << ii << " ] = " << ruleStrVec
      );
    }
    
    //
    // get fieldRules
    //
    std::vector< std::string > tmpFieldRule 
    = 
    qtXmlPrimitive::getAttributeRareStrVector("fieldRule", element);
    
    //
    // prepare fieldRules
    //
    _fieldRule.resize(tmpFieldRule.size());
    dt__forAllIndex(tmpFieldRule, ii) {
      //
      // replace dependencies
      //
      dt__forAllRefAuto(tmpFieldRule, aRule) {
        aRule = dtXmlParser::replaceDependencies(aRule, cV, aF, aG);
      }
      
      _fieldRule[ii] 
      = 
      stringPrimitive::convertToStringVector(":", ":", tmpFieldRule[ii]);
      
      dt__throwIfWithMessage( 
        _fieldRule[ii].size()<3, 
        init(),
        << dt__eval(_fieldRule[ii])
      );
      dt__info(init(), << "_fieldRule[ " << ii << " ] = " << _fieldRule[ii]);
    }
    
    //
    // get boundedVolumes
    //
    std::vector< ::QDomElement > bVChild
    =
    qtXmlBase::getChildVector("boundedVolume", element);
      
    dt__forAllRefAuto(bVChild, aChild) {
      _bV.push_back(
        bV->get( dtXmlParser::getAttributeStr("label", aChild) )
      );
    }
    
    _runCommand = qtXmlPrimitive::getAttributeRareStr("runCommand", element);
    dt__throwIf(_runCommand.empty(), init());
  }
  
  void feOpenFOAMCase::initMeshVectors( 
    std::vector< ::MVertex * > & allVerts, 
    std::vector< std::pair< ::MElement *, dtInt > > & allElems,
    std::map< int, std::string > & physicalNames
  ) {
    // model, dimension, physical number
    std::map< std::tuple< dtGmshModel *, int, dtInt >, dtInt > globLoc;
    dtInt nVerts = 0;
    dtInt nElems = 0;    

    //
    // calculate number of elements and vertices
    //    
    dt__forAllRefAuto(_bV, aBV) {
      dt__ptrAss( dtGmshModel * gm, aBV->getModel() );    
      std::vector< ::GEntity * > entities;
      gm->getEntities(entities);

      dt__forAllRefAuto(entities, anEntity) {
        nVerts = nVerts + anEntity->getNumMeshVertices();
        nElems = nElems + anEntity->getNumMeshElements();
      }
    }
    
    //
    // allocate
    //    
    allVerts.resize(nVerts);
    allElems.resize(nElems);

    // vertex and element counter
    dtInt vC = 0;
    dtInt eC = 0;
    dt__forAllRefAuto(_bV, aBV) {
      dt__ptrAss( dtGmshModel * gm, aBV->getModel() );
      std::vector< ::GEntity * > entities;
      gm->getEntities(entities);
      
      dt__forAllRefAuto(entities, anEntity) {
        //
        // get all mesh vertex
        //
        dt__forFromToIndex(0, anEntity->getNumMeshVertices(), ii) {
          allVerts[ vC ] = anEntity->getMeshVertex(ii);
          vC++;
        }

        //
        // local and global physical number
        //
        dtInt dimension = anEntity->dim();
        dtInt locPhysInt 
        = 
        gm->getPhysicalNumber(
          dimension, gm->getPhysicalString(anEntity)
        );
        dtInt globPhysInt = -1;                
        // check if  it is already in map
        std::map< 
          std::tuple< dtGmshModel *, int, dtInt >, dtInt 
        >::iterator thisPhysical
        =
        globLoc.find( 
          std::tuple< dtGmshModel *, int, dtInt >(gm, dimension, locPhysInt) 
        );
        // not in map
        if ( thisPhysical == globLoc.end() ) {
          globPhysInt = globLoc.size()+1;
          globLoc[ 
            std::tuple< dtGmshModel *, int, dtInt >(gm, dimension, locPhysInt) 
          ]
          =
          globPhysInt; 

          physicalNames[globPhysInt] = gm->getPhysicalString(anEntity);
        }
        // in map
        else globPhysInt = thisPhysical->second;
          
        //
        // get all mesh elements
        // add global physical label for patch creation
        //
        dt__forFromToIndex(0, anEntity->getNumMeshElements(), ii) {
          allElems[ eC ] 
          = 
          std::pair< ::MElement * , dtInt >(
            anEntity->getMeshElement(ii), globPhysInt
          );
          eC++;
        }
      }    
    }
    dt__info( initMeshVectors(), << "physicalNames = " << physicalNames );
  }

  void feOpenFOAMCase::runCurrentState(void) {
    //
    // modify wDir for each run if necessary
    //
    dt__throwIf(_cV->size()==0, runCurrentState());
    std::string dirName = getLabel();
    if ( !_cV->empty()) {
      //
      // check if dtXmlParser is in a state, if not create a new state
      //
      if ( !dtXmlParser::constReference().stateLoaded() ) {
        dtXmlParser::constReference().write(_cV);
      }
      dirName = dirName+"_"+dtXmlParser::constReference().currentState();
    }
    
    //
    // check if already simulated
    //
    if ( inPipeline(dtXmlParser::constReference().currentState()) ) return;
    
    std::string wDir 
    = 
    staticPropertiesHandler::getInstance()->getOption("workingDirectory")
    +
    "/"
    +
    dirName;
    
    dt__info(
      runCurrentState(),
      << dt__eval(dirName) << std::endl
      << dt__eval(wDir)
    );
    
    //
    // check if boundedVolumes already meshed
    //
    dt__forAllRefAuto(_bV, aBV) {
      if ( !aBV->isMeshed() ) {
        aBV->makePreGrid();
        aBV->makeGrid();
      }
    }
    
    //
    // create system folder
    //
    dt__onlyMaster {
      if ( systemHandling::directoryExists(wDir) ) {      
        systemHandling::deleteDirectory(wDir);
      }
      systemHandling::createDirectory(wDir);
      systemHandling::createDirectory(wDir+"/system");
    
      //
      // enable exception throwing
      //
      ::Foam::FatalError.throwExceptions();    
      ::Foam::FatalIOError.throwExceptions();    

      ::Foam::argList::noParallel();
      dtInt argc = 3;
      std::vector< std::string > argvStr(3);
      argvStr[0] = getLabel();
      argvStr[1] = std::string("-case");
      argvStr[2] = wDir;

      char ** argv = new char*[3];
      argv[0] = const_cast< char *>(argvStr[0].c_str());
      argv[1] = const_cast< char *>(argvStr[1].c_str());
      argv[2] = const_cast< char *>(argvStr[2].c_str());

      //
      // check if working directory exists
      //				
      dt__throwIf( !systemHandling::directoryExists(wDir), init() );

      std::vector< ::MVertex * > allVerts;
      std::vector< std::pair< ::MElement *, dtInt > > allElems;
      std::map< int, std::string > physicalNames;

      //
      // read vertices and elements
      //
      initMeshVectors(allVerts, allElems, physicalNames);

      //
      // create feOpenFOAM rootCase and time
      //
      try {
        // disable floating point exception trapping
        dtFoamLibrary::unsetFpe();

        //
        // create rootCase
        //
        ::Foam::argList args(argc, argv);
        if (!args.checkRootCase()) {
          Foam::FatalError.exit();
        }

        //
        // write user dictionaries according to dictRule
        //
        dtFoamLibrary::writeControlDict(wDir, _dictRule);


        dt__info(runCurrentState(), << "Create time");

        ::Foam::Time runTime(
            Foam::Time::controlDictName,
            args.rootPath(),
            args.caseName(),
            "system",
            "constant",
            !args.optionFound("noFunctionObjects")
        );

        //
        // set runTime to timestep constant
        // - write mesh to constant
        //
        runTime.setTime( ::Foam::instant(runTime.constant()), 0 );
        
        //
        // create mesh
        //
        dt__pH(::Foam::polyMesh) mesh( 
          dtFoamLibrary::readMesh(
            allVerts, allElems, physicalNames, runTime, 
            optionHandling::optionTrue("keepOrientation")
          ) 
        );

        //
        // execute rules
        //
        dt__forAllRefAuto(_setupRule, aRulePair) {
          std::vector< std::string > const & aRule = aRulePair.first;
          dt__pH(feOpenFOAMSetupRule) const & exRule = aRulePair.second;        
          
          exRule->executeOnMesh(aRule, *mesh);
        }

        //
        // write mesh
        //
        mesh->write();

        //
        // set runTime to timestep 0
        // - write fields to 0
        //
        runTime.setTime( ::Foam::instant("0"), 1 );            

        //
        // create fvMesh for fields
        //
        ::Foam::fvMesh fvMesh(*mesh);

        std::vector< ::Foam::volVectorField > volVector_;
        std::vector< ::Foam::volScalarField > volScalar;

        dt__forAllRefAuto(_fieldRule, aRule) {
          if (aRule[0] == "volVectorField") {
            ::Foam::IStringStream is(aRule[2]);

            volVector_.push_back(
              ::Foam::volVectorField(
                ::Foam::IOobject(
                  aRule[1],
                  runTime.timeName(),
                  runTime,
                  ::Foam::IOobject::NO_READ,
                  ::Foam::IOobject::AUTO_WRITE
                ),
                fvMesh,
                ::Foam::dimensionedVector(
                  aRule[1], 
                  ::Foam::dimensionSet(::Foam::IStringStream(aRule[2])()), 
                  ::Foam::vector::zero
                )
              )            
            );
            if (aRule.size() == 4) {            
              volVector_.back().internalField() 
              = 
              ::Foam::vector(::Foam::IStringStream(aRule[3])());
            };          
          }
          else if (aRule[0] == "volScalarField") {
            volScalar.push_back(
              ::Foam::volScalarField(
                ::Foam::IOobject(
                  aRule[1],
                  runTime.timeName(),
                  runTime,
                  ::Foam::IOobject::NO_READ,
                  ::Foam::IOobject::AUTO_WRITE
                ),
                fvMesh,
                ::Foam::dimensionedScalar(
                  aRule[1], 
                  ::Foam::dimensionSet(::Foam::IStringStream(aRule[2])()), 
                  ::Foam::scalar(0.)
                )
              )           
            );
            if (aRule.size() == 4) {
              volScalar.back().internalField() 
              = 
              ::Foam::scalar(qtXmlBase::muParseString(aRule[3]));
            };
          }
          else dt__throwUnexpected(runCurrentState());
        };

        //
        // execute rules
        //
        dt__forAllRefAuto(_setupRule, aRulePair) {   
          std::vector< std::string > const & aRule = aRulePair.first;
          dt__pH(feOpenFOAMSetupRule) const & exRule = aRulePair.second;
          
          dt__forAllRefAuto(volVector_, aField) {
            exRule->executeOnVolVectorField(aRule, aField);
          }
          dt__forAllRefAuto(volScalar, aField) {
            exRule->executeOnVolScalarField(aRule, aField);
          }        
        }

        //
        // write all fields
        //
        dt__forAllRefAuto(volVector_, aField) aField.write();
        dt__forAllRefAuto(volScalar, aField) aField.write();

        dtFoamLibrary::writeDicts(fvMesh, wDir, _dictRule, _noWriteRule);

        dt__info(runCurrentState(), << "Done");    
      }
      catch (::Foam::error & err) {
        dt__throw(
          runCurrentState(), 
          << "Instance of ::Foam::error thrown." << std::endl
          << dt__eval(err.what()) << std::endl
          << dt__eval(err.message())
        );
      }
    
      if (!_runCommand.empty()) {
        systemHandling::changeDirectory(wDir);
        systemHandling::command(_runCommand);
        systemHandling::changeDirectory(
          staticPropertiesHandler::getInstance()->getOption("workingDirectory")   
        );
      }

      //
      // destroy pseudo arguments
      //
      delete [] argv;
    }
  }
}
