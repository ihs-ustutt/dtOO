boost::python::class_< dtOO::dtXmlParser >(
  "dtXmlParser", boost::python::no_init
)
  .def(
    "init", 
    &dtOO::dtXmlParser::init, 
    boost::python::return_value_policy<
      boost::python::reference_existing_object
    >()
  )
  .staticmethod("init")
  .def("parse", &dtOO::dtXmlParser::parse)
  .def("load", &dtOO::dtXmlParser::load)
  .def("loadStateToConst", &dtOO::dtXmlParser::loadStateToConst)
  .def(
    "reference", 
    &dtOO::dtXmlParser::reference,
    boost::python::return_value_policy<
      boost::python::reference_existing_object
    >()
  )
  .staticmethod("reference")  
  .def(
    "reinit", 
    &dtOO::dtXmlParser::reinit,
    boost::python::return_value_policy<
      boost::python::reference_existing_object
    >()
  )
  .staticmethod("reinit")    
  .def( "dump", &dtOO::dtXmlParser::dump )
  .def<
    void (dtOO::dtXmlParser::*)(dtOO::cVPtrVec &) const
  >( 
    "createConstValue", &dtOO::dtXmlParser::createConstValue 
  )
  .def("destroyAndCreate", &dtOO::dtXmlParser::destroyAndCreate)
  .def("currentState", &dtOO::dtXmlParser::currentState)
  .def("getStates", &dtOO::dtXmlParser::getStates)
  .def<
    void (dtOO::dtXmlParser::*)(std::string const, dtOO::cVPtrVec const &) const
  >( 
    "write", &dtOO::dtXmlParser::write 
  )
  .def(
    "replaceDependencies", &dtOO::dtXmlParserBase::replaceDependenciesRef        
  )
  .staticmethod("replaceDependencies")    
;