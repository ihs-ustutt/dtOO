#ifndef DTGMSHREGION_H
#define	DTGMSHREGION_H

#include <gmsh/GRegion.h>
#include <logMe/dtMacros.h>

class GModel;
class GFace;

namespace dtOO {
  class dtGmshFace;
  
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
    void meshTransfinite( void );
    void meshUnstructured( void );
    void addFace( ::GFace * face, int const ori );
    void addGEntity( ::GEntity * const gEnt );
    void addElement( ::MElement * me );
  public:
    mutable ::GEntity::MeshGenerationStatus _status;
  };
}
#endif	/* DTGMSHREGION_H */

