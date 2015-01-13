#include "dtGmshRegionHex.h"
#include <progHelper.h>
#include <gmsh/GFace.h>
#include <gmsh/GEdge.h>
#include <gmsh/GModel.h>
#include <list>

namespace dtOO {
/* Vertices and edges are addressed in the following order:
 *
 *      7-------6    0-1 > 0
 *     /|      /|    1-2 > 1
 *    / |     / |    2-3 > 2
 *   4-------5  |    3-0 > 3
 *   |  3----|--2    4-5 > 4
 *   | /     | /     5-6 > 5
 *   |/      |/      6-7 > 6
 *   0-------1       7-4 > 7
 *                   0-4 > 8
 *                   1-5 > 9
 *                   2-6 > 10
 *                   3-7 > 11
 */                  
  dtGmshRegionHex::dtGmshRegionHex( 
    GModel *m, int tag, 
    const std::vector<int> & geTag,
		const std::list< dtGmshFace * > &faces, 
    const std::vector<int> & ori
  ) : dtGmshRegion(m, tag, faces, ori) {
		_hexEdges.resize(12);
    for (int ii=0; ii<12; ii++) _hexEdges[ii] = geTag[ii];
  }

  void dtGmshRegionHex::meshTransfinite( int const nU, int const nV, int const nW ) {
		dtGmshRegion::meshTransfinite();

    dtGmshRegion::model()->getEdgeByTag(_hexEdges[0])->meshAttributes.nbPointsTransfinite = nU;
		dtGmshRegion::model()->getEdgeByTag(_hexEdges[2])->meshAttributes.nbPointsTransfinite = nU;
		dtGmshRegion::model()->getEdgeByTag(_hexEdges[6])->meshAttributes.nbPointsTransfinite = nU;
		dtGmshRegion::model()->getEdgeByTag(_hexEdges[4])->meshAttributes.nbPointsTransfinite = nU;
    dtGmshRegion::model()->getEdgeByTag(_hexEdges[1])->meshAttributes.nbPointsTransfinite = nV;
		dtGmshRegion::model()->getEdgeByTag(_hexEdges[3])->meshAttributes.nbPointsTransfinite = nV;
		dtGmshRegion::model()->getEdgeByTag(_hexEdges[7])->meshAttributes.nbPointsTransfinite = nV;
		dtGmshRegion::model()->getEdgeByTag(_hexEdges[5])->meshAttributes.nbPointsTransfinite = nV;		
    dtGmshRegion::model()->getEdgeByTag(_hexEdges[8])->meshAttributes.nbPointsTransfinite = nW;
		dtGmshRegion::model()->getEdgeByTag(_hexEdges[9])->meshAttributes.nbPointsTransfinite = nW;
		dtGmshRegion::model()->getEdgeByTag(_hexEdges[10])->meshAttributes.nbPointsTransfinite = nW;
		dtGmshRegion::model()->getEdgeByTag(_hexEdges[11])->meshAttributes.nbPointsTransfinite = nW;		
  }
	
  void dtGmshRegionHex::setGrading( std::vector< float > const & grading, std::vector< float > & type ) {
    dt__THROW_IF(grading.size() != 12, setGrading());
    dt__THROW_IF(type.size() != 12, setGrading());
		
		for (int ii=0; ii<grading.size(); ii++) {
			dtGmshRegion::model()->getEdgeByTag(
				_hexEdges[ii]
			)->meshAttributes.coeffTransfinite = (_hexEdges[ii]>0 ? grading[ii] : 1./grading[ii]);
			dtGmshRegion::model()->getEdgeByTag(
				_hexEdges[ii]
			)->meshAttributes.typeTransfinite = type[ii];
		}
  }	
}

