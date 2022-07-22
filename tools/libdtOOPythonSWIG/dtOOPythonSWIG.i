%import ../../doc/swig_includeAll.i

%module dtOOPythonSWIG
%{
#include <logMe/logMe.h>
#include <mainConceptFwd.h>
#include <dtOOTypeDef.h>
#include <exceptionHeaven/eGeneral.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/optionHandling.h>
#include <interfaceHeaven/vectorHandling.h>
#include <interfaceHeaven/labeledVectorHandling.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_3.h>  
#include <dtLinearAlgebra.h>
namespace dtOO {
  typedef ::CGAL::Exact_predicates_inexact_constructions_kernel dtKernel;
  typedef ::CGAL::Point_3< dtKernel > dtPoint3;
  typedef ::CGAL::Vector_3< dtKernel > dtVector3;
  typedef ::CGAL::Point_2< dtKernel > dtPoint2;
  typedef ::CGAL::Vector_2< dtKernel > dtVector2;  
}
#include <baseContainerHeaven/baseContainer.h>
#include <baseContainerHeaven/pointContainer.h>
#include <baseContainerHeaven/vectorContainer.h>
#include <baseContainerHeaven/transformerContainer.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/aFX.h>
#include <analyticFunctionHeaven/aFY.h>
#include <analyticFunctionHeaven/scaFunction.h>
#include <analyticFunctionHeaven/scaOneD.h>
#include <analyticFunctionHeaven/scaMultiOneD.h>
#include <analyticFunctionHeaven/scaMuParserOneD.h>
#include <analyticFunctionHeaven/scaCurve2dOneD.h>
#include <analyticFunctionHeaven/scaTwoD.h>
#include <analyticFunctionHeaven/scaMuParserTwoD.h>
#include <analyticFunctionHeaven/transIntCube.h>
#include <analyticFunctionHeaven/scaThreeD.h>
#include <analyticFunctionHeaven/scaMuParserThreeD.h>
#include <analyticFunctionHeaven/vec2dFunction.h>
#include <analyticFunctionHeaven/vec2dOneD.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <analyticFunctionHeaven/vec2dTwoD.h>
#include <analyticFunctionHeaven/vec2dSurface2dTwoD.h>
#include <analyticFunctionHeaven/vec2dMuParserTwoD.h>
#include <analyticFunctionHeaven/vec2dBiLinearTwoD.h>
#include <analyticFunctionHeaven/vec2dMultiBiLinearTwoD.h>
#include <analyticFunctionHeaven/vec3dFunction.h>
#include <analyticFunctionHeaven/vec3dTwoD.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <analyticFunctionHeaven/vec3dThickedTwoD.h>
#include <analyticFunctionHeaven/vec3dMuParserTwoD.h>
#include <analyticFunctionHeaven/vec3dOneD.h>
#include <analyticFunctionHeaven/vec3dMuParserOneD.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <analyticFunctionHeaven/vec3dCurve2dInSurfaceOneD.h>
#include <analyticFunctionHeaven/vec3dThreeD.h>
//#include <analyticFunctionHeaven/vec3dMultiThreeD.h>
#include <analyticFunctionHeaven/vec3dTransVolThreeD.h>
#include <analyticFunctionHeaven/vec3dMuParserThreeD.h>
#include <analyticFunctionHeaven/vec3dBoxThreeD.h>
#include <analyticFunctionHeaven/vec3dTriLinearThreeD.h>
#include <analyticFunctionHeaven/vec3dBiLinearTwoD.h>
#include <analyticFunctionHeaven/scaTanhGradingOneD.h>
#include <analyticFunctionHeaven/scaTanhUnitGradingOneD.h>
#include <analyticFunctionHeaven/scaOneDPolyInterface.h>

#include <dtTransformerHeaven/dtTransformer.h>
#include <dtTransformerHeaven/dtTransformerInvThreeD.h>
#include <dtTransformerHeaven/thicknessIncreasing.h>
#include <dtTransformerHeaven/biThicknessIncreasing.h>
#include <dtTransformerHeaven/doNothing.h>
#include <dtTransformerHeaven/offset.h>
#include <dtTransformerHeaven/translate.h>
#include <dtTransformerHeaven/radialTranslate.h>
#include <dtTransformerHeaven/rotate.h>
#include <dtTransformerHeaven/pickMap3dTo3dRangePercent.h>
#include <dtTransformerHeaven/pickMap2dTo3dRangePercent.h>
#include <dtTransformerHeaven/addConstCoordinate.h>
#include <dtTransformerHeaven/makePolynomial.h>
#include <dtTransformerHeaven/predefinedExtension.h>
#include <dtTransformerHeaven/closeGaps.h>
#include <dtTransformerHeaven/pickLengthRange.h>
#include <dtTransformerHeaven/pickLengthPercentRange.h>
#include <dtTransformerHeaven/uVw_phirMs.h>
#include <dtTransformerHeaven/uVw_skewPhirMs.h>
#include <dtTransformerHeaven/uVw_phiMs.h>
#include <dtTransformerHeaven/uVw_deltaMs.h>
#include <dtTransformerHeaven/averagePoints.h>
#include <dtTransformerHeaven/projectOnSurface.h>
#include <dtTransformerHeaven/reparamInSurface.h>
#include <dtTransformerHeaven/pickVec3dTwoDRangePercent.h>
#include <dtTransformerHeaven/analyticAddNormal.h>
#include <dtTransformerHeaven/discreteAddNormal.h>
#include <dtTransformerHeaven/approxInSurface.h>
#include <dtTransformerHeaven/normalOffsetInSurface.h>
#include <dtTransformerHeaven/closeGapsArithmetic.h>
#include <dtTransformerHeaven/scale.h>
#include <dtTransformerHeaven/xYz_rPhiZ.h>
#include <dtTransformerHeaven/pickMap2dTo3dRectanglePercent.h>
#include <dtTransformerHeaven/reverse.h>
#include <dtTransformerHeaven/pickMap1dTo3dLinePercent.h>
#include <dtTransformerHeaven/applyVec3dThreeD.h>
#include <dtTransformerHeaven/xYz_localCoordinates.h>
#include <dtTransformerHeaven/pickVec3dTwoDRectanglePercent.h>

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
#include <jsonHeaven/jsonPrimitive.h>
using namespace dtOO;
#ifdef DTOO_HAS_PYTHONOCC
  #include <TColStd_module.hxx>
  #include <TCollection_module.hxx>
  #include <Message_module.hxx>
  #include <Geom_module.hxx>
  #include <TColgp_module.hxx>
#endif
%}
%feature("autodoc", "1");

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

%include <std_vector.i>
%include <std_string.i>
%include <exception.i>
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
  catch (std::exception &e) {
    PyErr_SetString( PyExc_Exception, const_cast<char*>(e.what()) );
    SWIG_fail;
	}    
  catch (...) {
    PyErr_SetString(PyExc_Exception, "dtOOPythonSWIG catch exception");
    SWIG_fail;
	}
}
    
%include dtOOTypeDef.h
%include logMe/dtMacros.h
%include interfaceHeaven/ptrHandling.h
namespace dtOO {
  class logMe {
    public:
      static std::string initLog( std::string const & logFileName );
  };
}
%include interfaceHeaven/labelHandling.h
%include interfaceHeaven/optionHandling.h
namespace dtOO {
  class dtKernel;  
}
namespace CGAL {
  template <class R_ > 
  class Point_3 {
    public:
      virtual ~Point_3();
      Point_3(
        const ::dtOO::dtReal& x, 
        const ::dtOO::dtReal& y, 
        const ::dtOO::dtReal& z
      );
      Point_3( const Point_3& p );      
      ::dtOO::dtReal x() const;
      ::dtOO::dtReal y() const;
      ::dtOO::dtReal z() const;
  };
  template <class R_ > 
  class Vector_3 {
    public:
      virtual ~Vector_3();
      Vector_3(
        const ::dtOO::dtReal& x, 
        const ::dtOO::dtReal& y, 
        const ::dtOO::dtReal& z
      );
      Vector_3( const Vector_3& p );      
      ::dtOO::dtReal x() const;
      ::dtOO::dtReal y() const;
      ::dtOO::dtReal z() const;
  };
  template <class R_ > 
  class Point_2 {
    public:
      virtual ~Point_2();
      Point_2(
        const ::dtOO::dtReal& x, 
        const ::dtOO::dtReal& y
      );
      Point_2( const Point_2& p );      
      ::dtOO::dtReal x() const;
      ::dtOO::dtReal y() const;
  };
  template <class R_ > 
  class Vector_2 {
    public:
      virtual ~Vector_2();
      Vector_2(
        const ::dtOO::dtReal& x, 
        const ::dtOO::dtReal& y
      );
      Vector_2( const Vector_2& p );      
      ::dtOO::dtReal x() const;
      ::dtOO::dtReal y() const;
  };
}
namespace dtOO {
  %template(dtPoint3)       ::CGAL::Point_3< dtKernel >;
  %template(dtVector3)      ::CGAL::Vector_3< dtKernel >;
  %template(dtPoint2)       ::CGAL::Point_2< dtKernel >;
  %template(dtVector2)      ::CGAL::Vector_2< dtKernel >;
}
%include dtLinearAlgebra.h
%include baseContainerHeaven/baseContainer.h
%include baseContainerHeaven/pointContainer.h
%include baseContainerHeaven/vectorContainer.h
%include baseContainerHeaven/transformerContainer.h
%include constValueHeaven/constValue.h
%include analyticFunctionHeaven/analyticFunction.h
%include analyticGeometryHeaven/analyticGeometry.h
%include boundedVolume.h
%include dtCase.h
%include dtPlugin.h
%include <jsonHeaven/jsonPrimitive.h>  
namespace dtOO {
  %template(vectorInt)              ::std::vector< dtInt >;
  %template(vectorReal)             ::std::vector< dtReal >;
  %template(vectorStr)              ::std::vector< std::string >;
  %template(vectorJsonPrimitive)    ::std::vector< jsonPrimitive >;
  %template(vectorConstValue)       ::std::vector< constValue * >;
  %template(vectorAnalyticFunction) ::std::vector< analyticFunction * >;
  %template(vectorAnalyticGeometry) ::std::vector< analyticGeometry * >;
  %template(vectorBoundedVolume)    ::std::vector< boundedVolume * >;
  %template(vectorDtCase)           ::std::vector< dtCase * >;
  %template(vectorDtPlugin)         ::std::vector< dtPlugin * >;
  
  template < typename T >
  class vectorHandling : public std::vector< T > {
    public:
      T * set( T const & toSet);
  };
  %template(vectorHandlingConstValue)       vectorHandling< constValue * >;
  %template(vectorHandlingAnalyticFunction) vectorHandling< analyticFunction * >;
  %template(vectorHandlingAnalyticGeometry) vectorHandling< analyticGeometry * >;
  %template(vectorHandlingBoundedVolume)    vectorHandling< boundedVolume * >;
  %template(vectorHandlingDtCase)           vectorHandling< dtCase * >;
  %template(vectorHandlingDtPlugin)         vectorHandling< dtPlugin * >;
  
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
%template(lookupStr) dtOO::jsonPrimitive::lookup< std::string >;
%template(lookupInt) dtOO::jsonPrimitive::lookup< dtOO::dtInt >;
%template(lookupReal) dtOO::jsonPrimitive::lookup< dtOO::dtReal >;
%template(lookupJsonPrimitive) dtOO::jsonPrimitive::lookup< dtOO::jsonPrimitive >;
%template(lookupVectorInt) dtOO::jsonPrimitive::lookup< std::vector< dtOO::dtInt > >;
%template(lookupVectorReal) dtOO::jsonPrimitive::lookup< std::vector< dtOO::dtReal > >;
%template(lookupVectorStr) dtOO::jsonPrimitive::lookup< std::vector< std::string > >;
%template(lookupVectorJsonPrimitive) dtOO::jsonPrimitive::lookup< std::vector< dtOO::jsonPrimitive > >;

%include <geometryEngine/dtSurface.h>
%include <geometryEngine/dtOCCSurfaceBase.h>
%include <geometryEngine/dtOCCSurface.h>
%include <geometryEngine/dtOCCBSplineSurface.h>

//
// Classes has to be not abstract, otherwise no constructors will be created, 
// see https://www.swig.org/Doc4.0/SWIGPlus.html#SWIGPlus_nn9
//
%feature("notabstract") scaMultiOneD;
%feature("notabstract") scaMuParserOneD;
%feature("notabstract") scaCurve2dOneD;
%feature("notabstract") scaMuParserTwoD;
%feature("notabstract") transIntCube;
%feature("notabstract") scaMuParserThreeD;
%feature("notabstract") vec2dCurve2dOneD;
%feature("notabstract") vec2dSurface2dTwoD;
%feature("notabstract") vec2dMuParserTwoD;
%feature("notabstract") vec2dBiLinearTwoD;
%feature("notabstract") vec2dMultiBiLinearTwoD;
%feature("notabstract") vec3dSurfaceTwoD;
%feature("notabstract") vec3dThickedTwoD;
%feature("notabstract") vec3dMuParserTwoD;
%feature("notabstract") vec3dMuParserOneD;
%feature("notabstract") vec3dCurveOneD;
%feature("notabstract") vec3dCurve2dInSurfaceOneD;
%feature("notabstract") vec3dTransVolThreeD;
%feature("notabstract") vec3dMuParserThreeD;
%feature("notabstract") vec3dBoxThreeD;
%feature("notabstract") vec3dTriLinearThreeD;
%feature("notabstract") vec3dBiLinearTwoD;
%feature("notabstract") scaTanhGradingOneD;
%feature("notabstract") scaTanhUnitGradingOneD;

%include <analyticFunctionHeaven/aFX.h>
%include <analyticFunctionHeaven/aFY.h>
%include <analyticFunctionHeaven/scaFunction.h>
%include <analyticFunctionHeaven/scaOneD.h>
%include analyticFunctionHeaven/scaMultiOneD.h
%include <analyticFunctionHeaven/scaMuParserOneD.h>
%include analyticFunctionHeaven/scaCurve2dOneD.h
%include analyticFunctionHeaven/scaTwoD.h
%include analyticFunctionHeaven/scaMuParserTwoD.h
%include analyticFunctionHeaven/transIntCube.h
%include analyticFunctionHeaven/scaThreeD.h
%include analyticFunctionHeaven/scaMuParserThreeD.h
%include analyticFunctionHeaven/vec2dFunction.h
%include analyticFunctionHeaven/vec2dOneD.h
%include analyticFunctionHeaven/vec2dCurve2dOneD.h
%include analyticFunctionHeaven/vec2dTwoD.h
%include analyticFunctionHeaven/vec2dSurface2dTwoD.h
%include analyticFunctionHeaven/vec2dMuParserTwoD.h
%include analyticFunctionHeaven/vec2dBiLinearTwoD.h
%include analyticFunctionHeaven/vec2dMultiBiLinearTwoD.h
%include analyticFunctionHeaven/vec3dFunction.h
%include analyticFunctionHeaven/vec3dTwoD.h
%include analyticFunctionHeaven/vec3dSurfaceTwoD.h
%include analyticFunctionHeaven/vec3dThickedTwoD.h
%include analyticFunctionHeaven/vec3dMuParserTwoD.h
%include analyticFunctionHeaven/vec3dOneD.h
%include analyticFunctionHeaven/vec3dMuParserOneD.h
%include analyticFunctionHeaven/vec3dCurveOneD.h
%include analyticFunctionHeaven/vec3dCurve2dInSurfaceOneD.h
%include analyticFunctionHeaven/vec3dThreeD.h
//%include analyticFunctionHeaven/vec3dMultiThreeD.h
%include analyticFunctionHeaven/vec3dTransVolThreeD.h
%include analyticFunctionHeaven/vec3dMuParserThreeD.h
%include analyticFunctionHeaven/vec3dBoxThreeD.h
%include analyticFunctionHeaven/vec3dTriLinearThreeD.h
%include analyticFunctionHeaven/vec3dBiLinearTwoD.h
%include analyticFunctionHeaven/scaTanhGradingOneD.h
%include analyticFunctionHeaven/scaTanhUnitGradingOneD.h

%include dtTransformerHeaven/dtTransformer.h
%include dtTransformerHeaven/dtTransformerInvThreeD.h
%include dtTransformerHeaven/thicknessIncreasing.h
%include dtTransformerHeaven/biThicknessIncreasing.h
%include dtTransformerHeaven/doNothing.h
%include dtTransformerHeaven/offset.h
%include dtTransformerHeaven/translate.h
%include dtTransformerHeaven/radialTranslate.h
%include dtTransformerHeaven/rotate.h
%include dtTransformerHeaven/pickMap3dTo3dRangePercent.h
%include dtTransformerHeaven/pickMap2dTo3dRangePercent.h
%include dtTransformerHeaven/addConstCoordinate.h
%include dtTransformerHeaven/makePolynomial.h
%include dtTransformerHeaven/predefinedExtension.h
%include dtTransformerHeaven/closeGaps.h
%include dtTransformerHeaven/pickLengthRange.h
%include dtTransformerHeaven/pickLengthPercentRange.h
%include dtTransformerHeaven/uVw_phirMs.h
%include dtTransformerHeaven/uVw_skewPhirMs.h
%include dtTransformerHeaven/uVw_phiMs.h
%include dtTransformerHeaven/uVw_deltaMs.h
%include dtTransformerHeaven/averagePoints.h
%include dtTransformerHeaven/projectOnSurface.h
%include dtTransformerHeaven/reparamInSurface.h
%include dtTransformerHeaven/pickVec3dTwoDRangePercent.h
%include dtTransformerHeaven/analyticAddNormal.h
%include dtTransformerHeaven/discreteAddNormal.h
%include dtTransformerHeaven/approxInSurface.h
%include dtTransformerHeaven/normalOffsetInSurface.h
%include dtTransformerHeaven/closeGapsArithmetic.h
%include dtTransformerHeaven/scale.h
%include dtTransformerHeaven/xYz_rPhiZ.h
%include dtTransformerHeaven/pickMap2dTo3dRectanglePercent.h
%include dtTransformerHeaven/reverse.h
%include dtTransformerHeaven/pickMap1dTo3dLinePercent.h
%include dtTransformerHeaven/applyVec3dThreeD.h
%include dtTransformerHeaven/xYz_localCoordinates.h
%include dtTransformerHeaven/pickVec3dTwoDRectanglePercent.h
        
%include analyticGeometryHeaven/map2dTo3d.h
%include analyticGeometryHeaven/analyticSurface.h
