//
// constValue
//
boost::python::class_< 
  labeledVectorHandlingWrap< dtOO::constValue * >, 
  boost::noncopyable 
>("labeledVectorHandlingConstValue")
.def<
  dtOO::labeledVectorHandling< dtOO::constValue * >::const_reference 
  (dtOO::labeledVectorHandling< dtOO::constValue * >::*)
  ( std::string const & ) const
>(
  "get", 
  &labeledVectorHandlingWrap< dtOO::constValue * >::get,
  boost::python::return_value_policy<
    boost::python::reference_existing_object
  >()
)
.def(
  "getLabel", 
  &labeledVectorHandlingWrap< dtOO::constValue * >::getLabel
)
.def("dump", &labeledVectorHandlingWrap< dtOO::constValue * >::dump)  
.def("size", &std::vector< dtOO::constValue * >::size)
;
  
//
// analyticFunction
//
boost::python::class_< 
  labeledVectorHandlingWrap< dtOO::analyticFunction * >, boost::noncopyable 
>("labeledVectorHandlingAnalyticFunction")
.def<
  dtOO::labeledVectorHandling< dtOO::analyticFunction * >::const_reference 
  (dtOO::labeledVectorHandling< dtOO::analyticFunction * >::*)
  ( std::string const & ) const
>(
  "get", 
  &labeledVectorHandlingWrap< dtOO::analyticFunction * >::get,
  boost::python::return_value_policy<
    boost::python::reference_existing_object
  >()
)
.def(
  "getLabel", 
  &labeledVectorHandlingWrap< dtOO::analyticFunction * >::getLabel
)
.def("dump", &labeledVectorHandlingWrap< dtOO::analyticFunction * >::dump)  
.def("size", &std::vector< dtOO::analyticFunction * >::size)
;  
  
//
// analyticGeometry
//
boost::python::class_< 
  labeledVectorHandlingWrap< dtOO::analyticGeometry * >, boost::noncopyable 
>("labeledVectorHandlingAnalyticGeometry")
.def<
  dtOO::labeledVectorHandling< dtOO::analyticGeometry * >::const_reference 
  (dtOO::labeledVectorHandling< dtOO::analyticGeometry * >::*)
  ( std::string const & ) const
>(
  "get", 
  &labeledVectorHandlingWrap< dtOO::analyticGeometry * >::get,
  boost::python::return_value_policy<
    boost::python::reference_existing_object
  >()
)
.def(
  "getLabel", 
  &labeledVectorHandlingWrap< dtOO::analyticGeometry * >::getLabel
)
.def("dump", &labeledVectorHandlingWrap< dtOO::analyticGeometry * >::dump)  
.def("size", &std::vector< dtOO::analyticGeometry * >::size)  
;    
  
//
// boundedVolume
//
boost::python::class_< 
  labeledVectorHandlingWrap< dtOO::boundedVolume * >, boost::noncopyable 
>("labeledVectorHandlingBoundedVolume")
.def<
  dtOO::labeledVectorHandling< dtOO::boundedVolume * >::const_reference 
  (dtOO::labeledVectorHandling< dtOO::boundedVolume * >::*)
  ( std::string const & ) const
>(
  "get", 
  &labeledVectorHandlingWrap< dtOO::boundedVolume * >::get,
  boost::python::return_value_policy<
    boost::python::reference_existing_object
  >()
)
.def(
  "getLabel", 
  &labeledVectorHandlingWrap< dtOO::boundedVolume * >::getLabel
)
.def("dump", &labeledVectorHandlingWrap< dtOO::boundedVolume * >::dump)  
.def("size", &std::vector< dtOO::boundedVolume * >::size)
;      
  
//
// dtCase
//
boost::python::class_< 
  labeledVectorHandlingWrap< dtOO::dtCase * >, boost::noncopyable 
>("labeledVectorHandlingDtCase")
.def<
  dtOO::labeledVectorHandling< dtOO::dtCase * >::const_reference 
  (dtOO::labeledVectorHandling< dtOO::dtCase * >::*)
  ( std::string const & ) const
>(
  "get", 
  &labeledVectorHandlingWrap< dtOO::dtCase * >::get,
  boost::python::return_value_policy<
    boost::python::reference_existing_object
  >()
)
.def(
  "getLabel", 
  &labeledVectorHandlingWrap< dtOO::dtCase * >::getLabel
)
.def("dump", &labeledVectorHandlingWrap< dtOO::dtCase * >::dump)  
.def("size", &std::vector< dtOO::dtCase * >::size)
;     
  
//
// dtPlugin
//
boost::python::class_< 
  labeledVectorHandlingWrap< dtOO::dtPlugin * >, boost::noncopyable 
>("labeledVectorHandlingDtPlugin")
.def<
  dtOO::labeledVectorHandling< dtOO::dtPlugin * >::const_reference 
  (dtOO::labeledVectorHandling< dtOO::dtPlugin * >::*)
  ( std::string const & ) const
>(
  "get", 
  &labeledVectorHandlingWrap< dtOO::dtPlugin * >::get,
  boost::python::return_value_policy<
    boost::python::reference_existing_object
  >()
)
.def(
  "getLabel", 
  &labeledVectorHandlingWrap< dtOO::dtPlugin * >::getLabel
)
.def("dump", &labeledVectorHandlingWrap< dtOO::dtPlugin * >::dump)
.def("size", &std::vector< dtOO::dtPlugin * >::size)
;       