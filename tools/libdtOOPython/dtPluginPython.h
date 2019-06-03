boost::python::class_< dtPluginWrap, boost::noncopyable >(
  "dtPlugin"
)
  .def("create", boost::python::pure_virtual( &dtOO::dtPlugin::create),
    boost::python::return_value_policy<
      boost::python::manage_new_object
    >()  
  )
  .def( "apply", boost::python::pure_virtual( &dtOO::dtPlugin::apply) )
;