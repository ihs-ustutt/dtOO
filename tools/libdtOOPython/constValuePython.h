boost::python::class_< constValueWrap, boost::noncopyable >("constValue")
  .def("clone", boost::python::pure_virtual( &dtOO::constValue::clone),
    boost::python::return_value_policy<
      boost::python::manage_new_object
    >()  
  )
  .def("dump", boost::python::pure_virtual( &dtOO::constValue::dump) )
  .def("writeToElement", boost::python::pure_virtual( &dtOO::constValue::writeToElement) )
  .def("getValue", &dtOO::constValue::getValue)
  .def("setValue", &dtOO::constValue::setValue)
;