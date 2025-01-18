import logging
logging.basicConfig(
  format='[ %(asctime)s - %(levelname)8s - %(filename)s:%(lineno)d ]'
         ' - %(message)s',
  datefmt='%d-%b-%y %H:%M:%S',
  level=logging.INFO
)

logging.debug("Debug")
logging.info("Info")
logging.warning("Warning")
logging.error("Error")

import dtOOPythonSWIG as dtOO

import numpy as np

def Init():
  container = dtOO.dtBundle()
  dtOO.logMe.initLog('build.log')

  # Init staticProperties
  dtOO.staticPropertiesHandler.getInstance().jInit(
    dtOO.jsonPrimitive(
      '{'
        '"option" : ['
          '{"name" : "reparamOnFace_precision", "value" : "1.e-05"},'
          '{"name" : "reparamInVolume_precision","value" : "1.e-05"},'
          '{"name" : "reparam_internalRestarts", "value" : "10"},'
          '{"name" : "reparam_restarts", "value" : "10"},'
          '{"name" : "reparam_restartIncreasePrecision", "value" : "10."},'
          '{'
            '"name" : "reparam_internalRestartDecreasePrecision",'
            ' "value" : "0.9"'
          '},'
          '{'
            '"name" : "reparamOnFace_minimizer", '
            '"value" : ":Minuit2::kMigrad:"'
          '},'
          '{'
            '"name" : "reparamInVolume_minimizer", '
            '"value" : ":Minuit2::kMigrad:"'
          '},'
          '{"name" : "invY_precision", "value" : "1.e-04"},'
          '{"name" : "xyz_resolution", "value" : "1.e-05"},'
          '{"name" : "XYZ_resolution", "value" : "1.e-04"},'
          '{"name" : "uvw_resolution", "value" : "1.e-04"},'
          '{"name" : "point_render_diameter", "value" : "0.005"},'
          '{"name" : "vector_render_size", "value" : "0.05"},'
          '{"name" : "function_render_resolution_u", "value" : "21"},'
          '{"name" : "function_render_resolution_v", "value" : "21"},'
          '{"name" : "function_render_resolution_w", "value" : "21"},'
          '{"name" : "geometry_render_resolution_u", "value" : "21"},'
          '{"name" : "geometry_render_resolution_v", "value" : "21"},'
          '{"name" : "geometry_render_resolution_w", "value" : "21"},'
          '{"name" : "root_printLevel", "value" : "0"},'
          '{"name" : "root_maxIterations", "value" : "1000"},'
          '{"name" : "root_maxFunctionCalls", "value" : "1000000"},'
          '{"name" : "ompNumThreads", "value" : "2"},'
          '{"name" : "map1dTo3d_deltaPer", "value" : "0.01"},'
          '{"name" : "map2dTo3d_deltaPer", "value" : "0.01"},'
          '{"name" : "map3dTo3d_deltaPer", "value" : "0.01"},'
          '{"name" : "logLevel", "value" : "99"},'
          '{"name" : "isEqualExtendCheck", "value" : "false"}'
        ']'
      '}'
    )
  )

  return container

def Geometry( container ):
  bC = container.cptr_bC()
  cV = container.cptr_cV()
  # clearing prevents segmentation fault
  dtOO.lVHOstateHandler.clear()
  dtOO.lVHOstateHandler( dtOO.jsonPrimitive(), cV ).thisown = False
 
  aF = container.cptr_aF()
  aG = container.cptr_aG()
  bV = container.cptr_bV()
  dC = container.cptr_dC()
  dP = container.cptr_dP()

  # channel
  from dtOOPythonApp.builder import (
    rotatingMap2dTo3d_hubShroudCreate
  )
  container = rotatingMap2dTo3d_hubShroudCreate(
    "xyz_channel",
    shroudPoints = [
      dtOO.dtPoint3(2.00, 0.00, 2.00),
      dtOO.dtPoint3(2.00, 0.00, 0.00),
    ],
    hubPoints = [
      dtOO.dtPoint3(0.50, 0.00, 2.00),
      dtOO.dtPoint3(0.50, 0.00, 0.00),
    ],
    order = 1,
    rotVector = dtOO.dtVector3(0, 0, -1)
  ).buildExtract( container )

  # conformalMapping
  theT = dtOO.uVw_phirMs()
  theT.jInit(
    dtOO.jsonPrimitive()\
      .appendStr("label", "uVw_phirMs")\
      .appendInt("_nV", 31)\
      .appendInt("_nW", 11)\
      .appendAnalyticGeometry(\
        "_rM2d", \
        aG.get("xyz_channel")\
      ),\
    None, None, None, aG \
  )
  bC.dtTransformer().add( theT.clone() )

  # meanplane
  from dtOOPythonApp.builder import (
    analyticSurface_threePointMeanplane,
    analyticSurface_threePointMeanplaneFromRatio,
    scaOneD_scaCurve2dOneDPointConstruct
  )
  # ru
  container = analyticSurface_threePointMeanplaneFromRatio(
    "meanplane",
    spanwiseCuts = [
      0.00,
      0.50,
      1.00,
    ],
    alphaOne = scaOneD_scaCurve2dOneDPointConstruct(
      [
        dtOO.dtPoint2(0.00, (np.pi/180.) * 90.0),
        dtOO.dtPoint2(1.00, (np.pi/180.) * 90.0),
      ],
      1
    )(),
    alphaTwo = scaOneD_scaCurve2dOneDPointConstruct(
      [
        dtOO.dtPoint2(0.00, (np.pi/180.) * 80.0),
        dtOO.dtPoint2(1.00, (np.pi/180.) * 80.0),
      ],
      1
    )(),
    ratioX = scaOneD_scaCurve2dOneDPointConstruct(
      [
        dtOO.dtPoint2(0.00, 0.50),
        dtOO.dtPoint2(1.00, 0.50),
      ],
      1
    )(),
    deltaY = scaOneD_scaCurve2dOneDPointConstruct(
      [
        dtOO.dtPoint2(0.00, 0.30),
        dtOO.dtPoint2(1.00, 0.80),
      ],
      1
    )(),
    offX = scaOneD_scaCurve2dOneDPointConstruct(
      [
        dtOO.dtPoint2(0.00, 0.0),
        dtOO.dtPoint2(1.00, 0.0),
      ],
      1
    )(),
    offY = scaOneD_scaCurve2dOneDPointConstruct(
      [
        dtOO.dtPoint2(0.00, 1.0),
        dtOO.dtPoint2(1.00, 1.0),
      ],
      1
    )(),
    targetLength = scaOneD_scaCurve2dOneDPointConstruct(
      [
        dtOO.dtPoint2(0.00, 0.4),
        dtOO.dtPoint2(0.50, 1.6),
        dtOO.dtPoint2(1.00, 1.4),
      ],
      2
    )(),
    targetLengthTolerance = 0.01,
    originOnLengthPercent = 0.5
  ).buildExtract( container )


  #
  # do conformal mapping
  #
  for ii in ["meanplane",]:
    theAG = dtOO.vec3dTwoDInMap3dTo3d(
      dtOO.vec3dTwoD.MustConstDownCast(
        bC.ptrTransformerContainer().get(
          "uVw_phirMs"
        ).applyAnalyticFunction(
          aF[ii].clone()
        )
      ),
      dtOO.map3dTo3d.ConstDownCast( aG["xyz_channel"] )
    )
    theAG.setLabel("xyz_"+ii)
    aG.push_back( theAG.clone() )

  return container

def GetRender( cc ):
  from dtOOPythonApp.vis import dtOOInParaVIEW
  rr = dtOOInParaVIEW( cc )
  return rr

#def Reparam( cc ):
#  mp = dtOO.map2dTo3d.DownCast( cc.cptr_aG()["xyz_meanplane"] )
#  v0 = dtOO.vectorDtPoint3()
#  v1 = dtOO.vectorDtPoint3()
#  v2 = dtOO.vectorDtPoint3()
#  v3 = dtOO.vectorDtPoint3()
#  for uv in np.linspace(0.0,1.0,11):
#    v0.push_back( mp.getPoint( mp.reparamOnFace(mp.getPointPercent(0.0,uv)) ) )
#    v1.push_back( mp.getPoint( mp.reparamOnFace(mp.getPointPercent(1.0,uv)) ) )
#    v2.push_back( mp.getPoint( mp.reparamOnFace(mp.getPointPercent(uv,0.0)) ) )
#    v3.push_back( mp.getPoint( mp.reparamOnFace(mp.getPointPercent(uv,1.0)) ) )
#
#  for v,l in zip([v0,v1,v2,v3],['xyz_u0', 'xyz_u1', 'xyz_v0', 'xyz_v1']):
#    theAG = dtOO.analyticCurve(
#      dtOO.bSplineCurve_pointConstructOCC(v, 1).result()
#    )
#    theAG.setLabel(l)
#    cc.cptr_aG().push_back( theAG.clone() )
#
#  return cc

def Mesh( cc ):
  gbv = dtOO.gmshBoundedVolume()
  gbv.jInit(
    dtOO.jsonPrimitive(
    '{'
      '"label" : "mesh", '
      '"option" : ['
        '{"name" : "[gmsh]General.Terminal", "value" : "1."},'
        '{"name" : "[gmsh]General.Verbosity", "value" : "100."},'
        '{"name" : "[gmsh]General.ExpertMode", "value" : "1."},'
        '{'
          '"name" : "[gmsh]Mesh.LcIntegrationPrecision", '
          '"value" : "1.0E-04"'
        '},'
        '{'
          '"name" : "[gmsh]Mesh.CharacteristicLengthMin", '
          '"value" : "0.01"'
        '},'
        '{'
          '"name" : "[gmsh]Mesh.CharacteristicLengthMax", '
          '"value" : "0.05"'
        '},'
        '{"name" : "[gmsh]Mesh.Algorithm", "value" : "1"},'
        '{'
          '"name" : "[gmsh]Mesh.MeshSizeExtendFromBoundary", '
          '"value" : "1"'
        '},'
        '{"name" : "[gmsh]Mesh.MeshSizeFromPoints", "value" : "1"}'
      '],'
      '"analyticGeometry" : []'
    '}'
    ),
    None, None, None, None, None
  )
  
  gbv.getModel().addIfFaceToGmshModel( 
    dtOO.map2dTo3d.DownCast( cc.cptr_aG()["xyz_meanplane"] ) 
  )
  gbv.getModel().tagPhysical(
    gbv.getModel().getDtGmshFaceByTag(1), "meanplane"
  )

  gbv.getModel().getDtGmshEdgeByTag(1).setNElements(20)
  gbv.getModel().getDtGmshEdgeByTag(2).setNElements(20)
  gbv.getModel().getDtGmshEdgeByTag(3).setNElements(20)
  gbv.getModel().getDtGmshEdgeByTag(4).setNElements(20)
  
#  ob = dtOO.bVONameFaces()
#  ob.jInit( dtOO.jsonPrimitive('{ "_faceLabel" : [], "_face" : [] }'), gbv )
#  ob.preUpdate()

  ob = dtOO.bVOMeshRule()
  ob.thisown = False
  ob.jInit(
    dtOO.jsonPrimitive( 
      '{'
        '"option" : ['
          '{"name" : "debug", "value" : "true"}'
        '],'
        '"_rule1D" : ['
          '"dtMeshGEdge(meanplane->*)"'
        '],'
        '"_rule2D" : ['
          '"dtMeshTransfiniteGFace(*)",'
          '"dtMeshGFace(meanplane)"'
        '],'
        '"_rule3D" : ['
          '"dtMeshGRegion(*)"'
        '],'
        '"_only" : [],'
        '"dtMeshOperator" : ['
          '{'
            '"name" : "dtMeshGEdge",'
            '"label" : "dtMeshGEdge"'
          '},'
          '{'
            '"name" : "dtMeshGFace",'
            '"label" : "dtMeshGFace"'
          '},'
          '{'
            '"name" : "dtMeshTransfiniteGFace",'
            '"label" : "dtMeshTransfiniteGFace"'
          '},'
          '{'
            '"name" : "dtMeshGRegion",'
            '"label" : "dtMeshGRegion",'
            '"_minQShapeMetric" : 0.0,' 
            '"_relax" : 0.1,'
            '"_nPyramidOpenSteps" : 10,'
            '"_nSmooths" : 3'
          '}'
        ']'
      '}'
    ),
    None, None, None, None, None, gbv
  )
  gbv.attachBVObserver(ob)

  ob = dtOO.bVOWriteMSH()
  ob.thisown = False
  ob.jInit(
    dtOO.jsonPrimitive( '{"_filename" : "", "_saveAll" : true}' ), gbv 
  )
  gbv.attachBVObserver(ob)

  #ob = dtOO.bVODumpModel()
  #ob.thisown = False
  #ob.jInit(dtOO.jsonPrimitive(), gbv)
  #gbv.attachBVObserver(ob)

  gbv.makeGrid()
  
  gbv.thisown = False
  cc.cptr_bV().push_back( gbv )

  return cc

cc = Init()
cc = Geometry(cc)
cc = Mesh(cc)

print("!")
print("! Variable cc defined type(%s)" % type(cc) )
print("!")
aG = cc.cptr_aG()
bV = cc.cptr_bV()
for ty, st in zip([aG, bV],["aG", "bV"]):
  print("! Variable %s (%s)" % (st, type(ty)) )
  counter = 0
  for obj in ty:
    print(
      "!   %s[ %s / %d] -> %s"
      %
      (
        st, obj.getLabel(), counter, obj.virtualClassName()
      )
    )
    counter = counter + 1
  print("!")
print("!")
