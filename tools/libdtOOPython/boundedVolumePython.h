boost::python::class_< boundedVolumeWrap, boost::noncopyable >(
  "boundedVolume"
)
  .def("create", boost::python::pure_virtual( &dtOO::boundedVolume::create),
    boost::python::return_value_policy<
      boost::python::manage_new_object
    >()  
  )
  .def("makeGrid", boost::python::pure_virtual( &dtOO::boundedVolume::makeGrid) )
  .def("makePreGrid", boost::python::pure_virtual( &dtOO::boundedVolume::makePreGrid) )
  .def("getMeshTags", boost::python::pure_virtual( &dtOO::boundedVolume::getMeshTags) )
  .def("getFace", boost::python::pure_virtual( &dtOO::boundedVolume::getFace),
    boost::python::return_value_policy<
      boost::python::manage_new_object
    >()  
  )
  .def("getRegion", boost::python::pure_virtual( &dtOO::boundedVolume::getRegion),
    boost::python::return_value_policy<
      boost::python::manage_new_object
    >()  
  )
  .def("getModel", boost::python::pure_virtual( &dtOO::boundedVolume::getModel),
    boost::python::return_value_policy<
      boost::python::manage_new_object
    >()  
  )
;