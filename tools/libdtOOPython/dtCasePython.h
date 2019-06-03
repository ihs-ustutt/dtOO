boost::python::class_< dtCaseWrap, boost::noncopyable >(
  "dtCase"
)
  .def("runCurrentState", boost::python::pure_virtual( &dtOO::dtCase::runCurrentState))
;