boost::python::class_< CGAL::Point_3< dtOO::dtKernel > >(
  "dtPoint3", boost::python::init<>()
)
.def( 
  "x", 
  &CGAL::Point_3< dtOO::dtKernel >::x,
  boost::python::return_value_policy<
    boost::python::copy_const_reference
  >() 
)
.def( 
  "y", 
  &CGAL::Point_3< dtOO::dtKernel >::y,
  boost::python::return_value_policy<
    boost::python::copy_const_reference
  >() 
)
.def( 
  "z", 
  &CGAL::Point_3< dtOO::dtKernel >::z,
  boost::python::return_value_policy<
    boost::python::copy_const_reference
  >() 
)  
;