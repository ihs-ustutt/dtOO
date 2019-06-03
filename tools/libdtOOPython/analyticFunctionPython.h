boost::python::class_< analyticFunctionWrap, boost::noncopyable >(
  "analyticFunction"
)
  .def("clone", boost::python::pure_virtual( &dtOO::analyticFunction::clone),
    boost::python::return_value_policy<
      boost::python::manage_new_object
    >()  
  )
  .def("cloneTransformed", boost::python::pure_virtual( &dtOO::analyticFunction::cloneTransformed ),
    boost::python::return_value_policy<
      boost::python::manage_new_object
    >()  
  )
  .def("create", boost::python::pure_virtual( &dtOO::analyticFunction::create),
    boost::python::return_value_policy<
      boost::python::manage_new_object
    >()  
  )
  .def("Y", boost::python::pure_virtual( &dtOO::analyticFunction::Y) )
  .def("xDim", boost::python::pure_virtual( &dtOO::analyticFunction::xDim) )
  .def("yDim", boost::python::pure_virtual( &dtOO::analyticFunction::yDim) )
  //.def("isClosed", boost::python::pure_virtual( &dtOO::analyticFunction::isClosed) )
  .def<
    float (dtOO::analyticFunction::*)(int const &) const
  >("xMin", &dtOO::analyticFunction::xMin)
  .def<
    float (dtOO::analyticFunction::*)(int const &) const
  >("xMax", &dtOO::analyticFunction::xMax)
;