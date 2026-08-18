
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
from typing import List


class axMeridional:
    
	
    def __init__(
        self,
    ):
        """Initalize the radMeridional object. 

        This constructor:
        - Creates the main dtOO container
        - Initializes logging
        - Configures static dtOO properties
        - Sets up base container and vector handlers
        - Initializes labeled vector handling objects (bC, cV, aF, aG, bV, dC, dP)
        
        Parameters
        ----------
        None

        Returns
        -------
        None

        """ 
        # Create an empty container
        self.container = dtOO.dtBundle()

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
                '{"name" : "map1dTo3d_deltaPer", "value" : "0.01"},'
                '{"name" : "map2dTo3d_deltaPer", "value" : "0.01"},'
                '{"name" : "map3dTo3d_deltaPer", "value" : "0.01"},'
                #'{"name" : "logLevel", "value" : "99"},'
                '{"name" : "logLevel", "value" : "2"},'
                '{"name" : "isEqualExtendCheck", "value" : "true"}'
              ']'
            '}'
          )
        )

        # Init baseContainer and labeledVectorHandlings
        self.bC = self.container.cptr_bC()
        self.cV = self.container.cptr_cV()

        self.cV.push_back(
            dtOO.sliderFloatParam("QIn", 0.5, 0.0, 1.0).clone()
        )
        # clearing prevents segmentation fault
        dtOO.lVHOstateHandler.clear()
        dtOO.lVHOstateHandler( dtOO.jsonPrimitive(), self.cV ).thisown = False

        self.aF = self.container.cptr_aF()
        self.aG = self.container.cptr_aG()
        self.bV = self.container.cptr_bV()
        self.dC = self.container.cptr_dC()
        self.dP = self.container.cptr_dP()

    def createMeridional(self): 
        
        #
        # Create the interface curves and the regular channels as well as the special 
        # hub and shroud curves.
        # The radMeridionalContour object is used in the other methods to return the 
        # geometries.
        #
        from dtOOPythonApp.builder import analyticGeometry_piecewiseMeridionalRotContour
        self.radMeridionalContour = analyticGeometry_piecewiseMeridionalRotContour( 
          label = "axMeridionalContour",
          hubCurves = [
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(0.50, 0.00, 2.00)  
                  << dtOO.dtPoint3(0.50, 0.00, 0.00),  
                1
              ).result()
            ),
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(0.50, 0.00, 0.00)  
                  << dtOO.dtPoint3(0.50, 0.00, -3.00),  
                1
              ).result()
            )
          ],
          shroudCurves = [
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(2.00, 0.00, 2.00)  
                  << dtOO.dtPoint3(2.00, 0.00, 0.00),  
                1
              ).result()
            ),
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(2.00, 0.00, 0.00)  
                  << dtOO.dtPoint3(2.50, 0.00, -3.00),  
                1
              ).result()
            )
          ],
          interface_hub = [[0, 1.00],
                           [1, 1.00]],
          interface_shroud = [[0, 1.00],
                              [1, 1.00]],
          interface_curvature =[[0.00, 0.5, 1],
                                [0.00, 0.5, 1]], 
          #rotVector = dtOO.dtVector3(0, 0, -1), 
        ).enableDebug()#.buildExtract( self.container )
        self.container = self.radMeridionalContour.buildExtract(self.container)
 
    def createLayerRegion(self):
        

        self.aG.push_back(
            self.radMeridionalContour.getRegChannel(
                1, 5
            ) << "xyz_gridDiffuser"
        )
        
        from dtOOPythonApp.builder import (
          map3dTo3dGmsh_gridFromChannel
        )
        self.container = map3dTo3dGmsh_gridFromChannel(
          label = "meshDiffuser",
          channel = self.aG["xyz_gridDiffuser"],
          nBoundaryLayers = 6,
          nElementsRadial = 30,
          nElementsCircumferential = 10,
          nElementsMeridional = 20,
          firstElementSizeHubToShroud = 0.005,
          firstElementSizeInlet = 0.05,
          channelInletOutletDir = 2,
          channelHubShroudDir = 3,
          channelSuctionPressureDir = 1
        ).enableDebug().buildExtract( self.container )

        ##
        ## Return the special hub and shroud curves and inlet and outlet curves
        ## of the layered region from the radMeridionalContour object, which was 
        ## created in the createMeridional method.
        ##
        #speHub, speShroud, inOutCurves = self.radMeridionalContour.getLayerRegionCurves()
        #
        ##
        ## Build the layer region geometry as transfinite layers and 
        ## a multiple bounded volume
        ##
        #from dtOOPythonApp.builder import analyticGeometry_layerRegion
        #layerRegion = analyticGeometry_layerRegion( 
        #  label = "axMeridionalContour",
        #  speHub = speHub,
        #  speShroud = speShroud,
        #  inOutCurves = inOutCurves,
        #  layer_thickness = 0.0775,
        #  layer_supports = [0.5],
        #).enableDebug()#.buildExtract( self.container )
        #self.container = layerRegion.buildExtract(self.container)
        #
        ##
        ## Apply mesh settings to the layer region
        ##
        #
        ##
        ## Return  the data of the layers a nested list with the following format:
        ## layers = Tuple[
        ##     Tuple[
        ##         List[analyticGeometry], List[bool]
        ##     ], 
        ##     Tuple[
        ##         List[analyticGeometry], List[bool]
        ##     ]
        ## ]
        ## with:
        ## layers[0] : Hub Layers
        ## layers[1] : Shroud Layers
        ## layers[i][0] : List of layer volumes
        ## layers[i][1] : List of bool values which tell if the layer is on a radius of zero
        ##
        #layers = layerRegion.getLayerList(5)    
        #
        ##
        ## The volume inside the flow channels is retrned as the mutliple bounded volume mv
        ## its bounding surfaces are returned in the list bs
        ##
        #mv, bs = layerRegion.getUnstructuredRegion(5)
        #
        #from dtOOPythonApp.builder import ( map3dTo3dGmsh_gridFromLayers ) 
        #
        ## 
        ## Apply the mesh settings to the layers and the multiple bounded volume
        ##
        #self.container = map3dTo3dGmsh_gridFromLayers(
        #        mv = mv,
        #        bs = bs,
        #        label = "meshLayers",
        #        layers = layers,
        #        nElementsLayer = 6,
        #        firstElement = 0.005,
        #        elementSize_sw = 0.02,
        #        elementSize_circ = 0.01,
        #        charLengthMin = 0.02,
        #        charLengthMax = 0.04,
        #    ).buildExtract(self.container)
       


    def createBlade(self):
        
        #
        # Assign the entries of the configuration dictionary to variables.
        #
        label = "ru" 
        regChannel = 0 
        nBlades = 5

        alpha_1_0 = 90.0 
        alpha_1_1 = 90.0 
        alpha_2_0 = 80.0 
        alpha_2_1 = 40.0

        #
        # Return the regular channel from the radMeridionalContour object,
        # which is created in the method createMeridional.
        #
        self.aG.push_back(
            self.radMeridionalContour.getRegChannel(
                regChannel, 1
            ) << "xyz_"+label+"_channel"
        )
         
        # conformalMapping
        conMap = dtOO.uVw_phirMs()
        conMap.jInit(
          dtOO.jsonPrimitive()\
            .appendStr("label", "uVw_phirMs")\
            .appendInt("_nV", 31)\
            .appendInt("_nW", 11)\
            .appendAnalyticGeometry(\
              "_rM2d", \
              self.aG.get("xyz_"+label+"_channel")\
            ),\
          None, None, None, self.aG \
        )
        self.bC.dtTransformer().add( conMap.clone() )

        # meanplane
        from dtOOPythonApp.builder import (
          analyticSurface_threePointMeanplane,
          analyticSurface_threePointMeanplaneFromRatio,
          scaOneD_scaCurve2dOneDPointConstruct
        )
        # ru
        self.container = analyticSurface_threePointMeanplaneFromRatio(
          label + "_meanplane",
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
        ).buildExtract( self.container )

        # blade thickness distribution
        from dtOOPythonApp.builder import (
          vec3dSurfaceTwoD_fivePointsBSplineThicknessDistribution
        )
        self.container = vec3dSurfaceTwoD_fivePointsBSplineThicknessDistribution(
          label + "_thicknessDistribution",
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
        ).buildExtract( self.container )

        #
        # Combine the meanplane and the thickness distribution in the parameter space.
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
        
        ##
        ## Create a conformal mapping object with the regular channel
        ## and the adjustRadius setting.
        ##
        #conMap = dtOO.uVw_phirMs()
        #conMap.jInit(
        #  dtOO.jsonPrimitive()\
        #    .appendStr("label", "uVw_phirMs")\
        #    .appendInt("_nV", 31)\
        #    .appendInt("_nW", 11)\
        #    .appendBool("_adjustRadius", adjustRadius)\
        #    .appendAnalyticGeometry(\
        #      "_rM2d", \
        #      self.aG.get("xyz_" + label + "_channel")\
        #    ),\
        #  None, None, None, self.aG \
        #)
        #self.bC.dtTransformer().add( conMap.clone() )
         
        ##
        ## depiction in 3d Space of blade and meanplane
        ##
        #for ii in [label+"_meanplane", label+"_blade",]:         
        #  theAG = dtOO.vec3dTwoDInMap3dTo3d(
        #    dtOO.vec3dTwoD.MustConstDownCast(
        #      conMap.applyAnalyticFunction(self.aF[ii].clone())
        #    ),
        #    dtOO.map3dTo3d.ConstDownCast( self.aG["xyz_"+label+"_channel"] )   
        #  )
        #  self.aG.push_back( theAG << "xyz_"+str(ii) )
        
        #
        # Create a surface around the blade surface with a normal distance
        # specified by meshBlock_thickness.
        #
        meshBlock_thickness = 0.15
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
        # The class vec3dThreeD_skinAndSplit does the following tasks:
        #   - Create mesh blocks by splitting the mesh black surface.
        #   - Create trailing edge mesh blocks based on the first and last
        #     mesh block.
        #   - Creates a meanplane curve by offseting mesh block edges in the
        #     tangential directions of the mesh blocks.
        #     These curves will later be used to create peridoc faces of the
        #     grid channel.
        #     Two meanplane curves are created, one offset from the first mesh
        #     block extending towards the outlet and one offset from the meshblock
        #     specified with nMeanplaneBlocks extending towards the inlet of the
        #     regular channel.
        #

        # number of mesh block faces which will be part of the meanplane
        #  starts from 0
        nMeanplaneBlocks = 3

        # split mesh block and create curves for meanplane
        from dtOOPythonApp.builder import vec3dThreeD_skinAndSplit
        self.container = vec3dThreeD_skinAndSplit(
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
          meanplaneExtOut = 0.07,
          meanplaneExtIn = 0.1,
          nMeanplaneBlocks = nMeanplaneBlocks
        ).buildExtract(self.container)
        
        #
        # Build the two meanplane faces extending from the mesh block edges 
        # to the tangentially offset meanplane curves.
        #
        for i in range(2):
            #
            # curve seqence is switched with c0 and c1 between inlet and outlet in 
            # order to keep u and v directions consistent with the mesh block faces.
            #
            if i == 0:
                at = "in"
                c0 = 0
                c1 = 1
            elif i == 1:
                at = "out"
                c0 = 1
                c1 = 0
            # 
            # Create the fe_meanplane faces. Those will later be used as part of the
            # periodic faces of the grid channel.
            #
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
        # Apply the conformal mapping object the following geometries.
        #
        #   - faces of the blade meanplane, the blade and the mesh blocks
        #
        for ii in [label+"_meanplane", label+"_blade", label+"_meshBlock",]:
          theAG = dtOO.vec3dTwoDInMap3dTo3d(
            dtOO.vec3dTwoD.MustConstDownCast(
              conMap.applyAnalyticFunction(self.aF[ii].clone())
            ),
            dtOO.map3dTo3d.ConstDownCast( self.aG["xyz_"+label+"_channel"] )
          )
          theAG.setLabel("xyz_"+ii)
          self.aG.push_back( theAG.clone() )
        #
        #   - fe_meanplane faces extending tangentially from the mesh blocks.
        #
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
        #
        #   - mesh block volumes
        #
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
        #
        #   - mesh block curves which were used to create the fe_meanplane faces
        #
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
        
        #
        # Collect the curves for the fe_meanplane faces.
        # The curves extend form hub to shroud and are tangentially 
        # offset from the specified mesh block faces
        #
        meshBlockCurves = dtOO.labeledVectorHandlingAnalyticGeometry()
        for iNum in self.aG.getIndices("xyz_"+label+"_meshBlockCurve_*"):
            ii = self.aG.getLabel( iNum )
            meshBlockCurves.push_back(self.aG[ii].clone())
        
        #
        # Create the fe_meanplane surfaces extending from the tangentially 
        # offset meshblock curves to the inlet or the outlet.
        #
        from dtOOPythonApp.builder import analyticSurface_inOutFeMeanplane
        self.container = analyticSurface_inOutFeMeanplane(
            prefix = "xyz",
            label = label,
            channel = self.aG["xyz_"+label+"_channel"],
            curves = meshBlockCurves
        ).enableDebug().buildExtract(self.container)
         
        #
        # Order the gemetries for the cration of the grid channel.
        #
        # Collect and organize the mesh block volumes.
        # The mesh blocks are ordered by their number.
        #
        blocks = []
        for iNum in self.aG.getIndices("xyz_"+label+"_meshBlock_*"):
            blocks.append( self.aG[ self.aG.getLabel( iNum ) ] )
        blocks.sort(key=lambda x: int(x.getLabel().split('_')[-1]))
        
        #
        # Collect the coupling and meanplane faces.
        # The list couplingFaces contains the faces which will connect the
        # mesh block to the grid channel.
        # The list meanplaneFaces contains the faces which will form the 
        # periodic boundaries of the grid.
        #
        couplingFaces = []
        meanplaneFaces = []
        
        #
        # Faces extending from outlet to the mesh block trailing edge. 
        #
        meanplaneFaces.append(dtOO.map2dTo3d.MustDownCast(self.aG["xyz_"+label+"_fe_meanplane_out1"]))
        meanplaneFaces.append(dtOO.map2dTo3d.MustDownCast(self.aG["xyz_"+label+"_fe_meanplane_out0"]))
        
        #
        # Iterate over the mesh blocks and sort the faces.
        #
        for i, block in enumerate(blocks):
          face = dtOO.map3dTo3d.MustDownCast( block ).segmentConstWPercent( 1.0 )
          
          #
          # Append the specified meanplane and coupling to the lists. 
          #
          if i <= nMeanplaneBlocks:
            meanplaneFaces.append(face)
            self.aG.push_back(face.clone() << "xyz_"+label+"_meanplaneFaceOnBlock_"+str(i))
          else:
            couplingFaces.append(face)
        
        #
        # Faces extending from the mesh block leading edge area to the inlet.
        #
        meanplaneFaces.append(dtOO.map2dTo3d.MustDownCast(self.aG["xyz_"+label+"_fe_meanplane_in0"]))
        meanplaneFaces.append(dtOO.map2dTo3d.MustDownCast(self.aG["xyz_"+label+"_fe_meanplane_in1"]))
        
        #
        # Last two coupling faces at the trailing edge.
        #
        couplingFaces.append(dtOO.map3dTo3d.MustDownCast(blocks[-1]).segmentConstUPercent( 1.0 ))
        couplingFaces.append(dtOO.map3dTo3d.MustDownCast(blocks[0]).segmentConstUPercent( 0.0 ))
        
        #
        # Create the grid channel.
        #
        from dtOOPythonApp.builder import multipleBoundedVolume_gridChannel
        gridChannel = multipleBoundedVolume_gridChannel(
            label = label,
            channel = self.aG["xyz_"+label+"_channel"],
            meanplanes = meanplaneFaces,
            couplings = couplingFaces,
            nBlades = nBlades,
            #rotVector = dtOO.dtVector3(0, 0, -1),
        ).enableDebug()
        self.container = gridChannel.buildExtract(self.container)
        
        #
        # Return the multiple bounded volume of the grid channel as well
        # as the list with its boundary faces.
        #
        gc, gcFaces = gridChannel.getGridChannel()
        self.aG.push_back(
            gc << "xyz_"+label+"_gridChannel"
        )
         
        #
        # Apply the mesh settings to the grid channel.
        #
        from dtOOPythonApp.builder import (
          map3dTo3dGmsh_gridFromMultipleBoundedVolumeAndBlocks
        )
        self.container = map3dTo3dGmsh_gridFromMultipleBoundedVolumeAndBlocks(
            label = label+"_mesh",
            channel = self.aG["xyz_"+label+"_gridChannel"],
            channelFaces = gcFaces,
            blocks = blocks,
            nMeanplaneBlocks = nMeanplaneBlocks,
            blade = self.aG["xyz_"+label+"_blade"],
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
            charLengthMax=0.05,
            charLengthMin=0.025,
            meshTEBlocks = True,

        ).enableDebug().buildExtract( self.container )

    def createOfCase(self):

        from dtOOPythonApp.builder import (
          ofOpenFOAMCase_turboMachine,
          ofOpenFOAMCase_setupWrapper
        )

        dtOO.lVHOstateHandler().makeState("axial_alpha2_1_40")

        self.container = ofOpenFOAMCase_turboMachine(
          label = "of",
          bVs = [
            self.bV["ru_mesh"], self.bV["meshDiffuser"],
          ],
          dictRule = \
              ofOpenFOAMCase_setupWrapper.controlDict(
                application = "simpleFoam",
                endTime = 200,
                QPatches = ['ru_mesh_inlet','ru_mesh_outlet',
                            'meshDiffuser_inlet', 'meshDiffuser_outlet',
                            'ru_mesh_suction', 'ru_mesh_pressure',
                            'meshDiffuser_pressure', 'meshDiffuser_suction'],
                PTPatches = ['ru_mesh_inlet','ru_mesh_outlet',
                            'meshDiffuser_inlet', 'meshDiffuser_outlet',
                            'ru_mesh_suction', 'ru_mesh_pressure',
                            'meshDiffuser_pressure', 'meshDiffuser_suction'],
                FPatches = ['ru_mesh_blade',],
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
            #    cellZones = ["ru_mesh",], 
            #    omegas    = [-9.81,], 
            #    nonRotatingPatches = [
            #      [
            #        "ru_mesh_suction", "ru_mesh_pressure", 
            #        "ru_mesh_inlet", "ru_mesh_outlet",
            #        "ru_mesh_shroud",
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
                "ru_mesh_inlet", 
                ["U"], 
                [ [0,-1,-2], ]
              ), 
              ofOpenFOAMCase_setupWrapper.inletRuleString(
                "ru_mesh_inlet", 
                ["p", "k", "omega",], 
                [ [0], [0.0, 0.10], [0.001, 0.1] ]
              ),
              ofOpenFOAMCase_setupWrapper.wallRuleString(
                "ru_mesh_shroud", 
                ["omega", "U", "p", "k", "nut"]
              ),
               ofOpenFOAMCase_setupWrapper.wallRuleString(
                "ru_mesh_hub", 
                ["omega", "U", "p", "k", "nut"]
              ),
              ofOpenFOAMCase_setupWrapper.wallRuleString(
                "ru_mesh_blade", 
                ["omega", "U", "p", "k", "nut"]
              ),
              ofOpenFOAMCase_setupWrapper.cyclicAmiRuleString(
                "ru_mesh_pressure", "ru_mesh_suction"
              ),
              ofOpenFOAMCase_setupWrapper.mixingPlaneRuleString(
                "ru_mesh_outlet", "meshDiffuser_inlet", 
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
           
        ).buildExtract( self.container )

        self.dC["of"].runCurrentState()
 
    def getContainer(self):
        """Return the container object.
        
        Is used to create openFOAM cases.

        Parameters
        ----------
        None

        Returns
        -------
        container: dtBundle
          Initialization of the dtBundle.
        """
        return self.container

    def getbVAnddC(self):
        """Return the bV and dC object.
        
        Are used to create the meshes and openFoam cases.

        Parameters
        ----------
        None

        Returns
        -------
        bV: lvH_boundedVolume
          Labeled vector handling of bounded volumes.
        dC: lvh_dtCase
          Labeled vector handling of cases.
        """
        return self.bV, self.dC

def run(*args, **kwargs):
    from dtOOPythonApp.vis import dtOOInParaVIEW

    generate = axMeridional()

    generate.createMeridional()
    generate.createBlade()
    generate.createLayerRegion()

    cc = generate.getContainer()
    
    rr = dtOOInParaVIEW( cc )
    
    #bV, dC = generate.getbVAnddC()
            
    #stateLbl = "test"
    #indiv = "0" 
                  
    #createOFCase(cc, bV, dC, stateLbl, indiv)

    return cc, rr

if __name__ == "__main__":
    
    generate = axMeridional()

    generate.createMeridional()
    generate.createBlade()
    generate.createLayerRegion()
    generate.createOfCase() 
     
    # ------------------- EOF ------------------- #

