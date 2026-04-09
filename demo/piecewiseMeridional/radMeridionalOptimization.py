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

import re
import numpy as np
import sys
import importlib

class radMeridionalOptimization:

    def __init__(self, data: dict, createOFCase: bool):
        
        # constructiong variables for the class from the Config Class
        for key, value in data.items():
            setattr(self, key, value)

        self.createOFCase_ = createOFCase
            
    def create(self,) -> dtOO.dtBundle:
        # Create an empty container
        container = dtOO.dtBundle()
        
        #importlib.reload(builder)
        # Create log file
        dtOO.logMe.initLog('build.log')

        # Init staticProperties
        dtOO.staticPropertiesHandler.getInstance().jInit(
          dtOO.jsonPrimitive(
            '{'
              '"option" : ['
                '{"name" : "reparamOnFace_precision", "value" : "1.e-05"},'
                '{"name" : "reparamInVolume_precision","value" : "1.e-05"},'
                '{"name" : "invY_precision", "value" : "1.e-04"},'
                '{"name" : "xyz_resolution", "value" : "1.e-05"},'
                '{"name" : "XYZ_resolution", "value" : "1.e-04"},'
                '{"name" : "uvw_resolution", "value" : "1.e-03"},'
                '{"name" : "point_render_diameter", "value" : "0.005"},'
                '{"name" : "vector_render_size", "value" : "0.05"},'
                '{"name" : "function_render_resolution_u", "value" : "50"},'
                '{"name" : "function_render_resolution_v", "value" : "50"},'
                '{"name" : "function_render_resolution_w", "value" : "50"},'
                '{"name" : "geometry_render_resolution_u", "value" : "50"},'
                '{"name" : "geometry_render_resolution_v", "value" : "50"},'
                '{"name" : "geometry_render_resolution_w", "value" : "50"},'
                '{"name" : "ompNumThreads", "value" : "2"},'
                '{"name" : "map1dTo3d_deltaPer", "value" : "0.01"},'
                '{"name" : "map2dTo3d_deltaPer", "value" : "0.01"},'
                '{"name" : "map3dTo3d_deltaPer", "value" : "0.01"},'
                '{"name" : "logLevel", "value" : "99"},'
                '{"name" : "isEqualExtendCheck", "value" : "true"}'
              ']'
            '}'
          )
        )

        # Init baseContainer and labeledVectorHandlings
        self.bC = container.cptr_bC()
        self.cV = container.cptr_cV()
        
        self.cV.push_back(
            dtOO.sliderFloatParam("QIn", 0.5, 0.0, 1.0).clone()
        )
        # clearing prevents segmentation fault
        dtOO.lVHOstateHandler.clear()
        dtOO.lVHOstateHandler( dtOO.jsonPrimitive(), self.cV ).thisown = False

        self.aF = container.cptr_aF() 
        self.aG = container.cptr_aG() 
        self.bV = container.cptr_bV() 
        self.dC = container.cptr_dC() 
        self.dP = container.cptr_dP() 
        
        # creating the meridional contour
        modname = "dtOOPythonApp.builder.analyticGeometry_piecewiseMeridionalRotContour"
        module = self.reloadModule(modname)
        radMeridionalContour = module.analyticGeometry_piecewiseMeridionalRotContour( 
          self.label,
          self.hubCurves,
          self.shroudCurves,
          self.interface_hub,
          self.interface_shroud,
          self.interface_curvature,
        ).enableDebug()#.buildExtract( container )
        container = radMeridionalContour.buildExtract(container)
        
        # 
        # guide vane
        #
        gvLabel = "gv"
        self.aG.push_back( 
            radMeridionalContour.getRegChannel(0, 1) << "xyz_"+gvLabel+"_channel" 
        )
        spanwiseCuts = [0.00, 1.00,]
        gv_alpha_1 = [(np.pi/180.) * -55.0]
        gv_alpha_2 = [(np.pi/180.) * -16.0]
        gv_ratioX = [0.5]
        gv_deltaY = [0.12]
        gv_offX = [-0.046]
        gv_offY = [0.077]
        
        gv_t_le = [0.01]
        gv_u_le = [0.00]
        gv_t_mid = [0.03]
        gv_u_mid = [0.20]
        gv_t_te = [0.01]
        gv_u_te = [0.80]
        
        print("guide vane")
        self.buildBlade(
                container,
                gvLabel,
                self.nGvBlades, False,
                spanwiseCuts, gv_alpha_1, gv_alpha_2, gv_ratioX, gv_deltaY, gv_offX, gv_offY,
                spanwiseCuts, gv_t_le, gv_u_le, gv_t_mid, gv_u_mid, gv_t_te, gv_u_te,
            ) 
        
        #
        # runner
        #
        ruLabel = "ru"
        self.aG.push_back( 
            radMeridionalContour.getRegChannel(1, 1) << "xyz_"+ruLabel+"_channel" 
        )
        spanwiseCuts_mp = [0.00, 0.33,  0.66, 1.00,]
        ru_alpha_1 = [
                (np.pi/180.) * 95.,
                (np.pi/180.) * 85.,
                (np.pi/180.) * 60.
                ]
        ru_alpha_2 = [
                (np.pi/180.) * 16.,
                (np.pi/180.) * 15.,
                (np.pi/180.) * 20.,
                (np.pi/180.) * 8.
                ]
        ru_ratioX = [
                0.44,
                #0.35,
                0.50,
                0.50,
                0.22
                ]
        ru_deltaY = [
                0.80,
                0.84,
                0.64,
                0.52
                ]
        ru_offX = [
                0.105,
                0.11,
                0.0
                ]
        ru_offY = [
                0.065,
                0.075,
                0.035
                ]
        
        spanwiseCuts_td = [0.00, 1.00,]
        ru_t_le = [0.020,
                   0.018]
        ru_u_le = [0.00]
        ru_t_mid = [0.04,
                    0.03]
        ru_u_mid = [0.50]
        ru_t_te = [0.02]
        ru_u_te = [0.80]
        
        print("runner")
        self.buildBlade(
                container,
                ruLabel,
                self.nRuBlades, True,
                spanwiseCuts_mp, ru_alpha_1, ru_alpha_2, ru_ratioX, ru_deltaY, ru_offX, ru_offY,
                spanwiseCuts_td, ru_t_le, ru_u_le, ru_t_mid, ru_u_mid, ru_t_te, ru_u_te,
            )
 
        #
        # Building Layer Region
        #
        speHub, speShroud, inOutCurves = radMeridionalContour.getLayerRegionCurves()
        # creating the layer region object
        modname = "dtOOPythonApp.builder.analyticGeometry_layerRegion"
        module = self.reloadModule(modname)
        layerRegion = module.analyticGeometry_layerRegion( 
          self.label,
          speHub,
          speShroud,
          inOutCurves,
          self.layer_thickness,
          self.layer_supports,
        ).enableDebug()#.buildExtract( container )
        container = layerRegion.buildExtract(container)
        
        #
        # Meshing of layer region
        #
        # returning the hub and shroud layers
        layers = layerRegion.getLayerList(self.nRuBlades)    
        # returns layer data in the following nested list:
        # layers = [[hub layer lists],[shroud layer list]]
        # with:
        # [hub layer lists] = [[3d layer domain], [bool list radius zero]]

        # returns the unstructured region and its surfaces
        mv, bs = layerRegion.getUnstructuredRegion(self.nRuBlades)
        
        modname = "dtOOPythonApp.builder.map3dTo3dGmsh_gridFromLayers"
        module = self.reloadModule(modname)
        #from dtOOPythonApp.builder import ( map3dTo3dGmsh_gridFromLayers )
        # module.map3dTo3dGmsh_gridFromLayers(
            # label, layer data, nElementsLayer, firstElementSize, meshSizeSW, meshSizeCIRC, unstructRegion
        # )
        
        # creating the mesh of the suction area with wall layers and the unstructured region
        container = module.map3dTo3dGmsh_gridFromLayers(
                label = "meshLayers",   
                layers = layers,        
                nElementsLayer = 20,     
                firstElement = 0.001,    
                elementSize_sw = 0.01,  
                elementSize_circ = 0.03,
                mv = mv,                
                bs = bs                  
            ).buildExtract(container)
        
        if self.createOFCase_ == True:
            
            stateLbl = "myState"
            dtOO.lVHOstateHandler().makeState(stateLbl)
            
            #self.bV[gvLabel+"_mesh"].makeGrid() 
            #self.bV[ruLabel+"_mesh"].makeGrid() 
            #self.bV["meshLayers"].makeGrid()
            
            #
            # draft tube
            #
            gbv = dtOO.gmshBoundedVolume()
            gbv.jInit(dtOO.jsonPrimitive('{"label" : "dt_mesh"}'), None, None, None, None, None)
            gbv.thisown = False
            self.bV.push_back( gbv )

            rmsh = dtOO.bVOReadMSH()
            rmsh.jInit(dtOO.jsonPrimitive('{"_filename" : "dt_mesh.msh"}'), gbv)
            rmsh.preUpdate()
            
            # naming physical faces
            gm = gbv.getModel()
            for i in range( gm.getNumFaces() ):
              gm.untagPhysical( gm.getFaceByIndex(i) )
              gm.tagPhysical( gm.getFaceByIndex(i), "dt_mesh_wall" )

            for i in [0,36,]:
              gm.untagPhysical( gm.getFaceByIndex(i) )
              gm.tagPhysical( gm.getFaceByIndex(i), "dt_mesh_inlet" )

            for i in [35,37,]:
              gm.untagPhysical( gm.getFaceByIndex(i) )
              gm.tagPhysical( gm.getFaceByIndex(i), "dt_mesh_outlet" )
            
            # translating the draft tube
            trans = dtOO.dtVector3(0,0,-self.h_shroud-0.16)
            for i in range(gm.getNumMeshVertices()):
              print(i)
              gm.translatePosition( gm.getMeshVertexByTag(i+1), trans )
             
            #
            # of case setup
            #
            from dtOOPythonApp.builder import (
              ofOpenFOAMCase_turboMachine,
              ofOpenFOAMCase_setupWrapper
            )

            container = ofOpenFOAMCase_turboMachine(
              label = "of",
              bVs = [
                self.bV["gv_mesh"], self.bV["ru_mesh"], self.bV["meshLayers"], self.bV["dt_mesh"],
              ],
              dictRule = \
                  ofOpenFOAMCase_setupWrapper.controlDict(
                    application = "simpleFoam",
                    endTime = 2000,
                    # Patches where Q and PT is tracked
                    QPatches = ['gv_mesh_inlet', 'gv_mesh_outlet', 
                                'ru_mesh_inlet', 'ru_mesh_outlet',
                                'meshLayers_inlet', 'meshLayers_outlet',
                                'dt_mesh_inlet', 'dt_mesh_outlet',
                                'gv_mesh_suction', 'gv_mesh_pressure',
                                'ru_mesh_suction', 'ru_mesh_pressure',
                                'meshLayers_periodic0', 'meshLayers_periodic1'],
                    PTPatches = ['gv_mesh_inlet', 'gv_mesh_outlet',
                                'ru_mesh_inlet', 'ru_mesh_outlet',
                                'meshLayers_inlet', 'meshLayers_outlet',
                                'dt_mesh_inlet', 'dt_mesh_outlet',
                                'gv_mesh_suction', 'gv_mesh_pressure',
                                'ru_mesh_suction', 'ru_mesh_pressure',
                                'meshLayers_periodic0', 'meshLayers_periodic1'],
                    FPatches = ['gv_mesh_blade', 'ru_mesh_blade'],
                    libs = [
                      "libsimpleFunctionObjects.so",
                      #"libsimpleSwakFunctionObjects.so",
                      #"libmappedFieldFixedValue.so",
                      "libmixingPlane.so",
                    ]
                  )
                + ofOpenFOAMCase_setupWrapper.fvSchemes()
                + ofOpenFOAMCase_setupWrapper.fvSolution()
                + ofOpenFOAMCase_setupWrapper.transportModel()
                + ofOpenFOAMCase_setupWrapper.turbulenceProperties()
                + ofOpenFOAMCase_setupWrapper.MRFProperties(
                    cellZones = ["ru_mesh",],
                    omegas    = [375*2*np.pi/60,],
                    nonRotatingPatches = [
                      [
                        "ru_mesh_suction", "ru_mesh_pressure",
                        "ru_mesh_inlet", "ru_mesh_outlet",
                      ],
                    ],
                    patches = [],
                    axes = [dtOO.dtVector3(0,0,-1)],
                    origins = [dtOO.dtPoint3(0,0,0)]
                  ),
                fieldRules = [
                  ofOpenFOAMCase_setupWrapper.fieldRuleString("U", [0.0,0.0,-1.0,]),
                  ofOpenFOAMCase_setupWrapper.fieldRuleString("p", [0.0,]),
                  ofOpenFOAMCase_setupWrapper.fieldRuleString("k", [0.1,]),
                  ofOpenFOAMCase_setupWrapper.fieldRuleString("omega", [0.1,]),
                  ofOpenFOAMCase_setupWrapper.fieldRuleString("nut", [0.1,]),
                ],
                setupRules = [
                  ofOpenFOAMCase_setupWrapper.emptyRuleString(),
                  # meshInlet
                  ofOpenFOAMCase_setupWrapper.cylindricalInletRuleString(
                    "gv_mesh_inlet",
                    ["U"],
                    [ [-2.92*2.7,-7.23*2.7,0], ]
                  ),
                  ofOpenFOAMCase_setupWrapper.inletRuleString(
                    "gv_mesh_inlet",
                    ["p", "k", "omega",],
                    [ [0], [0.0, 0.10], [0.032*self.h_inlet, 0.1] ]
                  ),
                  ofOpenFOAMCase_setupWrapper.wallRuleString(
                    "gv_mesh_shroud",
                    ["omega", "U", "p", "k", "nut"]
                  ),
                   ofOpenFOAMCase_setupWrapper.wallRuleString(
                    "gv_mesh_hub",
                    ["omega", "U", "p", "k", "nut"]
                  ),
                  ofOpenFOAMCase_setupWrapper.wallRuleString(
                    "gv_mesh_blade",
                    ["omega", "U", "p", "k", "nut"]
                  ),
                  ofOpenFOAMCase_setupWrapper.cyclicAmiRuleString(
                    "gv_mesh_suction", "gv_mesh_pressure"
                  ),
                  # mixing plane?
                  #ofOpenFOAMCase_setupWrapper.cyclicAmiRuleString(
                  #  "meshInlet_outlet", "meshChannel_inlet",
                  #  rotAxis = None,
                  #  rotCentre = None
                  #),
                  ofOpenFOAMCase_setupWrapper.mixingPlaneRuleString(
                    "gv_mesh_outlet", "ru_mesh_inlet",
                    ["U", "p", "k", "omega",],
                    axis = dtOO.dtVector3(0,0,1),
                    origin = dtOO.dtPoint3(0,0,0),
                    stackAxis = "Z",
                    #discretization = "userDefined",
                    #planes = 13,
                    #planesBl = 7,
                    #gradingIf = True
                  ),

                  # runner
                  ofOpenFOAMCase_setupWrapper.wallRuleString(
                    "ru_mesh_hub",
                    ["omega", "U", "p", "k", "nut"]
                  ),
                  ofOpenFOAMCase_setupWrapper.wallRuleString(
                    "ru_mesh_shroud",
                    ["omega", "U", "p", "k", "nut"]
                  ),
                  ofOpenFOAMCase_setupWrapper.wallRuleString(
                    "ru_mesh_blade",
                    ["omega", "U", "p", "k", "nut"]
                  ),
                  ofOpenFOAMCase_setupWrapper.cyclicAmiRuleString(
                    "ru_mesh_suction", "ru_mesh_pressure"
                  ),
                  #ofOpenFOAMCase_setupWrapper.cyclicAmiRuleString(
                  #  "meshChannel_outlet", "meshLayers_inlet",
                  #  rotAxis = None,
                  #  rotCentre = None
                  #),
                  ofOpenFOAMCase_setupWrapper.mixingPlaneRuleString(
                    "ru_mesh_outlet", "meshLayers_inlet",
                    ["U", "p", "k", "omega",],
                    axis = dtOO.dtVector3(0,0,1),
                    origin = dtOO.dtPoint3(0,0,0),
                    stackAxis = "R",
                    discretization = "userDefined",
                    planes = 5,
                    planesBl = 3,
                    gradingIf = "false"
                  ),

                  # mesh layers
                  ofOpenFOAMCase_setupWrapper.wallRuleString(
                    "meshLayers_hub",
                    ["omega", "U", "p", "k", "nut"]
                  ),
                  ofOpenFOAMCase_setupWrapper.wallRuleString(
                    "meshLayers_shroud",
                    ["omega", "U", "p", "k", "nut"]
                  ),
                  ofOpenFOAMCase_setupWrapper.cyclicAmiRuleString(
                    "meshLayers_periodic0", "meshLayers_periodic1"
                  ),
                  ofOpenFOAMCase_setupWrapper.mixingPlaneRuleString(
                    "meshLayers_outlet", "dt_mesh_inlet",
                    ["U", "p", "k", "omega",],
                    axis = dtOO.dtVector3(0,0,1),
                    origin = dtOO.dtPoint3(0,0,0),
                    stackAxis = "R"
                  ),
                  
                  # draft tube
                  ofOpenFOAMCase_setupWrapper.wallRuleString(
                    "dt_mesh_wall",
                    ["omega", "U", "p", "k", "nut"]
                  ),
                  ofOpenFOAMCase_setupWrapper.outletRuleString(
                    "dt_mesh_outlet",  
                    ["U", "p", "k", "omega",]
                  ),
                ]

            ).buildExtract( container )
            
            self.dC["of"].runCurrentState()
            """
            import foamlib
            
            cDir = self.dC["of"].getDirectory(
                dtOO.lVHOstateHandler().commonState()
            )
            
             
            fc = foamlib.FoamCase( cDir )
            fc.control_dict['writeInterval'] = 20
            fc.control_dict['endTime'] = 1000
            fc.turbulence_properties["simulationType"] = "laminar"
            fc.turbulence_properties["RAS"]["turbulence"] = False
            
            # Set mixingInterface block
            fc.fv_schemes["mixingInterface"] = {
                "U": "areaAveraging",
                "p": "areaAveraging",
                "k": "areaAveraging",
                "epsilon": "areaAveraging",
                "omega": "areaAveraging"
            }
            """ 
        return container  
    
    def buildBlade(self,
                   container,
                   label,
                   nBlades, adjustRadius,
                   spanwiseCuts_mp, alpha_1, alpha_2, ratioX, deltaY, offX, offY, 
                   spanwiseCuts_td, t_le, u_le, t_mid, u_mid, t_te, u_te):
        #
        # meanplane
        #
        from dtOOPythonApp.builder import (
            analyticSurface_threePointMeanplaneFromRatio,
            scaOneD_scaCurve2dOneDPointConstruct
        )
        container = analyticSurface_threePointMeanplaneFromRatio(
          label + "_meanplane",
          spanwiseCuts_mp,
          alphaOne = scaOneD_scaCurve2dOneDPointConstruct(
            *self.fillInputList(alpha_1)
          )(),
          alphaTwo = scaOneD_scaCurve2dOneDPointConstruct(
            *self.fillInputList(alpha_2)
          )(),
          ratioX = scaOneD_scaCurve2dOneDPointConstruct(
            *self.fillInputList(ratioX)
          )(),
          deltaY = scaOneD_scaCurve2dOneDPointConstruct(
            *self.fillInputList(deltaY)
          )(),
          offX = scaOneD_scaCurve2dOneDPointConstruct(
            *self.fillInputList(offX)
          )(),
          offY = scaOneD_scaCurve2dOneDPointConstruct(
            *self.fillInputList(offY)
          )(),
        ).buildExtract( container )
        
        #
        # thickness distribution
        #
        from dtOOPythonApp.builder import (
          vec3dSurfaceTwoD_fivePointsBSplineThicknessDistribution
        )
        container = vec3dSurfaceTwoD_fivePointsBSplineThicknessDistribution(
          label + "_thicknessDistribution",
          spanwiseCuts_td,
          tLe = scaOneD_scaCurve2dOneDPointConstruct(
            *self.fillInputList(t_le)
          )(),
          uLe = scaOneD_scaCurve2dOneDPointConstruct(
            *self.fillInputList(u_le)
          )(),
          tMid = scaOneD_scaCurve2dOneDPointConstruct(
            *self.fillInputList(t_mid)
          )(),
          uMid = scaOneD_scaCurve2dOneDPointConstruct(
            *self.fillInputList(u_mid)
          )(),
          tTe = scaOneD_scaCurve2dOneDPointConstruct(
            *self.fillInputList(t_te)
          )(),
          uTe = scaOneD_scaCurve2dOneDPointConstruct(
            *self.fillInputList(u_te)
          )()
        ).buildExtract( container )

        #
        # Combination of the Meanplane and the Thickness distribution
        #
        dAdd = dtOO.discreteAddNormal()
        dAdd.jInit(
          dtOO.jsonPrimitive(
            '{"option" : [{"name" : "debug", "value" : "false"}]}'
          )\
            .appendAnalyticFunction("_tt", self.aF[label + "_thicknessDistribution"])\
            .appendInt("_nU", 61)\
            .appendInt("_nV", 41)\
            .appendInt("_order", 3)\
            .appendDtVector3("_nf", dtOO.dtVector3(0,0,1)),
          None, None, self.aF, None
        )
        theAF = dAdd.applyAnalyticFunction(
          self.aF[label + "_meanplane"]
        )
        theAF.setLabel(label + "_blade")
        self.aF.push_back( theAF.clone() )
        
        #
        # conformalMapping
        #
        conMap = dtOO.uVw_phirMs()
        conMap.jInit(
          dtOO.jsonPrimitive()\
            .appendStr("label", "uVw_phirMs")\
            .appendInt("_nV", 31)\
            .appendInt("_nW", 11)\
            .appendBool("_adjustRadius", False)\
            .appendAnalyticGeometry(\
              "_rM2d", \
              self.aG.get("xyz_" + label + "_channel")\
            ),\
          None, None, None, self.aG \
        )
        self.bC.dtTransformer().add( conMap.clone() )
         
        #
        # depiction in 3d Space of blade and meanplane
        #
        for ii in [label+"_meanplane", label+"_blade",]:         
          theAG = dtOO.vec3dTwoDInMap3dTo3d(
            dtOO.vec3dTwoD.MustConstDownCast(
              conMap.applyAnalyticFunction(self.aF[ii].clone())
            ),
            dtOO.map3dTo3d.ConstDownCast( self.aG["xyz_"+label+"_channel"] )   
          )
          self.aG.push_back( theAG << "xyz_"+str(ii) )
        
        #
        # mesh block
        #
        meshBlock_thickness = 0.025 
        fRef = dtOO.vec3dMuParserTwoD(
          "1.0*"+str(meshBlock_thickness)+", xx, yy", "xx", "yy"
        )
        fRef.setLabel(label + "_thicknessMeshBlock")
        for i in range(2):
          fRef.setMin(i, +0.0)
          fRef.setMax(i, +1.0)
        self.aF.set( fRef.clone() )
        dAdd = dtOO.discreteAddNormal()
        dAdd.jInit(
          dtOO.jsonPrimitive(
            '{"option" : [{"name" : "debug", "value" : "false"}]}'
            )\
            .appendAnalyticFunction("_tt", self.aF[label+"_thicknessMeshBlock"])\
            .appendInt("_nU", 61)\
            .appendInt("_nV", 41)\
            .appendInt("_order", 3)\
            .appendDtVector3("_nf", dtOO.dtVector3(0,0,1)),
          None, None, self.aF, None
        )
        theAF = dAdd.applyAnalyticFunction( self.aF[label+"_blade"] )
        theAF.setLabel(label+"_meshBlock")
        self.aF.push_back( theAF.clone() )
        
        #
        # split mesh block
        #

        # number of mesh block faces which will be part of the meanplane
        #  starts from 0
        nMeanplaneBlocks = 3

        # split mesh block and create curves for meanplane
        modname = "dtOOPythonApp.builder.vec3dThreeD_skinAndSplit"
        module = self.reloadModule(modname)
        #from dtOOPythonApp.builder import vec3dThreeD_skinAndSplit
        container = module.vec3dThreeD_skinAndSplit(
          label =label+"_meshBlock",
          aFOne = self.aF[label+"_blade"],
          aFTwo = self.aF[label+"_meshBlock"],
          splitDim = 0,
          splits = [
            [0.00, 0.10],
            [0.10, 0.30],
            [0.30, 0.45],
            [0.45, 0.55],
            [0.55, 0.70],
            [0.70, 0.90],
            [0.90, 1.00],
          ],
          tEMeshBlockThickness = meshBlock_thickness,
          meanplaneFromBlocks = True,
          meanplaneExtOut = 0.02,
          meanplaneExtIn = 0.03,
          nMeanplaneBlocks = nMeanplaneBlocks
        ).buildExtract(container)
        
        # building the meanplane faces extending from the mesh blocks 
        #  to the tangentially offset meanplane curve
        for i in range(2):
            # curve seqence is switched between inlet and outlet in 
            #  order to keep u and v directions consistent with the 
            #  mesh block faces
            if i == 0:
                at = "in"
                c0 = 0
                c1 = 1
            elif i == 1:
                at = "out"
                c0 = 1
                c1 = 0
            # generating faces
            surf = dtOO.vec3dSurfaceTwoD(
                dtOO.bSplineSurface_exchangeSurfaceConstructOCC(
                    dtOO.bSplineSurface_skinConstructOCC(
                        dtOO.vec3dCurveOneD.MustDownCast(
                            self.aF[label+"_meshBlockCurve_"+at+str(c0)]
                        ).ptrConstDtCurve(),
                        dtOO.vec3dCurveOneD.MustDownCast(
                            self.aF[label+"_meshBlockCurve_"+at+str(c1)]
                        ).ptrConstDtCurve()
                    ).result()
                ).result()
            )
            self.aF.push_back(surf << label+"_fe_meanplane_"+at+str(0))
 
        #
        # do conformal mapping
        #
        #  mapping single faces
        for ii in [label+"_meanplane", label+"_blade", label+"_meshBlock",]:
          theAG = dtOO.vec3dTwoDInMap3dTo3d(
            dtOO.vec3dTwoD.MustConstDownCast(
              conMap.applyAnalyticFunction(self.aF[ii].clone())
            ),
            dtOO.map3dTo3d.ConstDownCast( self.aG["xyz_"+label+"_channel"] )
          )
          theAG.setLabel("xyz_"+ii)
          self.aG.push_back( theAG.clone() )
        #  mapping multiple faces (aF of meanplanefaces)
        for iNum in self.aF.getIndices(label+"_fe_meanplane_*"):
          ii = self.aF.getLabel( iNum )
          theAG = dtOO.vec3dTwoDInMap3dTo3d(
            dtOO.vec3dTwoD.MustConstDownCast(
              conMap.applyAnalyticFunction(self.aF[ii].clone())
            ),
            dtOO.map3dTo3d.ConstDownCast( self.aG["xyz_"+label+"_channel"] )
          )
          theAG.setLabel("xyz_"+ii)
          self.aG.push_back( theAG.clone() )
        #  mapping multiple volumes
        for iNum in self.aF.getIndices(label+"_meshBlock_*"):
          ii = self.aF.getLabel( iNum )
          theAG = dtOO.vec3dThreeDInMap3dTo3d(
            dtOO.vec3dThreeD.MustConstDownCast(
              conMap.applyAnalyticFunction(self.aF[ii].clone())
            ),
            dtOO.map3dTo3d.ConstDownCast( self.aG["xyz_"+label+"_channel"] )
          )
          theAG.setLabel("xyz_"+ii)
          self.aG.push_back( theAG.clone() )
        #  mapping multiple curves (meshBlockCurves)
        for iNum in self.aF.getIndices(label+"_meshBlockCurve_*"):
          ii = self.aF.getLabel( iNum )
          theAG = dtOO.vec3dOneDInMap3dTo3d(
            dtOO.vec3dOneD.MustConstDownCast(
              conMap.applyAnalyticFunction(self.aF[ii].clone())
            ),
            dtOO.map3dTo3d.ConstDownCast( self.aG["xyz_"+label+"_channel"] )
          )
          theAG.setLabel("xyz_"+ii)
          self.aG.push_back( theAG.clone() )
        

        # collecting the curves for the meanplane
        # mesh block curves extend form hub to shroud and are tangentially 
        #  offset from the specified mesh block faces
        meshBlockCurves = dtOO.labeledVectorHandlingAnalyticGeometry()
        for iNum in self.aG.getIndices("xyz_"+label+"_meshBlockCurve_*"):
            ii = self.aG.getLabel( iNum )
            meshBlockCurves.push_back(self.aG[ii].clone())
        
        # creating the meanplane surfaces extending from the tangentially 
        #  offset meshblock curves to the inlet or the outlet
        modname = "dtOOPythonApp.builder.analyticSurface_inOutFeMeanplane"
        module = self.reloadModule(modname)
        container = module.analyticSurface_inOutFeMeanplane(
            prefix = "xyz",
            label = label,
            channel = self.aG["xyz_"+label+"_channel"],
            curves = meshBlockCurves
        ).enableDebug().buildExtract(container)
         
        #
        # ordering geometries for meshing
        #
        # collecting and organizing mesh blocks
        blocks = []
        for iNum in self.aG.getIndices("xyz_"+label+"_meshBlock_*"):
            blocks.append( self.aG[ self.aG.getLabel( iNum ) ] )
        # sorting the blocks by number
        blocks.sort(key=lambda x: int(x.getLabel().split('_')[-1]))
        
        # collecting relevant faces
        # couplingFaces contains the faces which will connect the
        #   mesh block to the grid channel
        # meanplaneFaces are the faces which will form the periodic
        #   boundaries of the grid
        couplingFaces = []
        meanplaneFaces = []

        # faces extending from outlet to the mesh block trailing edge 
        meanplaneFaces.append(dtOO.map2dTo3d.MustDownCast(self.aG["xyz_"+label+"_fe_meanplane_out1"]))
        meanplaneFaces.append(dtOO.map2dTo3d.MustDownCast(self.aG["xyz_"+label+"_fe_meanplane_out0"]))

        for i, block in enumerate(blocks):
          face = dtOO.map3dTo3d.MustDownCast( block ).segmentConstWPercent( 1.0 )
          
          # appending the specified meanplane and coupling faces on the mesh blocks 
          if i <= nMeanplaneBlocks:
            meanplaneFaces.append(face)
            self.aG.push_back(face.clone() << "xyz_"+label+"_meanplaneFaceOnBlock_"+str(i))
          else:
            couplingFaces.append(face)
        
        # faces extending from the mesh block leading edge area to the inlet
        meanplaneFaces.append(dtOO.map2dTo3d.MustDownCast(self.aG["xyz_"+label+"_fe_meanplane_in0"]))
        meanplaneFaces.append(dtOO.map2dTo3d.MustDownCast(self.aG["xyz_"+label+"_fe_meanplane_in1"]))
        
        # last two coupling faces at trailing edge
        couplingFaces.append(dtOO.map3dTo3d.MustDownCast(blocks[-1]).segmentConstUPercent( 1.0 ))
        couplingFaces.append(dtOO.map3dTo3d.MustDownCast(blocks[0]).segmentConstUPercent( 0.0 ))
        
        #
        # create grid channel
        #
        modname = "dtOOPythonApp.builder.multipleBoundedVolume_gridChannel"
        module = self.reloadModule(modname)
        gridChannel = module.multipleBoundedVolume_gridChannel(
            label = label,
            channel = self.aG["xyz_"+label+"_channel"],
            meanplanes = meanplaneFaces,
            couplings = couplingFaces,
            nBlades = nBlades
        ).enableDebug()
        container = gridChannel.buildExtract(container)
        
        gc, gcFaces = gridChannel.getGridChannel()
        self.aG.push_back(
            gc << "xyz_"+label+"_gridChannel"
        )
        
        #
        # generating mesh
        #
        modname = "dtOOPythonApp.builder.map3dTo3dGmsh_gridFromMultipleBoundedVolumeAndBlocks"
        module = self.reloadModule(modname)
        #from dtOOPythonApp.builder import (
        #  map3dTo3dGmsh_gridFromMultipleBoundedVolumeAndBlocks
        #)
        container = module.map3dTo3dGmsh_gridFromMultipleBoundedVolumeAndBlocks(
          label = label+"_mesh",
          channel = self.aG["xyz_"+label+"_gridChannel"],
          channelFaces = gcFaces,
          blocks = blocks,
          nMeanplaneBlocks = nMeanplaneBlocks,
          blade = self.aG["xyz_"+label+"_blade"],
          nBoundaryLayers = 7,
          nElementsSpanwise = 40,
          nElementsNormal = 7,
          firstElementSizeHubToShroud = 0.001,
          firstElementSizeNormalBlade = 0.001,
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
          charLengthMax=0.05,
          charLengthMin=0.025,
          meshTEBlocks = True,
        ).enableDebug().buildExtract( container )
         
    #
    # returns a list with dtPoint2 types and spline orders
    #  with spanwise cut percentage and blade input parameters
    #
    def fillInputList(self, inList):
        
        # only one entry
        if len(inList) == 1:
            # the inputs will be the same at spanwiseCut 0 and 1
            outList = [
                        dtOO.dtPoint2(0.00, inList[0]), 
                        dtOO.dtPoint2(1.00, inList[0]),
                      ]      
            order = 1

        else:
            outList = []
            for nL in range(len(inList)):
                # calculating the percentage of the spanwise cut from normalized index
                cutPercent = nL/(len(inList)-1)
                # appending percentage and input
                outList.append(dtOO.dtPoint2(cutPercent, inList[nL]),)
            
            #the order is one less than the number of inputs
            order = len(inList) -1
        
        return outList, order

    def reloadModule(self, modname):  
        if modname in sys.modules: 
            print(f"reloading: {modname}")
            module=importlib.reload(sys.modules[modname])
        else:
            print(f"loading: {modname}")
            module=importlib.import_module(modname)
                
        return module
