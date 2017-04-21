#ifndef DTGMSHREGION_H
#define	DTGMSHREGION_H

#include <gmsh/GRegion.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

class GModel;
class GFace;

namespace dtOO {
  class dtGmshFace;
  class dtGmshModel;
  
  class dtGmshRegion : public ::GRegion {
  public:
    dt__class(dtGmshRegion, ::GEntity);
    dtGmshRegion( ::GModel *m, int tag );
    dtGmshRegion( 
      ::GModel *m, int tag, 
      const std::list< ::GFace * > &faces, const std::vector<int> &ori 
    );
    dtGmshRegion( 
      ::GModel *m, int tag, 
      const std::list<dtGmshFace*> &faces, const std::vector<int> &ori 
    );
    dtGmshModel const & refDtGmshModel( void ) const;
    virtual ::GEntity::GeomType geomType( void ) const; 
    void setGeomType( ::GEntity::GeomType const & gT );     
    void meshTransfinite( void );
    void meshTransfiniteRecursive( void );
    void meshWNElements( 
      int const & nElementsU, int const & nElementsV, int const & nElementsW
    );
    void meshRecombine( void );        
    void meshRecombineRecursive( void );    
    void meshUnstructured( void );
    void addFace( ::GFace * face, int const ori );
    int faceOrientation( ::GFace * face ) const;
    void addGEntity( ::GEntity * const gEnt );
    void addElement( ::MElement * me );
    bool isEqual( ::GRegion const * const gr ) const;    
    static bool isEqual(
      ::GRegion const * const gr0, ::GRegion const * const gr1 
    );
    void replaceFace( 
      ::GFace const * const toReplace, ::GFace * const with     
    );
    void setGrading( 
      std::vector< float > const & grading, std::vector< float > & type 
    );
    void setGrading( 
      float const & gU, float const & gV, float const & gW,
      float const & tU, float const & tV, float const & tW
    );
    std::list< dtGmshFace const * > constFaceList(
      std::vector< std::string > const & label
    ) const;
    std::list< dtGmshFace * > faceList(
      std::vector< std::string > const & label
    );    
    std::string dumpToString( void ) const;
    std::string getPhysicalString( void ) const;
  public:
    mutable ::GEntity::MeshGenerationStatus _status;
    ::GEntity::GeomType _geomType;
  };
}
#endif	/* DTGMSHREGION_H */

