boost::python::class_< 
  map1dTo3dWrap, 
  boost::python::bases< dtOO::analyticGeometry >,
  map1dTo3dWrap,
  boost::noncopyable
>("map1dTo3d", boost::python::no_init)
  .def(
    "DownCast", &dtOO::map1dTo3d::DownCast,    
    boost::python::return_value_policy< boost::python::manage_new_object >()  
  )
  .staticmethod("DownCast")  
  .def("clone", boost::python::pure_virtual( &dtOO::map1dTo3d::clone),
    boost::python::return_value_policy<
      boost::python::manage_new_object
    >()  
  )
  .def("cloneTransformed", boost::python::pure_virtual( &dtOO::map1dTo3d::cloneTransformed),
    boost::python::return_value_policy<
      boost::python::manage_new_object
    >()  
  )
  .def("create", boost::python::pure_virtual( &dtOO::map1dTo3d::create),
    boost::python::return_value_policy<
      boost::python::manage_new_object
    >()  
  )
  .def(
    "getPoint", 
    boost::python::pure_virtual<
      dtOO::dtPoint3 (dtOO::map1dTo3d::*)(float const &) const
    >( &dtOO::map1dTo3d::getPoint ) 
  )
  .def("getUMin", &dtOO::map1dTo3d::getUMin)
  .def("getUMax", &dtOO::map1dTo3d::getUMax)
  .def("length", &dtOO::map1dTo3d::length)
  .def("isClosed", &dtOO::map1dTo3d::isClosedU)
;