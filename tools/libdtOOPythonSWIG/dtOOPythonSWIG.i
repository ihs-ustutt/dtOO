%feature("autodoc", "1");

%include ../../doc/swig_includeAll.i

%module dtOOPythonSWIG

%warnfilter(401) NCollection_DataMap;
%warnfilter(401) NCollection_Sequence;
%warnfilter(401) NCollection_IndexedDataMap;
%warnfilter(401) NCollection_IndexedMap;
%warnfilter(401) NCollection_TListIterator;
%warnfilter(401) NCollection_List;
%warnfilter(401) NCollection_Map;
%warnfilter(325) Iterator;
%warnfilter(325) MapNode;
%warnfilter(325) Node;
%warnfilter(325) IndexedMapNode;
%warnfilter(325) DoubleMapNode;
%warnfilter(325) ObjBnd;
%warnfilter(325) TreeNode;
%warnfilter(325) DataMapNode;
%warnfilter(325) Selector;
%warnfilter(325) ConstIterator;
%warnfilter(325) rebind;
%warnfilter(325) StreamBuffer;
%warnfilter(325) NCollection_CellFilter_Inspector;
%warnfilter(325) Callback;

%{
#include <logMe/logMe.h>
#include <mainConceptFwd.h>
#include <dtOOTypeDef.h>
namespace dtOO {
  typedef double                    dtReal;
  typedef int                       dtInt;
  typedef unsigned int              dtUnsInt;
  typedef long unsigned int         dtLongUnsInt;
}
#include <exceptionHeaven/eGeneral.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/optionHandling.h>
#include <interfaceHeaven/vectorHandling.h>
#include <interfaceHeaven/labeledVectorHandling.h>
#include <interfaceHeaven/lVHOSubject.h>
#include <interfaceHeaven/lVHOInterface.h>
#include <interfaceHeaven/lVHOjsonLoad.h>
#include <gmsh/GModel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_3.h>  
#include <dtLinearAlgebra.h>
namespace CGAL {
  typedef Epick Exact_predicates_inexact_constructions_kernel;
}
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
#include <constValueHeaven/sliderFloatParam.h>
#include <constValueHeaven/intParam.h>
#include <constValueHeaven/constrainedFloatParam.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/analyticFunctionCompound.h>

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
#include <analyticFunctionHeaven/scaOneDPolyInterface.h>
#include <analyticFunctionHeaven/scaTanhGradingOneD.h>
#include <analyticFunctionHeaven/scaTanhUnitGradingOneD.h>
namespace dtOO {
  typedef analyticFunctionCompound< scaTanhGradingOneD > scaTanhGradingOneDCompound;
}

#include <analyticFunctionHeaven/aFBuilder/float_scaOneDPoint.h>
#include <analyticFunctionHeaven/aFBuilder/vec3dTwoD_normalOffset.h>
#include <analyticFunctionHeaven/aFBuilder/dtPoint3_vec3dTwoD.h>
#include <analyticFunctionHeaven/aFBuilder/vec3dTwoD_closeArithmetic.h>
#include <analyticFunctionHeaven/aFBuilder/x_vec3dTwoDClosestPointToPoint.h>
#include <analyticFunctionHeaven/aFBuilder/x_vec3dClosestPointToPoint.h>
#include <analyticFunctionHeaven/aFBuilder/vec3dTransVolThreeD_skinBSplineSurfaces.h>

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
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/analyticRotatingMap1dTo3d.h>
#include <analyticGeometryHeaven/infinityMap3dTo3d.h>
#include <analyticGeometryHeaven/vec3dOneDInMap3dTo3d.h>
#include <analyticGeometryHeaven/vec3dTwoDInMap3dTo3d.h>
#include <analyticGeometryHeaven/vec3dThreeDInMap3dTo3d.h>
#include <analyticGeometryHeaven/rotatingMap2dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/map2dTo3d_constructMarginFaces.h>
#include <analyticGeometryHeaven/aGBuilder/dtPoint3_map1dTo3dEquidistantPoint.h>
#include <analyticGeometryHeaven/aGBuilder/dtPoint3_map1dTo3dPoint.h>
#include <analyticGeometryHeaven/aGBuilder/map1dTo3d_normalOffsetMap1dTo3dInMap2dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/map1dTo3d_closeGapsArithmetic.h>
#include <analyticGeometryHeaven/aGBuilder/dtPoint3_straightIntersectToMap2dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/map2dTo3d_approximateMap2dTo3dInMap3dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/map2dTo3d_fullExtentInMap3dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/dtPoint3_map2dTo3dPoint.h>
#include <analyticGeometryHeaven/aGBuilder/pairU_map1dTo3dClosestPointToMap1dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/uv_map2dTo3dClosestPointToPoint.h>
#include <analyticGeometryHeaven/aGBuilder/uvw_map3dTo3dClosestPointToPoint.h>
#include <analyticGeometryHeaven/aGBuilder/pairUUV_map1dTo3dClosestPointToMap2dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/float_map1dTo3dPointConstCartesian.h>
#include <analyticGeometryHeaven/aGBuilder/trans6SidedCube_splitTrans6SidedCube.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/dtOCCBSplineSurface.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCBSplineCurve.h>
#include <geometryEngine/geoBuilder/bSplineCurve_curveConnectConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h>
#include <geometryEngine/geoBuilder/geomShape_readOCC.h>
#include <geometryEngine/geoBuilder/surfaceOfRevolution_curveRotateConstructOCC.h>
#include <geometryEngine/geoBuilder/rectangularTrimmedSurface_curveRotateConstructOCC.h>
#include <geometryEngine/geoBuilder/circle_radiusCoordinateSystemConstructOCC.h>
#include <geometryEngine/geoBuilder/trimmedCurve_uBounds.h>
#include <geometryEngine/geoBuilder/bSplineSurface_skinConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve_normalOffsetGeomCurveOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurface_exchangeSurfaceConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurface_geomCurveFillConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve_poleWeightKnotMultOrderConstructOCC.h>
#include <jsonHeaven/aFJsonBuilder/bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder.h>
#include <meshEngine/dtGmshModel.h>
#include <bVObserver/bVOInterface.h>
#include <bVObserver/bVOSubject.h>
#include <boundedVolume.h>
#include <bVObserver/bVONameRegions.h>
#include <bVObserver/bVOAnalyticGeometryToFace.h>
#include <bVObserver/bVOAddFace.h>
#include <bVObserver/bVOAddInternalEdge.h>
#include <bVObserver/bVOTransfiniteRegions.h>
#include <bVObserver/bVORecombineRecursiveRegions.h>
#include <bVObserver/bVOSetPrescribedMeshSizeAtPoints.h>
#include <bVObserver/bVOSetNElements.h>
#include <bVObserver/bVOSetGradingToFaceRule.h>
#include <bVObserver/bVOSetGrading.h>
#include <bVObserver/bVOSetPrescribedFirstElementSize.h>
#include <bVObserver/bVOReadMSH.h>
#include <bVObserver/bVOSetRotationalPeriodicity.h>
#include <bVObserver/bVOWriteMSH.h>
#include <bVObserver/bVOFaceToPatchRule.h>
#include <bVObserver/bVORemoveElements.h>
#include <bVObserver/bVOOrientCellVolumes.h>
#include <bVObserver/bVODumpModel.h>
#include <bVObserver/bVOTransfiniteFaces.h>
#include <bVObserver/bVORecombine.h>
#include <meshEngine/dtMeshOperator.h>
#include <bVObserver/bVOMeshRule.h>
#include <gmshBoundedVolume.h>
#include <map3dTo3dGmsh.h>
#include <customGmsh.h>
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
%include <std_pair.i>
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
//%include dtOOTypeDef.h
namespace dtOO {
  typedef double                    dtReal;
  typedef int                       dtInt;
  typedef unsigned int              dtUnsInt;
  typedef long unsigned int         dtLongUnsInt;
}
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
%include <gmsh/GModel.h>

namespace CGAL {
  class Epick;
  typedef Epick Exact_predicates_inexact_constructions_kernel;
}
namespace dtOO {
  typedef ::CGAL::Exact_predicates_inexact_constructions_kernel dtKernel;
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
%include geometryEngine/dtCurve.h
%include geometryEngine/dtSurface.h
%include baseContainerHeaven/baseContainer.h
%include baseContainerHeaven/pointContainer.h
%include baseContainerHeaven/vectorContainer.h
%include baseContainerHeaven/transformerContainer.h
%include constValueHeaven/constValue.h
%include analyticFunctionHeaven/analyticFunction.h
%include analyticGeometryHeaven/analyticGeometry.h
%include bVObserver/bVOInterface.h
%include bVObserver/bVOSubject.h
%include boundedVolume.h
%include bVObserver/bVONameRegions.h
%include bVObserver/bVOAnalyticGeometryToFace.h
%include bVObserver/bVOAddFace.h
%include bVObserver/bVOAddInternalEdge.h
%include bVObserver/bVOTransfiniteRegions.h
%include bVObserver/bVORecombineRecursiveRegions.h
%include bVObserver/bVOSetPrescribedMeshSizeAtPoints.h
%include bVObserver/bVOSetNElements.h
%include bVObserver/bVOSetGradingToFaceRule.h
%include bVObserver/bVOSetGrading.h
%include bVObserver/bVOSetPrescribedFirstElementSize.h
%include bVObserver/bVOReadMSH.h
%include bVObserver/bVOSetRotationalPeriodicity.h
%include bVObserver/bVOWriteMSH.h
%include bVObserver/bVOFaceToPatchRule.h
%include bVObserver/bVORemoveElements.h
%include bVObserver/bVOOrientCellVolumes.h
%include bVObserver/bVODumpModel.h
%include bVObserver/bVOTransfiniteFaces.h
%include bVObserver/bVORecombine.h
%include meshEngine/dtMeshOperator.h
%include bVObserver/bVOMeshRule.h
%include gmshBoundedVolume.h
%include map3dTo3dGmsh.h
%include customGmsh.h
%include dtCase.h
%include dtPlugin.h
%include jsonHeaven/jsonPrimitive.h
%include interfaceHeaven/lVHOSubject.h
namespace dtOO {
  %template(vectorInt)              ::std::vector< dtInt >;
  %template(vectorBool)             ::std::vector< bool >;
  %template(vectorReal)             ::std::vector< dtReal >;
  %template(vectorStr)              ::std::vector< std::string >;
  %template(vectorJsonPrimitive)    ::std::vector< jsonPrimitive >;
  %template(vectorLabelHandling)    ::std::vector< labelHandling * >;
  %template(vectorConstValue)       ::std::vector< constValue * >;
  %template(vectorAnalyticFunction) ::std::vector< analyticFunction * >;
  %template(vectorAnalyticGeometry) ::std::vector< analyticGeometry * >;
  %template(vectorBoundedVolume)    ::std::vector< boundedVolume * >;
  %template(vectorDtCase)           ::std::vector< dtCase * >;
  %template(vectorDtPlugin)         ::std::vector< dtPlugin * >;
  %template(vectorDtPoint2)         ::std::vector< dtPoint2 >;
  %template(vectorDtVector2)        ::std::vector< dtVector2 >;
  %template(vectorDtPoint3)         ::std::vector< dtPoint3 >;
  %template(vectorDtVector3)        ::std::vector< dtVector3 >;
  %template(vectorDtCurve)          ::std::vector< dtCurve * >;
  %template(vectorDtSurface)        ::std::vector< dtSurface * >;

  %template(vectorConstLabelHandling)    ::std::vector< labelHandling const * >;
  %template(vectorConstConstValue)       ::std::vector< constValue const * >;
  %template(vectorConstAnalyticFunction) ::std::vector< analyticFunction const * >;
  %template(vectorConstAnalyticGeometry) ::std::vector< analyticGeometry const * >;
  %template(vectorConstBoundedVolume)    ::std::vector< boundedVolume const * >;
  %template(vectorConstDtCase)           ::std::vector< dtCase const * >;
  %template(vectorConstDtPlugin)         ::std::vector< dtPlugin const * >;
  %template(vectorConstDtCurve)          ::std::vector< dtCurve const * >;
  %template(vectorConstDtSurface)        ::std::vector< dtSurface const * >;
  
  template < typename T >
  class vectorHandling : public std::vector< T > {
    public:
      using std::vector< T >::push_back;
  };
  %extend vectorHandling {
    T __getitem__(int const & ii) {
      return $self->operator[](ii);
    }    
  }
  typedef vectorHandling< constValue * >       vH_constValue;
  typedef vectorHandling< analyticFunction * > vH_analyticFunction;
  typedef vectorHandling< analyticGeometry * > vH_analyticGeometry;
  typedef vectorHandling< boundedVolume * >    vH_boundedVolume;
  typedef vectorHandling< dtCase * >           vH_dtCase;
  typedef vectorHandling< dtPlugin * >         vH_dtPlugin;  
  typedef vectorHandling< dtCurve * >          vH_dtCurve;
  typedef vectorHandling< dtSurface * >        vH_dtSurface;
  %template(vectorHandlingLabelHandling)    vectorHandling< labelHandling * >;
  %template(vectorHandlingConstValue)       vectorHandling< constValue * >;
  %template(vectorHandlingAnalyticFunction) vectorHandling< analyticFunction * >;
  %template(vectorHandlingAnalyticGeometry) vectorHandling< analyticGeometry * >;
  %template(vectorHandlingBoundedVolume)    vectorHandling< boundedVolume * >;
  %template(vectorHandlingDtCase)           vectorHandling< dtCase * >;
  %template(vectorHandlingDtPlugin)         vectorHandling< dtPlugin * >;
  %template(vectorHandlingDtCurve)          vectorHandling< dtCurve * >;
  %template(vectorHandlingDtSurface)        vectorHandling< dtSurface * >;

  %template(vectorHandlingConstLabelHandling)    vectorHandling< labelHandling const * >;
  %template(vectorHandlingConstConstValue)       vectorHandling< constValue const * >;
  %template(vectorHandlingConstAnalyticFunction) vectorHandling< analyticFunction const * >;
  %template(vectorHandlingConstAnalyticGeometry) vectorHandling< analyticGeometry const * >;
  %template(vectorHandlingConstBoundedVolume)    vectorHandling< boundedVolume const * >;
  %template(vectorHandlingConstDtCase)           vectorHandling< dtCase const * >;
  %template(vectorHandlingConstDtPlugin)         vectorHandling< dtPlugin const * >;
  %template(vectorHandlingConstDtCurve)          vectorHandling< dtCurve const * >;
  %template(vectorHandlingConstDtSurface)        vectorHandling< dtSurface const * >;
  
  template < typename T >
  class labeledVectorHandling : public vectorHandling< T >, public lVHOSubject {
    public:
      typedef typename std::vector< T >::iterator iterator;
      typedef typename std::vector< T >::reference reference;
      typedef typename std::vector< T >::const_reference const_reference;    
    public:
      void set( T const & toSet);
      const_reference get( std::string const & label) const;
      ::std::vector< dtOO::dtInt > getIndices( std::string const & label) const;
      std::string getLabel( dtInt const pos ) const;
      std::vector< std::string > labels( void ) const;
      dtInt getPosition( std::string const label) const;
      void dump(void) const;          
  };
  %extend labeledVectorHandling {
    T __getitem__(std::string const & str) {
      return $self->operator[](str);
    } 
    T __getitem__(int const & ii) {
      return $self->operator[](ii);
    }    
  }
  
  typedef labeledVectorHandling< constValue * >       lvH_constValue;
  typedef labeledVectorHandling< analyticFunction * > lvH_analyticFunction;
  typedef labeledVectorHandling< analyticGeometry * > lvH_analyticGeometry;
  typedef labeledVectorHandling< boundedVolume * >    lvH_boundedVolume;
  typedef labeledVectorHandling< dtCase * >           lvH_dtCase;
  typedef labeledVectorHandling< dtPlugin * >         lvH_dtPlugin;  
  %template(labeledVectorHandlingLabelHandling)    labeledVectorHandling< labelHandling * >;
  %template(labeledVectorHandlingConstValue)       labeledVectorHandling< constValue * >;
  %template(labeledVectorHandlingAnalyticFunction) labeledVectorHandling< analyticFunction * >;
  %template(labeledVectorHandlingAnalyticGeometry) labeledVectorHandling< analyticGeometry * >;
  %template(labeledVectorHandlingBoundedVolume)    labeledVectorHandling< boundedVolume * >;
  %template(labeledVectorHandlingDtCase)           labeledVectorHandling< dtCase * >;
  %template(labeledVectorHandlingDtPlugin)         labeledVectorHandling< dtPlugin * >;
}

%include <interfaceHeaven/lVHOInterface.h>
%include <interfaceHeaven/lVHOjsonLoad.h>
%include <xmlHeaven/dtXmlParserBase.h>
%include <xmlHeaven/dtXmlParser.h>     
%template(lookupStr) dtOO::jsonPrimitive::lookup< std::string >;
%template(lookupInt) dtOO::jsonPrimitive::lookup< dtOO::dtInt >;
%template(lookupBool) dtOO::jsonPrimitive::lookup< bool >;
%template(lookupReal) dtOO::jsonPrimitive::lookup< dtOO::dtReal >;
%template(lookupJsonPrimitive) dtOO::jsonPrimitive::lookup< dtOO::jsonPrimitive >;
%template(lookupDtPoint2) dtOO::jsonPrimitive::lookup< dtOO::dtPoint2 >;
%template(lookupDtPoint3) dtOO::jsonPrimitive::lookup< dtOO::dtPoint3 >;
%template(lookupDtVector3) dtOO::jsonPrimitive::lookup< dtOO::dtVector3 >;
%template(lookupVectorInt) dtOO::jsonPrimitive::lookup< std::vector< dtOO::dtInt > >;
%template(lookupVectorBool) dtOO::jsonPrimitive::lookup< std::vector< bool > >;
%template(lookupVectorReal) dtOO::jsonPrimitive::lookup< std::vector< dtOO::dtReal > >;
%template(lookupVectorStr) dtOO::jsonPrimitive::lookup< std::vector< std::string > >;
%template(lookupVectorJsonPrimitive) dtOO::jsonPrimitive::lookup< std::vector< dtOO::jsonPrimitive > >;
%template(appendStr) dtOO::jsonPrimitive::append< std::string >;
%template(appendInt) dtOO::jsonPrimitive::append< dtOO::dtInt >;
%template(appendBool) dtOO::jsonPrimitive::append< bool >;
%template(appendJsonPrimitive) dtOO::jsonPrimitive::append< dtOO::jsonPrimitive >;
%template(appendReal) dtOO::jsonPrimitive::append< dtOO::dtReal >;
%template(appendDtPoint2) dtOO::jsonPrimitive::append< dtOO::dtPoint2 >;
%template(appendDtPoint3) dtOO::jsonPrimitive::append< dtOO::dtPoint3 >;
%template(appendDtVector2) dtOO::jsonPrimitive::append< dtOO::dtVector2 >;
%template(appendDtVector3) dtOO::jsonPrimitive::append< dtOO::dtVector3 >;
%template(appendAnalyticFunction) dtOO::jsonPrimitive::append< dtOO::analyticFunction const * >;
%template(appendAnalyticGeometry) dtOO::jsonPrimitive::append< dtOO::analyticGeometry const * >;
%template(appendDtTransformer) dtOO::jsonPrimitive::append< dtOO::dtTransformer const * >;
%template(appendVectorInt) dtOO::jsonPrimitive::append< std::vector< dtOO::dtInt > >;

%include constValueHeaven/sliderFloatParam.h
%include constValueHeaven/intParam.h
%include constValueHeaven/constrainedFloatParam.h

%include geometryEngine/dtOCCCurveBase.h
%include geometryEngine/dtOCCCurve.h
%include geometryEngine/dtOCCBSplineCurve.h
%include geometryEngine/dtOCCSurfaceBase.h
%include geometryEngine/dtOCCSurface.h
%include geometryEngine/dtOCCBSplineSurface.h
%include geometryEngine/geoBuilder/bSplineCurve_curveConnectConstructOCC.h
%include geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h
%include geometryEngine/geoBuilder/geomShape_readOCC.h
%include geometryEngine/geoBuilder/surfaceOfRevolution_curveRotateConstructOCC.h
%include geometryEngine/geoBuilder/rectangularTrimmedSurface_curveRotateConstructOCC.h
%include geometryEngine/geoBuilder/circle_radiusCoordinateSystemConstructOCC.h
%include geometryEngine/geoBuilder/trimmedCurve_uBounds.h
%include geometryEngine/geoBuilder/bSplineSurface_skinConstructOCC.h
%include geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h
%include geometryEngine/geoBuilder/bSplineCurve_normalOffsetGeomCurveOCC.h
%include geometryEngine/geoBuilder/bSplineSurface_exchangeSurfaceConstructOCC.h
%include geometryEngine/geoBuilder/bSplineSurface_geomCurveFillConstructOCC.h
%include geometryEngine/geoBuilder/bSplineCurve_poleWeightKnotMultOrderConstructOCC.h
%include jsonHeaven/aFJsonBuilder/bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder.h

%include meshEngine/dtGmshModel.h

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
%feature("notabstract") analyticFunctionCompound;

%include analyticFunctionHeaven/analyticFunctionCompound.h
%include analyticFunctionHeaven/aFX.h
%include analyticFunctionHeaven/aFY.h
%include analyticFunctionHeaven/scaFunction.h
%include analyticFunctionHeaven/scaOneD.h
%include analyticFunctionHeaven/scaMultiOneD.h
%include analyticFunctionHeaven/scaMuParserOneD.h
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
%include analyticFunctionHeaven/scaOneDPolyInterface.h
%include analyticFunctionHeaven/scaTanhGradingOneD.h
%include analyticFunctionHeaven/scaTanhUnitGradingOneD.h

namespace dtOO {
  %template(scaTanhGradingOneDCompound)    analyticFunctionCompound< scaTanhGradingOneD >;
  typedef analyticFunctionCompound< scaTanhGradingOneD > scaTanhGradingOneDCompound;
}
%include analyticFunctionHeaven/aFBuilder/float_scaOneDPoint.h
%include analyticFunctionHeaven/aFBuilder/vec3dTwoD_normalOffset.h
%include analyticFunctionHeaven/aFBuilder/dtPoint3_vec3dTwoD.h
%include analyticFunctionHeaven/aFBuilder/vec3dTwoD_closeArithmetic.h
%include analyticFunctionHeaven/aFBuilder/x_vec3dTwoDClosestPointToPoint.h
%include analyticFunctionHeaven/aFBuilder/x_vec3dClosestPointToPoint.h
%include analyticFunctionHeaven/aFBuilder/vec3dTransVolThreeD_skinBSplineSurfaces.h

namespace dtOO {
  %rename(applyDtPoint2) dtTransformer::apply(dtPoint2 const&) const;
  %rename(retractDtPoint2) dtTransformer::retract(dtPoint2 const&) const;
  %rename(applyDtPoint3) dtTransformer::apply(dtPoint3 const&) const;
  %rename(retractDtPoint3) dtTransformer::retract(dtPoint3 const&) const;
  %rename(applyAnalyticGeometry) dtTransformer::apply(analyticGeometry const * const) const;
  %rename(applyAnalyticFunction) dtTransformer::apply(analyticFunction const * const) const;
  %rename(applyLabeledVectorHandlingAnalyticFunction) dtTransformer::apply(lvH_analyticFunction const * const) const;
}       
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

%feature("notabstract") analyticCurve;
%feature("notabstract") analyticSurface;
%feature("notabstract") analyticRotatingMap1dTo3d;
%feature("notabstract") infinityMap3dTo3d;
%feature("notabstract") vec3dOneDInMap3dTo3d;
%feature("notabstract") vec3dTwoDInMap3dTo3d;
%feature("notabstract") vec3dThreeDInMap3dTo3d;
%feature("notabstract") rotatingMap2dTo3d;

%include analyticGeometryHeaven/map1dTo3d.h
%include analyticGeometryHeaven/map2dTo3d.h
%include analyticGeometryHeaven/map3dTo3d.h
%include analyticGeometryHeaven/analyticCurve.h
%include analyticGeometryHeaven/analyticSurface.h
%include analyticGeometryHeaven/analyticRotatingMap1dTo3d.h
%include analyticGeometryHeaven/infinityMap3dTo3d.h
%include analyticGeometryHeaven/vec3dOneDInMap3dTo3d.h
%include analyticGeometryHeaven/vec3dTwoDInMap3dTo3d.h
%include analyticGeometryHeaven/vec3dThreeDInMap3dTo3d.h
%include analyticGeometryHeaven/rotatingMap2dTo3d.h
namespace dtOO {
  %template(pairDtRealDtPoint2) ::std::pair< dtReal, dtPoint2 >;
}
%include analyticGeometryHeaven/aGBuilder/map2dTo3d_constructMarginFaces.h
%include analyticGeometryHeaven/aGBuilder/dtPoint3_map1dTo3dEquidistantPoint.h
%include analyticGeometryHeaven/aGBuilder/dtPoint3_map1dTo3dPoint.h
%include analyticGeometryHeaven/aGBuilder/map1dTo3d_normalOffsetMap1dTo3dInMap2dTo3d.h
%include analyticGeometryHeaven/aGBuilder/map1dTo3d_closeGapsArithmetic.h
%include analyticGeometryHeaven/aGBuilder/dtPoint3_straightIntersectToMap2dTo3d.h
%include analyticGeometryHeaven/aGBuilder/map2dTo3d_approximateMap2dTo3dInMap3dTo3d.h
%include analyticGeometryHeaven/aGBuilder/map2dTo3d_fullExtentInMap3dTo3d.h
%include analyticGeometryHeaven/aGBuilder/dtPoint3_map2dTo3dPoint.h
%include analyticGeometryHeaven/aGBuilder/pairU_map1dTo3dClosestPointToMap1dTo3d.h
%include analyticGeometryHeaven/aGBuilder/uv_map2dTo3dClosestPointToPoint.h
%include analyticGeometryHeaven/aGBuilder/uvw_map3dTo3dClosestPointToPoint.h
%include analyticGeometryHeaven/aGBuilder/pairUUV_map1dTo3dClosestPointToMap2dTo3d.h
%include analyticGeometryHeaven/aGBuilder/float_map1dTo3dPointConstCartesian.h
%include analyticGeometryHeaven/aGBuilder/trans6SidedCube_splitTrans6SidedCube.h
