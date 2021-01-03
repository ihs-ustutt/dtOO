boost::python::class_< dtCaseWrap, boost::noncopyable >(
  "dtCase"
)
  .def("create", boost::python::pure_virtual( &dtOO::dtCase::create),
    boost::python::return_value_policy<
      boost::python::manage_new_object
    >()  
  )
  .def("runCurrentState", boost::python::pure_virtual( &dtOO::dtCase::runCurrentState))
;