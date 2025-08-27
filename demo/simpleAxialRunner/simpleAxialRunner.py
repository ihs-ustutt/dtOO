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

class simpleAxialRunner:
  """Simple machine consisting of an axial runner only.

  Create a very simple machine that consists only of an axial runner. First
  an object is created by:

  >>> machine = simpleAxialRunner()

  The channel is created by two splines each containing two control points. 
  Within the channel a mean plane is defined and a thickness distribution
  is added. The machine is created by: 

  >>> bundle = machine.create(alpha_2_1=40.0) 
  
  This results in a machine with an outlet angle at the shroud of 40 degree.

  The OpenFOAMCase is created by:

  >>> bundle.cptr_dC()["of"].runCurrentState()

  The current case is then written to a directory:

  >>> cDir = bundle.cptr_dC()["of"].getDirectory( 
  ...   dtOO.lVHOstateHandler().commonState() 
  ... )

  The `foamlib` package is used to modify the dicts.

  >>> import foamlib
  
  Initialize ``foamlib`` object

  >>> fc = foamlib.FoamCase( cDir )

  Before running the simulation the `writeInterval` and `endTime` is adjusted 
  by:

  >>> fc.control_dict['writeInterval'] = 25
  >>> fc.control_dict['endTime'] = 50

  Additionally, the flow is treated as laminar:

  >>> fc.turbulence_properties["RAS"]["turbulence"] = False

  The `foamlib` package is used to start the simulation:

  >>> fc.run()

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
    alpha_1_0: float = 90.0, 
    alpha_1_1: float = 90.0, 
    alpha_2_0: float = 80.0, 
    alpha_2_1: float = 80.0
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
    dtOO.logMe.initLog('build.log')

    # Init staticProperties
    dtOO.staticPropertiesHandler.getInstance().jInit(
      dtOO.jsonPrimitive(
        '{'
          '"option" : ['
            '{"name" : "reparamOnFace_precision", "value" : "1.e-06"},'
            '{"name" : "reparamInVolume_precision","value" : "1.e-06"},'
            '{"name" : "reparam_internalRestarts", "value" : "10"},'
            '{"name" : "reparam_restarts", "value" : "10"},'
            '{"name" : "reparam_restartIncreasePrecision", "value" : "10."},'
            '{'
              '"name" : "reparam_internalRestartDecreasePrecision",'
              ' "value" : "0.9"'
            '},'
            '{"name" : "invY_precision", "value" : "1.e-04"},'
            '{"name" : "xyz_resolution", "value" : "1.e-06"},'
            '{"name" : "XYZ_resolution", "value" : "1.e-05"},'
            '{"name" : "uvw_resolution", "value" : "1.e-04"},'
            '{"name" : "point_render_diameter", "value" : "0.005"},'
            '{"name" : "vector_render_size", "value" : "0.05"},'
            '{"name" : "function_render_resolution_u", "value" : "21"},'
            '{"name" : "function_render_resolution_v", "value" : "21"},'
            '{"name" : "function_render_resolution_w", "value" : "21"},'
            '{"name" : "geometry_render_resolution_u", "value" : "21"},'
            '{"name" : "geometry_render_resolution_v", "value" : "21"},'
            '{"name" : "geometry_render_resolution_w", "value" : "21"},'
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
          dtOO.dtPoint2(0.00, (np.pi/180.) * alpha_1_0),  
          dtOO.dtPoint2(1.00, (np.pi/180.) * alpha_1_1),
        ],
        1
      )(),
      alphaTwo = scaOneD_scaCurve2dOneDPointConstruct(
        [
          dtOO.dtPoint2(0.00, (np.pi/180.) * alpha_2_0),  
          dtOO.dtPoint2(1.00, (np.pi/180.) * alpha_2_1),
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

    # blade thickness distribution
    from dtOOPythonApp.builder import (
      vec3dSurfaceTwoD_fivePointsBSplineThicknessDistribution
    )
    container = vec3dSurfaceTwoD_fivePointsBSplineThicknessDistribution(
      "thicknessDistribution",
      spanwiseCuts = [
        0.00,  
        1.00,
      ],
      tLe = scaOneD_scaCurve2dOneDPointConstruct(
        [
          dtOO.dtPoint2(0.00, 0.05),  
          dtOO.dtPoint2(1.00, 0.05),
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
          dtOO.dtPoint2(0.00, 0.07),  
          dtOO.dtPoint2(1.00, 0.07),
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
        ],
        1
      )(),
      uTe = scaOneD_scaCurve2dOneDPointConstruct(
        [
          dtOO.dtPoint2(0.00, 0.80),  
          dtOO.dtPoint2(1.00, 0.80),
        ],
        1
      )()
    ).buildExtract( container )

    # blade
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
   
    # mesh block
    fRef = dtOO.vec3dMuParserTwoD(
      "1.0*0.15, xx, yy", "xx", "yy"
    )
    fRef.setLabel("thicknessMeshBlock")
    for i in range(2):
      fRef.setMin(i, +0.0)
      fRef.setMax(i, +1.0)
    aF.set( fRef.clone() )
    dAdd = dtOO.discreteAddNormal()
    dAdd.jInit(
      dtOO.jsonPrimitive(
        '{"option" : [{"name" : "debug", "value" : "false"}]}'
        )\
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

    #
    # do conformal mapping
    #
    for ii in ["meanplane", "blade", "meshBlock",]:
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
        dtOO.map3dTo3d.ConstDownCast( aG["xyz_channel"] )
      )
      theAG.setLabel("xyz_"+ii)
      aG.push_back( theAG.clone() )

    # fe_meanplane
    from dtOOPythonApp.builder import (
      vec3dTwoDInMap3dTo3d_approximateAndFullExtendMeanplane
    )
    container = vec3dTwoDInMap3dTo3d_approximateAndFullExtendMeanplane( 
      label = "xyz_fe_meanplane",
      channel = aG["xyz_channel"],
      meanplane = aG["xyz_meanplane"],
      ratioOut = scaOneD_scaCurve2dOneDPointConstruct(
        [
          dtOO.dtPoint2(0.00, 0.50),
          dtOO.dtPoint2(0.50, 0.00),
          dtOO.dtPoint2(1.00, 0.50)
        ]
      )(),
      segUs = [0.0, 0.1, 0.5, 0.9, 1.0],
      segVs = [0.0, 0.5, 1.0 ]
    ).buildExtract( container )

    # grid channel
    from dtOOPythonApp.builder import (
      rotatingMap2dTo3d_gridChannel
    )
    container = rotatingMap2dTo3d_gridChannel(
      label = "xyz_gridChannel", 
      channelSide = aG["xyz_fe_meanplane"], 
      numberOfSections = 5,
      rotVector = dtOO.dtVector3(0, 0, -1)
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
    from dtOOPythonApp.builder import (
      map3dTo3dGmsh_gridFromChannelAndBlocks
    )
    container = map3dTo3dGmsh_gridFromChannelAndBlocks(
      label = "mesh",
      channel = aG["xyz_gridChannel"],
      blocks = blocks,
      blade = aG["xyz_blade"],
      couplingFaces = couplingFaces,
      nBoundaryLayers = 6,
      nElementsSpanwise = 30,
      nElementsNormal = 10,
      firstElementSizeHubToShroud = 0.005,
      firstElementSizeNormalBlade = 0.005,
      bladeHubElementSize = scaOneD_scaCurve2dOneDPointConstruct(
        [
          dtOO.dtPoint2(0.00, 0.010),  
          dtOO.dtPoint2(0.45, 0.007),  
          dtOO.dtPoint2(0.50, 0.007),  
          dtOO.dtPoint2(0.55, 0.007),  
          dtOO.dtPoint2(1.00, 0.010),
        ], 1
      )(),
      bladeHubElementScale = 0.10,
      channelInletOutletDir = 2,
      channelHubShroudDir = 3,
      charLengthMax=0.05,
      charLengthMin=0.025
    ).enableDebug().buildExtract( container )

    # channelExtension
    container = rotatingMap2dTo3d_hubShroudCreate(
      "xyz_diffuser",
      shroudPoints = [
        dtOO.dtPoint3(2.00, 0.00, 0.00),  
        dtOO.dtPoint3(2.50, 0.00, -3.00),  
      ],
      hubPoints = [
        dtOO.dtPoint3(0.50, 0.00, 0.00),  
        dtOO.dtPoint3(0.50, 0.00, -3.00),  
      ],
      order = 1,
      rotVector = dtOO.dtVector3(0, 0, -1)
    ).buildExtract( container )
    container = rotatingMap2dTo3d_gridChannel(
      label = "xyz_gridDiffuser", 
      channelSide = dtOO.map3dTo3d.MustConstDownCast( 
        aG["xyz_diffuser"] 
      ).segmentConstUPercent(0.0),
      numberOfSections = 5,
      rotVector = dtOO.dtVector3(0, 0, 1)
    ).buildExtract( container )
    from dtOOPythonApp.builder import (
      map3dTo3dGmsh_gridFromChannel
    )
    container = map3dTo3dGmsh_gridFromChannel(
      label = "meshDiffuser",
      channel = aG["xyz_gridDiffuser"],
      nBoundaryLayers = 6,
      nElementsRadial = 30,
      nElementsCircumferential = 10,
      nElementsMeridional = 20,
      firstElementSizeHubToShroud = 0.005,
      firstElementSizeInlet = 0.05,
      channelInletOutletDir = 2,
      channelHubShroudDir = 3,
      channelSuctionPressureDir = 1
    ).enableDebug().buildExtract( container )

    from dtOOPythonApp.builder import (
      ofOpenFOAMCase_turboMachine,
      ofOpenFOAMCase_setupWrapper
    )
    container = ofOpenFOAMCase_turboMachine(
      label = "of",
      bVs = [
        bV["mesh"], bV["meshDiffuser"],
      ],
      dictRule = \
          ofOpenFOAMCase_setupWrapper.controlDict(
            application = "simpleFoam",
            endTime = 200,
            QPatches = ['mesh_inlet','mesh_outlet',],
            PTPatches = ['mesh_inlet', 'mesh_outlet',],
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
        + ofOpenFOAMCase_setupWrapper.turbulenceProperties(),
        #+ ofOpenFOAMCase_setupWrapper.MRFProperties(
        #    cellZones = ["mesh",], 
        #    omegas    = [9.81,], 
        #    nonRotatingPatches = [
        #      [
        #        "mesh_suction", "mesh_pressure", 
        #        "mesh_inlet", "mesh_outlet",
        #        "mesh_shroud",
        #      ],
        #    ], 
        #    patches = [], 
        #    axes = [dtOO.dtVector3(0,0,1)],
        #    origins = [dtOO.dtPoint3(0,0,0)]
        #  ),
        fieldRules = [ 
          ofOpenFOAMCase_setupWrapper.fieldRuleString("U", [0.0,0.0,-1.0,]),
          ofOpenFOAMCase_setupWrapper.fieldRuleString("p", [0.0,]),
          ofOpenFOAMCase_setupWrapper.fieldRuleString("k", [0.1,]),
          ofOpenFOAMCase_setupWrapper.fieldRuleString("omega", [0.1,]),
          ofOpenFOAMCase_setupWrapper.fieldRuleString("nut", [0.1,]),
        ],
        setupRules = [
          ofOpenFOAMCase_setupWrapper.emptyRuleString(),
          ofOpenFOAMCase_setupWrapper.cylindricalInletRuleString(
            "mesh_inlet", 
            ["U"], 
            [ [0,1,-2], ]
          ), 
          ofOpenFOAMCase_setupWrapper.inletRuleString(
            "mesh_inlet", 
            ["p", "k", "omega",], 
            [ [0], [0.0, 0.10], [0.001, 0.1] ]
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


    return container
  
def CreateAndShow( *args, **kwargs ):
  """Create and show machine.

  Parameters
  ----------
  None

  Returns
  -------
  List[ dtOO.dtBundle, dtOOInParaVIEW ]
    List containing the dtBundle and the render object for ParaVIEW.

  See Also:
  ---------
  simpleAxialRunner.create : Create machine.

  """
  from dtOOPythonApp.vis import dtOOInParaVIEW
  cc = simpleAxialRunner().create(*args, **kwargs)

  rr = dtOOInParaVIEW( cc )
  rr.Show( rr.Find( "xyz_channel.*", True), "xyz_channel" )
  rr.Show( rr.Find( "xyz_diffuser.*", True), "xyz_diffuser" )
  rr.Show( rr.Find( "xyz_meanplane.*", True), "xyz_meanplane" )
  rr.Show( rr.Find( "xyz_blade.*", True), "xyz_blade" )
  rr.Show( rr.Find( "xyz_meshBlock.*", True), "xyz_meshBlock" )
  rr.Show( rr.Find( "meanplane.*", True), "meanplane" )
  rr.Show( rr.Find( "blade.*", True), "blade" )
  rr.Show( rr.Find( "meshBlock.*", True), "meshBlock" )
  rr.Show( rr.Find( "xyz_fe_meanplane.*", True), "xyz_fe_meanplane" )
  rr.Show( rr.Find( "xyz_gridChannel.*", True), "xyz_gridChannel" )
  rr.Show( rr.Find( "xyz_gridDiffuser.*", True), "xyz_gridDiffuser" )

  return cc, rr

if __name__ == "__main__":
  simpleAxialRunner().create()

