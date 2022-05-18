%module dtOOPythonSWIG
%{
#include <logMe/logMe.h>
#include <mainConceptFwd.h>
#include <exceptionHeaven/eGeneral.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec3dFunction.h>
#include <analyticFunctionHeaven/vec3dTwoD.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/dtOCCBSplineSurface.h>
#include <boundedVolume.h>
#include <dtCase.h>
#include <dtPlugin.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <xmlHeaven/dtXmlParser.h>
using namespace dtOO;
#ifdef DTOO_HAS_PYTHONOCC
  #include <TColStd_module.hxx>
  #include <TCollection_module.hxx>
  #include <Message_module.hxx>
  #include <Geom_module.hxx>
  #include <TColgp_module.hxx>
#endif
%}

namespace dtOO {
  class eGeneral : public std::exception {
  public:
    eGeneral();
    eGeneral( std::ostream & msg);
    virtual ~eGeneral();
    void clear(void);
    virtual const char* what() const;
  };
}

%include <std_string.i>
#ifdef DTOO_HAS_PYTHONOCC
%import Standard.i
%import math.i
%import Geom.i
#endif
%exception;

%exception {
	try {
		$action
  } 
  catch (eGeneral &e) {
    PyErr_SetString( PyExc_Exception, const_cast<char*>(e.what()) );  
    SWIG_fail;
	}  
  catch (...) {
    PyErr_SetString(PyExc_Exception, "dtOOPythonSWIG catch exception");
    SWIG_fail;
	}
}

%include <dtOOTypeDef.h>
%include <logMe/dtMacros.h>
%include <interfaceHeaven/ptrHandling.h>
namespace dtOO {
  class logMe {
    public:
      static std::string initLog( std::string const & logFileName );
  };
}

%include <baseContainerHeaven/baseContainer.h>
%include <constValueHeaven/constValue.h>
%include <analyticFunctionHeaven/analyticFunction.h>
%include <analyticGeometryHeaven/analyticGeometry.h>
%include <boundedVolume.h>
%include <dtCase.h>
%include <dtPlugin.h>

namespace dtOO {
  template < typename T >
  class labeledVectorHandling : public vectorHandling< T > {
    public:
      T const & get( std::string const & label) const;
      std::string getLabel( dtInt const pos ) const;
      T & getRef( std::string const label);      
      dtInt getPosition( std::string const label) const;
      void dump(void) const;
  };
    
  typedef labeledVectorHandling< constValue * > cVPtrVec;
  typedef labeledVectorHandling< analyticFunction * > aFPtrVec;
  typedef labeledVectorHandling< analyticGeometry * > aGPtrVec;
  typedef labeledVectorHandling< boundedVolume * > bVPtrVec;
  typedef labeledVectorHandling< dtCase * > dCPtrVec;  
  typedef labeledVectorHandling< dtPlugin * > dPPtrVec;    
  %template(labeledVectorHandlingConstValue) labeledVectorHandling< constValue * >;
  %template(labeledVectorHandlingAnalyticFunction) labeledVectorHandling< analyticFunction * >;
  %template(labeledVectorHandlingAnalyticGeometry) labeledVectorHandling< analyticGeometry * >;
  %template(labeledVectorHandlingBoundedVolume) labeledVectorHandling< boundedVolume * >;
  %template(labeledVectorHandlingDtCase) labeledVectorHandling< dtCase * >;
  %template(labeledVectorHandlingDtPlugin) labeledVectorHandling< dtPlugin * >;
}
%include <xmlHeaven/dtXmlParserBase.h>
%include <xmlHeaven/dtXmlParser.h>
        
%include geometryEngine/dtSurface.h
%include geometryEngine/dtOCCSurfaceBase.h
%include geometryEngine/dtOCCSurface.h
%include geometryEngine/dtOCCBSplineSurface.h

%include analyticFunctionHeaven/vec3dFunction.h
%include analyticFunctionHeaven/vec3dTwoD.h
%include analyticFunctionHeaven/vec3dSurfaceTwoD.h
        
%include analyticGeometryHeaven/map2dTo3d.h
%include analyticGeometryHeaven/analyticSurface.h