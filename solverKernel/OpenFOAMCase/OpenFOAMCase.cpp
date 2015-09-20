#include "OpenFOAMCase.h"

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

#include "openFOAMSetupRule.h"

namespace dtOO { 
  OpenFOAMCase::OpenFOAMCase() {
  }

  OpenFOAMCase::~OpenFOAMCase() {
  }
  
	void OpenFOAMCase::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG,
		vectorHandling< boundedVolume * > const * const bV,
		vectorHandling< dtCase * > const * const dC
	) {
	  dtCase::init(element, bC, cV, aF, aG, bV, dC);
    
    //
    // get working directory and cVTag if available
    //
    _workingDirectoryPattern
    = 
    dtXmlParser::getAttributeStr("workingDirectory", element);
    if ( stringPrimitive::stringContains("#", _workingDirectoryPattern) ) {
      std::vector< std::string > cVLabels 
      = 
      stringPrimitive::convertToStringVector(
        "#", "#", _workingDirectoryPattern
      );
      
      dt__forAllRefAuto(cVLabels, aLabel) _cVTag.push_back( cV->get(aLabel) );
    }
   
    _dictRule
    = 
    dtXmlParser::getAttributeRareStr("dictRule", element);    
    
    //
    // get setupRules
    //
    std::vector< std::string > tmpSetupRule 
    = 
    qtXmlPrimitive::getAttributeRareStrVector("setupRule", element);
    
    //
    // prepare setupRules
    //
    _setupRule.resize(tmpSetupRule.size());
    dt__forAllIndex(tmpSetupRule, ii) {
      //
      // replace dependencies
      //
      dt__forAllRefAuto(tmpSetupRule, aRule) {
        aRule = dtXmlParser::replaceDependencies(aRule, cV, aF, aG);
      }
      
      _setupRule[ii] 
      = 
      stringPrimitive::convertToStringVector(":", ":", tmpSetupRule[ii]);
      
      dt__throwIf( _setupRule[ii].size()<3, init());
      
      dt__info(init(), << "_setupRule[ " << ii << " ] = " << _setupRule[ii]);
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
//      //
//      // replace dependencies
//      //
//      dt__forAllRefAuto(tmpFieldRule, aRule) {
//        aRule = dtXmlParser::replaceDependencies(aRule, cV, aF, aG);
//      }
      
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
  }
  
  void OpenFOAMCase::initMeshVectors( 
    std::vector< ::MVertex * > & allVerts, 
    std::vector< std::pair< ::MElement *, int > > & allElems,
    std::map< int, std::string > & physicalNames
  ) {
    // model, dimension, physical number
    std::map< std::tuple< dtGmshModel *, int, int >, int > globLoc;
    int nVerts = 0;
    int nElems = 0;    

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
    int vC = 0;
    int eC = 0;
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
        int dimension = anEntity->dim();
        int locPhysInt 
        = 
        gm->getPhysicalNumber(
          dimension, gm->getPhysicalString(anEntity)
        );
        int globPhysInt = -1;                
        // check if  it is already in map
        std::map< 
          std::tuple< dtGmshModel *, int, int >, int 
        >::iterator thisPhysical
        =
        globLoc.find( 
          std::tuple< dtGmshModel *, int, int >(gm, dimension, locPhysInt) 
        );
        // not in map
        if ( thisPhysical == globLoc.end() ) {
          globPhysInt = globLoc.size()+1;
          globLoc[ 
            std::tuple< dtGmshModel *, int, int >(gm, dimension, locPhysInt) 
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
          std::pair< ::MElement * , int >(
            anEntity->getMeshElement(ii), globPhysInt
          );
          eC++;
        }
      }    
    }
    dt__info( initMeshVectors(), << "physicalNames = " << physicalNames );
  }
  
  void OpenFOAMCase::apply(void) {
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
    // modify wDir for each run if necessary
    //
    std::string wDir = _workingDirectoryPattern;
    if (!_cVTag.empty()) wDir = dtXmlParser::replaceDependencies(wDir, &_cVTag);
    
    dt__info(
      apply(),
      << dt__eval(_workingDirectoryPattern) << std::endl
      << dt__eval(_cVTag) << std::endl
      << dt__eval(wDir)
    );
    
    //
    // create system folder
    //
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
		int argc = 3;
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
    std::vector< std::pair< ::MElement *, int > > allElems;
    std::map< int, std::string > physicalNames;
    
    //
    // read vertices and elements
    //
    initMeshVectors(allVerts, allElems, physicalNames);
      
		//
		// create openFOAM rootCase and time
		//
    try {
      // disable floating point exception trapping
      systemHandling::unsetEnv("FOAM_SIGFPE");
      
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

    
      dt__info(apply(), << "Create time");

      ::Foam::Time runTime(
          Foam::Time::controlDictName,
          args.rootPath(),
          args.caseName(),
          "system",
          "constant",
          !args.optionFound("noFunctionObjects")
      );
    
      //
      // create mesh
      //
      dt__pH(::Foam::polyMesh) mesh( 
        dtFoamLibrary::readMesh(allVerts, allElems, physicalNames, runTime) 
      );
    
      //
      // execute rules
      //
      dt__forAllRefAuto(_setupRule, aRule) {
        dt__pH(openFOAMSetupRule) exRule(
          openFOAMSetupRule::create( aRule[0] )
        );
        exRule->executeOnMesh(aRule, *mesh);
      }

      //
      // set runTime to timestep consant
      // - write mesh to constant
      //
      runTime.setTime( ::Foam::instant(runTime.constant()), 0 );
      
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
        else dt__throwUnexpected(apply());
      };

      //
      // execute rules
      //
      dt__forAllRefAuto(_setupRule, aRule) {   
        dt__pH(openFOAMSetupRule) exRule(
          openFOAMSetupRule::create( aRule[0] )
        );
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
      
      dtFoamLibrary::writeDicts(fvMesh, wDir, _dictRule);
      
      dt__info(apply(), << "Done");    
    }
    catch (::Foam::error & err) {
      dt__throw(
        apply(), 
        << "Instance of ::Foam::error thrown." << std::endl
        << dt__eval(err.what()) << std::endl
        << dt__eval(err.message())
      );
    }
    
    if (!_runCommand.empty()) {
      systemHandling::command(
        "cd "+wDir
        +
        " && "
        +
        _runCommand
        +
        " && "
        +
        "cd "
        +
        staticPropertiesHandler::getInstance()->getOption("workingDirectory")    
      );
    }
    
    //
    // destroy pseudo arguments
    //
    delete [] argv;
  }    
}
