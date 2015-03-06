#ifndef DTLINEARALGEBRA_H
#define DTLINEARALGEBRA_H

#include <criticalHeaven/unsetGmsh.h>

#include <logMe/dtMacros.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Cartesian_d.h>
#include <CGAL/Point_3.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Origin.h>
#include <CGAL/Linear_algebraCd.h>

namespace dtOO {
  typedef float dtFt;
  typedef CGAL::Exact_predicates_inexact_constructions_kernel dtKernel;
  typedef CGAL::Point_3< dtKernel > dtPoint3;
  typedef CGAL::Vector_3< dtKernel > dtVector3;
  typedef CGAL::Aff_transformation_3< dtKernel > dtAffTransformation3;
  typedef CGAL::Point_2< dtKernel > dtPoint2;
  typedef CGAL::Vector_2< dtKernel > dtVector2;
  typedef CGAL::Aff_transformation_2< dtKernel > dtAffTransformation2;
  typedef CGAL::Linear_algebraCd< dtFt >::Matrix dtMatrix;
  typedef CGAL::Linear_algebraCd< dtFt >::Vector dtMatrixVector;
  
  class dtLinearAlgebra {
  public:
    static dtVector3 unitNormal0;
    static dtVector3 unitNormal1;
    static dtVector3 unitNormal2;
  private:
    dtLinearAlgebra();     
  public:
    dt__CLASSNAME(dtLinearAlgebra);   
    virtual ~dtLinearAlgebra();
    static dtAffTransformation3 getRotation(
      dtVector3 const vector, 
      const float angle
    );
    static dtAffTransformation3 getDiagTrans(
      dtVector3 const vector
    );    
    static dtVector3 crossProduct(
      dtVector3 const & v0, 
      dtVector3 const & v1
    );
    static float dotProduct(
      dtVector3 const & v0, 
      dtVector3 const & v1
    );    
	  static dtVector3 normalize( dtVector3 const & v0 );    
    static float length( dtVector3 const & v0 );    
    static dtVector3 toDtVector3(dtPoint3 const & pp);
    static dtVector2 toDtVector2(dtPoint2 const & pp);
    static dtPoint3 toDtPoint3(dtVector3 const & pp);
    static dtPoint2 toDtPoint2(dtVector2 const & pp);   
    static std::vector< dtPoint3 > toDtPoint3Vector(
      std::vector< double > const & xx,
      std::vector< double > const & yy,
      std::vector< double > const & zz
    );
    static dtVector2 unitNormal( dtVector2 const & vv);
    static dtMatrix createMatrixGiveColumns( std::vector< dtVector3 > const & cols );
    static dtMatrix createMatrixGiveRows( std::vector< dtVector3 > const & rows );
    static dtMatrixVector createMatrixVector( dtVector3 const & vec );
    static dtMatrixVector createMatrixVector( dtVector2 const & vec );
    static dtVector2 toDtVector2(dtMatrixVector const & vec);
    static dtVector3 toDtVector3(dtMatrixVector const & vec);
    static dtMatrix invertMatrix(dtMatrix const & mat);
    static dtMatrixVector solveMatrix(dtMatrix const & mat, dtMatrixVector const & rhs);
    static dtMatrix transposeMatrix(dtMatrix const mat);
    static dtMatrix invert2x3Matrix(dtMatrix const mat);
    static dtPoint3 returnFarthestPointTo(dtPoint3 pp, dtPoint3 p0, dtPoint3 p1);
    static void makeOrdered(std::vector< dtPoint3 > & pp);
    static std::vector<dtPoint2> getGaussLegendre(int const & nPoints);
    static std::pair< dtPoint2, dtPoint2 > boundingBox( std::vector< dtPoint2 > const & pp );
    static std::pair< dtPoint3, dtPoint3 > boundingBox( std::vector< dtPoint3 > const & pp );
	  static bool isStraightLine( std::pair< dtPoint3, dtPoint3 > const & bBox, float const & eps );
    static float distance( dtPoint2 const & p0, dtPoint2 const & p1 );
    static float distance( dtPoint3 const & p0, dtPoint3 const & p1 );
    static float angle( dtVector3 const & v0, dtVector3 const & v1 );
    static float angleDegree( dtVector3 const & v0, dtVector3 const & v1 );
    static dtVector2 ignoreZ( dtVector3 const & v0 );
    static dtPoint2 ignoreZ( dtPoint3 const & v0 );
    static std::string directionString( dtVector3 const & vv );
  };
}
#endif  /* DTLINEARALGEBRA_H */

