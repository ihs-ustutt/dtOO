#ifndef DTGMSHREGION_H
#define	DTGMSHREGION_H

#include <dtOOTypeDef.h>

#include <gmsh/GRegion.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

class GModel;
class GFace;

namespace dtOO {
  class dtGmshEdge;
  class dtGmshFace;
  class dtGmshModel;
  
  class dtGmshRegion : public ::GRegion {
  public:
    dt__class(dtGmshRegion, ::GEntity);
    dtGmshRegion( ::GModel *m, dtInt tag );
    dtGmshRegion( 
      ::GModel *m, dtInt tag, 
      const std::list< ::GFace * > &faces, const std::vector<int> &ori 
    );
    dtGmshRegion( 
      ::GModel *m, dtInt tag, 
      const std::list<dtGmshFace*> &faces, const std::vector<int> &ori 
    );
    dtGmshModel const & refDtGmshModel( void ) const;
    virtual ::GEntity::GeomType geomType( void ) const; 
    void setGeomType( ::GEntity::GeomType const & gT );     
    void meshTransfinite( void );
    void meshTransfiniteRecursive( void );
    void meshWNElements( 
      dtInt const & nElementsU, dtInt const & nElementsV, dtInt const & nElementsW
    );
    void meshRecombine( void );        
    void meshRecombineRecursive( void );    
    void meshUnstructured( void );
    void addFace( ::GFace * face, dtInt const ori );
    void deleteFace( ::GFace * face );
    dtInt faceOrientation( ::GFace * face ) const;
    void addGEntity( ::GEntity * const gEnt );
    void addElement( ::MElement * me );
    bool isEqual( ::GRegion const * const gr ) const;    
    static bool isEqual(
      ::GRegion const * const gr0, ::GRegion const * const gr1 
    );
    void replaceFaces( 
      std::vector< GFace * > & new_faces
    );    
    void replaceFace( 
      ::GFace const * const toReplace, ::GFace * const with     
    );
    void setGrading( 
      std::vector< dtReal > const & grading, const std::vector< dtReal > & type 
    );
    std::list< dtGmshFace const * > constFaceList(
      std::vector< std::string > const & label
    ) const;
    std::list< dtGmshFace * > faceList(
      std::vector< std::string > const & label
    );    
    std::string dumpToString( void ) const;
    std::string getPhysicalString( void ) const;
    std::list< dtGmshFace * > dtFaces( void ) const;
    std::list< dtGmshEdge * > dtEdges( void ) const;
  public:
    mutable ::GEntity::MeshGenerationStatus _status;
    ::GEntity::GeomType _geomType;
  };
}
#endif	/* DTGMSHREGION_H */

