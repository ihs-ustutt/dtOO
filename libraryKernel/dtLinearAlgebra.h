#ifndef DTLINEARALGEBRA_H
#define DTLINEARALGEBRA_H

//
// gmsh fix:
// definition of sign(x) in Numeric.h (gmsh) conflicts with cgal
//
#ifdef _NUMERIC_H_
  #undef sign
#endif

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
    static dtVector2 unitNormal( dtVector2 const & vv);
    static dtMatrix invertMatrix(dtMatrix const & mat);
    static dtMatrix transposeMatrix(dtMatrix const mat);
    static dtMatrix invert2x3Matrix(dtMatrix const mat);
    static dtPoint3 returnFarthestPointTo(dtPoint3 pp, dtPoint3 p0, dtPoint3 p1);
    static void makeOrdered(std::vector< dtPoint3 > & pp);
    static std::vector<dtPoint2> getGaussLegendre(int const & nPoints);
    static std::pair< dtPoint2, dtPoint2 > boundingBox( std::vector< dtPoint2 > const & pp );
    static std::pair< dtPoint3, dtPoint3 > boundingBox( std::vector< dtPoint3 > const & pp );
    static float distance( dtPoint2 const & p0, dtPoint2 const & p1 );
    static float distance( dtPoint3 const & p0, dtPoint3 const & p1 );
    static float angle( dtVector3 const & v0, dtVector3 const & v1 );
    static float angleDegree( dtVector3 const & v0, dtVector3 const & v1 );
  };
}
#endif  /* DTLINEARALGEBRA_H */

