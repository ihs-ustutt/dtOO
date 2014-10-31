#include "dtGmshRegionHex.h"
#include <progHelper.h>
#include <gmsh/GFace.h>
#include <gmsh/GEdge.h>
#include <list>

namespace dtOO {
/* Vertices are addressed in the following order:
 *
 *      7-------6
 *     /|      /|
 *    / |     / |
 *   4-------5  |
 *   |  3----|--2
 *   | /     | /
 *   |/      |/
 *   0-------1
 */
  dtGmshRegionHex::dtGmshRegionHex( 
    GModel *m, int tag, 
    const std::vector<int> & geTag,
    const std::list<GFace*> & faces,
    const std::vector<int> & ori
  ) : dtGmshRegion(m, tag, faces, ori) {
    _hexEdges.resize(12);

    for (int ii=0; ii<12; ii++) {
      _hexEdges[ii] = geTag[ii];
    }
  }
  
  void dtGmshRegionHex::reorderBlock( void ) {
//      std::list< GFace* > gfs = faces();
//      GFace * gfsBegin = gfs.begin();
//      gfs.erase( gfs.begin() );
//      std::list<GFace*>::iterator gf_it = gfs.begin();
//      
//      std::list<GEdge*> geX;
//      std::list<GEdge*> geY;
//      std::list<GEdge*> geZ;
//      for (gf_it; gf_it != faces.end(); ++gf_it) {
//        (*gf_it)->containsEdge( gfs.begin()->edges().begin() );
//        
//      }
//    
//    

  }
}

