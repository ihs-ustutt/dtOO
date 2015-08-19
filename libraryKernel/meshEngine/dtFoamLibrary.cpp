#include "dtFoamLibrary.h"

#include <logMe/logMe.h>

#include <meshEngine/dtGmshModel.h>
#include <gmsh/MVertex.h>
#include <gmsh/MElement.h>

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
//          << "Not using gmsh face " << meshF
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
    const ::Foam::cellModel& hex = *(::Foam::cellModeller::lookup("hex"));
    const ::Foam::cellModel& prism = *(::Foam::cellModeller::lookup("prism"));
    const ::Foam::cellModel& pyr = *(::Foam::cellModeller::lookup("pyr"));
    const ::Foam::cellModel& tet = *(::Foam::cellModeller::lookup("tet"));

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
    ::Foam::label nTet = 0;
    ::Foam::label nPyr = 0;
    ::Foam::label nPrism = 0;
    ::Foam::label nHex = 0;


    // From gmsh physical region to Foam patch
    ::Foam::Map< ::Foam::label > physToPatch;

    // From gmsh physical region to Foam cellZone
    ::Foam::Map< ::Foam::label > physToZone;


    int eC = 0;
    dt__forAllRefAuto(allElems, aPair) {
      ::Foam::label elmNumber(eC);
      ::Foam::label elmType(aPair.first->getTypeForMSH());
      ::Foam::label regPhys(aPair.second);

      if (elmType == dtFoamLibrary::MSHTRI) {
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
      << "    tet  :" << nTet 
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
}
