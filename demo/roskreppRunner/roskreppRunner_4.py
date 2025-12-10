import logging
logging.basicConfig(
  format='[ %(asctime)s - %(levelname)8s - %(filename)s:%(lineno)d ]'
         ' - %(message)s', 
  datefmt='%d-%b-%y %H:%M:%S', 
  level=logging.DEBUG
)

logging.debug("Debug")
logging.info("Info")
logging.warning("Warning")
logging.error("Error")

import dtOOPythonSWIG as dtOO

import numpy as np

class roskreppRunner_4:
  """Simple machine consisting of an radial-axial runner only.

  Create a very simple machine that consists only of an radial-axial runner.
  First an object is created by:


  >>> machine = roskreppRunner()

  The channel is created by two splines each containing two control points. 
  Within the channel a mean plane is defined and a thickness distribution
  is added. The machine is created by: 

  >>> machine.create()

  The mean plane can be adjusted by the constructor's parameters:

  >>> machine.create(alpha_2_1=10.0) 

  This results in a machine with an outlet angle at the shourd of 10 degree.

  Attributes
  ----------
  None
  """
 
  def __init__(self):
    """Constructor.

    Parameters
    ----------
    None

    Returns
    -------
    None
    """
    pass

  def create( 
    self, 
    alpha_1_0: float = 37.90,
    alpha_1_1: float = 49.40,
    alpha_2_0: float = 17.723,
    alpha_2_1: float = 58.998,
    halfDivide = 0,
  ) -> dtOO.dtBundle:
    """Create machine.
  
    The machine is created and dtOO's logfile is created as `build.log`.
  
    Parameters
    ----------
    alpha_1_0: float, default = 90.0
      Inlet angle at hub.
    alpha_1_1: float, default = 90.0
      Inlet angle at shroud.
    alpha_2_0: float, default = 80.0
      Outlet angle at hub.
    alpha_2_1: float, default = 80.0
      Outlet angle at shroud.
    
    Returns
    -------
    dtContainer
      Container that contains machine's parts.
    """
    # Create an empty container
    container = dtOO.dtBundle()

    # Create log file
    dtOO.logMe.initLog('build_2.log')

    # Init staticProperties
    dtOO.staticPropertiesHandler.getInstance().jInit(
      dtOO.jsonPrimitive(
        '{'
          '"option" : ['
            '{"name" : "reparamOnFace_precision", "value" : "1.e-005"},'#-0
            '{"name" : "reparamInVolume_precision","value" : "1.e-005"},'#-0
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
            '{"name" : "invY_precision", "value" : "1.e-004"},' #-0
            '{"name" : "xyz_resolution", "value" : "1.e-005"},' #-0
            '{"name" : "XYZ_resolution", "value" : "1.e-004"},' #-0
            '{"name" : "uvw_resolution", "value" : "1.e-004"},' #-0
            '{"name" : "point_render_diameter", "value" : "0.005"},'
            '{"name" : "vector_render_size", "value" : "0.005"},'
            '{"name" : "function_render_resolution_u", "value" : "21"},'
            '{"name" : "function_render_resolution_v", "value" : "21"},'
            '{"name" : "function_render_resolution_w", "value" : "21"},'
            '{"name" : "geometry_render_resolution_u", "value" : "51"},'#-100 Genauigkeit der Flächen
            '{"name" : "geometry_render_resolution_v", "value" : "51"},'#-100 Genauigkeit der Flächen
            '{"name" : "geometry_render_resolution_w", "value" : "51"},'#-100 Genauigkeit der Flächen
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
  
    # Init baseContainer and labeledVectorHandlings
    bC = container.cptr_bC()
    cV = container.cptr_cV()

    # at least one constValue is necessary for creating states
    cV.push_back(
      dtOO.sliderFloatParam("QIn", 0.5, 0.0, 1.0).clone()
    )

    # clearing prevents segmentation fault
    dtOO.lVHOstateHandler.clear()
    dtOO.lVHOstateHandler( dtOO.jsonPrimitive(), cV ).thisown = False
   
    aF = container.cptr_aF() 
    aG = container.cptr_aG() 
    bV = container.cptr_bV() 
    dC = container.cptr_dC() 
    dP = container.cptr_dP() 

    #draft tube elbow centerline
    from draftTubeCenterline import (
      draftTubeCenterline
    )
    container = draftTubeCenterline(
      "draftTubeElbowCenterline",
      splinePoints = [
        dtOO.dtPoint3(0.0000, 0.00, -2.4255),
        dtOO.dtPoint3(0.0406, 0.00, -3.0568),
        dtOO.dtPoint3(0.1554, 0.00, -3.6806),
        dtOO.dtPoint3(0.3431, 0.00, -4.2920),
        dtOO.dtPoint3(0.6091, 0.00, -4.8812),
        dtOO.dtPoint3(0.9625, 0.00, -5.4306),
        dtOO.dtPoint3(1.4157, 0.00, -5.9041),
        dtOO.dtPoint3(1.9687, 0.00, -6.2559),
        dtOO.dtPoint3(2.5754, 0.00, -6.4869),
        dtOO.dtPoint3(3.2146, 0.00, -6.5664),
        dtOO.dtPoint3(3.8300, 0.00, -6.6002),
        dtOO.dtPoint3(4.4304, 0.00, -6.5959),
      ],
      order = 2
    ).buildExtract( container )

    #draft tube diffusor centerline
    from draftTubeCenterline import (
      draftTubeCenterline
    )
    container = draftTubeCenterline(
      "draftTubeDiffusorCenterline",
      splinePoints = [
        dtOO.dtPoint3(4.4304, 0.00, -6.5959),
        dtOO.dtPoint3(5.0680, 0.00, -6.5648),
        dtOO.dtPoint3(5.7240, 0.00, -6.5032),
        dtOO.dtPoint3(7.3635, 0.00, -6.3816),
        dtOO.dtPoint3(9.6733, 0.00, -6.1620),
        dtOO.dtPoint3(24.000, 0.00, -4.5324),
      ],
      order = 2
    ).buildExtract( container )

    #draft tube cone
    from draftTubeCone import (
      draftTubeCone
    )
    container = draftTubeCone(
      "draftTubeCone",
      coordPoints = [
        dtOO.dtPoint3(0.00, 0.00, 0.00),
        dtOO.dtPoint3(0.00, 0.00, -2.4255),
        #Vector of coordinate points
      ],
      rotationVectors = [
        dtOO.dtVector3(0.00, 0.00, 1.00),
        dtOO.dtVector3(0.00, 0.00, 1.00),
        #Vector of rotation
      ],
      radiusList = [
        1.510,
        1.700,
        #Vector of radii
      ],
      halfDivide = halfDivide
    ).buildExtract( container )

    #draft tube (section) parameter distribution inclusion
    from draftTubeParameterDistribution import (
      draftTubeParameterDistribution
    )
    container = draftTubeParameterDistribution(
      "draftTubeParameterDistribution",
      order = 2
    ).buildExtract( container )

    #draft tube elbow
    from draftTubeElbow import (
      draftTubeElbow
    )
    container = draftTubeElbow(
      "draftTubeElbow",
      hDistCurve = aF["h-distribution"],
      bDistCurve = aF["b-distribution"],
      rDistCurve = aF["r-distribution"],
      surgeCurve = aG["draftTubeElbowCenterline"],
      resolution = 30,
      halfDivide = halfDivide,
    ).buildExtract( container )

    #draft tube diffusor
    from draftTubeDiffusor import (
      draftTubeDiffusor
    )
    container = draftTubeDiffusor(
      "draftTubeDiffusor",
      hDistCurve = aF["h-distribution"],
      bDistCurve = aF["b-distribution"],
      rDistCurve = aF["r-distribution"],
      surgeCurve = aG["draftTubeDiffusorCenterline"],
      resolution = 10,
      halfDivide = halfDivide,
    ).buildExtract( container )

    # channel
    from dtOOPythonApp.builder import rotatingMap2dTo3d_hubShroudCreate 
    
    container = rotatingMap2dTo3d_hubShroudCreate(
      "channel",
      shroudPoints = [
        dtOO.dtPoint3(1.65, 0.00, 0.860), #z: +0.04
        dtOO.dtPoint3(1.507, 0.00, 0.860), #z: +0.04
        dtOO.dtPoint3(1.432, 0.00, 0.601), #z: +0.04
        dtOO.dtPoint3(1.51, 0.00, 0.0),
      ],
      hubPoints = [
        dtOO.dtPoint3(1.65, 0.00, 1.780), #z: +0.04
        dtOO.dtPoint3(1.119, 0.00, 1.780), #z: +0.04
        dtOO.dtPoint3(0.687, 0.00, 1.621), #z: +0.04
        dtOO.dtPoint3(0.423, 0.00, 1.360), #z: +0.04
        dtOO.dtPoint3(0.293, 0.00, 1.138), #z: +0.04
        dtOO.dtPoint3(0.134, 0.00, 0.824), #z: +0.04
        dtOO.dtPoint3(0.100, 0.00, 0.000),
      ],
      order = 2,
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
            aG.get("channel")\
        ),\
        None, None, None, aG \
    )
    bC.dtTransformer().add( theT.clone() )

    # meanplane
    from dtOOPythonApp.builder import analyticSurface_threePointMeanplane 
    from dtOOPythonApp.builder import (
      analyticSurface_threePointMeanplaneFromRatio
    )
    from dtOOPythonApp.builder import scaOneD_scaCurve2dOneDPointConstruct 
    # runner
    container = analyticSurface_threePointMeanplaneFromRatio(
        "meanplane",
        spanwiseCuts = [
        1.00,
        0.75,
        0.50,
        0.25,
        0.00,
        # 0.00,
        # 0.25,
        # 0.50,
        # 0.75,
        # 1.00,
        ],
        alphaOne = scaOneD_scaCurve2dOneDPointConstruct(
        [
            dtOO.dtPoint2(1.00, (np.pi/180.) * alpha_1_0),
            dtOO.dtPoint2(0.75, (np.pi/180.) * 40.30),#42.189),
            dtOO.dtPoint2(0.50, (np.pi/180.) * 42.15),#42.189),
            dtOO.dtPoint2(0.25, (np.pi/180.) * 43.00),#42.189),
            dtOO.dtPoint2(0.00, (np.pi/180.) * alpha_1_1),
        ],
        1
        )(),
        alphaTwo = scaOneD_scaCurve2dOneDPointConstruct(
        [
            dtOO.dtPoint2(1.00, (np.pi/180.) * alpha_2_0),
            dtOO.dtPoint2(0.75, (np.pi/180.) * 35.967),
            dtOO.dtPoint2(0.50, (np.pi/180.) * 35.967),
            dtOO.dtPoint2(0.25, (np.pi/180.) * 35.967),
            dtOO.dtPoint2(0.00, (np.pi/180.) * alpha_2_1),
        ],
        1
        )(),
        ratioX = scaOneD_scaCurve2dOneDPointConstruct(
        [
            dtOO.dtPoint2(1.00, 0.40), #0.00, 0.50 #shroud
            dtOO.dtPoint2(0.75, 0.45), #0.00, 0.50
            dtOO.dtPoint2(0.50, 0.50), #0.00, 0.50
            dtOO.dtPoint2(0.25, 0.55), #0.00, 0.60
            dtOO.dtPoint2(0.00, 0.60), #1.00, 0.70
        ],
        1
        )(),
        deltaY = scaOneD_scaCurve2dOneDPointConstruct(
        [
            dtOO.dtPoint2(1.00, 2.80), #shroud
            dtOO.dtPoint2(0.50, 2.5),
            dtOO.dtPoint2(0.00, 2.70),
        ],
        1
        )(),
        offX = scaOneD_scaCurve2dOneDPointConstruct(
        [
            dtOO.dtPoint2(1.00, 0.0), #shroud #0.1
            dtOO.dtPoint2(0.75, 0.0),#0.00
            dtOO.dtPoint2(0.50, 0.00), #0.00
            dtOO.dtPoint2(0.25, 0.0),#0.00
            dtOO.dtPoint2(0.00, 0.0), #0.00
        ],
        1
        )(),
        offY = scaOneD_scaCurve2dOneDPointConstruct(
        [
            # dtOO.dtPoint2(1.00, 0.30),
            # dtOO.dtPoint2(0.75, 0.55),
            # dtOO.dtPoint2(0.50, 0.70),
            # dtOO.dtPoint2(0.25, 0.80),
            # dtOO.dtPoint2(0.00, 0.90),

            dtOO.dtPoint2(1.00, 0.60), #shroud
            dtOO.dtPoint2(0.75, 0.9),
            dtOO.dtPoint2(0.50, 0.85),
            dtOO.dtPoint2(0.25, 0.8),
            dtOO.dtPoint2(0.00, 0.975),

            #pump
            # dtOO.dtPoint2(0.00, 1.6),
            # dtOO.dtPoint2(0.50, 0.80),
            # dtOO.dtPoint2(0.75, 0.50),
            # dtOO.dtPoint2(1.00, 0.30),
        ],
        1
        )(),
        targetLength = scaOneD_scaCurve2dOneDPointConstruct(
        [
            # dtOO.dtPoint2(0.00, 0.72),
            # dtOO.dtPoint2(0.25, 0.62),
            # dtOO.dtPoint2(0.50, 0.58),
            # dtOO.dtPoint2(0.75, 0.45),
            # dtOO.dtPoint2(1.00, 0.425),

            dtOO.dtPoint2(1.00, 1.5), #shroud
            dtOO.dtPoint2(0.75, 1.5),
            dtOO.dtPoint2(0.50, 1.3),
            dtOO.dtPoint2(0.25, 1.10),
            dtOO.dtPoint2(0.00, 1.20),
        ],
        2
        )(),
        targetLengthTolerance = 0.01,
        originOnLengthPercent = 0.5
    ).buildExtract( container )

    # blade thickness distribution
    from dtOOPythonApp.builder import (
      vec3dSurfaceTwoD_fivePointsBSplineThicknessDistribution
    )
    container = vec3dSurfaceTwoD_fivePointsBSplineThicknessDistribution(
        "thicknessDistribution",
        spanwiseCuts = [
        0.00,
        #0.25,
        #0.50,
        #0.75,
        1.00,
        ],
        tLe = scaOneD_scaCurve2dOneDPointConstruct(
        [
            dtOO.dtPoint2(0.00, 0.03),
            dtOO.dtPoint2(1.00, 0.03),

            #dtOO.dtPoint2(0.00, 0.05),
            #dtOO.dtPoint2(1.00, 0.05),
        ],
        1
        )(),
        uLe = scaOneD_scaCurve2dOneDPointConstruct(
        [
            dtOO.dtPoint2(0.00, 0.00),
            dtOO.dtPoint2(1.00, 0.00),
        ],
        1
        )(),
        tMid = scaOneD_scaCurve2dOneDPointConstruct(
        [
            dtOO.dtPoint2(0.00, 0.01),
            dtOO.dtPoint2(1.00, 0.01),

            #dtOO.dtPoint2(0.00, 0.07),
            #dtOO.dtPoint2(1.00, 0.07),
        ],
        1
        )(),
        uMid = scaOneD_scaCurve2dOneDPointConstruct(
        [
            dtOO.dtPoint2(0.00, 0.50),
            dtOO.dtPoint2(1.00, 0.50),
        ],
        1
        )(),
        tTe = scaOneD_scaCurve2dOneDPointConstruct(
        [
            dtOO.dtPoint2(0.00, 0.01),
            dtOO.dtPoint2(1.00, 0.01),

            #dtOO.dtPoint2(0.00, 0.01),
            #dtOO.dtPoint2(1.00, 0.01),
        ],
        1
        )(),
        uTe = scaOneD_scaCurve2dOneDPointConstruct(
        [
            dtOO.dtPoint2(0.00, 0.70),
            dtOO.dtPoint2(1.00, 0.70),
        ],
        1
        )()
    ).buildExtract( container )

    #blade
    dAdd = dtOO.discreteAddNormal()
    dAdd.jInit(
        dtOO.jsonPrimitive(
        '{"option" : [{"name" : "debug", "value" : "false"}]}'
        )\
        .appendAnalyticFunction("_tt", aF["thicknessDistribution"])\
        .appendInt("_nU", 61)\
        .appendInt("_nV", 41)\
        .appendInt("_order", 3)\
        .appendDtVector3("_nf", dtOO.dtVector3(0,0,1)),
        bC, cV, aF, aG
    )
    theAF = dAdd.applyAnalyticFunction(
        aF["meanplane"]
    )

    theAF.setLabel("blade")
    aF.push_back( theAF.clone() )

    #theAG = dtOO.map3dTo3d.ConstDownCast( aG["blade"] )
    #theAG.setLable("blade")
    #aG.push_back( theAG.clone() )

    # mesh block
    fRef = dtOO.vec3dMuParserTwoD(
      "1.0*0.05, xx, yy", "xx", "yy" #0.055
    )
    fRef.setLabel("thicknessMeshBlock")
    for i in range(2):
      fRef.setMin(i, +0.0)
      fRef.setMax(i, +1.0)
    aF.set( fRef.clone() )
    dAdd = dtOO.discreteAddNormal()
    dAdd.jInit(
      dtOO.jsonPrimitive('{"option" : [{"name" : "debug", "value" : "false"}]}')\
        .appendAnalyticFunction("_tt", aF["thicknessMeshBlock"])\
        .appendInt("_nU", 61)\
        .appendInt("_nV", 41)\
        .appendInt("_order", 3)\
        .appendDtVector3("_nf", dtOO.dtVector3(0,0,1)),
      bC, cV, aF, aG
    )
    theAF = dAdd.applyAnalyticFunction( aF["blade"] )
    theAF.setLabel("meshBlock")
    aF.push_back( theAF.clone() )

    # split mesh block
    from dtOOPythonApp.builder import vec3dThreeD_skinAndSplit
    container = vec3dThreeD_skinAndSplit(
      label = "meshBlock",
      aFOne = aF["blade"],
      aFTwo = aF["meshBlock"],
      splitDim = 0,
      splits = [
        [0.00, 0.10],
        [0.10, 0.30],
        [0.30, 0.45],
        [0.45, 0.55],
        [0.55, 0.70],
        [0.70, 0.90],
        [0.90, 1.00],
      ]
    ).buildExtract(container)

    #do conformal mapping

    for ii in ["meanplane", "blade", "meshBlock",]:
        theAG = dtOO.vec3dTwoDInMap3dTo3d(
        dtOO.vec3dTwoD.MustConstDownCast(
            bC.ptrTransformerContainer().get(
            "uVw_phirMs"
            ).applyAnalyticFunction(
            aF[ii].clone()
            )
        ),
        dtOO.map3dTo3d.ConstDownCast( aG["channel"] )
        )
        theAG.setLabel("xyz_"+ii)
        aG.push_back( theAG.clone() )
    for iNum in aF.getIndices("meshBlock_*"):
      ii = aF.getLabel( iNum )
      theAG = dtOO.vec3dThreeDInMap3dTo3d(
        dtOO.vec3dThreeD.MustConstDownCast(
          bC.ptrTransformerContainer().get(
            "uVw_phirMs"
          ).applyAnalyticFunction(
            aF[ii].clone()
          )
        ),
        dtOO.map3dTo3d.ConstDownCast( aG["channel"] )
      )
      theAG.setLabel("xyz_"+ii)
      aG.push_back( theAG.clone() )

    ## fe_meanplane
    from dtOOPythonApp.builder import (
      vec3dTwoDInMap3dTo3d_approximateAndFullExtendMeanplane
    )
    container = vec3dTwoDInMap3dTo3d_approximateAndFullExtendMeanplane(
        label = "xyz_fe_meanplane",
        channel = aG["channel"],
        meanplane = aG["xyz_meanplane"],
        ratioOut = scaOneD_scaCurve2dOneDPointConstruct(
        [
            dtOO.dtPoint2(0.00, 1.00), # (0.00, 0.50)
            dtOO.dtPoint2(0.25, 0.80), # neu hinzugekommen
            dtOO.dtPoint2(0.50, 0.60), # (0.50, 0.00)
            dtOO.dtPoint2(0.75, 0.40), # neu hinzugekommen
            dtOO.dtPoint2(1.00, 0.20)
        ]
        )(),
        # ratioBladeExt = scaOneD_scaCurve2dOneDPointConstruct(
        # [
        #     dtOO.dtPoint2(0.00, 0.00), #0.25
        #     dtOO.dtPoint2(0.50, 0.00), #0.05
        #     dtOO.dtPoint2(1.00, 0.00) #0.10
        # ],
        # )(),
        segUs = [0.0, 0.1, 0.2, 0.4, 0.6, 0.8, 0.9, 1.0],
        #segUs = [0.0, 0.1, 0.5, 0.7, 0.9, 1.0],
        segVs = [0.0, 0.05, 0.1, 0.2, 0.4, 0.6, 0.8, 0.9, 0.95, 1.0]
    ).buildExtract( container )

    from dtOOPythonApp.builder import rotatingMap2dTo3d_gridChannel 
    container = rotatingMap2dTo3d_gridChannel(
        label = "xyz_gridChannel",
        channelSide = aG["xyz_fe_meanplane"],
        numberOfSections = 10,
        rotVector = dtOO.dtVector3(0, 0, 1)
    ).buildExtract( container )

    # create mesh's topology
    blocks = []
    for iNum in aG.getIndices("xyz_meshBlock_*"):
      blocks.append( aG[ aG.getLabel( iNum ) ] )
    couplingFaces = []
    couplingFaces.append(
      dtOO.map3dTo3d.MustDownCast( blocks[0] ).segmentConstUPercent( 0.0 )
    )
    for block in blocks:
      couplingFaces.append(
        dtOO.map3dTo3d.MustDownCast( block ).segmentConstWPercent( 1.0 )
      )
    couplingFaces.append(
      dtOO.map3dTo3d.MustDownCast( blocks[-1] ).segmentConstUPercent( 1.0 )
    )
    from dtOOPythonApp.builder import map3dTo3dGmsh_gridFromChannelAndBlocks
    container = map3dTo3dGmsh_gridFromChannelAndBlocks(
      label = "mesh",
      channel = aG["xyz_gridChannel"],
      blocks = blocks,
      blade = aG["xyz_blade"],
      couplingFaces = couplingFaces,
      nBoundaryLayers = 15,
      nElementsSpanwise = 75,
      nElementsNormal = 20, #25
      firstElementSizeHubToShroud = 0.0008,
      firstElementSizeNormalBlade = 0.0008,
      bladeHubElementSize = scaOneD_scaCurve2dOneDPointConstruct(
        [
          dtOO.dtPoint2(0.00, 0.006), #0.005 Austritt
          dtOO.dtPoint2(0.25, 0.006), #0.005
          dtOO.dtPoint2(0.50, 0.001), #0.0008
          dtOO.dtPoint2(0.75, 0.006), #0.005
          dtOO.dtPoint2(1.00, 0.006), #0.005
        ], 1
      )(),
      bladeHubElementScale = 0.40,
      # bladeShroudElementSize = scaOneD_scaCurve2dOneDPointConstruct(
      #   [
      #     dtOO.dtPoint2(0.00, 0.007), #0.01 Austritt
      #     dtOO.dtPoint2(0.45, 0.007), #0.007
      #     dtOO.dtPoint2(0.50, 0.001), #0.007
      #     dtOO.dtPoint2(0.55, 0.007), #0.007
      #     dtOO.dtPoint2(1.00, 0.007), #0.01
      #   ], 1
      # )(),
      # bladeShroudElementScale = 0.20,
      channelInletOutletDir = 2,
      channelHubShroudDir = 3,
      charLengthMax=0.03, # 0.04
      charLengthMin=0.015 # 0.025
    ).enableDebug().buildExtract( container )


    # channel Diffusor Extension
    container = rotatingMap2dTo3d_hubShroudCreate(
      "xyz_diffuser",
      shroudPoints = [
        # originaler Saugrohrkonus
        # dtOO.dtPoint3(1.51, 0.00, 0.00),
        # dtOO.dtPoint3(1.70, 0.00, -2.4255),

        # Hilfe Geometrieerstellung
        # dtOO.dtPoint3(0.30, 0.00, 0),
        # dtOO.dtPoint3(1.4375, 0.00, -2.4255),

        # Zylindrisches Saugrohr
        dtOO.dtPoint3(1.51, 0.00, 0.00),
        dtOO.dtPoint3(1.51, 0.00, -2.4255),
      ],
      hubPoints = [
        dtOO.dtPoint3(0.1, 0.00, 0.00),
        dtOO.dtPoint3(0.1, 0.00, -2.4255),
      ],
      order = 1,
      rotVector = dtOO.dtVector3(0, 0, 1)
    ).buildExtract( container )

    # channel Diffusor Extension grid making
    container = rotatingMap2dTo3d_gridChannel(
      label = "xyz_gridDiffuser",
      channelSide = dtOO.map3dTo3d.MustConstDownCast(
        aG["xyz_diffuser"]
      ).segmentConstUPercent(0.0),
      numberOfSections = 10,
      rotVector = dtOO.dtVector3(0, 0, 1)
    ).buildExtract( container )

    from dtOOPythonApp.builder import map3dTo3dGmsh_gridFromChannel
    container = map3dTo3dGmsh_gridFromChannel(
      label = "meshDiffuser",
      channel = aG["xyz_gridDiffuser"],
      nBoundaryLayers = 12, #4
      nElementsRadial = 60, # cells in radial-direction
      nElementsCircumferential = 60, #10, cells in circumferential-direction
      nElementsMeridional = 60, #20, cells in m-direction
      firstElementSizeHubToShroud = 0.001, #0.010
      firstElementSizeInlet = 0.01, #0.05
      channelInletOutletDir = 2,
      channelHubShroudDir = 3,
      channelSuctionPressureDir = 1
    ).enableDebug().buildExtract( container )

    # openFOAM case setup
    from dtOOPythonApp.builder import ofOpenFOAMCase_turboMachine
    from dtOOPythonApp.builder import ofOpenFOAMCase_setupWrapper
    container = ofOpenFOAMCase_turboMachine(
      label = "of",
      bVs = [
        bV["mesh"], bV["meshDiffuser"]
      ],
      dictRule = \
          ofOpenFOAMCase_setupWrapper.controlDict(
            application = "simpleFoam",
            endTime = 300,
            QPatches = ['mesh_inlet','mesh_outlet','meshDiffuser_outlet', 'meshDiffuser_inlet'],
            PTPatches = ['mesh_inlet', 'mesh_outlet', 'meshDiffuser_outlet', 'meshDiffuser_inlet'],
            FPatches = ['mesh_blade',],
            libs = [
              "libsimpleFunctionObjects.so",
              "libsimpleSwakFunctionObjects.so",
              "libmappedFieldFixedValue.so",
              "libmixingPlane.so",
            ]
          )
        + ofOpenFOAMCase_setupWrapper.fvSchemes()
        + ofOpenFOAMCase_setupWrapper.fvSolution()
        + ofOpenFOAMCase_setupWrapper.transportModel()
        + ofOpenFOAMCase_setupWrapper.turbulenceProperties()
        + ofOpenFOAMCase_setupWrapper.MRFProperties(
           cellZones = ["mesh",],
           omegas    = [26.1799388],
           nonRotatingPatches = [
             [
               "mesh_suction", "mesh_pressure",
               "mesh_inlet", "mesh_outlet",
             ],
           ],
           patches = [],
           axes = [dtOO.dtVector3(0,0,1)],
           origins = [dtOO.dtPoint3(0,0,0)]
         ),
        fieldRules = [
          ofOpenFOAMCase_setupWrapper.fieldRuleString("U", [0.0,0.0,-7.0,]),
          ofOpenFOAMCase_setupWrapper.fieldRuleString("p", [0.0,]),
          ofOpenFOAMCase_setupWrapper.fieldRuleString("k", [0.1,]),
          ofOpenFOAMCase_setupWrapper.fieldRuleString("omega", [10.0,]),
          ofOpenFOAMCase_setupWrapper.fieldRuleString("nut", [0.1,]),
        ],
        setupRules = [
          ofOpenFOAMCase_setupWrapper.emptyRuleString(),
          ofOpenFOAMCase_setupWrapper.cylindricalInletRuleString(
            "mesh_inlet",
            ["U"],
            [ [-7.3391,np.tan(25.092*np.pi/180)*7.3391,0], ] #6.7

          ),
          ofOpenFOAMCase_setupWrapper.inletRuleString(
            "mesh_inlet",
            ["p", "k", "omega",],
            [ [0], [0.0, 0.05], [2.0] ]#cV["QIn"]()
          ),
          ofOpenFOAMCase_setupWrapper.wallRuleString(
            "mesh_shroud",
            ["omega", "U", "p", "k", "nut"]
          ),
           ofOpenFOAMCase_setupWrapper.wallRuleString(
            "mesh_hub",
            ["omega", "U", "p", "k", "nut"]
          ),
          ofOpenFOAMCase_setupWrapper.wallRuleString(
            "mesh_blade",
            ["omega", "U", "p", "k", "nut"]
          ),
          ofOpenFOAMCase_setupWrapper.cyclicAmiRuleString(
            "mesh_pressure", "mesh_suction"
          ),
          ofOpenFOAMCase_setupWrapper.mixingPlaneRuleString(
            "mesh_outlet", "meshDiffuser_inlet",
            ["U", "p", "k", "omega",],
            axis = dtOO.dtVector3(0,0,1),
            origin = dtOO.dtPoint3(0,0,0),
            stackAxis = "R"
          ),
          ofOpenFOAMCase_setupWrapper.wallRuleString(
            "meshDiffuser_hub",
            ["omega", "U", "p", "k", "nut"]
          ),
          ofOpenFOAMCase_setupWrapper.wallRuleString(
            "meshDiffuser_shroud",
            ["omega", "U", "p", "k", "nut"]
          ),
          ofOpenFOAMCase_setupWrapper.cyclicAmiRuleString(
            "meshDiffuser_pressure", "meshDiffuser_suction"
          ),
          ofOpenFOAMCase_setupWrapper.outletRuleString(
            "meshDiffuser_outlet",
            ["U", "p", "k", "omega",]
          ),
        ]

    ).buildExtract( container )

    #from dtOOPythonSWIG import IVHOstateHandler
    dtOO.lVHOstateHandler().makeState("roskreppRunner_4_1")

    container.cptr_dC()["of"].runCurrentState()


    return container


def createStep(container):

  # STEP output aG
  from dtOO2OCC import dtOO2OCC
  dtOO2OCC.WriteSTEP(
    dtOO2OCC.TopoDS(
      [
        dtOO.rotatingMap2dTo3d.MustConstDownCast(
          container.cptr_aG()["channel"]
        ).segmentConstUPercent(0.0),
        dtOO.rotatingMap2dTo3d.MustConstDownCast(
          container.cptr_aG()["channel"]
        ).segmentConstVPercent(0.0),
        dtOO.rotatingMap2dTo3d.MustConstDownCast(
          container.cptr_aG()["channel"]
        ).segmentConstVPercent(1.0),
        dtOO.rotatingMap2dTo3d.MustConstDownCast(
          container.cptr_aG()["channel"]
        ).segmentConstWPercent(0.0),
        dtOO.rotatingMap2dTo3d.MustConstDownCast(
          container.cptr_aG()["channel"]
        ).segmentConstWPercent(1.0),
        dtOO2OCC.analyticSurface_analyticGeometry(
          container.cptr_aG()["xyz_meanplane"]
        ).approx(),
        dtOO2OCC.analyticSurface_analyticGeometry(
          container.cptr_aG()["xyz_blade"]
        ).approx(),
        container.cptr_aG()["draftTubeCone"],
        container.cptr_aG()["draftTubeElbow"],
        container.cptr_aG()["draftTubeDiffusor"],
      ]
    ),
    "writeSTEP_aG.stp"
  )
  # STEP output aF
  dtOO2OCC.WriteSTEP(
    dtOO2OCC.TopoDSFunction(
      [
        container.cptr_aF()["blade"],
      ]
    ),
    "writeSTEP_aF.stp"
  )

  return


def makeGrid():
  cc = roskreppRunner_4().create()

  bV = cc.cptr_bV()
  bV["mesh"].makeGrid()
  bV["meshDiffuser"].makeGrid()

  return

  
def CreateAndShow( *args, **kwargs ):
  """Create and show machine.

  # Parameters
  ----------
  None

  Returns
  -------
  List[ dtOO.dtBundle, dtOOInParaVIEW ]
    List containing the dtBundle and the render object for ParaVIEW.

  See Also:
  ---------
  roskreppRunner.create : Create machine.

  """
  from dtOOPythonApp.vis import dtOOInParaVIEW
  cc = roskreppRunner_4().create(*args, **kwargs)

  rr = dtOOInParaVIEW( cc )
  rr.Show( rr.Find( "channel.*", True), "channel" )
  rr.Show( rr.Find( "xyz_diffuser.*", True), "xyz_diffuser" )
  rr.Show( rr.Find( "xyz_meanplane.*", True), "xyz_meanplane" )
  rr.Show( rr.Find( "xyz_blade.*", True), "xyz_blade" )
#  rr.Show( rr.Find( "xyz_meshBlock.*", True), "xyz_meshBlock" )
  rr.Show( rr.Find( "meanplane.*", True), "meanplane" )
  rr.Show( rr.Find( "blade.*", True), "blade" )
#  rr.Show( rr.Find( "meshBlock.*", True), "meshBlock" )
  rr.Show( rr.Find( "xyz_fe_meanplane.*", True), "xyz_fe_meanplane" )
  rr.Show( rr.Find( "xyz_gridChannel.*", True), "xyz_gridChannel" )
#  rr.Show( rr.Find( "draftTubeElbowCenterline.*", True), "draft_tube_elbow_centerline" )
#  rr.Show( rr.Find( "draftTubeDiffusorCenterline.*", True), "draft_tube_diffusor_centerline" )
#  rr.Show( rr.Find( "draftTubeCone.*", True), "draft_tube_cone" )
#  rr.Show( rr.Find( "draftTubeElbow.*", True), "draft_tube_elbow" )
#  rr.Show( rr.Find( "draftTubeDiffusor.*", True), "draft_tube_diffusor" )
#  rr.Show( rr.Find( "xyz_gridDiffuser.*", True), "xyz_gridDiffuser" )

  return cc, rr

if __name__ == "__main__":
  roskreppRunner_4().create()
