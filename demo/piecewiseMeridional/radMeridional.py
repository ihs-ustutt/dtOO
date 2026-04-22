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
"""
introduction
"""
class radMeridional:
    """
    how do the class

    Attributes
    ----------

    Examples
    --------
    >>> import dtOOPythonSWIG as dtOO

    >>> d_inlet = 2.58
    >>> l_inExt = 0.27
    >>> d_outHub = 0.4
    >>> d_outShroud = 1.865

    >>> l_hub0 = 0.38
    >>> angle_hub0 = 0 * np.pi/180
    >>> l_hub1 = 0.38
    >>> angle_hub1 = 65 * np.pi/180

    >>> l_shroud0 = 0.13
    >>> angle_shroud0 = 0 * np.pi/180
    >>> l_shroud1 = 0.18
    >>> angle_shroud1 = 90 * np.pi/180

    >>> h_inlet = 0.36
    >>> h_hub = 0.68
    >>> h_shroud = 0.38
    >>> 
    >>> dx_hub1 = np.cos(angle_hub1)*l_hub1
    >>> dz_hub1 = np.sin(angle_hub1)*l_hub1

    >>> hubCurves = [
    ...     dtOO.analyticCurve(
    ...       dtOO.bSplineCurve_pointConstructOCC(
    ...         dtOO.vectorDtPoint3()
    ...           << dtOO.dtPoint3(+(d_inlet/2 + l_inExt), +0.00, +h_inlet)
    ...           << dtOO.dtPoint3(+d_inlet/2, +0.00, +h_inlet),
    ...         1
    ...       ).result()
    ...     ),
    ...     dtOO.analyticCurve(
    ...       dtOO.bSplineCurve_pointConstructOCC(
    ...         dtOO.vectorDtPoint3()
    ...           << dtOO.dtPoint3(+d_inlet/2, +0.00, +h_inlet)
    ...           << dtOO.dtPoint3(+d_inlet/2-l_hub0, +0.00, +h_inlet)
    ...           << dtOO.dtPoint3(+d_outHub/2+dx_hub1, +0.00, -h_hub+dz_hub1+h_inlet)
    ...           << dtOO.dtPoint3(+d_outHub/2, +0.00, -h_hub+h_inlet),
    ...         2
    ...       ).result()
    ...     ),
    ...     dtOO.analyticCurve(
    ...       dtOO.bSplineCurve_pointConstructOCC(
    ...         dtOO.vectorDtPoint3()
    ...           << dtOO.dtPoint3(+d_outHub/2, +0.00, -h_hub+h_inlet)
    ...           << dtOO.dtPoint3(+0.00, +0.00, -h_hub+h_inlet),
    ...         1
    ...       ).result()
    ...     ),
    ...     dtOO.analyticCurve(
    ...       dtOO.bSplineCurve_pointConstructOCC(
    ...         dtOO.vectorDtPoint3()
    ...           << dtOO.dtPoint3(+0.00, +0.00, -h_hub+h_inlet)
    ...           << dtOO.dtPoint3(+0.00, +0.00, -2.55),
    ...         1
    ...       ).result()
    ...     )
    ...   ]

    >>> shroudCurves = [
    ...     dtOO.analyticCurve(
    ...       dtOO.bSplineCurve_pointConstructOCC(
    ...         dtOO.vectorDtPoint3()
    ...           << dtOO.dtPoint3(+(d_inlet/2 + l_inExt), +0.00, +0.00)
    ...           << dtOO.dtPoint3(+d_inlet/2, +0.00, +0.00),
    ...         1
    ...       ).result()
    ...     ),
    ...     dtOO.analyticCurve(
    ...       dtOO.bSplineCurve_pointConstructOCC(
    ...         dtOO.vectorDtPoint3()
    ...           << dtOO.dtPoint3(+d_inlet/2, +0.00, +0.00)
    ...           << dtOO.dtPoint3(+d_inlet/2-l_shroud0, +0.00, +0.00)
    ...           << dtOO.dtPoint3(+d_outShroud/2, +0.00, -h_shroud+l_shroud1)
    ...           << dtOO.dtPoint3(+d_outShroud/2, +0.00, -h_shroud),
    ...         2
    ...       ).result()
    ...     ),
    ...     dtOO.analyticCurve(
    ...       dtOO.bSplineCurve_pointConstructOCC(
    ...         dtOO.vectorDtPoint3()
    ...           << dtOO.dtPoint3(+d_outShroud/2, +0.00, -h_shroud)
    ...           << dtOO.dtPoint3(+d_outShroud/2, +0.00, -0.54),
    ...         1
    ...       ).result()
    ...     ),
    ...     dtOO.analyticCurve(
    ...       dtOO.bSplineCurve_pointConstructOCC(
    ...         dtOO.vectorDtPoint3()
    ...           << dtOO.dtPoint3(+d_outShroud/2, +0.00, -0.54)
    ...           << dtOO.dtPoint3(+1.15, +0.00, -2.55),
    ...         1
    ...       ).result()
    ...     )
    ...   ]

    >>> configMeridional = {
    ...     "label" : "radMeridionalContour",
    ...
    ...     "interface_hub" : [[1, 0.00],
    ...                        [1, 0.7],],                # [curve, percent]
    ...     "interface_shroud" : [[1, 0.00],
    ...                           [2, 0.5],],
    ...     "interface_curvature" : [[0.0, 0.5, 1],
    ...                              [0.4, 0.5, -1],],
    ... }

    >>> configGuideVane = {
    ...     "nGvBlades" : 24,
    ...
    ...     "spanwiseCuts" : [0.00, 1.00,],
    ...     "gv_alpha_1" : [round((np.pi/180.) * -55.0, 4)],
    ...     "gv_alpha_2" : [round((np.pi/180.) * -16.0, 4)],
    ...     "gv_ratioX" : [0.5],
    ...     "gv_deltaY" : [0.12],
    ...     "gv_offX" : [-0.046],
    ...     "gv_offY" : [0.077],
    ...
    ...     "gv_t_le" : [0.01],
    ...     "gv_u_le" : [0.00],
    ...     "gv_t_mid" : [0.03],
    ...     "gv_u_mid" : [0.20],
    ...     "gv_t_te" : [0.01],
    ...     "gv_u_te" : [0.80],
    ... }
     
    >>> configRunner = {
    ...     "nRuBlades" : 15,
    ...
    ...     "spanwiseCuts_mp" : [0.00, 0.33,  0.66, 1.00,],
    ...     "ru_alpha_1" : [
    ...             round((np.pi/180.) * 90., 4),
    ...             round((np.pi/180.) * 75., 4),
    ...             round((np.pi/180.) * 52., 4)
    ...         ],
    ...     "ru_alpha_2" : [
    ...             round((np.pi/180.) * 45., 4),
    ...             round((np.pi/180.) * 31., 4),
    ...             round((np.pi/180.) * 32., 4),
    ...             round((np.pi/180.) * 10., 4)
    ...         ],
    ...     "ru_ratioX" : [
    ...             0.65,
    ...             0.70,
    ...             0.35,
    ...             0.22
    ...         ],
    ...     "ru_deltaY" : [
    ...             0.80,
    ...             0.55,
    ...             0.90,
    ...             0.55
    ...         ],
    ...     "ru_offX" : [
    ...             0.125,
    ...             0.125,
    ...             0.0
    ...         ],
    ...     "ru_offY" : [
    ...             0.065,
    ...             0.085,
    ...             0.035
    ...         ],
    ...
    ...     "spanwiseCuts_td" : [0.00, 1.00,],
    ...     "ru_t_le" : [0.020,0.018],
    ...     "ru_u_le" : [0.00],
    ...     "ru_t_mid" : [0.04,0.03],
    ...     "ru_u_mid" : [0.50],
    ...     "ru_t_te" : [0.02],
    ...     "ru_u_te" : [0.80],
    ... }
    
    >>> configLayer = {
    ...     "layer_thickness" : 0.2,
    ...     "layer_supports" : [0.5],
    ... }

    >>> config = configMeridional | configGuideVane | configRunner | configLayer

    >>> turbine = radMeridional(config, hubCurves, shroudCurves)
    >>> container =  turbine.create()
    """

    def __init__(
        self,
        #data: dict,
        #hubCurves: List[dtOO.analyticGeometry],
        #shroudCurves: List[dtOO.analyticGeometry],
        #stateLbl: str = "pvState",
        #indiv: int = 0,
        #createOFCase: bool = False
    ):
        """Constructor.

        Parameters
        ----------

        Returns
        -------
        None

        """

        ## constructiong variables for the class from the Config Class
        #for key, value in data.items():
        #    setattr(self, key, value)

        #self.hubCurves = hubCurves
        #self.shroudCurves = shroudCurves

        #self.stateLbl = stateLbl
        #self.indiv = indiv

        #self.createOFCase_ = createOFCase
        
        # Create an empty self.container
        self.container = dtOO.dtBundle()

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

    def createMeridional(self, configM, hubCurves, shroudCurves):
         
        # creating the meridional contour
        from dtOOPythonApp.builder import analyticGeometry_piecewiseMeridionalRotContour
        self.radMeridionalContour = analyticGeometry_piecewiseMeridionalRotContour( 
          label = configM["label"],
          hubCurves = hubCurves,
          shroudCurves = shroudCurves,
          interface_hub = configM["interface_hub"],
          interface_shroud = configM["interface_shroud"],
          interface_curvature = configM["interface_curvature"],
        ).enableDebug()#.buildExtract( self.container )
        self.container = self.radMeridionalContour.buildExtract(self.container)
    
    def createLayerRegion(self, configL):
        #
        # Building Layer Region
        #
        speHub, speShroud, inOutCurves = self.radMeridionalContour.getLayerRegionCurves()
        # creating the layer region object
        from dtOOPythonApp.builder import analyticGeometry_layerRegion
        layerRegion = analyticGeometry_layerRegion( 
          label = configL["label"],
          speHub = speHub,
          speShroud = speShroud,
          inOutCurves = inOutCurves,
          layer_thickness = configL["layer_thickness"],
          layer_supports = configL["layer_supports"],
        ).enableDebug()#.buildExtract( self.container )
        self.container = layerRegion.buildExtract(self.container)
        
        #
        # Meshing of layer region
        #
        # returning the hub and shroud layers
        layers = layerRegion.getLayerList(configL["nSlices"])    
        # returns layer data in the following nested list:
        # layers = [[hub layer lists],[shroud layer list]]
        # with:
        # [hub layer lists] = [[3d layer domain], [bool list radius zero]]

        # returns the unstructured region and its surfaces
        mv, bs = layerRegion.getUnstructuredRegion(configL["nSlices"])
        
        from dtOOPythonApp.builder import ( map3dTo3dGmsh_gridFromLayers ) 
        # creating the mesh of the suction area with wall layers and the unstructured region
        self.container = map3dTo3dGmsh_gridFromLayers(
                mv = mv,
                bs = bs,
                label = "meshLayers",
                layers = layers,
                nElementsLayer = 15,
                firstElement = 0.001,
                elementSize_sw = 0.02,
                elementSize_circ = 0.03,
                charLengthMin = 0.02,
                charLengthMax = 0.04,
            ).buildExtract(self.container)
        
    #
    # build blade function
    #
    def createBlade(self, configB):
        
        label = configB["label"]
        regChannel = configB["regChannel"]
        nBlades = configB["nBlades"]

        spanwiseCuts_mp = configB["spanwiseCuts_mp"]
        alpha_1 = configB["alpha_1"]
        alpha_2 = configB["alpha_2"]
        ratioX = configB["ratioX"]
        deltaY = configB["deltaY"]
        offX = configB["offX"]
        offY = configB["offY"]

        spanwiseCuts_td = configB["spanwiseCuts_td"]
        t_le = configB["t_le"]
        u_le = configB["u_le"]
        t_mid = configB["t_mid"]
        u_mid = configB["u_mid"]
        t_te = configB["t_te"]
        u_te = configB["u_te"]
        
        adjustRadius = configB["adjustRadius"]

        self.aG.push_back(
            self.radMeridionalContour.getRegChannel(
                regChannel, 1
            ) << "xyz_"+label+"_channel"
        )
        #
        # meanplane
        #
        from dtOOPythonApp.builder import (
            analyticSurface_threePointMeanplaneFromRatio,
            scaOneD_scaCurve2dOneDPointConstruct
        )
        self.container = analyticSurface_threePointMeanplaneFromRatio(
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
        ).buildExtract( self.container )
        
        #
        # thickness distribution
        #
        from dtOOPythonApp.builder import (
          vec3dSurfaceTwoD_fivePointsBSplineThicknessDistribution
        )
        self.container = vec3dSurfaceTwoD_fivePointsBSplineThicknessDistribution(
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
        ).buildExtract( self.container )

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
            .appendBool("_adjustRadius", adjustRadius)\
            .appendAnalyticGeometry(\
              "_rM2d", \
              self.aG.get("xyz_" + label + "_channel")\
            ),\
          None, None, None, self.aG \
        )
        self.bC.dtTransformer().add( conMap.clone() )
         
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
        # mesh block
        #
        meshBlock_thickness = 0.01 
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
          meanplaneExtOut = 0.02,
          meanplaneExtIn = 0.03,
          nMeanplaneBlocks = nMeanplaneBlocks
        ).buildExtract(self.container)
        
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
        from dtOOPythonApp.builder import analyticSurface_inOutFeMeanplane
        self.container = analyticSurface_inOutFeMeanplane(
            prefix = "xyz",
            label = label,
            channel = self.aG["xyz_"+label+"_channel"],
            curves = meshBlockCurves
        ).enableDebug().buildExtract(self.container)
         
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
        from dtOOPythonApp.builder import multipleBoundedVolume_gridChannel
        gridChannel = multipleBoundedVolume_gridChannel(
            label = label,
            channel = self.aG["xyz_"+label+"_channel"],
            meanplanes = meanplaneFaces,
            couplings = couplingFaces,
            nBlades = nBlades
        ).enableDebug()
        self.container = gridChannel.buildExtract(self.container)
        
        gc, gcFaces = gridChannel.getGridChannel()
        self.aG.push_back(
            gc << "xyz_"+label+"_gridChannel"
        )
        
        #
        # generating mesh
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
            nBoundaryLayers = 15,
            nElementsSpanwise = 50,
            nElementsNormal = 7,
            firstElementSizeHubToShroud = 0.001,
            firstElementSizeNormalBlade = 0.001,
            bladeHubElementSize = scaOneD_scaCurve2dOneDPointConstruct(
                [
                    dtOO.dtPoint2(0.00, 0.024),
                    dtOO.dtPoint2(0.45, 0.007),
                    dtOO.dtPoint2(0.50, 0.007),
                    dtOO.dtPoint2(0.55, 0.007),
                    dtOO.dtPoint2(1.00, 0.024),
                ], 1
            )(),
            bladeHubElementScale = 0.3,
            charLengthMax=0.04,
            charLengthMin=0.02,
            meshTEBlocks = True,
        ).enableDebug().buildExtract( self.container )
    
    def getContainer(self):
        return self.container

    def getbVAnddC(self):
        return self.bV, self.dC
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

