import numpy as np
import copy
import importlib
import radMeridional
import dtOOPythonSWIG as dtOO

class ConfigHubZero():

    def __init__(self):

        print("Setting up input dictionary")
        
        d_inlet = 2.58
        l_inExt = 0.27
        d_outHub = 0.4
        d_outShroud = 1.865

        l_hub0 = 0.38
        angle_hub0 = 0 * np.pi/180
        l_hub1 = 0.38
        angle_hub1 = 65 * np.pi/180

        l_shroud0 = 0.13
        angle_shroud0 = 0 * np.pi/180
        l_shroud1 = 0.18
        angle_shroud1 = 90 * np.pi/180

        #h_inlet = 0.18
        h_inlet = 0.36
        h_hub = 0.68
        h_shroud = 0.38

        dx_hub1 = np.cos(angle_hub1)*l_hub1
        dz_hub1 = np.sin(angle_hub1)*l_hub1

        self.hubCurves = [
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(+(d_inlet/2 + l_inExt), +0.00, +h_inlet)
                  << dtOO.dtPoint3(+d_inlet/2, +0.00, +h_inlet),
                1
              ).result()
            ),
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(+d_inlet/2, +0.00, +h_inlet)
                  << dtOO.dtPoint3(+d_inlet/2-l_hub0, +0.00, +h_inlet)
                  << dtOO.dtPoint3(+d_outHub/2+dx_hub1, +0.00, -h_hub+dz_hub1+h_inlet)
                  << dtOO.dtPoint3(+d_outHub/2, +0.00, -h_hub+h_inlet),
                2
              ).result()
            ),
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(+d_outHub/2, +0.00, -h_hub+h_inlet)
                  << dtOO.dtPoint3(+0.00, +0.00, -h_hub+h_inlet),
                1
              ).result()
            ),
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(+0.00, +0.00, -h_hub+h_inlet)
                  << dtOO.dtPoint3(+0.00, +0.00, -2.55),
                1
              ).result()
            )
          ]

        self.shroudCurves = [
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(+(d_inlet/2 + l_inExt), +0.00, +0.00)
                  << dtOO.dtPoint3(+d_inlet/2, +0.00, +0.00),
                1
              ).result()
            ),
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(+d_inlet/2, +0.00, +0.00)
                  << dtOO.dtPoint3(+d_inlet/2-l_shroud0, +0.00, +0.00)
                  << dtOO.dtPoint3(+d_outShroud/2, +0.00, -h_shroud+l_shroud1)
                  << dtOO.dtPoint3(+d_outShroud/2, +0.00, -h_shroud),
                2
              ).result()
            ),
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(+d_outShroud/2, +0.00, -h_shroud)
                  << dtOO.dtPoint3(+d_outShroud/2, +0.00, -0.54),
                1
              ).result()
            ),
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(+d_outShroud/2, +0.00, -0.54)
                  << dtOO.dtPoint3(+1.15, +0.00, -2.55),
                1
              ).result()
            )
          ]

        self.configMeridional = {
            "label" : "radMeridionalContour",
            
            "interface_hub" : [[1, 0.00],
                               [1, 0.7],],                # [curve, percent]
            "interface_shroud" : [[1, 0.00],
                                  [2, 0.5],],
            "interface_curvature" : [[0.0, 0.5, 1],
                                     [0.35, 0.5, -1],],
        }
        ## Curved Guide Vane Configuration
        #self.configGuideVane = {
        #    "label" : "gv",
        #    "regChannel" : 0,
        #    "nBlades" : 24,
        #    
        #    "spanwiseCuts_mp" : [0.00, 1.00,],
        #    "alpha_1" : [round((np.pi/180.) * -55.0, 4)],
        #    "alpha_2" : [round((np.pi/180.) * -16.0, 4)],
        #    "ratioX" : [0.5],
        #    "deltaY" : [0.12],
        #    "offX" : [-0.046],
        #    "offY" : [0.077],

        #    "spanwiseCuts_td" : [0.00, 1.00,],
        #    "t_le" : [0.01],
        #    "u_le" : [0.00],
        #    "t_mid" : [0.03],
        #    "u_mid" : [0.20],
        #    "t_te" : [0.01],
        #    "u_te" : [0.80],

        #    "adjustRadius" : False,
        #    "orientation" : -1,
        #}

        # Straight Guide Vane Configuration
        self.configGuideVane = {
            "label" : "gv",
            "regChannel" : 0,
            "nBlades" : 24,
            
            "offRad" : 1.4125,
            "offAng" : np.pi/180 * -10,
            "alpha" : np.pi/180 * 68,
            "l_tot" : 0.4275,
            "ratioL" : 0.4675,

            "spanwiseCuts_td" : [0.00, 1.00,],
            "t_le" : [0.0115],
            "u_le" : [0.00],
            "t_mid" : [0.0365],
            "u_mid" : [0.20],
            "t_te" : [0.0195],
            #"u_te" : [1.0],
            "u_te" : [0.70],

            "orientation" : -1,
        }
        self.configRunner = {
            "label" : "ru",
            "regChannel" : 1,
            "nBlades" : 15,
            
            "spanwiseCuts_mp" : [0.00, 0.33,  0.66, 1.00,],
            "alpha_1" : [
                     round((np.pi/180.) * 90., 4),
                     round((np.pi/180.) * 75., 4),
                     round((np.pi/180.) * 52., 4)
                 ],
            "alpha_2" : [
                     round((np.pi/180.) * 45., 4),
                     round((np.pi/180.) * 31., 4),
                     round((np.pi/180.) * 32., 4),
                     round((np.pi/180.) * 10., 4)
                 ],
            "ratioX" : [
                     0.65,
                     0.70,
                     0.35,
                     0.22
                 ],
            "deltaY" : [
                     0.80,
                     0.55,
                     0.90,
                     0.55
                 ],
            "offX" : [
                     0.125,
                     0.125,
                     0.0
                 ],
            "offY" : [
                     0.065,
                     0.085,
                     0.035
                 ],

            "spanwiseCuts_td" : [0.00, 1.00,],
            "t_le" : [0.020,0.018],
            "u_le" : [0.00],
            "t_mid" : [0.04,0.03],
            "u_mid" : [0.50],
            "t_te" : [0.02],
            "u_te" : [1.00],

            "adjustRadius" : True,
            "orientation" : 1,

        }
        self.configLayer = {
            "label" : "radMeridionalContour",
            "nSlices" : 15,
            "layer_thickness" : 0.2,
            "layer_supports" : [0.5],
        }



    
    def getConfig(self):
        return self.configMeridional, self.configGuideVane, self.configRunner, self.configLayer

    def getCurves(self):
        return self.hubCurves, self.shroudCurves

def createOFCase(container, stateLbl, indiv):

    bV = container.cptr_bV()
    dC = container.cptr_dC()

    dtOO.lVHOstateHandler().makeState(stateLbl+"_"+str(indiv))

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
        bV["gv_mesh"], bV["ru_mesh"], bV["meshLayers"],
      ],
      dictRule = \
          ofOpenFOAMCase_setupWrapper.controlDict(
            application = "simpleFoam",
            endTime = 2000,
            # Patches where Q and PT is tracked
            QPatches = ['gv_mesh_inlet', 'gv_mesh_outlet',
                        'ru_mesh_inlet', 'ru_mesh_outlet',
                        'meshLayers_inlet', 'meshLayers_outlet',
                        'gv_mesh_suction', 'gv_mesh_pressure',
                        'ru_mesh_suction', 'ru_mesh_pressure',
                        'meshLayers_periodic0', 'meshLayers_periodic1'],
            PTPatches = ['gv_mesh_inlet', 'gv_mesh_outlet',
                        'ru_mesh_inlet', 'ru_mesh_outlet',
                        'meshLayers_inlet', 'meshLayers_outlet',
                        'gv_mesh_suction', 'gv_mesh_pressure',
                        'ru_mesh_suction', 'ru_mesh_pressure',
                        'meshLayers_periodic0', 'meshLayers_periodic1'],
            FPatches = ['gv_mesh_blade', 'ru_mesh_blade'],
            libs = [
              "libsimpleFunctionObjects.so",
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
            [ [0], [0.0, 0.10], [0.032*0.36, 0.1] ]
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
            discretization = "userDefined",
            planes = 30,
            planesBl = 7,
            gradingIf = "false"
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
            planes = 30,
            planesBl = 7,
            gradingIf = "false"
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

def run(*args, **kwargs):
    from dtOOPythonApp.vis import dtOOInParaVIEW

    machine = ConfigHubZero()
    configM, configGV, configRu, configL = machine.getConfig()
    hubCurves, shroudCurves = machine.getCurves()

    generate = radMeridional.radMeridional()

    generate.createMeridional(configM, hubCurves, shroudCurves)
    #generate.createBlade(configGV)
    generate.createBlade(configRu)
    #generate.createLayerRegion(configL)

    cc = generate.getContainer()
    

    rr = dtOOInParaVIEW( cc )
    
    #rr.Show( rr.Find( "xyz_gv_channel", True), "xyz_gv_channel" )
    #rr.Show( rr.Find( "debug_guideVaneRotAxis", True), "gv_rotAxis" )
    ##rr.Show( rr.Find( "debug_offVec", True), "gv_offVec" )
    #rr.Show( rr.Find( "debug_mpCurveHub", True), "mpCurveHub" )
    #rr.Show( rr.Find( "debug_mpCurveShr", True), "mpCurveShr" )
    #rr.Show( rr.Find( "gv_meanplane", True), "gv_meanplane" )
    #rr.Show( rr.Find( "xyz_gv_blade", True), "xyz_gv_blade" )
    
    #bV = cc.cptr_bV()
    
    #bV["meshLayers"].makeGrid()
    #bV["gv_mesh"].makeGrid()
    #bV["ru_mesh"].makeGrid()
            
    #stateLbl = "test"
    #indiv = "0" 
                  
    #createOFCase(cc, stateLbl, indiv)

    return cc, rr

if __name__ == "__main__":
    machine = ConfigHubZero()
    configM, configGV, configRu, configL = machine.getConfig()
    hubCurves, shroudCurves = machine.getCurves()
    
    generate = radMeridional.radMeridional()

    generate.createMeridional(configM, hubCurves, shroudCurves)
    #generate.createBlade(configGV)
    generate.createBlade(configRu)
    #generate.createLayerRegion(configL)
    
    container = generate.getContainer()
    """
    from dtOOPythonApp.tools import dtOO2OCC
    
    lab = "ru"
    
    # lists for hub and shroud edges
    hubEdges = []
    shroudEdges = []
    
    # lists for inlet and outlet surfaces and periodic surfaces
    inOutList = []
    perList0 = []
    perList1 = []
    
    #
    # Get surfaces of the bladed channel segment as wel as the 
    # hub and shroud edges
    #
    # blade
    blade = dtOO2OCC.analyticSurface_analyticGeometry(
        container.cptr_aG()["xyz_"+lab+"_blade"]
      ).approx(uInts=np.linspace(0.0, 1.0, 501))
    bladeHub = blade.segmentConstUPercent(0)
    bladeShroud = blade.segmentConstUPercent(1)
    
    # inlet
    inlet = dtOO2OCC.analyticSurface_analyticGeometry(
        container.cptr_aG()["debug_gridChannelFace_"+lab+"_inlet"]
      ).approx(uInts=np.linspace(0.0, 1.0, 501))
    inOutList.append(inlet)
    hubEdges.append(inlet.segmentConstUPercent(0))
    shroudEdges.append(inlet.segmentConstUPercent(1))
    
    # outlet
    outlet = dtOO2OCC.analyticSurface_analyticGeometry(
        container.cptr_aG()["debug_gridChannelFace_"+lab+"_outlet"]
      ).approx(uInts=np.linspace(0.0, 1.0, 501))
    inOutList.append(outlet)
    hubEdges.append(outlet.segmentConstUPercent(0))
    shroudEdges.append(outlet.segmentConstUPercent(1))
    
    # bounding surface of the hub-mbs
    hub = dtOO2OCC.TopoDS([
      dtOO2OCC.analyticSurface_analyticGeometry(
        dtOO.multipleBoundedSurface.MustDownCast(
          container.cptr_aG()["debug_gridChannelFace_"+lab+"_hub"]
        ).surfaceConstPtr()
      ).approx(uInts=np.linspace(0.0, 1.0, 501))
      ])
    
    # bounding surface of the shroud-mbs
    shroud = dtOO2OCC.TopoDS([
      dtOO2OCC.analyticSurface_analyticGeometry(
        dtOO.multipleBoundedSurface.MustDownCast(
          container.cptr_aG()["debug_gridChannelFace_"+lab+"_shroud"]
        ).surfaceConstPtr()
      ).approx(uInts=np.linspace(0.0, 1.0, 501))
      ])
    
    # mesh block faces forming the periodic menaplane
    # -> periodic 0
    for iNum in container.cptr_aG().getIndices("xyz_"+lab+"_meanplaneFaceOnBlock_*"):
        ii = container.cptr_aG().getLabel( iNum )
        
        per = dtOO2OCC.analyticSurface_analyticGeometry(
                container.cptr_aG()[ii]
            ).approx(uInts=np.linspace(0.0, 1.0, 501))
        hubEdges.append(per.segmentConstUPercent(0))
        shroudEdges.append(per.segmentConstUPercent(1))
        perList0.append(per)
    
    # fe-meanplane faces
    # -> periodic 0
    for iNum in container.cptr_aG().getIndices("xyz_"+lab+"_fe_meanplane_*"):
        ii = container.cptr_aG().getLabel( iNum )

        per = dtOO2OCC.analyticSurface_analyticGeometry(
                container.cptr_aG()[ii]
            ).approx(uInts=np.linspace(0.0, 1.0, 501))
        hubEdges.append(per.segmentConstUPercent(0))
        shroudEdges.append(per.segmentConstUPercent(1))
        perList0.append(per)
    
    # -> periodic 1
    for iNum in container.cptr_aG().getIndices("debug_gridChannelFace_"+lab+"_pressure_*"):
        ii = container.cptr_aG().getLabel( iNum )

        per = dtOO2OCC.analyticSurface_analyticGeometry(
                container.cptr_aG()[ii]
            ).approx(uInts=np.linspace(0.0, 1.0, 501))
        hubEdges.append(per.segmentConstUPercent(0))
        shroudEdges.append(per.segmentConstUPercent(1))
        perList1.append(per)
    
    # order the edges on the bladed channels hub and shroud 
    # by connectivity. Changing the directions if necessary
    from dtOOPythonApp.builder import vectorHandlingAnalyticGeometry_sortCurves
    sort = vectorHandlingAnalyticGeometry_sortCurves(
            curvesUnsorted = hubEdges
        ).enableDebug()
    container = sort.buildExtract(container)
    hubEdges = sort.getSortedCurves()
    sort = vectorHandlingAnalyticGeometry_sortCurves(
            curvesUnsorted = shroudEdges
        ).enableDebug()
    container = sort.buildExtract(container)
    shroudEdges = sort.getSortedCurves()
    
    # transform the blade and channel hub and shroud edges into 
    # occ-objects
    bladeHub = dtOO2OCC.TopoDS([bladeHub])
    bladeShroud = dtOO2OCC.TopoDS([bladeShroud])
    hubEdges = dtOO2OCC.TopoDS(hubEdges)
    shroudEdges = dtOO2OCC.TopoDS(shroudEdges)
    
    # trimm the bounding surfaces of the hub and shroud with the
    # edge lists
    hub = dtOO2OCC.makeTopoDS_FaceAndEdges(
            face = hub[0], 
            edgesTrim = hubEdges, 
            edgesHole = bladeHub
        )
    shroud = dtOO2OCC.makeTopoDS_FaceAndEdges(
            face = shroud[0], 
            edgesTrim = shroudEdges, 
            edgesHole = bladeShroud
        )
    
    # split blade into two surfaces
    blade0 = dtOO.analyticSurface(
        dtOO.bSplineSurface_bSplineSurfaceSplitConstructOCC(
            blade.ptrConstDtSurface(),
            1,
            blade.v_percent(0),
            blade.v_percent(0.5),
        ).result()
    )
    blade1 = dtOO.analyticSurface(
        dtOO.bSplineSurface_bSplineSurfaceSplitConstructOCC(
            blade.ptrConstDtSurface(),
            1,
            blade.v_percent(0.5),
            blade.v_percent(1.0),
        ).result()
    )
     
    # create a shell of all surfaces
    omniShell = dtOO2OCC.createShell(
        dtOO2OCC.TopoDS( 
            perList0
        ) + dtOO2OCC.TopoDS(
            perList1
        ) + dtOO2OCC.TopoDS(
            inOutList 
        ) + [ 
            hub, 
            shroud
        ] + dtOO2OCC.TopoDS(
            [ blade0, blade1 ]
        )
    )
    
    # create a solid body bounded by the shell
    solid = dtOO2OCC.createSolid(omniShell)
    

    solid = dtOO2OCC.scale(solid, 1000) 

    dtOO2OCC.WriteSTEP(
      [solid],
      #[walls, periodic0, periodic1]+inOutList,
      #[hub, shroud] + inOutList,
      #dtOO2OCC.TopoDS([blade0, blade1]), 
      lab + ".stp"
    )
    """
    #bV = container.cptr_bV()
    #bV["ru_mesh"].makeGrid()
    bV["gv_mesh"].makeGrid()
    stateLbl = "hubZero"
    indiv = "0"
    #createOFCase(container, stateLbl, indiv)

    # ------------------- EOF ------------------- #
