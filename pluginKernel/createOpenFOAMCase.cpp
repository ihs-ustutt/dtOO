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
#include <gmsh/MTriangle.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/MTetrahedron.h>
#include <gmsh/MHexahedron.h>
#include <gmsh/MPyramid.h>

#include <criticalHeaven/prepareOpenFOAM.h>
#include <argList.H>
#include <Time.H>
#include <IOdictionary.H>
#include <OFstream.H>
#include <IFstream.H>
#include <IOstream.H>
#include <IOobject.H>
#include <polyMesh.H>
#include <cellModeller.H>
#include <repatchPolyTopoChanger.H>
#include <cellSet.H>
#include <faceSet.H>

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
    
    _toWrite 
    = 
    bV->get( 
      dtXmlParser::getAttributeStr(
        "label", qtXmlBase::getChild("boundedVolume", element) 
      )      
    );
  }
  
  void createOpenFOAMCase::initMeshVectors( void ) {
		dt__ptrAss( dtGmshModel * gm, _toWrite->getModel() );
    std::vector< ::GEntity * > entities;
    gm->getEntities(entities);

    //
    // get number of elements and vertices
    //
    int nVerts = 0;
    int nElems = 0;    
    dt__forAllRefAuto(entities, anEntity) {
      nVerts = nVerts + anEntity->getNumMeshVertices();
      nElems = nElems + anEntity->getNumMeshElements();
    }

    //
    // allocate
    //    
    _allVerts.resize(nVerts);
    _allElems.resize(nElems);
    int vC = 0;
    int eC = 0;
    dt__forAllRefAuto(entities, anEntity) {
      dt__forFromToIndex(0, anEntity->getNumMeshVertices(), ii) {
        _allVerts[ vC ] = anEntity->getMeshVertex(ii);
        vC++;
      }
      dt__forFromToIndex(0, anEntity->getNumMeshElements(), ii) {
        _allElems[ eC ] 
        = 
        std::pair< ::MElement * , int >(
          anEntity->getMeshElement(ii), 
          gm->getPhysicalNumber(anEntity->dim(), gm->getPhysicalString(anEntity))
        );
        eC++;
      }
    }    
  }
  
  void createOpenFOAMCase::apply(void) {
		dt__ptrAss( dtGmshModel * gm, _toWrite->getModel() );

    //
    // enable exception throwing
    //
    ::Foam::FatalError.throwExceptions();    
    ::Foam::FatalIOError.throwExceptions();    
		
    ::Foam::argList::noParallel();
		int argc = 3;
		char ** argv = new char*[3];
		argv[0] = const_cast< char * >("antoineInTheBuilding");
		argv[1] = const_cast< char * >("-case");
		argv[2] = const_cast< char * >(_workingDirectory.c_str());
//		Foam::argList args(argc, argv, false, false);

		//
		// check if working directory exists
		//				
		dt__throwIf( !systemHandling::directoryExists(_workingDirectory), init() );

    //
    // read vertices and elements
    //
    initMeshVectors();
      
		//
		// create openFOAM rootCase and time
		//
    try {
      #include "setRootCase.H"
      #include "createTime.H"

      bool keepOrientation = false;//args.optionFound("keepOrientation");

      // Storage for points
      ::Foam::pointField points;
      ::Foam::Map< ::Foam::label > mshToFoam;

      // Storage for all cells.
      ::Foam::cellShapeList cells;

      // Map from patch to gmsh physical region
      ::Foam::labelList patchToPhys;
      // Storage for patch faces.
      ::Foam::List< ::Foam::DynamicList< ::Foam::face > > patchFaces(0);

      // Map from cellZone to gmsh physical region
      ::Foam::labelList zoneToPhys;
      // Storage for cell zones.
      ::Foam::List< ::Foam::DynamicList< ::Foam::label > > zoneCells(0);

      // Name per physical region
      ::Foam::Map< ::Foam::word > physicalNames;


      //
      // read physicals
      //
      dtFoamLibrary::readPhysNames(gm, physicalNames);

      //
      // read vertices
      //
      dtFoamLibrary::readPoints(_allVerts, points, mshToFoam);

      //
      // read elements
      //
      dtFoamLibrary::readCells(
        _allElems, keepOrientation, points, mshToFoam, cells, patchToPhys, 
        patchFaces, zoneToPhys, zoneCells
      );


      ::Foam::label nValidCellZones = 0;

      forAll(zoneCells, zoneI) if (zoneCells[zoneI].size()) nValidCellZones++;


      // Problem is that the orientation of the patchFaces does not have to
      // be consistent with the outwards orientation of the mesh faces. So
      // we have to construct the mesh in two stages:
      // 1. define mesh with all boundary faces in one patch
      // 2. use the read patchFaces to find the corresponding boundary face
      //    and repatch it.


      // Create correct number of patches
      // (but without any faces in it)
      ::Foam::faceListList boundaryFaces(patchFaces.size());

      ::Foam::wordList boundaryPatchNames(boundaryFaces.size());

      forAll(boundaryPatchNames, patchI) {
        ::Foam::label physReg = patchToPhys[patchI];

        ::Foam::Map< ::Foam::word >::const_iterator iter 
        = 
        physicalNames.find(physReg);

        if (iter != physicalNames.end()) boundaryPatchNames[patchI] = iter();
        else {
          boundaryPatchNames[patchI] 
          = 
          ::Foam::word("patch") + ::Foam::name(patchI);
        }
        
        dt__info(
          apply(), 
          << "Patch " << patchI << " gets name " << boundaryPatchNames[patchI]
        );
      }

      ::Foam::wordList boundaryPatchTypes(
        boundaryFaces.size(), ::Foam::polyPatch::typeName
      );
      ::Foam::word defaultFacesName = "defaultFaces";
      ::Foam::word defaultFacesType = ::Foam::polyPatch::typeName;
      ::Foam::wordList boundaryPatchPhysicalTypes(
        boundaryFaces.size(), ::Foam::polyPatch::typeName
      );

      ::Foam::polyMesh mesh(
        ::Foam::IOobject(
          ::Foam::polyMesh::defaultRegion,
          runTime.constant(),
          runTime
        ),
        ::Foam::xferMove(points),
        cells,
        boundaryFaces,
        boundaryPatchNames,
        boundaryPatchTypes,
        defaultFacesName,
        defaultFacesType,
        boundaryPatchPhysicalTypes
      );

      ::Foam::repatchPolyTopoChanger repatcher(mesh);

      // Now use the patchFaces to patch up the outside faces of the mesh.

      // Get the patch for all the outside faces (= default patch added as last)
      const ::Foam::polyPatch& pp 
      = 
      mesh.boundaryMesh()[mesh.boundaryMesh().size()-1];

      // Storage for faceZones.
      ::Foam::List< 
        ::Foam::DynamicList< ::Foam::label > 
      > zoneFaces(patchFaces.size());


      // Go through all the patchFaces and find corresponding face in pp.
      forAll(patchFaces, patchI) {
        const ::Foam::DynamicList< ::Foam::face >& pFaces 
        = 
        patchFaces[patchI];

        dt__info(apply(), << "Finding faces of patch " << patchI);

        forAll(pFaces, i) {
          const ::Foam::face& f = pFaces[i];

          // Find face in pp using all vertices of f.
          ::Foam::label patchFaceI = dtFoamLibrary::findFace(pp, f);

          if (patchFaceI != -1) {
            ::Foam::label meshFaceI = pp.start() + patchFaceI;

            repatcher.changePatchID(meshFaceI, patchI);
          }
          else {
            // Maybe internal face? If so add to faceZone with same index
            // - might be useful.
            ::Foam::label meshFaceI 
            = 
            dtFoamLibrary::findInternalFace(mesh, f);

            if (meshFaceI != -1) zoneFaces[patchI].append(meshFaceI);
            else {
              dt__warning(apply(), 
//                << "Could not match gmsh face " << f
                << " to any of the interior or exterior faces"
                << " that share the same 0th point"
              );
            }
          }
        }
      }

      // Face zones
      ::Foam::label nValidFaceZones = 0;

      forAll(zoneFaces, zoneI) {
        zoneFaces[zoneI].shrink();

        const ::Foam::labelList& zFaces = zoneFaces[zoneI];

        if (zFaces.size()) {
          nValidFaceZones++;

          dt__info(
            apply(), << "zoneFaces = " << zoneI << " size = " << zFaces.size()
          );
        }
      }


      //Get polyMesh to write to constant
      runTime.setTime( ::Foam::instant(runTime.constant()), 0 );

      repatcher.repatch();

      ::Foam::List< ::Foam::cellZone * > cz;
      ::Foam::List< ::Foam::faceZone * > fz;

      // Construct and add the zones. Note that cell ordering does not change
      // because of repatch() and neither does internal faces so we can
      // use the zoneCells/zoneFaces as is.

      if (nValidCellZones > 0) {
        cz.setSize(nValidCellZones);

        nValidCellZones = 0;

        forAll(zoneCells, zoneI) {
          if (zoneCells[zoneI].size()) {
            ::Foam::label physReg = zoneToPhys[zoneI];

            ::Foam::Map< ::Foam::word >::const_iterator iter 
            = 
            physicalNames.find(physReg);

            ::Foam::word zoneName = "cellZone_" + ::Foam::name(zoneI);

            if (iter != physicalNames.end()) zoneName = iter();

            dt__info(
              apply(), 
              << "Writing zone " << zoneI << " to cellZone "
              << zoneName << " and cellSet"
            );

            ::Foam::cellSet cset(
              mesh, zoneName, ::Foam::labelHashSet(zoneCells[zoneI])
            );
            cset.write();

            cz[nValidCellZones] 
            = 
            new ::Foam::cellZone (
              zoneName, zoneCells[zoneI], nValidCellZones, mesh.cellZones()
            );
            nValidCellZones++;
          }
        }
      }

      if (nValidFaceZones > 0) {
        fz.setSize(nValidFaceZones);

        nValidFaceZones = 0;

        forAll(zoneFaces, zoneI) {
          if (zoneFaces[zoneI].size()) {
            ::Foam::label physReg = zoneToPhys[zoneI];

            ::Foam::Map< ::Foam::word >::const_iterator iter 
            = 
            physicalNames.find(physReg);

            ::Foam::word zoneName = "faceZone_" + ::Foam::name(zoneI);

            if (iter != physicalNames.end()) zoneName = iter();

            dt__info(
              apply(), 
              << "Writing zone " << zoneI << " to faceZone "
              << zoneName << " and faceSet"
            );

            ::Foam::faceSet fset(
              mesh, zoneName, ::Foam::labelHashSet(zoneFaces[zoneI])
            );
            fset.write();

            fz[nValidFaceZones] 
            = 
            new ::Foam::faceZone (
              zoneName, 
              zoneFaces[zoneI], 
              ::Foam::boolList(
                zoneFaces[zoneI].size(), true
              ),
              nValidFaceZones,
              mesh.faceZones()
            );

            nValidFaceZones++;
          }
        }
      }

      if (cz.size() || fz.size()) {
        mesh.addZones( ::Foam::List< ::Foam::pointZone * >(0), fz, cz);
      }

      mesh.write();

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
