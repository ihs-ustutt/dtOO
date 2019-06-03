#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <dtLinearAlgebra.h>
#include <baseContainerHeaven/baseContainer.h>
#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParser.h>
#include <constValueHeaven/constValue.h>
#include <interfaceHeaven/labeledVectorHandling.h>
#include <dtTransformerHeaven/dtTransformer.h>
#include <analyticFunctionHeaven/aFX.h>
#include <analyticFunctionHeaven/aFY.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshModel.h>
#include <boundedVolume.h>
#include <dtCase.h>
#include <dtPlugin.h>


#include "constValuePythonPre.h"
#include "analyticFunctionPythonPre.h"
#include "analyticGeometryPythonPre.h"
#include "map1dTo3dPythonPre.h"
#include "map2dTo3dPythonPre.h"
#include "map3dTo3dPythonPre.h"
#include "boundedVolumePythonPre.h"
#include "dtCasePythonPre.h"
#include "dtPluginPythonPre.h"
#include "labeledVectorHandlingPre.h"

BOOST_PYTHON_MODULE(libdtOOPython){
  boost::python::class_<std::vector<float> >("stdVectorFloat")
    .def(boost::python::vector_indexing_suite<std::vector<float> >());
  boost::python::class_<std::vector<std::string> >("stdVectorString")
    .def(boost::python::vector_indexing_suite<std::vector< std::string > >());    
  #include "logMePython.h"
  #include "dtLinearAlgebraPython.h"
  #include "baseContainerPython.h"
  #include "dtXmlParserPython.h"
  #include "constValuePython.h"
  #include "analyticFunctionPython.h"
  #include "analyticGeometryPython.h"
  #include "map1dTo3dPython.h"
  #include "map2dTo3dPython.h"
  #include "map3dTo3dPython.h"
  #include "boundedVolumePython.h"
  #include "dtCasePython.h"
  #include "dtPluginPython.h"
  #include "labeledVectorHandling.h"
}


