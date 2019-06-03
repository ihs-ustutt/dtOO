boost::python::class_< dtOO::logMe >(
  "logMe", boost::python::no_init
)
  .def(
    "initLog", 
    &dtOO::logMe::initLog
  )
  .staticmethod("initLog")
;
