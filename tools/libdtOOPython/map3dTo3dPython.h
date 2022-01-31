boost::python::class_< 
  map3dTo3dWrap, 
  boost::python::bases< dtOO::analyticGeometry >,
  map3dTo3dWrap,
  boost::noncopyable
>("map3dTo3d", boost::python::no_init)
  .def(
    "DownCast", &dtOO::map3dTo3d::DownCast,    
    boost::python::return_value_policy< boost::python::manage_new_object >()  
  )
  .staticmethod("DownCast")  
  .def("clone", boost::python::pure_virtual( &dtOO::map3dTo3d::clone),
    boost::python::return_value_policy<
      boost::python::manage_new_object
    >()  
  )
  .def(
    "cloneTransformed", 
    boost::python::pure_virtual( &dtOO::map3dTo3d::cloneTransformed),
    boost::python::return_value_policy<
      boost::python::manage_new_object
    >()  
  )
  .def("create", boost::python::pure_virtual( &dtOO::map3dTo3d::create),
    boost::python::return_value_policy<
      boost::python::manage_new_object
    >()  
  )
  .def(
    "getPoint", 
    boost::python::pure_virtual<
      dtOO::dtPoint3 (dtOO::map3dTo3d::*)(
        dtOO::dtReal const &, dtOO::dtReal const &, dtOO::dtReal const &
      ) const
    >( &dtOO::map3dTo3d::getPoint ) 
  )
  .def("getUMin", &dtOO::map3dTo3d::getUMin)
  .def("getUMax", &dtOO::map3dTo3d::getUMax)
  .def("isClosedU", &dtOO::map3dTo3d::isClosedU)
  .def("getVMin", &dtOO::map3dTo3d::getVMin)
  .def("getVMax", &dtOO::map3dTo3d::getVMax)
  .def("isClosedV", &dtOO::map3dTo3d::isClosedV)
  .def("getWMin", &dtOO::map3dTo3d::getWMin)
  .def("getWMax", &dtOO::map3dTo3d::getWMax)
  .def("isClosedW", &dtOO::map3dTo3d::isClosedW)
;