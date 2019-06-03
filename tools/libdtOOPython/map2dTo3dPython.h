boost::python::class_< 
  map2dTo3dWrap, 
  boost::python::bases< dtOO::analyticGeometry >,
  map2dTo3dWrap,
  boost::noncopyable
>("map2dTo3d", boost::python::no_init)
  .def(
    "DownCast", &dtOO::map2dTo3d::DownCast,    
    boost::python::return_value_policy< boost::python::manage_new_object >()  
  )
  .staticmethod("DownCast")  
  .def("clone", boost::python::pure_virtual( &dtOO::map2dTo3d::clone),
    boost::python::return_value_policy<
      boost::python::manage_new_object
    >()  
  )
  .def("cloneTransformed", boost::python::pure_virtual( &dtOO::map2dTo3d::cloneTransformed),
    boost::python::return_value_policy<
      boost::python::manage_new_object
    >()  
  )
  .def("create", boost::python::pure_virtual( &dtOO::map2dTo3d::create),
    boost::python::return_value_policy<
      boost::python::manage_new_object
    >()  
  )
  .def(
    "getPoint", 
    boost::python::pure_virtual<
      dtOO::dtPoint3 (dtOO::map2dTo3d::*)(float const &, float const &) const
    >( &dtOO::map2dTo3d::getPoint ) 
  )
  .def("getUMin", &dtOO::map2dTo3d::getUMin)
  .def("getUMax", &dtOO::map2dTo3d::getUMax)
  .def("isClosedU", &dtOO::map2dTo3d::isClosedU)
  .def("getVMin", &dtOO::map2dTo3d::getVMin)
  .def("getVMax", &dtOO::map2dTo3d::getVMax)
  .def("isClosedV", &dtOO::map2dTo3d::isClosedV)
;