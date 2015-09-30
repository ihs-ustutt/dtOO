#ifndef DTFOAMLIBRARY_H
#define	DTFOAMLIBRARY_H

#include <logMe/dtMacros.h>
#include <map>

#include <criticalHeaven/prepareOpenFOAM.h>
#include <polyMesh.H>
#include <cellModeller.H>
#include <cellSet.H>
#include <faceSet.H>
#include <fvMesh.H>

class MVertex;
class MElement;

namespace dtOO {
  class dtGmshModel;
  
  class dtFoamLibrary {
  public:
    dt__classOnlyName(dtFoamLibrary);
    static ::Foam::polyMesh * readMesh(     
      std::vector< ::MVertex * > allVerts,
      std::vector< std::pair< ::MElement *, int > > allElems,
      std::map< int, std::string > physicalInt,
      ::Foam::Time const & runTime
    ); 
    static void patchToFaceZoneNoFlipMap(
      ::Foam::polyPatch const & patch, 
      ::Foam::word const & faceZone,
      ::Foam::polyMesh & mesh
    );
    static void boxToCellZone(
      ::Foam::point const & min, 
      ::Foam::point const & max,
      ::Foam::word const & cellZone,
      ::Foam::polyMesh & mesh
    );    
    static void writeControlDict( 
      std::string const & workingDirectory, std::string const & content
    );    
    static void writeDicts( 
      ::Foam::fvMesh & fvMesh, 
      std::string const & workingDirectory, 
      std::string const & content,
      std::vector< std::string > const & noWriteRule
    );        
  private:
    dtFoamLibrary();
    virtual ~dtFoamLibrary();
    static void renumber(
      const ::Foam::Map< ::Foam::label >& mshToFoam, ::Foam::labelList& labels
    );
    static ::Foam::label findFace(
      const ::Foam::primitivePatch& pp, const ::Foam::labelList& meshF
    );    
    static ::Foam::label findInternalFace(
      const ::Foam::primitiveMesh& mesh, const ::Foam::labelList& meshF
    );
    static bool correctOrientation(
      const ::Foam::pointField& points, const ::Foam::cellShape& shape
    );
    static void storeCellInZone(
      const ::Foam::label regPhys,
      const ::Foam::label cellI,
      ::Foam::Map< ::Foam::label >& physToZone,
      ::Foam::labelList& zoneToPhys,
      ::Foam::List< ::Foam::DynamicList< ::Foam::label > >& zoneCells
    );
    static void readPhysNames(
      dtGmshModel * gm, ::Foam::Map< ::Foam::word >& physicalNames
    );
    static void readPoints( 
      std::vector< ::MVertex * > allVerts,  
      ::Foam::pointField& points, ::Foam::Map< ::Foam::label >& mshToFoam
    );
    static void readCells(
      std::vector< std::pair< ::MElement *, int > > allElems,
      const bool keepOrientation,
      const ::Foam::pointField& points,
      const ::Foam::Map< ::Foam::label >& mshToFoam,
      ::Foam::cellShapeList& cells,
      ::Foam::labelList& patchToPhys,
      ::Foam::List< ::Foam::DynamicList< ::Foam::face > >& patchFaces,
      ::Foam::labelList& zoneToPhys,
      ::Foam::List< ::Foam::DynamicList< ::Foam::label > >& zoneCells
    );
  public:
    static const ::Foam::label MSHLINE  = 1;
    static const ::Foam::label MSHTRI   = 2;
    static const ::Foam::label MSHQUAD  = 3;
    static const ::Foam::label MSHTET   = 4;
    static const ::Foam::label MSHPYR   = 7;
    static const ::Foam::label MSHPRISM = 6;
    static const ::Foam::label MSHHEX   = 5;    
    static const ::Foam::label MSHPNT   = 15;

  };
}

#endif	/* DTFOAMLIBRARY_H */

