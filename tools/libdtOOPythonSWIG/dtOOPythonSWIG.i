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
#include <string>
#include <vector>
#include <logMe/logMe.h>
#include <mainConceptFwd.h>
namespace dtOO {
  template< typename T > class vectorHandling;
  typedef vectorHandling< constValue * >       vH_constValue;
  typedef vectorHandling< analyticFunction * > vH_analyticFunction;
  typedef vectorHandling< analyticGeometry * > vH_analyticGeometry;
  typedef vectorHandling< boundedVolume * >    vH_boundedVolume;
  typedef vectorHandling< dtCase * >           vH_dtCase;
  typedef vectorHandling< dtPlugin * >         vH_dtPlugin;  
  typedef vectorHandling< dtCurve * >          vH_dtCurve;
  typedef vectorHandling< dtSurface * >        vH_dtSurface;
  template< typename T > class labeledVectorHandling;
  typedef labeledVectorHandling< constValue * >       lvH_constValue;
  typedef labeledVectorHandling< analyticFunction * > lvH_analyticFunction;
  typedef labeledVectorHandling< analyticGeometry * > lvH_analyticGeometry;
  typedef labeledVectorHandling< boundedVolume * >    lvH_boundedVolume;
  typedef labeledVectorHandling< dtCase * >           lvH_dtCase;
  typedef labeledVectorHandling< dtPlugin * >         lvH_dtPlugin;  
}

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
#include <interfaceHeaven/twoDArrayHandling.h>
#include <interfaceHeaven/lVHOSubject.h>
#include <interfaceHeaven/lVHOInterface.h>
#include <interfaceHeaven/lVHOjsonLoad.h>
#include <interfaceHeaven/lVHOstateHandler.h>
#include <gmsh/GEntity.h>
#include <gmsh/GVertex.h>
#include <gmsh/GEdge.h>
#include <gmsh/GFace.h>
#include <gmsh/GRegion.h>
#include <gmsh/GModel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
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
#include <interfaceHeaven/renderInterface.h>
#include <discrete2dPoints.h>
#include <discrete3dPoints.h>
#include <discrete3dVector.h>
#include <solid2dLine.h>
#include <solid3dLine.h>
#include <solid3dSurface.h>
#include <unstructured3dMesh.h>
#include <unstructured3dSurfaceMesh.h>

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
#include <analyticFunctionHeaven/sca3PPointsBSplineOneD.h>
namespace dtOO {
  typedef analyticFunctionCompound< scaTanhGradingOneD >        scaTanhGradingOneDCompound;
  typedef analyticFunctionCompound< sca3PPointsBSplineOneD > sca3PPointsBSplineOneDCompound;
}

#include <analyticFunctionHeaven/aFBuilder/float_scaOneDPoint.h>
#include <analyticFunctionHeaven/aFBuilder/vec3dTwoD_normalOffset.h>
#include <analyticFunctionHeaven/aFBuilder/dtPoint3_vec3dTwoD.h>
#include <analyticFunctionHeaven/aFBuilder/vec3dTwoD_closeArithmetic.h>
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
#include <analyticGeometryHeaven/partRotatingMap2dTo3d.h>
#include <analyticGeometryHeaven/trans6SidedCube.h>
#include <analyticGeometryHeaven/translatingMap2dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/map2dTo3d_constructMarginFaces.h>
#include <analyticGeometryHeaven/aGBuilder/dtPoint3_map1dTo3dEquidistantPoint.h>
#include <analyticGeometryHeaven/aGBuilder/dtPoint3_map1dTo3dPoint.h>
#include <analyticGeometryHeaven/aGBuilder/map1dTo3d_normalOffsetMap1dTo3dInMap2dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/map1dTo3d_closeGapsArithmetic.h>
#include <analyticGeometryHeaven/aGBuilder/map2dTo3d_approximateMap2dTo3dInMap3dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/map2dTo3d_fullExtentInMap3dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/dtPoint3_map2dTo3dPoint.h>
#include <analyticGeometryHeaven/aGBuilder/uv_map2dTo3dClosestPointToPoint.h>
#include <analyticGeometryHeaven/aGBuilder/pairUUV_map1dTo3dClosestPointToMap2dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/float_map1dTo3dPointConstCartesian.h>
#include <analyticGeometryHeaven/aGBuilder/trans6SidedCube_splitTrans6SidedCube.h>
#include <analyticGeometryHeaven/aGBuilder/bool_map1dTo3dInMap2dTo3d.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/dtOCCBSplineSurface.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCBSplineCurve.h>
#include <geometryEngine/dtSurface2d.h>
#include <geometryEngine/dtOCCSurface2d.h>
#include <geometryEngine/dtOCCBSplineSurface2d.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/dtOCCCurve2dBase.h>
#include <geometryEngine/dtOCCCurve2d.h>
#include <geometryEngine/dtOCCBSplineCurve2d.h>

#include <geometryEngine/geoBuilder/bezierCurve_pointConstructOCC.h>
#include <geometryEngine/geoBuilder/bezierSurface_bezierCurveFillConstructOCC.h>
#include <geometryEngine/geoBuilder/bezierSurface_pointConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_angleDeltaXDeltaYConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_angleRatioDeltaYConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_angleRatioDeltaYPointOnePointTwoConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_curveConnectConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointInterpolateConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve_approxGeomCurve2dInGeomSurface.h>
#include <geometryEngine/geoBuilder/bSplineCurve_bSplineCurveSplitConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve_convertOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve_curveConnectConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve_normalOffsetGeomCurveOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructArcLengthParaOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointInterpolateConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve_poleWeightKnotMultOrderConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurface2d_bSplineCurve2dFillConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurface_bSplineCurveFillConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurface_bSplineSurfaceSplitConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurface_constrainedFillingConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurface_convertOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurface_exchangeSurfaceConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurface_extentSurfaceByLength.h>
#include <geometryEngine/geoBuilder/bSplineSurface_geomCurveFillConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurface_pipeConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurfaces_bSplineSurfaceSkinConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurface_skinConstructOCC.h>
#include <geometryEngine/geoBuilder/circle_radiusCoordinateSystemConstructOCC.h>
#include <geometryEngine/geoBuilder/compositeSurface_surfaceConnectConstructOCC.h>
#include <geometryEngine/geoBuilder/dtPoint2_surfaceEquidistantPoint.h>
#include <geometryEngine/geoBuilder/ellipse_radiusRadiusCoordinateSystemConstructOCC.h>
#include <geometryEngine/geoBuilder/geomCurve2d_baseConstructOCC.h>
#include <geometryEngine/geoBuilder/geomCurve2d_closeGapsArithmeticOCC.h>
#include <geometryEngine/geoBuilder/geomCurve2d_curve2dTranslateConstructOCC.h>
#include <geometryEngine/geoBuilder/geomCurve2d_ignoreZ.h>
#include <geometryEngine/geoBuilder/geomCurve2d_reparamInGeomSurfaceOCC.h>
#include <geometryEngine/geoBuilder/geomCurve_baseConstructOCC.h>
#include <geometryEngine/geoBuilder/geomCurve_closeGapsArithmeticOCC.h>
#include <geometryEngine/geoBuilder/geomCurve_convertGeomCurve2d.h>
#include <geometryEngine/geoBuilder/geomCurve_curveReverseConstructOCC.h>
#include <geometryEngine/geoBuilder/geomCurve_curveRotateConstructOCC.h>
#include <geometryEngine/geoBuilder/geomCurve_curveTranslateConstructOCC.h>
#include <geometryEngine/geoBuilder/geomCurve_projectOnGeomSurfaceOCC.h>
#include <geometryEngine/geoBuilder/geomFillBoundWithSurf_surfaceConstructOCC.h>
#include <geometryEngine/geoBuilder/geomFillSimpleBound_curveConstructOCC.h>
#include <geometryEngine/geoBuilder/geomShape_readOCC.h>
#include <geometryEngine/geoBuilder/geomSurface_baseConstructOCC.h>
#include <geometryEngine/geoBuilder/geomSurface_closeGapsArithmeticOCC.h>
#include <geometryEngine/geoBuilder/geomSurface_surfaceReverseConstructOCC.h>
#include <geometryEngine/geoBuilder/geomSurface_surfaceRotateConstructOCC.h>
#include <geometryEngine/geoBuilder/geomSurface_surfaceTranslateConstructOCC.h>
#include <geometryEngine/geoBuilder/rectangularTrimmedSurface_curveRotateConstructOCC.h>
#include <geometryEngine/geoBuilder/rectangularTrimmedSurface_uvBounds.h>
#include <geometryEngine/geoBuilder/surfaceOfRevolution_curveRotateConstructOCC.h>
#include <geometryEngine/geoBuilder/trimmedCurve2d_twoPointsConnectConstructOCC.h>
#include <geometryEngine/geoBuilder/trimmedCurve_twoPointsConnectConstructOCC.h>
#include <geometryEngine/geoBuilder/trimmedCurve_uBounds.h>
#include <geometryEngine/geoBuilder/u_geomCurveClosestPoint.h>
#include <geometryEngine/geoBuilder/uv_geomSurfaceClosestPoint.h>
#include <jsonHeaven/aFJsonBuilder.h>
#include <jsonHeaven/aFJsonBuilder/bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder.h>
#include <meshEngine/dtGmshVertex.h>
#include <meshEngine/dtGmshEdge.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshModel.h>
#include <bVObserver/bVOInterface.h>
#include <bVObserver/bVOSubject.h>
#include <boundedVolume.h>
#include <bVObserver/bVONameRegions.h>
#include <bVObserver/bVONameFaces.h>
#include <bVObserver/bVOAnalyticGeometryToFace.h>
#include <bVObserver/bVOAddFace.h>
#include <bVObserver/bVOAddInternalEdge.h>
#include <bVObserver/bVOTransfiniteRegions.h>
#include <bVObserver/bVORecombineRecursiveRegions.h>
#include <bVObserver/bVOSetPrescribedMeshSizeAtPoints.h>
#include <bVObserver/bVOSetNElements.h>
#include <bVObserver/bVOSetGradingToFaceRule.h>
#include <bVObserver/bVOSetGrading.h>
#include <bVObserver/bVOSetPrescribedElementSize.h>
#include <bVObserver/bVOReadMSH.h>
#include <bVObserver/bVOSetRotationalPeriodicity.h>
#include <bVObserver/bVOSetTranslationalPeriodicity.h>
#include <bVObserver/bVOWriteMSH.h>
#include <bVObserver/bVOFaceToPatchRule.h>
#include <bVObserver/bVORemoveElements.h>
#include <bVObserver/bVOOrientCellVolumes.h>
#include <bVObserver/bVODumpModel.h>
#include <bVObserver/bVOTransfiniteFaces.h>
#include <bVObserver/bVORecombine.h>
#include <meshEngine/dtMeshOperator.h>
#include <bVObserver/bVOMeshRule.h>
#include <bVObserver/bVOSetOrder.h>
#include <bVObserver/bVOWriteINP.h>
#include <gmshBoundedVolume.h>
#include <map3dTo3dGmsh.h>
#include <customGmsh.h>
#include <combineGmsh.h>
#include <dtCase.h>
#ifdef DTOO_HAS_OpenFOAM
  #include <ofOpenFOAMCase/ofOpenFOAMCase.h>
#endif    
 

#include <dtPlugin.h>
#include <parseHeaven/dtParser.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <xmlHeaven/dtXmlParser.h>
#include <jsonHeaven/jsonPrimitive.h>
#include <interfaceHeaven/dtBundle.h>
using namespace dtOO;
#ifdef DTOO_HAS_PYTHONOCC
  #include <TColStd_module.hxx>
  #include <TCollection_module.hxx>
  #include <Message_module.hxx>
  #include <Geom_module.hxx>
  #include <TColgp_module.hxx>
  #include <Standard_Persistent.hxx>
#endif
#include <attributionHeaven/floatAtt.h>
#include <attributionHeaven/pointGeometryDist.h>
#include <attributionHeaven/geometryGeometryDist.h>
#include <attributionHeaven/curveCurveDist.h>
#include <gslMinFloatAttr.h>
#include <gslGradMinFloatAttr.h>
%}

%include <std_except.i>
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

%include <std_list.i>
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
  catch ( eGeneral &e ) {
    PyErr_SetString( PyExc_Exception, const_cast<char*>(e.what()) );  
    SWIG_fail;
  }  
  catch ( std::exception &e ) {
    PyErr_SetString( PyExc_Exception, const_cast<char*>(e.what()) );
    SWIG_fail;
  }    
  catch ( ... ) {
    PyErr_SetString(PyExc_Exception, "dtOOPythonSWIG catch exception");
    SWIG_fail;
  }
}
namespace dtOO {
  template< typename T > class vectorHandling;
  typedef vectorHandling< constValue * >       vH_constValue;
  typedef vectorHandling< analyticFunction * > vH_analyticFunction;
  typedef vectorHandling< analyticGeometry * > vH_analyticGeometry;
  typedef vectorHandling< boundedVolume * >    vH_boundedVolume;
  typedef vectorHandling< dtCase * >           vH_dtCase;
  typedef vectorHandling< dtPlugin * >         vH_dtPlugin;  
  typedef vectorHandling< dtCurve * >          vH_dtCurve;
  typedef vectorHandling< dtSurface * >        vH_dtSurface;
  template< typename T > class labeledVectorHandling;
  typedef labeledVectorHandling< constValue * >       lvH_constValue;
  typedef labeledVectorHandling< analyticFunction * > lvH_analyticFunction;
  typedef labeledVectorHandling< analyticGeometry * > lvH_analyticGeometry;
  typedef labeledVectorHandling< boundedVolume * >    lvH_boundedVolume;
  typedef labeledVectorHandling< dtCase * >           lvH_dtCase;
  typedef labeledVectorHandling< dtPlugin * >         lvH_dtPlugin;  
}
namespace dtOO {
  typedef double                    dtReal;
  typedef int                       dtInt;
  typedef unsigned int              dtUnsInt;
  typedef long unsigned int         dtLongUnsInt;
}
%include logMe/dtMacros.h
namespace dtOO {
  class logMe {
    public:
      static std::string initLog( std::string const & logFileName );
  };
}
%include interfaceHeaven/labelHandling.h
%include interfaceHeaven/optionHandling.h
%include <gmsh/GEntity.h>
%include <gmsh/GVertex.h>
%include <gmsh/GEdge.h>
%include <gmsh/GFace.h>
%include <gmsh/GRegion.h>
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
  %extend Point_3 {
    ::dtOO::dtReal __getitem__( ::dtOO::dtInt const & ii) {
      return $self->operator[](ii);
    }
    ::std::vector< Point_3 > __lshift__( Point_3 const & el ) {
      ::std::vector< CGAL::Point_3< R_ > > vec;
      vec.push_back( *$self );
      vec.push_back( el );
      return vec;
    }
    // v = p - p
    ::CGAL::Vector_3< R_ > __sub__( ::CGAL::Point_3< R_ > const & v ) {
      return ::CGAL::Vector_3< R_ >(
        v.x() - $self->x(),
        v.y() - $self->y(),
        v.z() - $self->z()
      );
    }
    // p = p + v
    ::CGAL::Point_3< R_ > __add__( ::CGAL::Vector_3< R_ > const & v ) {
      return ::CGAL::Point_3< R_ >(
        v.x() + $self->x(),
        v.y() + $self->y(),
        v.z() + $self->z()
      );
    }   
  } 
 
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
  %extend Vector_3 {
    ::dtOO::dtReal __getitem__( ::dtOO::dtInt const & ii) {
      return $self->operator[](ii);
    }
    ::std::vector< Vector_3 > __lshift__( Vector_3 const & el ) {
      ::std::vector< CGAL::Vector_3< R_ > > vec;
      vec.push_back( *$self );
      vec.push_back( el );
      return vec;
    }
    // v = v * dtReal
    ::CGAL::Vector_3< R_ > __mul__( ::dtOO::dtReal const & mul ) {
      return ::CGAL::Vector_3< R_ >(
        mul * $self->x(), mul * $self->y(), mul * $self->z()
      );
    }
    // v = v - v
    ::CGAL::Vector_3< R_ > __sub__( ::CGAL::Vector_3 const & vv ) {
      return ::CGAL::Vector_3< R_ >(
        $self->x()-vv.x(), $self->y()-vv.y(), $self->z()-vv.z()
      );
    }
    // v = v + v
    ::CGAL::Vector_3< R_ > __add__( ::CGAL::Vector_3 const & vv ) {
      return ::CGAL::Vector_3< R_ >(
        $self->x()+vv.x(), $self->y()+vv.y(), $self->z()+vv.z()
      );
    }
   
  }

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
  %extend Point_2 {
    ::dtOO::dtReal __getitem__( ::dtOO::dtInt const & ii) {
      return $self->operator[](ii);
    }
    ::std::vector< Point_2 > __lshift__( Point_2 const & el ) {
      ::std::vector< CGAL::Point_2< R_ > > vec;
      vec.push_back( *$self );
      vec.push_back( el );
      return vec;
    }
  }
 
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
  %extend Vector_2 {
    ::dtOO::dtReal __getitem__( ::dtOO::dtInt const & ii) {
      return $self->operator[](ii);
    }
    ::std::vector< Vector_2 > __lshift__( Vector_2 const & el ) {
      ::std::vector< CGAL::Vector_2< R_ > > vec;
      vec.push_back( *$self );
      vec.push_back( el );
      return vec;
    }
  } 
}
namespace CGAL {
  %template(dtPoint3)       Point_3< ::dtOO::dtKernel >;
  %template(dtVector3)      Vector_3< ::dtOO::dtKernel >;
  %template(dtPoint2)       Point_2< ::dtOO::dtKernel >;
  %template(dtVector2)      Vector_2< ::dtOO::dtKernel >;
}
%include dtLinearAlgebra.h
%include geometryEngine/dtCurve.h
namespace dtOO {
  %extend dtCurve {
    ::std::string __str__( void ) {
      return $self->dumpToString(); 
    }
  }
}
%include geometryEngine/dtSurface.h
%include geometryEngine/dtCurve2d.h
%include geometryEngine/dtSurface2d.h
%include baseContainerHeaven/baseContainer.h
%include baseContainerHeaven/pointContainer.h
%include interfaceHeaven/renderInterface.h
namespace dtOO {
  %extend pointContainer {
    dtPoint3 __getitem__( std::string const & str ) {
      return $self->operator[](str);
    } 
  }
}
%include baseContainerHeaven/vectorContainer.h
namespace dtOO {
  %extend vectorContainer {
    dtVector3 __getitem__( std::string const & str ) {
      return $self->operator[](str);
    } 
  }
}
%include baseContainerHeaven/transformerContainer.h
namespace dtOO {
  %extend transformerContainer {
    dtTransformer const * __getitem__( std::string const & str ) {
      return $self->operator[](str);
    } 
  }
}
%include constValueHeaven/constValue.h
%include analyticFunctionHeaven/analyticFunction.h
%include analyticGeometryHeaven/analyticGeometry.h
namespace dtOO {
  %extend analyticGeometry {
    analyticGeometry * __lshift__( std::string const & str ) {
      analyticGeometry * clone = $self->clone();
      clone->setLabel(str);
      return clone;
    } 
  }
 
}
%include bVObserver/bVOInterface.h
%include bVObserver/bVOSubject.h
%include boundedVolume.h
%include bVObserver/bVONameRegions.h
%include bVObserver/bVONameFaces.h
%include bVObserver/bVOAnalyticGeometryToFace.h
%include bVObserver/bVOAddFace.h
%include bVObserver/bVOAddInternalEdge.h
%include bVObserver/bVOTransfiniteRegions.h
%include bVObserver/bVORecombineRecursiveRegions.h
%include bVObserver/bVOSetPrescribedMeshSizeAtPoints.h
%include bVObserver/bVOSetNElements.h
%include bVObserver/bVOSetGradingToFaceRule.h
%include bVObserver/bVOSetGrading.h
%include bVObserver/bVOSetPrescribedElementSize.h
%include bVObserver/bVOReadMSH.h
%include bVObserver/bVOSetRotationalPeriodicity.h
%include bVObserver/bVOSetTranslationalPeriodicity.h
%include bVObserver/bVOWriteMSH.h
%include bVObserver/bVOFaceToPatchRule.h
%include bVObserver/bVORemoveElements.h
%include bVObserver/bVOOrientCellVolumes.h
%include bVObserver/bVODumpModel.h
%include bVObserver/bVOTransfiniteFaces.h
%include bVObserver/bVORecombine.h
%include meshEngine/dtMeshOperator.h
%include bVObserver/bVOMeshRule.h
%include bVObserver/bVOSetOrder.h
%include bVObserver/bVOWriteINP.h
%include gmshBoundedVolume.h
%include map3dTo3dGmsh.h
%include customGmsh.h
%include combineGmsh.h
%include dtCase.h
#ifdef DTOO_HAS_OpenFOAM
  %include ofOpenFOAMCase/ofOpenFOAMCase.h
#endif    
%include dtPlugin.h
%include jsonHeaven/jsonPrimitive.h
%include interfaceHeaven/lVHOSubject.h
namespace std {
  %template(listInt)                     list< ::dtOO::dtInt >;

  %template(vectorInt)                   vector< ::dtOO::dtInt >;
  %template(vectorBool)                  vector< bool >;
  %template(vectorReal)                  vector< ::dtOO::dtReal >;
  %template(vectorStr)                   vector< string >;
  %template(vectorJsonPrimitive)         vector< ::dtOO::jsonPrimitive >;
  %template(vectorLabelHandling)         vector< ::dtOO::labelHandling * >;
  %template(vectorConstValue)            vector< ::dtOO::constValue * >;
  %template(vectorAnalyticFunction)      vector< ::dtOO::analyticFunction * >;
  %template(vectorAnalyticGeometry)      vector< ::dtOO::analyticGeometry * >;
  %template(vectorMap1dTo3d)             vector< ::dtOO::map1dTo3d * >;
  %template(vectorMap2dTo3d)             vector< ::dtOO::map2dTo3d * >;
  %template(vectorMap3dTo3d)             vector< ::dtOO::map3dTo3d * >;
  %template(vectorRenderInterface)       vector< ::dtOO::renderInterface * >;

  %template(vectorBoundedVolume)         vector< ::dtOO::boundedVolume * >;
  %template(vectorDtCase)                vector< ::dtOO::dtCase * >;
  %template(vectorDtPlugin)              vector< ::dtOO::dtPlugin * >;
  %template(vectorDtPoint2)              vector< ::dtOO::dtPoint2 >;
  %extend vector< ::dtOO::dtPoint2 > {
    vector< ::dtOO::dtPoint2 > __lshift__( ::dtOO::dtPoint2 const & point ) {
      ::std::vector< ::dtOO::dtPoint2 > twin = *$self;
      twin.push_back( point );
      return twin;
    }
    vector< ::dtOO::dtPoint2 > __ilshift__( vector< ::dtOO::dtPoint2 > const & vec ) {
      for (int ii=0; ii<vec.size(); ii++) {
        $self->push_back( vec.at(ii) );
      }
      return *$self;
    }
    vector< ::dtOO::dtPoint2 > __ilshift__( ::dtOO::dtPoint2 const & point ) {
      $self->push_back( point );
      return *$self;
    }
   
  }
  %template(vectorDtVector2)             vector< ::dtOO::dtVector2 >;
  %extend vector< ::dtOO::dtVector2 > {
    vector< ::dtOO::dtVector2 > __lshift__( ::dtOO::dtVector2 const & point ) {
      ::std::vector< ::dtOO::dtVector2 > twin = *$self;
      twin.push_back( point );
      return twin;
    }
    vector< ::dtOO::dtVector2 > __ilshift__( vector< ::dtOO::dtVector2 > const & vec ) {
      for (int ii=0; ii<vec.size(); ii++) {
        $self->push_back( vec.at(ii) );
      }
      return *$self;
    }
    vector< ::dtOO::dtVector2 > __ilshift__( ::dtOO::dtVector2 const & point ) {
      $self->push_back( point );
      return *$self;
    }
  }
  %template(vectorDtPoint3)              vector< ::dtOO::dtPoint3 >;
  %extend vector< ::dtOO::dtPoint3 > {
    vector< ::dtOO::dtPoint3 > __lshift__( ::dtOO::dtPoint3 const & point ) {
      ::std::vector< ::dtOO::dtPoint3 > twin = *$self;
      twin.push_back( point );
      return twin;
    }
    vector< ::dtOO::dtPoint3 > __ilshift__( vector< ::dtOO::dtPoint3 > const & vec ) {
      for (int ii=0; ii<vec.size(); ii++) {
        $self->push_back( vec.at(ii) );
      }
      return *$self;
    }
    vector< ::dtOO::dtPoint3 > __ilshift__( ::dtOO::dtPoint3 const & point ) {
      $self->push_back( point );
      return *$self;
    }
  }
  %template(vectorDtVector3)             vector< ::dtOO::dtVector3 >;
  %extend vector< ::dtOO::dtVector3 > {
    vector< ::dtOO::dtVector3 > __lshift__( ::dtOO::dtVector3 const & point ) {
      ::std::vector< ::dtOO::dtVector3 > twin = *$self;
      twin.push_back( point );
      return twin;
    }
    vector< ::dtOO::dtVector3 > __ilshift__( vector< ::dtOO::dtVector3 > const & vec ) {
      for (int ii=0; ii<vec.size(); ii++) {
        $self->push_back( vec.at(ii) );
      }
      return *$self;
    }
    vector< ::dtOO::dtVector3 > __ilshift__( ::dtOO::dtVector3 const & point ) {
      $self->push_back( point );
      return *$self;
    }
   
  }
  %template(vectorDtCurve)               vector< ::dtOO::dtCurve * >;
  %template(vectorDtSurface)             vector< ::dtOO::dtSurface * >;
  %template(vectorDtCurve2d)             vector< ::dtOO::dtCurve2d * >;
  %template(vectorDtSurface2d)           vector< ::dtOO::dtSurface2d * >;
  
  %template(vectorConstLabelHandling)    vector< ::dtOO::labelHandling const * >;
  %template(vectorConstConstValue)       vector< ::dtOO::constValue const * >;
  %template(vectorConstAnalyticFunction) vector< ::dtOO::analyticFunction const * >;
  %template(vectorConstAnalyticGeometry) vector< ::dtOO::analyticGeometry const * >;
  %template(vectorConstBoundedVolume)    vector< ::dtOO::boundedVolume const * >;
  %template(vectorConstDtCase)           vector< ::dtOO::dtCase const * >;
  %template(vectorConstDtPlugin)         vector< ::dtOO::dtPlugin const * >;
  %template(vectorConstDtCurve)          vector< ::dtOO::dtCurve const * >;
  %template(vectorConstDtSurface)        vector< ::dtOO::dtSurface const * >;
  %template(vectorConstDtCurve2d)        vector< ::dtOO::dtCurve2d const * >;
  %template(vectorConstDtSurface2d)      vector< ::dtOO::dtSurface2d const * >;
}
namespace dtOO { 
  template < typename T >
  class vectorHandling : public ::std::vector< T > {
    public:
      using ::std::vector< T >::push_back;
      void destroy( void );
  };
  %extend vectorHandling {
    T __getitem__(int const & ii) {
      return $self->operator[](ii);
    }    
  }
  %template(vectorHandlingLabelHandling)         vectorHandling< labelHandling * >;
  %template(vectorHandlingConstValue)            vectorHandling< constValue * >;
  %template(vectorHandlingAnalyticFunction)      vectorHandling< analyticFunction * >;
  %template(vectorHandlingAnalyticGeometry)      vectorHandling< analyticGeometry * >;
  %template(vectorHandlingMap1dTo3d)             vectorHandling< map1dTo3d * >;
  %template(vectorHandlingMap2dTo3d)             vectorHandling< map2dTo3d * >;
  %template(vectorHandlingMap3dTo3d)             vectorHandling< map3dTo3d * >;
  %template(vectorHandlingBoundedVolume)         vectorHandling< boundedVolume * >;
  %template(vectorHandlingDtCase)                vectorHandling< dtCase * >;
  %template(vectorHandlingDtPlugin)              vectorHandling< dtPlugin * >;
  %template(vectorHandlingDtCurve)               vectorHandling< dtCurve * >;
  %template(vectorHandlingDtSurface)             vectorHandling< dtSurface * >;
  %template(vectorHandlingDtCurve2d)             vectorHandling< dtCurve2d * >;
  %template(vectorHandlingDtSurface2d)           vectorHandling< dtSurface2d * >;
  %template(vectorHandlingRenderInterface)       vectorHandling< renderInterface * >;

  %template(vectorHandlingConstLabelHandling)    vectorHandling< labelHandling const * >;
  %template(vectorHandlingConstConstValue)       vectorHandling< constValue const * >;
  %template(vectorHandlingConstAnalyticFunction) vectorHandling< analyticFunction const * >;
  %template(vectorHandlingConstAnalyticGeometry) vectorHandling< analyticGeometry const * >;
  %template(vectorHandlingConstBoundedVolume)    vectorHandling< boundedVolume const * >;
  %template(vectorHandlingConstDtCase)           vectorHandling< dtCase const * >;
  %template(vectorHandlingConstDtPlugin)         vectorHandling< dtPlugin const * >;
  %template(vectorHandlingConstDtCurve)          vectorHandling< dtCurve const * >;
  %template(vectorHandlingConstDtSurface)        vectorHandling< dtSurface const * >;
  %template(vectorHandlingConstDtCurve2d)        vectorHandling< dtCurve2d const * >;
  %template(vectorHandlingConstDtSurface2d)      vectorHandling< dtSurface2d const * >;
  
  template < typename T >
  class labeledVectorHandling : public vectorHandling< T >, public lVHOSubject {
    public:
      typedef typename ::std::vector< T >::iterator iterator;
      typedef typename ::std::vector< T >::reference reference;
      typedef typename ::std::vector< T >::const_reference const_reference;    
    public:
      void set( T const & toSet);
      const_reference get( std::string const & label) const;
      ::std::vector< dtOO::dtInt > getIndices( std::string const & label) const;
      std::string getLabel( dtOO::dtInt const pos ) const;
      ::std::vector< std::string > labels( void ) const;
      dtInt getPosition( std::string const label) const;
      void dump(void) const;          
  };
  %extend labeledVectorHandling {
    T __getitem__( std::string const & str ) {
      return $self->operator[](str);
    } 
    T __getitem__(int const & ii) {
      return $self->operator[](ii);
    }    
  }
  
  %template(labeledVectorHandlingLabelHandling)       labeledVectorHandling< labelHandling * >;
  %template(labeledVectorHandlingConstValue)          labeledVectorHandling< constValue * >;
  %template(labeledVectorHandlingAnalyticFunction)    labeledVectorHandling< analyticFunction * >;
  %template(labeledVectorHandlingAnalyticGeometry)    labeledVectorHandling< analyticGeometry * >;
  %template(labeledVectorHandlingBoundedVolume)       labeledVectorHandling< boundedVolume * >;
  %template(labeledVectorHandlingDtCase)              labeledVectorHandling< dtCase * >;
  %template(labeledVectorHandlingDtPlugin)            labeledVectorHandling< dtPlugin * >;
  
  template < typename T >
  class twoDArrayHandling : public ::std::vector< ::std::vector< T > > {
    public:
      dtOO::dtInt size( dtOO::dtInt const dim = 0) const;
      ::std::vector< T > fixJ( dtOO::dtInt const jj) const;
      ::std::vector< T > fixI( dtOO::dtInt const ii) const;
      void clear( void );
  };
  %extend twoDArrayHandling {
    ::std::vector< T > __getitem__(int const & ii) {
      return $self->operator[](ii);
    }    
  }
  %template(twoDArrayHandlingDtPoint3)     twoDArrayHandling< dtPoint3 >;
  %template(twoDArrayHandlingInt)          twoDArrayHandling< dtInt >;
}
namespace std {
  %template(pairDtPoint3)                  pair< ::dtOO::dtPoint3, ::dtOO::dtPoint3 >;
}
%include <interfaceHeaven/lVHOInterface.h>
%include <interfaceHeaven/lVHOjsonLoad.h>
%include <interfaceHeaven/lVHOstateHandler.h>
%include <parseHeaven/dtParser.h>
namespace dtOO {
  %extend dtParser {
    std::string __getitem__( std::string const & str ) {
      return $self->operator[](str);
    } 
  }
}
%include discrete2dPoints.h
%include discrete3dPoints.h
%include discrete3dVector.h
%include solid2dLine.h
%include solid3dLine.h
%include solid3dSurface.h
%include unstructured3dMesh.h
%include unstructured3dSurfaceMesh.h

%include <interfaceHeaven/staticPropertiesHandler.h>
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
%template(appendVectorStr) dtOO::jsonPrimitive::append< std::vector< std::string > >;
%template(appendVectorAnalyticGeometry) dtOO::jsonPrimitive::append< std::vector< analyticGeometry * > >;
%template(appendVectorBoundedVolume) dtOO::jsonPrimitive::append< std::vector< boundedVolume * > >;

%include constValueHeaven/sliderFloatParam.h
%include constValueHeaven/intParam.h
%include constValueHeaven/constrainedFloatParam.h

%include geometryEngine/dtOCCCurveBase.h
%include geometryEngine/dtOCCCurve.h
%include geometryEngine/dtOCCBSplineCurve.h
%include geometryEngine/dtOCCSurfaceBase.h
%include geometryEngine/dtOCCSurface.h
%include geometryEngine/dtOCCBSplineSurface.h
%include geometryEngine/geoBuilder/bezierCurve_pointConstructOCC.h
%include geometryEngine/geoBuilder/bezierSurface_bezierCurveFillConstructOCC.h
%include geometryEngine/geoBuilder/bezierSurface_pointConstructOCC.h
%include geometryEngine/geoBuilder/bSplineCurve2d_angleDeltaXDeltaYConstructOCC.h
%include geometryEngine/geoBuilder/bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC.h
%include geometryEngine/geoBuilder/bSplineCurve2d_angleRatioDeltaYConstructOCC.h
%include geometryEngine/geoBuilder/bSplineCurve2d_angleRatioDeltaYPointOnePointTwoConstructOCC.h
%include geometryEngine/geoBuilder/bSplineCurve2d_curveConnectConstructOCC.h
%include geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h
%include geometryEngine/geoBuilder/bSplineCurve2d_pointInterpolateConstructOCC.h
%include geometryEngine/geoBuilder/bSplineCurve_approxGeomCurve2dInGeomSurface.h
%include geometryEngine/geoBuilder/bSplineCurve_bSplineCurveSplitConstructOCC.h
%include geometryEngine/geoBuilder/bSplineCurve_convertOCC.h
%include geometryEngine/geoBuilder/bSplineCurve_curveConnectConstructOCC.h
%include geometryEngine/geoBuilder/bSplineCurve_normalOffsetGeomCurveOCC.h
%include geometryEngine/geoBuilder/bSplineCurve_pointConstructArcLengthParaOCC.h
%include geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h
%include geometryEngine/geoBuilder/bSplineCurve_pointInterpolateConstructOCC.h
%include geometryEngine/geoBuilder/bSplineCurve_poleWeightKnotMultOrderConstructOCC.h
%include geometryEngine/geoBuilder/bSplineSurface2d_bSplineCurve2dFillConstructOCC.h
%include geometryEngine/geoBuilder/bSplineSurface_bSplineCurveFillConstructOCC.h
%include geometryEngine/geoBuilder/bSplineSurface_bSplineSurfaceSplitConstructOCC.h
%include geometryEngine/geoBuilder/bSplineSurface_constrainedFillingConstructOCC.h
%include geometryEngine/geoBuilder/bSplineSurface_convertOCC.h
%include geometryEngine/geoBuilder/bSplineSurface_exchangeSurfaceConstructOCC.h
%include geometryEngine/geoBuilder/bSplineSurface_extentSurfaceByLength.h
%include geometryEngine/geoBuilder/bSplineSurface_geomCurveFillConstructOCC.h
%include geometryEngine/geoBuilder/bSplineSurface_pipeConstructOCC.h
%include geometryEngine/geoBuilder/bSplineSurfaces_bSplineSurfaceSkinConstructOCC.h
%include geometryEngine/geoBuilder/bSplineSurface_skinConstructOCC.h
%include geometryEngine/geoBuilder/circle_radiusCoordinateSystemConstructOCC.h
%include geometryEngine/geoBuilder/compositeSurface_surfaceConnectConstructOCC.h
%include geometryEngine/geoBuilder/dtPoint2_surfaceEquidistantPoint.h
%include geometryEngine/geoBuilder/ellipse_radiusRadiusCoordinateSystemConstructOCC.h
%include geometryEngine/geoBuilder/geomCurve2d_baseConstructOCC.h
%include geometryEngine/geoBuilder/geomCurve2d_closeGapsArithmeticOCC.h
%include geometryEngine/geoBuilder/geomCurve2d_curve2dTranslateConstructOCC.h
%include geometryEngine/geoBuilder/geomCurve2d_ignoreZ.h
%include geometryEngine/geoBuilder/geomCurve2d_reparamInGeomSurfaceOCC.h
%include geometryEngine/geoBuilder/geomCurve_baseConstructOCC.h
%include geometryEngine/geoBuilder/geomCurve_closeGapsArithmeticOCC.h
%include geometryEngine/geoBuilder/geomCurve_convertGeomCurve2d.h
%include geometryEngine/geoBuilder/geomCurve_curveReverseConstructOCC.h
%include geometryEngine/geoBuilder/geomCurve_curveRotateConstructOCC.h
%include geometryEngine/geoBuilder/geomCurve_curveTranslateConstructOCC.h
%include geometryEngine/geoBuilder/geomCurve_projectOnGeomSurfaceOCC.h
%include geometryEngine/geoBuilder/geomFillBoundWithSurf_surfaceConstructOCC.h
%include geometryEngine/geoBuilder/geomFillSimpleBound_curveConstructOCC.h
%include geometryEngine/geoBuilder/geomShape_readOCC.h
%include geometryEngine/geoBuilder/geomSurface_baseConstructOCC.h
%include geometryEngine/geoBuilder/geomSurface_closeGapsArithmeticOCC.h
%include geometryEngine/geoBuilder/geomSurface_surfaceReverseConstructOCC.h
%include geometryEngine/geoBuilder/geomSurface_surfaceRotateConstructOCC.h
%include geometryEngine/geoBuilder/geomSurface_surfaceTranslateConstructOCC.h
%include geometryEngine/geoBuilder/rectangularTrimmedSurface_curveRotateConstructOCC.h
%include geometryEngine/geoBuilder/rectangularTrimmedSurface_uvBounds.h
%include geometryEngine/geoBuilder/surfaceOfRevolution_curveRotateConstructOCC.h
%include geometryEngine/geoBuilder/trimmedCurve2d_twoPointsConnectConstructOCC.h
%include geometryEngine/geoBuilder/trimmedCurve_twoPointsConnectConstructOCC.h
%include geometryEngine/geoBuilder/trimmedCurve_uBounds.h
%include geometryEngine/geoBuilder/u_geomCurveClosestPoint.h
%include geometryEngine/geoBuilder/uv_geomSurfaceClosestPoint.h

%include jsonHeaven/aFJsonBuilder.h
%include jsonHeaven/aFJsonBuilder/bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder.h
namespace dtOO {
  %extend bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder {
    using aFJsonBuilder::buildPart;
  }
}

%include meshEngine/dtGmshVertex.h
%include meshEngine/dtGmshEdge.h
%include meshEngine/dtGmshFace.h
namespace std {
  %template(listDtGmshVertex)                   list< ::dtOO::dtGmshVertex * >;
  %template(listDtGmshEdge)                     list< ::dtOO::dtGmshEdge * >;
  %template(listDtGmshFace)                     list< ::dtOO::dtGmshFace * >;
  %template(listDtGmshRegion)                   list< ::dtOO::dtGmshRegion * >;
}
%include meshEngine/dtGmshRegion.h
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
%feature("notabstract") sca3PPointsBSplineOneD;
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
%include analyticFunctionHeaven/sca3PPointsBSplineOneD.h

namespace dtOO {
  %template(scaTanhGradingOneDCompound)                         analyticFunctionCompound< scaTanhGradingOneD >;
  typedef analyticFunctionCompound< scaTanhGradingOneD >        scaTanhGradingOneDCompound;
  %template(sca3PPointsBSplineOneDCompound)                  analyticFunctionCompound< sca3PPointsBSplineOneD >;
  typedef analyticFunctionCompound< sca3PPointsBSplineOneD > sca3PPointsBSplineOneDCompound;
}
%include analyticFunctionHeaven/aFBuilder/float_scaOneDPoint.h
%include analyticFunctionHeaven/aFBuilder/vec3dTwoD_normalOffset.h
%include analyticFunctionHeaven/aFBuilder/dtPoint3_vec3dTwoD.h
%include analyticFunctionHeaven/aFBuilder/vec3dTwoD_closeArithmetic.h
%include analyticFunctionHeaven/aFBuilder/vec3dTransVolThreeD_skinBSplineSurfaces.h

namespace dtOO {
  %rename(applyDtPoint2) dtTransformer::apply(dtPoint2 const&) const;
  %rename(retractDtPoint2) dtTransformer::retract(dtPoint2 const&) const;
  %rename(applyDtPoint3) dtTransformer::apply(dtPoint3 const&) const;
  %rename(retractDtPoint3) dtTransformer::retract(dtPoint3 const&) const;
  %rename(applyAnalyticGeometry) dtTransformer::apply(analyticGeometry const * const) const;
  %rename(applyAnalyticFunction) dtTransformer::apply(analyticFunction const * const) const;
  %rename(applyLabeledVectorHandlingAnalyticFunction) dtTransformer::apply(lvH_analyticFunction const * const) const;
  %extend dtTransformer {
    dtPoint2 __call__( dtPoint2 const& pp) { return $self->apply(pp); } 
    dtPoint3 __call__( dtPoint3 const& pp) { return $self->apply(pp); } 
    analyticGeometry * __call__( analyticGeometry const * const aG) {
      return $self->apply(aG);
    } 
    analyticFunction * __call__( analyticFunction const * const aF) {
      return $self->apply(aF);
    } 
    lvH_analyticFunction __call__( lvH_analyticFunction const * const lvh_aF) {
      return $self->apply(lvh_aF);
    } 
  }
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
%feature("notabstract") partRotatingMap2dTo3d;

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
%include analyticGeometryHeaven/partRotatingMap2dTo3d.h
%include analyticGeometryHeaven/trans6SidedCube.h
%include analyticGeometryHeaven/translatingMap2dTo3d.h
namespace std {
  %template(pairDtRealDtPoint2) pair< ::dtOO::dtReal, ::dtOO::dtPoint2 >;
}
%include analyticGeometryHeaven/aGBuilder/map2dTo3d_constructMarginFaces.h
%include analyticGeometryHeaven/aGBuilder/dtPoint3_map1dTo3dEquidistantPoint.h
%include analyticGeometryHeaven/aGBuilder/dtPoint3_map1dTo3dPoint.h
%include analyticGeometryHeaven/aGBuilder/map1dTo3d_normalOffsetMap1dTo3dInMap2dTo3d.h
%include analyticGeometryHeaven/aGBuilder/map1dTo3d_closeGapsArithmetic.h
%include analyticGeometryHeaven/aGBuilder/map2dTo3d_approximateMap2dTo3dInMap3dTo3d.h
%include analyticGeometryHeaven/aGBuilder/map2dTo3d_fullExtentInMap3dTo3d.h
%include analyticGeometryHeaven/aGBuilder/dtPoint3_map2dTo3dPoint.h
%include analyticGeometryHeaven/aGBuilder/uv_map2dTo3dClosestPointToPoint.h
%include analyticGeometryHeaven/aGBuilder/pairUUV_map1dTo3dClosestPointToMap2dTo3d.h
%include analyticGeometryHeaven/aGBuilder/float_map1dTo3dPointConstCartesian.h
%include analyticGeometryHeaven/aGBuilder/trans6SidedCube_splitTrans6SidedCube.h
%include analyticGeometryHeaven/aGBuilder/bool_map1dTo3dInMap2dTo3d.h
%include interfaceHeaven/dtBundle.h
%include attributionHeaven/floatAtt.h
%include attributionHeaven/pointGeometryDist.h
%include attributionHeaven/geometryGeometryDist.h
%include attributionHeaven/curveCurveDist.h
%include gslMinFloatAttr.h
namespace dtOO {
  %extend gslMinFloatAttr {
    gslMinFloatAttr( 
      floatAtt const * const att,
      std::vector< dtReal > const & guess,
      std::vector< dtReal > const & step,
      dtReal const & precision,
      dtInt const & maxIterations
    ) {
      return new gslMinFloatAttr(
        dt__pH(floatAtt)(att->clone()), 
        guess, 
        step, 
        precision, 
        maxIterations
      );
    } 
  }
}
%include gslGradMinFloatAttr.h
namespace dtOO {
  %extend gslGradMinFloatAttr {
    gslGradMinFloatAttr( 
      floatAtt const * const att,
      std::vector< dtReal > const & guess,
      std::vector< dtReal > const & step,
      dtReal const & precision,
      dtInt const & maxIterations
    ) {
      return new gslGradMinFloatAttr(
        dt__pH(floatAtt)(att->clone()), 
        guess, 
        step, 
        precision, 
        maxIterations
      );
    } 
  }
}

