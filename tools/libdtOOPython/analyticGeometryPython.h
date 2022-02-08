boost::python::class_< analyticGeometryWrap, boost::noncopyable >(
  "analyticGeometry"
)
  .def("clone", boost::python::pure_virtual( &dtOO::analyticGeometry::clone),
    boost::python::return_value_policy<
      boost::python::manage_new_object
    >()  
  )
  .def("create", boost::python::pure_virtual( &dtOO::analyticGeometry::create),
    boost::python::return_value_policy<
      boost::python::manage_new_object
    >()  
  )
  .def("dim", boost::python::pure_virtual( &dtOO::analyticGeometry::dim) )
  .def("isClosed", boost::python::pure_virtual( &dtOO::analyticGeometry::isClosed) )
  .def("getMin", &dtOO::analyticGeometry::getMin)
  .def("getMax", &dtOO::analyticGeometry::getMax)
  .def< 
    dtOO::dtPoint3 (dtOO::analyticGeometry::*)(std::vector< dtOO::dtReal > const &) const
  >(
    "getPoint", &dtOO::analyticGeometry::getPoint
  )
;