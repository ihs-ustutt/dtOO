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
#import dtOOPythonApp.builder as builder

import numpy as np
import sys
import importlib
#import foamlib

class radMeridional:

    def __init__(self, data: dict):
        
        # constructiong variables for the class from the Config Class
        for key, value in data.items():
            setattr(self, key, value)

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
                '{"name" : "uvw_resolution", "value" : "1.e-04"},'
                '{"name" : "point_render_diameter", "value" : "0.005"},'
                '{"name" : "vector_render_size", "value" : "0.05"},'
                '{"name" : "function_render_resolution_u", "value" : "21"},'
                '{"name" : "function_render_resolution_v", "value" : "21"},'
                '{"name" : "function_render_resolution_w", "value" : "21"},'
                '{"name" : "geometry_render_resolution_u", "value" : "200"},'
                '{"name" : "geometry_render_resolution_v", "value" : "200"},'
                '{"name" : "geometry_render_resolution_w", "value" : "200"},'
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
        modname = "dtOOPythonApp.builder.analyticGeometryLayers_piecewiseMeridionalRotContour"
        module = self.reloadModule(modname)
        radMeridionalContour = module.analyticGeometryLayers_piecewiseMeridionalRotContour( 
          self.label,
          self.hubCurves,
          self.shroudCurves,
          self.layer_thickness,
          self.layer_supports,
          self.interface_hub,
          self.interface_shroud,
          self.interface_curvature,
        ).enableDebug()#.buildExtract( container )
        container = radMeridionalContour.buildExtract(container)
        
        # guide vane
        gvLabel = "gv"

        self.aG.push_back( 
            radMeridionalContour.getRegChannel(0, 1) << "xyz_"+gvLabel+"_channel" 
        )
        spanwiseCuts = [0.00, 1.00,]
        gv_alpha_1 = [(np.pi/180.) * -22.5]
        gv_alpha_2 = gv_alpha_1
        gv_ratioX = [1]
        gv_deltaY = [0.151]
        gv_offX = [-0.05]
        gv_offY = [0.062]
        
        gv_t_le = [0.02]
        gv_u_le = [0.00]
        gv_t_mid = [0.03]
        gv_u_mid = [0.50]
        gv_t_te = [0.02]
        gv_u_te = [0.80]

        self.buildBlade(
                container,
                gvLabel,
                self.nGvBlades, False,
                spanwiseCuts, gv_alpha_1, gv_alpha_2, gv_ratioX, gv_deltaY, gv_offX, gv_offY,
                spanwiseCuts, gv_t_le, gv_u_le, gv_t_mid, gv_u_mid, gv_t_te, gv_u_te,
            ) 
        #self.bV[gvLabel+"_mesh"].makeGrid() 
        
        ## runner
        #ruLabel = "ru"
        #self.aG.push_back( 
        #    radMeridionalContour.getRegChannel(1, 1) << "xyz_"+ruLabel+"_channel" 
        #)
        #spanwiseCuts_mp = [0.00, 0.33,  0.66, 1.00,]
        #ru_alpha_1 = [
        #        (np.pi/180.) * 90.,
        #        (np.pi/180.) * 75.,
        #        (np.pi/180.) * 52.
        #        ]
        #ru_alpha_2 = [
        #        (np.pi/180.) * 45., 
        #        (np.pi/180.) * 31., 
        #        (np.pi/180.) * 32., 
        #        (np.pi/180.) * 10.
        #        ]
        #ru_ratioX = [
        #        0.65,
        #        #0.35,
        #        0.70,
        #        0.35,
        #        0.22
        #        ]
        #ru_deltaY = [
        #        0.80,
        #        0.55,
        #        0.90,
        #        0.55
        #        ]
        #ru_offX = [
        #        0.125,
        #        0.125,
        #        0.0
        #        ]
        #ru_offY = [
        #        0.065,
        #        0.085,
        #        0.035
        #        ]
        #
        #spanwiseCuts_td = [0.00, 1.00,]
        #ru_t_le = [0.020,
        #           0.018]
        #ru_u_le = [0.00]
        #ru_t_mid = [0.04,
        #            0.03]
        #ru_u_mid = [0.50]
        #ru_t_te = [0.02]
        #ru_u_te = [0.80]

        #self.buildBlade(
        #        container,
        #        ruLabel,
        #        self.nRuBlades, True,
        #        spanwiseCuts_mp, ru_alpha_1, ru_alpha_2, ru_ratioX, ru_deltaY, ru_offX, ru_offY,
        #        spanwiseCuts_td, ru_t_le, ru_u_le, ru_t_mid, ru_u_mid, ru_t_te, ru_u_te,
        #    )
        #self.bV[ruLabel+"_mesh"].makeGrid() 

        """ 
        # returning the hub and shroud layers
        layers = radMeridionalContour.getLayerList(self.nRuBlades)    
        # returns layer data in the following nested list:
        # layers = [[hub layer lists],[shroud layer list]]
        # with:
        # [hub layer lists] = [[3d layer domain], [bool list radius zero]]

        # returns the unstructured region and its surfaces
        mv, bs = radMeridionalContour.getUnstructuredRegion(self.nRuBlades)
        
        modname = "dtOOPythonApp.builder.map3dTo3dGmsh_gridFromLayers"
        module = self.reloadModule(modname)
        #from dtOOPythonApp.builder import ( map3dTo3dGmsh_gridFromLayers )
        # module.map3dTo3dGmsh_gridFromLayers(
            # label, layer data, nElementsLayer, firstElementSize, meshSizeSW, meshSizeCIRC, unstructRegion
        # )
        
        # creating the mesh of the suction area with wall layers and the unstructured region
        container = module.map3dTo3dGmsh_gridFromLayers(
                "meshLayers",       # label 
                layers,             # layer data
                7,                  # number of elements in layer
                0.01,               # first element size in layer
                0.05,               # element size streamwise
                0.03,               # element size circumferential
                mv,                 # multi bounded volume of unstructured region
                bs                  # surfaces of unstructured region
            ).buildExtract(container)
        self.bV["meshLayers"].makeGrid()
        
        modname = "dtOOPythonApp.builder.map3dTo3dGmsh_gridFromChannel"
        module = self.reloadModule(modname)
        #from dtOOPythonApp.builder import ( map3dTo3dGmsh_gridFromChannel )

        container = module.map3dTo3dGmsh_gridFromChannel(
                "meshInlet",                                            # label
                radMeridionalContour.getRegChannel(0, self.nRuBlades),    # channel
                5,                                                      # nBoundaryLayersH2S
                15,                                                     # nEHubToShroud
                20,                                                     # nECircumferential
                25,                                                     # nEMeridional
                0.01,                                                   # fESHubToShroud
                0.01,                                                   # fESInlet
            ).buildExtract(container)
        self.bV["meshInlet"].makeGrid()

        container = module.map3dTo3dGmsh_gridFromChannel(
                "meshChannel",                            
                radMeridionalContour.getRegChannel(1, self.nRuBlades),
                5,                                              
                15,                                             
                20,                                             
                25,                                             
                0.01,                                           
                0.01,                                           
            ).buildExtract(container)
        self.bV["meshChannel"].makeGrid()
        """
        """
        from dtOOPythonApp.builder import (
          ofOpenFOAMCase_turboMachine,
          ofOpenFOAMCase_setupWrapper
        )

        container = ofOpenFOAMCase_turboMachine(
          label = "of",
          bVs = [
            self.bV["meshInlet"], self.bV["meshChannel"], self.bV["meshLayers"],
          ],
          dictRule = \
              ofOpenFOAMCase_setupWrapper.controlDict(
                application = "simpleFoam",
                endTime = 2000,
                # Patches where Q and PT is tracked
                QPatches = ['meshInlet_inlet', 
                            'meshInlet_outlet', 'meshChannel_inlet', 
                            'meshChannel_outlet','meshLayers_inlet', 
                            'meshLayers_outlet',],
                PTPatches = ['meshInlet_inlet', 'meshLayers_outlet',],
                #FPatches = ['mesh_blade',],
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
              # meshInlet
              ofOpenFOAMCase_setupWrapper.cylindricalInletRuleString(
                "meshInlet_inlet",
                ["U"],
                [ [-2,1,0], ]
              ),
              ofOpenFOAMCase_setupWrapper.inletRuleString(
                "meshInlet_inlet",
                ["p", "k", "omega",],
                [ [0], [0.0, 0.10], [0.001, 0.1] ]
              ),
              ofOpenFOAMCase_setupWrapper.wallRuleString(
                "meshInlet_shroud",
                ["omega", "U", "p", "k", "nut"]
              ),
               ofOpenFOAMCase_setupWrapper.wallRuleString(
                "meshInlet_hub",
                ["omega", "U", "p", "k", "nut"]
              ),
              #ofOpenFOAMCase_setupWrapper.wallRuleString(
              #  "mesh_blade",
              #  ["omega", "U", "p", "k", "nut"]
              #),
              ofOpenFOAMCase_setupWrapper.cyclicAmiRuleString(
                "meshInlet_suction", "meshInlet_pressure"
              ),
              # mixing plane?
              #ofOpenFOAMCase_setupWrapper.cyclicAmiRuleString(
              #  "meshInlet_outlet", "meshChannel_inlet",
              #  rotAxis = None,
              #  rotCentre = None
              #),
              ofOpenFOAMCase_setupWrapper.mixingPlaneRuleString(
                "meshInlet_outlet", "meshChannel_inlet",
                ["U", "p", "k", "omega",],
                axis = dtOO.dtVector3(0,0,1),
                origin = dtOO.dtPoint3(0,0,0),
                stackAxis = "Z"
              ),
              # mesh Channel
              ofOpenFOAMCase_setupWrapper.wallRuleString(
                "meshChannel_hub",
                ["omega", "U", "p", "k", "nut"]
              ),
              ofOpenFOAMCase_setupWrapper.wallRuleString(
                "meshChannel_shroud",
                ["omega", "U", "p", "k", "nut"]
              ),
              ofOpenFOAMCase_setupWrapper.cyclicAmiRuleString(
                "meshChannel_suction", "meshChannel_pressure"
              ),
              #ofOpenFOAMCase_setupWrapper.cyclicAmiRuleString(
              #  "meshChannel_outlet", "meshLayers_inlet",
              #  rotAxis = None,
              #  rotCentre = None
              #),
              ofOpenFOAMCase_setupWrapper.mixingPlaneRuleString(
                "meshChannel_outlet", "meshLayers_inlet",
                ["U", "p", "k", "omega",],
                axis = dtOO.dtVector3(0,0,1),
                origin = dtOO.dtPoint3(0,0,0),
                stackAxis = "R"
              ),
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
              ofOpenFOAMCase_setupWrapper.outletRuleString(
                "meshLayers_outlet", 
                ["U", "p", "k", "omega",]
              ),
            ]

        ).buildExtract( container )
        
        self.dC["of"].runCurrentState()
        
        cDir = self.dC["of"].getDirectory(
            dtOO.lVHOstateHandler().commonState()
        )
        
         
        fc = foamlib.FoamCase( cDir )
        fc.control_dict['writeInterval'] = 50
        fc.control_dict['endTime'] = 1000
        fc.turbulence_properties["laminar"]["turbulence"] = False
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
        """ 
        #
        # depiction in 3d Space
        #
        for ii in [label+"_meanplane", label+"_blade",]:         
          theAG = dtOO.vec3dTwoDInMap3dTo3d(
            dtOO.vec3dTwoD.MustConstDownCast(
              conMap.applyAnalyticFunction(self.aF[ii].clone())
            ),
            dtOO.map3dTo3d.ConstDownCast( self.aG["xyz_"+label+"_channel"] )   
          )
          self.aG.push_back( theAG << "xyz_"+str(ii) )
        """
        meshBlock_thickness = 0.015 
        # mesh block
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
        
        # split mesh block
        #modname = "dtOOPythonApp.builder.vec3dThreeD_meshBlocks"
        #module = self.reloadModule(modname)

        from dtOOPythonApp.builder import vec3dThreeD_skinAndSplit
        container = vec3dThreeD_skinAndSplit(
        #container = module.vec3dThreeD_meshBlocks(
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
          tEMeshBlockThickness = meshBlock_thickness
          #tEMeshBlockThickness = None
        ).buildExtract(container)
        #
        # do conformal mapping
        #
        for ii in [label+"_meanplane", label+"_blade", label+"_meshBlock",]:
          theAG = dtOO.vec3dTwoDInMap3dTo3d(
            dtOO.vec3dTwoD.MustConstDownCast(
              conMap.applyAnalyticFunction(self.aF[ii].clone())
              #self.bC.ptrTransformerContainer().get(
              #  "uVw_phirMs"
              #).applyAnalyticFunction(
              #  self.aF[ii].clone()
              #)
            ),
            dtOO.map3dTo3d.ConstDownCast( self.aG["xyz_"+label+"_channel"] )
          )
          theAG.setLabel("xyz_"+ii)
          self.aG.push_back( theAG.clone() )
        for iNum in self.aF.getIndices(label+"_meshBlock_*"):
          ii = self.aF.getLabel( iNum )
          theAG = dtOO.vec3dThreeDInMap3dTo3d(
            dtOO.vec3dThreeD.MustConstDownCast(
              conMap.applyAnalyticFunction(self.aF[ii].clone())
              #self.bC.ptrTransformerContainer().get(
              #  "uVw_phirMs"
              #).applyAnalyticFunction(
              #  self.aF[ii].clone()
              #)
            ),
            dtOO.map3dTo3d.ConstDownCast( self.aG["xyz_"+label+"_channel"] )
          )
          theAG.setLabel("xyz_"+ii)
          self.aG.push_back( theAG.clone() )
         
        ## create mesh's topology
        #blocks = []
        #for iNum in self.aG.getIndices("xyz_"+label+"_meshBlock_*"):
        #  blocks.append( self.aG[ self.aG.getLabel( iNum ) ] )
        #from dtOOPythonApp.builder import (
        #  rotatingMap2dTo3d_splitFitted
        #)
        #container = rotatingMap2dTo3d_splitFitted(
        #  label = "xyz_"+label+"_fitChannel", 
        #  channel = self.aG["xyz_"+label+"_channel"], 
        #  internals = blocks, #[aG["xyz_"+label+"_meshBlock"],], 
        #  splitDir = 1,
        #  segmentDir = 2,
        #  segments = [0.0, 0.25, 0.50, 0.75, 1.0],
        #  deltaPer = 0.05
        #).enableDebug().buildExtract( container )
        
        #i fe_meanplane
        from dtOOPythonApp.builder import (
          vec3dTwoDInMap3dTo3d_approximateAndFullExtendMeanplane
        )
        container = vec3dTwoDInMap3dTo3d_approximateAndFullExtendMeanplane( 
          label = "xyz_fe_"+label+"_meanplane",
          channel = self.aG["xyz_"+label+"_channel"],
          meanplane = self.aG["xyz_"+label+"_meanplane"],
          ratioIn = scaOneD_scaCurve2dOneDPointConstruct(
            [
              dtOO.dtPoint2(0.00, 0.00),
              dtOO.dtPoint2(1.00, 0.00)
            ],
            1
          )(),
          ratioOut = scaOneD_scaCurve2dOneDPointConstruct(
            [
              dtOO.dtPoint2(0.00, 0.00),
              dtOO.dtPoint2(1.00, 0.00)
            ],
            1
          )(),
           ratioBladeExt = scaOneD_scaCurve2dOneDPointConstruct(
            [
              dtOO.dtPoint2(0.00, 0.50),
              dtOO.dtPoint2(1.00, 0.50)
            ],
            1
          )(),
          segUs = [0.0, 0.1, 0.5, 0.9, 1.0, ],
          segVs = [0.0, 0.5, 1.0, ],
          intermediatePoints = True,
        ).buildExtract( container )

        # grid channel
        from dtOOPythonApp.builder import (
          rotatingMap2dTo3d_gridChannel
        )
        container = rotatingMap2dTo3d_gridChannel(
          label = "xyz_"+label+"_gridChannel", 
          channelSide = self.aG["xyz_fe_"+label+"_meanplane"], 
          numberOfSections = nBlades,
          rotVector = dtOO.dtVector3(0, 0, -1)
        ).buildExtract( container )
         
        # create mesh's topology
        blocks = []
        for iNum in self.aG.getIndices("xyz_"+label+"_meshBlock_*"):
            blocks.append( self.aG[ self.aG.getLabel( iNum ) ] )
        # sorting the blocks by number
        blocks.sort(key=lambda x: int(x.getLabel().split('_')[-1]))
        
        nBlocksMP = 3
        couplingFaces = []
        couplingFaces.append( 
          dtOO.map3dTo3d.MustDownCast( blocks[0] ).segmentConstUPercent( 0.0 )
        )
        for i, block in enumerate(blocks):
          couplingFaces.append( 
            dtOO.map3dTo3d.MustDownCast( block ).segmentConstWPercent( 1.0 )
          )

          if i == nBlocksMP:
              MPfaces = couplingFaces.clone()
              MPfaces.append( 
                dtOO.map3dTo3d.MustDownCast(block).segmentConstUPercent( 1.0 ).clone()
              )

        couplingFaces.append( 
          dtOO.map3dTo3d.MustDownCast( blocks[-1] ).segmentConstUPercent( 1.0 )
        )

        
        modname = "dtOOPythonApp.builder.map3dTo3dGmsh_gridFromChannelAndBlocks"
        module = self.reloadModule(modname)

        #from dtOOPythonApp.builder import (
        #  map3dTo3dGmsh_gridFromChannelAndBlocks
        #)
        container = module.map3dTo3dGmsh_gridFromChannelAndBlocks(
          label = label+"_mesh",
          channel = self.aG["xyz_"+label+"_gridChannel"],
          blocks = blocks,
          blade = self.aG["xyz_"+label+"_blade"],
          couplingFaces = couplingFaces,
          nBoundaryLayers = 6,
          nElementsSpanwise = 30,
          nElementsNormal = 5,
          firstElementSizeHubToShroud = 0.005,
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
          channelInletOutletDir = 2,
          channelHubShroudDir = 3,
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
