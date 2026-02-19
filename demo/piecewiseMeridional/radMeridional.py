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
        bC = container.cptr_bC()
        cV = container.cptr_cV()
        
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
              
        # returning the hub and shroud layers
        layers = radMeridionalContour.getLayerList(self.nBlades)    
        # returns layer data in the following nested list:
        # layers = [[hub layer lists],[shroud layer list]]
        # with:
        # [hub layer lists] = [[3d layer domain], [bool list radius zero]]

        # returns the unstructured region and its surfaces
        mv, bs = radMeridionalContour.getUnstructuredRegion(self.nBlades)
        
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
        bV["meshLayers"].makeGrid()
        
        modname = "dtOOPythonApp.builder.map3dTo3dGmsh_gridFromChannel"
        module = self.reloadModule(modname)
        #from dtOOPythonApp.builder import ( map3dTo3dGmsh_gridFromChannel )

        container = module.map3dTo3dGmsh_gridFromChannel(
                "meshInlet",                                            # label
                radMeridionalContour.getRegChannel(0, self.nBlades),    # channel
                5,                                                      # nBoundaryLayersH2S
                15,                                                     # nEHubToShroud
                20,                                                     # nECircumferential
                25,                                                     # nEMeridional
                0.01,                                                   # fESHubToShroud
                0.01,                                                   # fESInlet
            ).buildExtract(container)
        bV["meshInlet"].makeGrid()

        container = module.map3dTo3dGmsh_gridFromChannel(
                "meshChannel",                            
                radMeridionalContour.getRegChannel(1, self.nBlades),
                5,                                              
                15,                                             
                20,                                             
                25,                                             
                0.01,                                           
                0.01,                                           
            ).buildExtract(container)
        bV["meshChannel"].makeGrid()

        
        from dtOOPythonApp.builder import (
          ofOpenFOAMCase_turboMachine,
          ofOpenFOAMCase_setupWrapper
        )

        container = ofOpenFOAMCase_turboMachine(
          label = "of",
          bVs = [
            bV["meshInlet"], bV["meshChannel"], bV["meshLayers"],
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
                [ [-2,0,0], ]
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
        
        dC["of"].runCurrentState()
        
        cDir = dC["of"].getDirectory(
            dtOO.lVHOstateHandler().commonState()
        )
        print(cDir)

        return container  

    def reloadModule(self, modname):  
        if modname in sys.modules: 
            print(f"reloading: {modname}")
            module=importlib.reload(sys.modules[modname])
        else:
            print(f"loading: {modname}")
            module=importlib.import_module(modname)
                
        return module
