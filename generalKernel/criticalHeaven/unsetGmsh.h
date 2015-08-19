//
// gmsh fix:
// definition of sign(x) in Numeric.h (gmsh) conflicts with cgal
//
#ifdef _NUMERIC_H_
  #undef sign
#endif

