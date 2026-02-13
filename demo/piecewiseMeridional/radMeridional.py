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
        for key, value in data.items():
            setattr(self, key, value)
        print(self.d_inlet)
        print(self.label)
        #passi

        #importlib.reload(builder)

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
                '{"name" : "geometry_render_resolution_u", "value" : "200"},'
                '{"name" : "geometry_render_resolution_v", "value" : "200"},'
                '{"name" : "geometry_render_resolution_w", "value" : "200"},'
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

        # clearing prevents segmentation fault
        dtOO.lVHOstateHandler.clear()
        dtOO.lVHOstateHandler( dtOO.jsonPrimitive(), cV ).thisown = False

        aF = container.cptr_aF() 
        aG = container.cptr_aG() 
        bV = container.cptr_bV() 
        dC = container.cptr_dC() 
        dP = container.cptr_dP() 
        
        #modname = "dtOOPythonApp.builder.analyticGeometryLayers_piecewiseMeridionalRotContour"
        #module = self.reloadModule(modname)
        #from dtOOPythonApp.builder import (
        #    analyticGeometryLayers_piecewiseMeridionalRotContour
        #)
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

        # returns the unstructured region
        mv = radMeridionalContour.getUnstructuredRegion(self.nBlades)
        
        modname = "dtOOPythonApp.builder.map3dTo3dGmsh_gridFromLayers"
        module = self.reloadModule(modname)
        #from dtOOPythonApp.builder import ( map3dTo3dGmsh_gridFromLayers )
        # module.map3dTo3dGmsh_gridFromLayers(
            # label, layer data, nElementsLayer, firstElementSize, meshSizeSW, meshSizeCIRC, unstructRegion
        # )
        #importlib.reload(builder.map3dTo3dGmsh_gridFromLayers)
        layerMesh = module.map3dTo3dGmsh_gridFromLayers(
                "meshLayers", 
                layers, 
                7, 
                0.01, 
                0.05, 
                0.03,  
                mv
            )
        layerMesh.buildLayerMesh()
        container = layerMesh.buildExtract(container)
        
        modname = "dtOOPythonApp.builder.map3dTo3dGmsh_gridFromChannel"
        module = self.reloadModule(modname)
        #from dtOOPythonApp.builder import ( map3dTo3dGmsh_gridFromChannel )
        inletMesh = module.map3dTo3dGmsh_gridFromChannel(
                "meshInlet",                            # label
                radMeridionalContour.getRegChannel(0, self.nBlades),    # channel
                5,                                              # nBoundaryLayersH2S
                15,                                             # nEHubToShroud
                20,                                             # nECircumferential
                25,                                             # nEMeridional
                0.01,                                           # fESHubToShroud
                0.01,                                            # fESInlet
            ).enableDebug()
        #inletMesh.build()
        container = inletMesh.buildExtract(container)

        channelMesh = module.map3dTo3dGmsh_gridFromChannel(
                "channelInlet",                            # label
                radMeridionalContour.getRegChannel(1, self.nBlades),    # channel
                5,                                              # nBoundaryLayersH2S
                15,                                             # nEHubToShroud
                20,                                             # nECircumferential
                25,                                             # nEMeridional
                0.01,                                           # fESHubToShroud
                0.01,                                            # fESInlet
            ).enableDebug()
        #inletMesh.build()
        container = inletMesh.buildExtract(container)

        
        print("A CHANGE IN radMeridional")

        return container  

    def reloadModule(self, modname):  
        if modname in sys.modules: 
            print(f"reloading: {modname}")
            module=importlib.reload(sys.modules[modname])
        else:
            print(f"loading: {modname}")
            module=importlib.import_module(modname)
                
        return module
