#include "dtFoamLibrary.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <interfaceHeaven/systemHandling.h>

#include <meshEngine/dtGmshModel.h>
#include <gmsh/MVertex.h>
#include <gmsh/MElement.h>

#include <criticalHeaven/prepareOpenFOAM.h>
#include <Time.H>
#include <repatchPolyTopoChanger.H>
#include <patchToFace.H>
#include <boxToCell.H>
#include <treeBoundBox.H>
#include <SortableList.H>
#include <IStringStream.H>
#include <dictionary.H>
#include <IOdictionary.H>
#include <fvMesh.H>
#include <wordList.H>
#include <sigFpe.H>

#ifdef DTOO_HAS_OpenFOAM
  #include <topoSet.H>
  #include <OFstream.H>
#endif

namespace dtOO {
  dtFoamLibrary::dtFoamLibrary() {
  }

  dtFoamLibrary::~dtFoamLibrary() {
  }

  void dtFoamLibrary::renumber(
    const ::Foam::Map< ::Foam::label >& mshToFoam, ::Foam::labelList& labels
  ) {
    forAll(labels, labelI) labels[labelI] = mshToFoam[labels[labelI]];
  }  
  
  ::Foam::label dtFoamLibrary::findFace(
    const ::Foam::primitivePatch& pp, const ::Foam::labelList& meshF
  ) {
      const ::Foam::Map< ::Foam::label >& meshPointMap = pp.meshPointMap();

      // meshF[0] in pp labels.
      if (!meshPointMap.found(meshF[0]))
      {
//        dt__warning(
//          findFace(),
//          << "Not using gmsh face " << meshF[0]
//          << " since zero vertex is not on boundary of polyMesh"
//        );
        return -1;
      }

      // Find faces using first point
      const ::Foam::labelList& pFaces = pp.pointFaces()[meshPointMap[meshF[0]]];

      // Go through all these faces and check if there is one which uses all of
      // meshF vertices (in any order ;-)
      forAll(pFaces, i) {
        ::Foam::label faceI = pFaces[i];

        const ::Foam::face& f = pp[faceI];

        // Count uses of vertices of meshF for f
        ::Foam::label nMatched = 0;

        forAll(f, fp) if (::Foam::findIndex(meshF, f[fp]) != -1) nMatched++;

        if (nMatched == meshF.size()) return faceI;
      }

      return -1;
  }
  
  ::Foam::label dtFoamLibrary::findInternalFace(
    const ::Foam::primitiveMesh& mesh, const ::Foam::labelList& meshF
  ) {
      const ::Foam::labelList& pFaces = mesh.pointFaces()[meshF[0]];

      forAll(pFaces, i) {
        ::Foam::label faceI = pFaces[i];

        const ::Foam::face& f = mesh.faces()[faceI];

        // Count uses of vertices of meshF for f
        ::Foam::label nMatched = 0;

        forAll(f, fp) if (::Foam::findIndex(meshF, f[fp]) != -1) nMatched++;

        if (nMatched == meshF.size()) return faceI;
      }
      return -1;
  }

  bool dtFoamLibrary::correctOrientation(
    const ::Foam::pointField& points, const ::Foam::cellShape& shape
  ) {
      // Get centre of shape.
      ::Foam::point cc(shape.centre(points));

      // Get outwards pointing faces.
      ::Foam::faceList faces(shape.faces());

      forAll(faces, i)
      {
        const ::Foam::face& f = faces[i];

        ::Foam::vector n(f.normal(points));

        // Check if vector from any point on face to cc points outwards
        if (((points[f[0]] - cc) & n) < 0) {
            // Incorrectly oriented            
            return false;
        }
      }

      return true;
  }  
  
  void dtFoamLibrary::storeCellInZone(
    const ::Foam::label regPhys,
    const ::Foam::label cellI,
    ::Foam::Map< ::Foam::label >& physToZone,
    ::Foam::labelList& zoneToPhys,
    ::Foam::List< ::Foam::DynamicList< ::Foam::label > >& zoneCells
  ) {
      ::Foam::Map< ::Foam::label >::const_iterator zoneFnd 
      = 
      physToZone.find(regPhys);

      if (zoneFnd == physToZone.end())
      {
        // New region. Allocate zone for it.
        ::Foam::label zoneI = zoneCells.size();
        zoneCells.setSize(zoneI+1);
        zoneToPhys.setSize(zoneI+1);

        dt__info(
          storeCellInZone(),
          << "Mapping region " << regPhys << " to Foam cellZone " << zoneI
        );
        physToZone.insert(regPhys, zoneI);

        zoneToPhys[zoneI] = regPhys;
        zoneCells[zoneI].append(cellI);
      }
      else
      {
        // Existing zone for region
        zoneCells[zoneFnd()].append(cellI);
      }
  }

  void dtFoamLibrary::readPhysNames(
    dtGmshModel * gm, ::Foam::Map< ::Foam::word >& physicalNames
  ) {
      //
      // physicals
      //
      dtGmshModel::intGEntityVMap oneD;    
      dtGmshModel::intGEntityVMap twoD;
      dtGmshModel::intGEntityVMap threeD;
      gm->getPhysicalGroups(1, oneD);
      gm->getPhysicalGroups(2, twoD);
      gm->getPhysicalGroups(3, threeD);

      //
      // 1D
      //
      dt__forAllRefAuto(oneD, aPair) {
        physicalNames.insert( 
          ::Foam::scalar(aPair.first), 
          ::Foam::string::validate< ::Foam::word >( 
            gm->getPhysicalName(1, aPair.first) 
          )
        );
        dt__info(readPhysNames(),
          << "Line " << aPair.first << " -> " 
          << ::Foam::string::validate< ::Foam::word >( 
            gm->getPhysicalName(1, aPair.first) 
          ) 
        );      
      }   

      //
      // 2D
      //
      dt__forAllRefAuto(twoD, aPair) {
        physicalNames.insert( 
          ::Foam::scalar(aPair.first), 
          ::Foam::string::validate< ::Foam::word >( 
            gm->getPhysicalName(2, aPair.first) 
          )
        );      
        dt__info(readPhysNames(),
          << "Surface " << aPair.first << " -> " 
          << ::Foam::string::validate< ::Foam::word >( 
            gm->getPhysicalName(2, aPair.first) 
          ) 
        );      
      }

      //
      // 3D
      //
      dt__forAllRefAuto(threeD, aPair) {
        physicalNames.insert( 
          ::Foam::scalar(aPair.first), 
          ::Foam::string::validate< ::Foam::word >( 
            gm->getPhysicalName(3, aPair.first) 
          )
        );      
        dt__info(readPhysNames(),
          << "Region " << aPair.first << " -> " 
          << ::Foam::string::validate< ::Foam::word >( 
            gm->getPhysicalName(3, aPair.first) 
          )
        );      
      }    
  }  

  void dtFoamLibrary::readPoints( 
    std::vector< ::MVertex * > allVerts,  
    ::Foam::pointField& points, ::Foam::Map< ::Foam::label >& mshToFoam
  ) {
    points.setSize( ::Foam::label(allVerts.size()) );
    mshToFoam.resize( 2* ::Foam::label(allVerts.size()) );

    int counter = 0;
    dt__forAllRefAuto(allVerts, mv) {

      ::Foam::point & pt = points[ ::Foam::label(counter) ];

      pt.x() = ::Foam::scalar(mv->x());
      pt.y() = ::Foam::scalar(mv->y());
      pt.z() = ::Foam::scalar(mv->z());

      mv->setIndex(counter+1);
      mshToFoam.insert( ::Foam::label(counter+1), ::Foam::label(counter) );
      counter++;
    }
    dt__info( readPoints(), << "Vertices read: " << mshToFoam.size() );
  }  
  
  void dtFoamLibrary::readCells(
    std::vector< std::pair< ::MElement *, int > > allElems,
    const bool keepOrientation,
    const ::Foam::pointField& points,
    const ::Foam::Map< ::Foam::label >& mshToFoam,
    ::Foam::cellShapeList& cells,
    ::Foam::labelList& patchToPhys,
    ::Foam::List< ::Foam::DynamicList< ::Foam::face > >& patchFaces,
    ::Foam::labelList& zoneToPhys,
    ::Foam::List< ::Foam::DynamicList< ::Foam::label > >& zoneCells
  ) {
#ifdef DTOO_HAS_FOAMEXT
    const ::Foam::cellModel& hex = *(::Foam::cellModeller::lookup("hex"));
    const ::Foam::cellModel& prism = *(::Foam::cellModeller::lookup("prism"));
    const ::Foam::cellModel& pyr = *(::Foam::cellModeller::lookup("pyr"));
    const ::Foam::cellModel& tet = *(::Foam::cellModeller::lookup("tet"));
#endif
#ifdef DTOO_HAS_OpenFOAM    
    const ::Foam::cellModel& hex = *(::Foam::cellModeller().lookup("hex"));
    const ::Foam::cellModel& prism = *(::Foam::cellModeller().lookup("prism"));
    const ::Foam::cellModel& pyr = *(::Foam::cellModeller().lookup("pyr"));
    const ::Foam::cellModel& tet = *(::Foam::cellModeller().lookup("tet"));
#endif    
    ::Foam::face triPoints(3);
    ::Foam::face quadPoints(4);
    ::Foam::labelList tetPoints(4);
    ::Foam::labelList pyrPoints(5);
    ::Foam::labelList prismPoints(6);
    ::Foam::labelList hexPoints(8);

    ::Foam::label nElems(allElems.size());

    // Storage for all cells. Too big. Shrink later
    cells.setSize(nElems);

    ::Foam::label cellI = 0;
    ::Foam::label nLine = 0;
    ::Foam::label nTet = 0;
    ::Foam::label nPyr = 0;
    ::Foam::label nPrism = 0;
    ::Foam::label nHex = 0;
    ::Foam::label nPnt = 0;


    // From gmsh physical region to Foam patch
    ::Foam::Map< ::Foam::label > physToPatch;

    // From gmsh physical region to Foam cellZone
    ::Foam::Map< ::Foam::label > physToZone;


    int eC = 0;
    dt__forAllRefAuto(allElems, aPair) {
      ::Foam::label elmType(aPair.first->getTypeForMSH());
      ::Foam::label regPhys(aPair.second);
      //------------------------------------------------------------------------
      // line (ignore)
      //
      if (elmType == dtFoamLibrary::MSHLINE) {
        ++nLine;
      }
      //------------------------------------------------------------------------
      // triangle
      //
      else if (elmType == dtFoamLibrary::MSHTRI) {
        dt__forFromToIndex(0, 3, ii) {
          triPoints[ii] = aPair.first->getVertex(ii)->getIndex();
        }

        dtFoamLibrary::renumber(mshToFoam, triPoints);

        ::Foam::Map< ::Foam::label >::iterator regFnd 
        = 
        physToPatch.find(regPhys);

        ::Foam::label patchI = -1;
        if (regFnd == physToPatch.end()) {
          // New region. Allocate patch for it.
          patchI = patchFaces.size();

          patchFaces.setSize(patchI + 1);
          patchToPhys.setSize(patchI + 1);

          dt__info(
            readCells(), 
            << "Mapping region " << regPhys << " to Foam patch " << patchI
          );
          physToPatch.insert(regPhys, patchI);
          patchToPhys[patchI] = regPhys;
        }
        // Existing patch for region
        else patchI = regFnd();

        // Add triangle to correct patchFaces.
        patchFaces[patchI].append(triPoints);
      }
      //------------------------------------------------------------------------
      // quadrangle
      //
      else if (elmType == dtFoamLibrary::MSHQUAD) {
        dt__forFromToIndex(0, 4, ii) {
          quadPoints[ii] = aPair.first->getVertex(ii)->getIndex();
        }          

        dtFoamLibrary::renumber(mshToFoam, quadPoints);

        ::Foam::Map< ::Foam::label >::iterator regFnd 
        = 
        physToPatch.find(regPhys);

        ::Foam::label patchI = -1;
        if (regFnd == physToPatch.end()) {
          // New region. Allocate patch for it.
          patchI = patchFaces.size();

          patchFaces.setSize(patchI + 1);
          patchToPhys.setSize(patchI + 1);

          dt__info(
            readCells(), 
            << "Mapping region " << regPhys << " to Foam patch " << patchI
          );
          physToPatch.insert(regPhys, patchI);
          patchToPhys[patchI] = regPhys;
        }
        // Existing patch for region
        else patchI = regFnd();

        // Add quad to correct patchFaces.
        patchFaces[patchI].append(quadPoints);
      }
      //------------------------------------------------------------------------
      // tetrahedra
      //
      else if (elmType == dtFoamLibrary::MSHTET) {
        dtFoamLibrary::storeCellInZone(
          regPhys, cellI, physToZone, zoneToPhys, zoneCells
        );

        dt__forFromToIndex(0, 4, ii) {
          tetPoints[ii] = aPair.first->getVertex(ii)->getIndex();
        }        
        dtFoamLibrary::renumber(mshToFoam, tetPoints);

        cells[cellI++] = ::Foam::cellShape(tet, tetPoints);

        nTet++;   
      }
      //------------------------------------------------------------------------
      // pyramids
      //
      else if (elmType == dtFoamLibrary::MSHPYR) {
        dtFoamLibrary::storeCellInZone(
          regPhys, cellI, physToZone, zoneToPhys, zoneCells
        );

        dt__forFromToIndex(0, 5, ii) {
          pyrPoints[ii] = aPair.first->getVertex(ii)->getIndex();
        }    
        dtFoamLibrary::renumber(mshToFoam, pyrPoints);

        cells[cellI++] = ::Foam::cellShape(pyr, pyrPoints);

        nPyr++;
      }
      //------------------------------------------------------------------------
      // prisms
      //
      else if (elmType == dtFoamLibrary::MSHPRISM) {
        dtFoamLibrary::storeCellInZone(
          regPhys, cellI, physToZone, zoneToPhys, zoneCells
        );

        dt__forFromToIndex(0, 6, ii) {
          prismPoints[ii] = aPair.first->getVertex(ii)->getIndex();
        }        

        dtFoamLibrary::renumber(mshToFoam, prismPoints);

        cells[cellI] = ::Foam::cellShape(prism, prismPoints);

        const ::Foam::cellShape& cell = cells[cellI];

        if (
          !keepOrientation && !dtFoamLibrary::correctOrientation(points, cell)
        ) {
          dt__info(readCells(), << "Inverting prism " << cellI);
          // Reorder prism.
          prismPoints[0] = cell[0];
          prismPoints[1] = cell[2];
          prismPoints[2] = cell[1];
          prismPoints[3] = cell[3];
          prismPoints[4] = cell[4];
          prismPoints[5] = cell[5];

          cells[cellI] = ::Foam::cellShape(prism, prismPoints);
        }

        cellI++;

        nPrism++;
      }
      //------------------------------------------------------------------------
      // hexahedron
      //
      else if (elmType == dtFoamLibrary::MSHHEX) {
        dtFoamLibrary::storeCellInZone(
          regPhys, cellI, physToZone, zoneToPhys, zoneCells
        );
        dt__forFromToIndex(0, 8, ii) {
          hexPoints[ii] = aPair.first->getVertex(ii)->getIndex();
        }                    

        dtFoamLibrary::renumber(mshToFoam, hexPoints);

        cells[cellI] = ::Foam::cellShape(hex, hexPoints);

        const ::Foam::cellShape& cell = cells[cellI];

        if (
          !keepOrientation && !dtFoamLibrary::correctOrientation(points, cell)
        ) {
            dt__info(readCells(), << "Inverting hex " << cellI);
            // Reorder hex.
            hexPoints[0] = cell[4];
            hexPoints[1] = cell[5];
            hexPoints[2] = cell[6];
            hexPoints[3] = cell[7];
            hexPoints[4] = cell[0];
            hexPoints[5] = cell[1];
            hexPoints[6] = cell[2];
            hexPoints[7] = cell[3];

            cells[cellI] = ::Foam::cellShape(hex, hexPoints);
        }

        cellI++;

        nHex++;
      }
      //------------------------------------------------------------------------
      // points (ignore)
      //
      else if (elmType == dtFoamLibrary::MSHPNT) {
        ++nPnt;
      }
      else dt__info( readCells(), << "Unhandled element " << elmType );

      //
      // increment element counter
      //
      eC++;
    }

    cells.setSize(cellI);

    forAll(patchFaces, patchI) patchFaces[patchI].shrink();

    dt__info(
      readCells(),
      << "Cells:" << std::endl
      << "    total:" << cells.size() << std::endl
      << "    hex  :" << nHex << std::endl
      << "    prism:" << nPrism << std::endl
      << "    pyr  :" << nPyr << std::endl
      << "    tet  :" << nTet << std::endl
      << "    line :" << nLine << " (ignored)" << std::endl
      << "    point:" << nPnt << " (ignored)"
    );

    if (cells.size() == 0) dt__throw(readCells(), << "No cells read." );

    forAll(zoneCells, zoneI) {
      zoneCells[zoneI].shrink();

      const ::Foam::labelList& zCells = zoneCells[zoneI];

      if (zCells.size()) {
        dt__info(
          readCells(), << "zoneCells = " << zoneI << " size = " << zCells.size()
        );
      }
    }
  }
  
  ::Foam::polyMesh * dtFoamLibrary::readMesh(
    std::vector< ::MVertex * > allVerts,
    std::vector< std::pair< ::MElement *, int > > allElems,
    std::map< int, std::string > physicalInt,
    ::Foam::Time const & runTime,
    bool keepOrientation
  ) {
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
//      dtFoamLibrary::readPhysNames(gm, physicalNames);
    dt__forAllRefAuto(physicalInt, aPair) {
      if (aPair.second == "") continue;

      physicalNames.insert( 
        ::Foam::label(aPair.first), 
        ::Foam::string::validate< ::Foam::word >(aPair.second)
      );                
    }

    //
    // read vertices
    //
    dtFoamLibrary::readPoints(allVerts, points, mshToFoam);

    //
    // read elements
    //
    dtFoamLibrary::readCells(
      allElems, keepOrientation, points, mshToFoam, cells, patchToPhys, 
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
        readMesh(), 
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

    ::Foam::polyMesh * ptrMesh 
    = 
    new ::Foam::polyMesh
    (
      ::Foam::IOobject(
        ::Foam::polyMesh::defaultRegion,
        runTime.constant(),
        runTime
      ),
#ifdef DTOO_HAS_FOAMEXT            
      ::Foam::xferMove(points),
#endif
#ifdef DTOO_HAS_OpenFOAM
      std::move(points), // https://develop.openfoam.com/Development/openfoam/-/issues/713
#endif            
      cells,
      boundaryFaces,
      boundaryPatchNames,
      boundaryPatchTypes,
      defaultFacesName,
      defaultFacesType,
      boundaryPatchPhysicalTypes
    );
    ::Foam::polyMesh & mesh = *ptrMesh; 

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

//      dt__info(readMesh(), << "Finding faces of patch " << patchI);

      forAll(pFaces, i) {
        const ::Foam::face& f = pFaces[i];

        // Find face in pp using all vertices of f.
        ::Foam::label patchFaceI = dtFoamLibrary::findFace(pp, f);

        if (patchFaceI != -1) {
//          dt__info(readMesh(), << "Correspoding patchFace " << patchFaceI);
          ::Foam::label meshFaceI = pp.start() + patchFaceI;

          repatcher.changePatchID(meshFaceI, patchI);
        }
        else {
          // Maybe internal face? If so add to faceZone with same index
          // - might be useful.
          ::Foam::label meshFaceI 
          = 
          dtFoamLibrary::findInternalFace(mesh, f);

          if (meshFaceI != -1) {
//            dt__info(readMesh(), << "Internal face: meshFaceI = " << meshFaceI);
            zoneFaces[patchI].append(meshFaceI);
          }
          else {
            dt__warning(
              readMesh(), 
              << "Internal face: meshFaceI = -1" << std::endl            
              << "Could not match gmsh face "
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
          readMesh(), << "zoneFaces = " << zoneI << " size = " << zFaces.size()
        );
      }
    }

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
            readMesh(), 
            << "Writing zone " << zoneI << " to cellZone "
            << zoneName << " and cellSet"
          );

          ::Foam::cellSet cset(
            mesh, zoneName, ::Foam::labelHashSet(zoneCells[zoneI])
          );
          cset.write();

          cz[nValidCellZones] 
          = 
          new ::Foam::cellZone(
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
          ::Foam::word zoneName = "faceZone_" + ::Foam::name(zoneI);
          if ( ! boundaryPatchNames[zoneI].empty() ) {
            zoneName = boundaryPatchNames[zoneI];
          }
          dt__info(
            readMesh(), 
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

    return ptrMesh;
  }

  void dtFoamLibrary::patchToFaceZoneNoFlipMap(
    ::Foam::polyPatch const & patch, 
    ::Foam::word const & faceZone, 
    ::Foam::polyMesh & mesh
  ) {
#ifdef DTOO_HAS_FOAMEXT               
    ::Foam::topoSet tS(
      mesh, 
      "faceSet", 
      faceZone,
      ::Foam::IOobject::READ_IF_PRESENT,
      ::Foam::IOobject::AUTO_WRITE
    );
    
    ::Foam::patchToFace(
      mesh, patch.name()
    ).applyToSet(
      ::Foam::topoSetSource::ADD, tS
    );
#endif
#ifdef DTOO_HAS_OpenFOAM
    ::Foam::faceSet tS(
      mesh, 
      faceZone,
      ::Foam::IOobject::READ_IF_PRESENT,
      ::Foam::IOobject::AUTO_WRITE
    );
    
    ::Foam::patchToFace(
      mesh, ::Foam::wordRe(patch.name())
    ).applyToSet(
      ::Foam::topoSetSource::ADD, tS
    );
#endif 
    
    ::Foam::SortableList< ::Foam::label > faceLabels(tS.toc());

    ::Foam::DynamicList< ::Foam::label > addressing(tS.size());
    ::Foam::DynamicList< bool > flipMap(tS.size());

    forAll(faceLabels, i) {
     ::Foam::label faceI = faceLabels[i];
     addressing.append(faceI);
     flipMap.append(false);
    }
    
    ::Foam::label zoneID = mesh.faceZones().findZoneID(tS.name());
    if (zoneID == -1) {
      dt__info(
        patchToFaceZoneNoFlipMap(), 
        << "Adding set " << tS.name() << " as a faceZone."
      );
      ::Foam::label sz = mesh.faceZones().size();
      mesh.faceZones().setSize(sz+1);
      mesh.faceZones().set(
        sz, 
        new ::Foam::faceZone(
          tS.name(), addressing.shrink(), flipMap.shrink(), sz, mesh.faceZones()
        )
      );
      mesh.faceZones().writeOpt() = ::Foam::IOobject::AUTO_WRITE;
      mesh.faceZones().instance() = mesh.facesInstance();
    }
    else {
      dt__info(patchToFaceZoneNoFlipMap(),
        << "Overwriting contents of existing faceZone " << zoneID
        << " with that of set " << tS.name() << "."
      );
      mesh.faceZones()[zoneID].resetAddressing(
        addressing.shrink(), flipMap.shrink()
      );
      mesh.faceZones().writeOpt() = ::Foam::IOobject::AUTO_WRITE;
      mesh.faceZones().instance() = mesh.facesInstance();
    }    
  }
   
  void dtFoamLibrary::boxToCellZone(
    ::Foam::point const & min, 
    ::Foam::point const & max,
    ::Foam::word const & cellZone,
    ::Foam::polyMesh & mesh
  ) {
#ifdef DTOO_HAS_FOAMEXT     
    ::Foam::topoSet tS(
      mesh, 
      "cellSet", 
      cellZone,
      ::Foam::IOobject::READ_IF_PRESENT,
      ::Foam::IOobject::AUTO_WRITE
    );
    
    ::Foam::boxToCell(
      mesh, ::Foam::treeBoundBox(min, max)
    ).applyToSet(
      ::Foam::topoSetSource::ADD, tS
    );
#endif
#ifdef DTOO_HAS_OpenFOAM
    ::Foam::cellSet tS(
      mesh, 
      cellZone,
      ::Foam::IOobject::READ_IF_PRESENT,
      ::Foam::IOobject::AUTO_WRITE
    );
    
    ::Foam::boxToCell(
      mesh, ::Foam::treeBoundBoxList( 1, ::Foam::treeBoundBox(min, max) )
    ).applyToSet(
      ::Foam::topoSetSource::ADD, tS
    );
#endif    
    ::Foam::SortableList< ::Foam::label > cellLabels(tS.toc());

    ::Foam::DynamicList< ::Foam::label > addressing(tS.size());

    forAll(cellLabels, i) {
     ::Foam::label faceI = cellLabels[i];
     addressing.append(faceI);
    }
    
    ::Foam::label zoneID = mesh.cellZones().findZoneID(tS.name());
    if (zoneID == -1) {
      dt__info(
        boxToCellSet(), 
        << "Adding set " << tS.name() << " as a cellZone."
      );
      ::Foam::label sz = mesh.cellZones().size();
      mesh.cellZones().setSize(sz+1);
      mesh.cellZones().set(
        sz, 
        new ::Foam::cellZone(
          tS.name(), addressing.shrink(), sz, mesh.cellZones()
        )
      );
      mesh.cellZones().writeOpt() = ::Foam::IOobject::AUTO_WRITE;
    }
    else {
      dt__info(boxToCellSet(),
        << "Overwriting contents of existing cellZone " << zoneID
        << " with that of set " << tS.name() << "."
      );
      mesh.cellZones().writeOpt() = ::Foam::IOobject::AUTO_WRITE;
    }        
  }    
    
  void dtFoamLibrary::writeControlDict(
    std::string const & workingDirectory, std::string const & content
  ) {
    //
    // create content dict
    //
    ::Foam::IStringStream is(content);
    ::Foam::dictionary contentDict( is() );
    
    //
    // create controlDict
    //    
    ::Foam::dictionary currentDict = contentDict.subOrEmptyDict("controlDict");    
    dt__throwIf(currentDict.empty(), writeControlDict()); 

    ::Foam::fileName fn 
    = 
#ifdef DTOO_HAS_FOAMEXT            
    currentDict.lookupOrDefault< ::Foam::fileName >("dtOOFileName", "", false, "");
#endif
#ifdef DTOO_HAS_OpenFOAM
    currentDict.getOrDefault(
      "dtOOFileName", ::Foam::fileName(""), ::Foam::keyType::option::LITERAL
    );    
#endif
    dt__throwIf(fn.empty(), writeControlDict());
    ::Foam::OFstream os(workingDirectory/fn);

    dt__info(
      writeControlDict(),
      << "Create controlDict:" << std::endl
      << dt__eval( ::Foam::fileName(workingDirectory) ) << std::endl
      << dt__eval( ::Foam::fileName(workingDirectory).name() )
    );

    //
    // pseudo time for writing
    //
    ::Foam::Time theTime(
        contentDict.subDict("controlDict"),
        ::Foam::fileName(workingDirectory),
        ::Foam::fileName(workingDirectory).name(),
        "system",
        "constant"
    );
    
#ifdef DTOO_HAS_FOAMEXT            
      theTime.controlDict().writeHeader(os());
#endif
#ifdef DTOO_HAS_OpenFOAM
      ::Foam::IOobject io("controlDict","system", theTime);
      io.writeHeader(os(), "dictionary");
#endif
      
    forAllConstIter(
      ::Foam::dictionary, contentDict.subDict("controlDict"), it
    ) {
      it().write( os() );
      os.endl();
    }
    os.flush();    
    
    //
    // make sure that file is written
    //
    bool isWritten = false;
    dt__forFromToIndex(1, 10, ii) {
      if ( systemHandling::fileExists( workingDirectory/fn ) ) {
        isWritten = true;
        break;
      }
      dt__warning(writeDicts(), << "File not written. Sleep 1 s.");
      sleep(1);
    }
    dt__throwIf(!isWritten, writeDicts());    
  }

  void dtFoamLibrary::writeDicts( 
    ::Foam::fvMesh & fvMesh, 
    std::string const & workingDirectory, 
    std::string const & content,
    std::vector< std::string > const & noWriteRule
  ) {
    //
    // create content dict
    //
    ::Foam::IStringStream is(content);
    ::Foam::dictionary contentDict( is() );
    
    ::Foam::wordList toc = contentDict.toc();
    
    forAll(toc, i) {
      if ( 
        (toc[i] != "controlDict") 
        && 
        !contentDict.subOrEmptyDict(toc[i]).empty() 
      ) {
        ::Foam::dictionary currentDict = contentDict.subOrEmptyDict(toc[i]);
        
        //
        // create output stream
        //
        ::Foam::fileName fn 
        = 
#ifdef DTOO_HAS_FOAMEXT                
        currentDict.lookupOrDefault< ::Foam::fileName >(
          "dtOOFileName", "", false, ""
        );
#endif
#ifdef DTOO_HAS_OpenFOAM
        currentDict.getOrDefault(
          "dtOOFileName", ::Foam::fileName(""), ::Foam::keyType::option::LITERAL
        );
#endif        
        dt__throwIf(fn.empty(), writeDicts());
        ::Foam::OFstream os(workingDirectory/fn);

        //
        // create IODictionary
        //
        ::Foam::IOdictionary ioDict(
          ::Foam::IOobject(
            fn.name(),
            fn.path(),
            fvMesh,
            ::Foam::IOobject::NO_READ,
            ::Foam::IOobject::AUTO_WRITE
          )
        );

        dt__info(
          writeDicts(),
          << "Create " << toc[i] << std::endl
          << dt__eval( ioDict.filePath() ) << std::endl
          << dt__eval( ioDict.caseName() )
        );
        
        //
        // write header and add current dictionary
        //
        ioDict.writeHeader(os());
        forAllConstIter(::Foam::dictionary, currentDict, it) {
          if ( 
            stringPrimitive::stringContains("noWriteRule", it().keyword()) 
          ) {
            ::Foam::label pos = ::Foam::readLabel(it().stream());
            os() << noWriteRule[ pos ].c_str();
            os.endl();
          }
          else {
            it().write( os() );
            os.endl();
          }
        }
        os.flush();
        
        //
        // make sure that file is written
        //
        bool isWritten = false;
        dt__forFromToIndex(1, 10, ii) {
          if ( systemHandling::fileExists( workingDirectory/fn ) ) {
            isWritten = true;
            break;
          }
          dt__warning(writeDicts(), << "File not written. Sleep 1 s.");
          sleep(1);
        }
        dt__throwIf(!isWritten, writeDicts());
      }
    }
  }
  
  dt__pH(::Foam::argList) dtFoamLibrary::initCase(
    std::string const & appName, std::string const & wDir
  ) {
    //
    // enable exception throwing
    //
    ::Foam::FatalError.throwExceptions();    
    ::Foam::FatalIOError.throwExceptions();    

    //
    // argList
    //
    ::Foam::argList::noParallel();
    int argc = 3;
    std::vector< std::string > argvStr(3);
    argvStr[0] = appName;
    argvStr[1] = std::string("-case");
    argvStr[2] = wDir;
    char ** argv = new char*[3];
    argv[0] = const_cast< char *>(argvStr[0].c_str());
    argv[1] = const_cast< char *>(argvStr[1].c_str());
    argv[2] = const_cast< char *>(argvStr[2].c_str());    

    // disable floating point exception trapping
    ::Foam::sigFpe::unset(true);

    //
    // create rootCase
    //
    ptrHandling<::Foam::argList> args( new ::Foam::argList(argc, argv) );
    if (!args->checkRootCase()) {
      Foam::FatalError.exit();
    }      
    
    return args;
  }  
  
  void dtFoamLibrary::unsetFpe( void ) {
#ifdef DTOO_HAS_FOAMEXT                
    systemHandling::unsetEnv("FOAM_SIGFPE");
    ::Foam::set(true)
#endif
#ifdef DTOO_HAS_OpenFOAM
    ::Foam::sigFpe::unset(true);
#endif
  }
}
