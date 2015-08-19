#include "createOpenFOAMCase.h"

#include <logMe/logMe.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>

#include <xmlHeaven/dtXmlParser.h>
#include <interfaceHeaven/systemHandling.h>
#include <meshEngine/dtFoamLibrary.h>
#include <meshEngine/dtGmshModel.h>

#include <gmsh/MVertex.h>
#include <gmsh/MElement.h>

#include <criticalHeaven/prepareOpenFOAM.h>
#include <argList.H>
#include <Time.H>
#include <polyMesh.H>

namespace dtOO { 
  createOpenFOAMCase::createOpenFOAMCase() {
  }

  createOpenFOAMCase::~createOpenFOAMCase() {
  }
  
	void createOpenFOAMCase::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG,
		vectorHandling< boundedVolume * > const * const bV,
		vectorHandling< dtPlugin * > const * const pL
	) {
	  dtPlugin::init(element, bC, cV, aF, aG, bV, pL);
    
    _workingDirectory
    = 
    dtXmlParser::getAttributeStr("workingDirectory", element);
    
    std::vector< ::QDomElement > bVChild
    =
    qtXmlBase::getChildVector("boundedVolume", element);
      
    dt__forAllRefAuto(bVChild, aChild) {
      _bV.push_back(
        bV->get( dtXmlParser::getAttributeStr("label", aChild) )
      );
    }
  }
  
  void createOpenFOAMCase::initMeshVectors( 
    std::vector< ::MVertex * > & allVerts, 
    std::vector< std::pair< ::MElement *, int > > & allElems,
    std::map< int, std::string > & physicalNames
  ) {
    std::map< std::pair< dtGmshModel *, int >, int > globLoc;
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
        int locPhysInt 
        = 
        gm->getPhysicalNumber(
          anEntity->dim(), gm->getPhysicalString(anEntity)
        );
        int globPhysInt = -1;                
        // check if  it is already in map
        std::map< std::pair< dtGmshModel *, int >, int >::iterator thisPhysical
        =
        globLoc.find( std::pair< dtGmshModel *, int >(gm, locPhysInt) );
        // not in map
        if ( thisPhysical == globLoc.end() ) {
          globPhysInt = globLoc.size()+1;
          globLoc[ std::pair< dtGmshModel *, int >(gm, locPhysInt) ]
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
  }
  
  void createOpenFOAMCase::apply(void) {
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
    // enable exception throwing
    //
    ::Foam::FatalError.throwExceptions();    
    ::Foam::FatalIOError.throwExceptions();    
		
    ::Foam::argList::noParallel();
		int argc = 3;
		char ** argv = new char*[3];
		argv[0] = const_cast< char * >(getLabel().c_str());
		argv[1] = const_cast< char * >("-case");
		argv[2] = const_cast< char * >(_workingDirectory.c_str());

		//
		// check if working directory exists
		//				
		dt__throwIf( !systemHandling::directoryExists(_workingDirectory), init() );

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
      #include "setRootCase.H"
      #include "createTime.H"

      //Get polyMesh to write to constant
      runTime.setTime( ::Foam::instant(runTime.constant()), 0 );
      
      dt__pH(::Foam::polyMesh) mesh( 
        dtFoamLibrary::readMesh(allVerts, allElems, physicalNames, runTime) 
      );
      mesh->write();

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
  }    
}
