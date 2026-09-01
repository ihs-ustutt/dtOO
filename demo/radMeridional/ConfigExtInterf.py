import numpy as np
import copy
import importlib
import radMeridional
import dtOOPythonSWIG as dtOO

class ConfigExtInterf():

    def __init__(self):

        print("Setting up input dictionary")
        
        d_inlet = 2.58
        l_inExt = 0.27
        d_outHub = 0.4
        d_outShroud = 1.885

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
            #dtOO.analyticCurve(
            #  dtOO.bSplineCurve_pointConstructOCC(
            #    dtOO.vectorDtPoint3()
            #      << dtOO.dtPoint3(+d_outHub/2, +0.00, -h_hub+h_inlet)
            #      << dtOO.dtPoint3(+d_outHub/2, +0.00, -2.55),
            #    1
            #  ).result()
            #)

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
            #dtOO.analyticCurve(
            #  dtOO.bSplineCurve_pointConstructOCC(
            #    dtOO.vectorDtPoint3()
            #      << dtOO.dtPoint3(+d_outShroud/2, +0.00, -0.54)
            #      << dtOO.dtPoint3(+1.15, +0.00, -2.55),
            #    1
            #  ).result()
            #)
          ]

        self.configMeridional = {
            "label" : "radMeridionalContour",
            
            "interface_hub" : [[1, 0.00],
                               [1, 1.0],],                # [curve, percent]
            "interface_shroud" : [[1, 0.00],
                                  [2, 0.5],],
            "interface_curvature" : [[0.0, 0.5, 1],
                                     [0.0, 0.5, -1],],
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
            "l_tot" : 0.41,
            "ratioL" : 0.485,

            "spanwiseCuts_td" : [0.00, 1.00,],
            "t_le" : [0.0115],
            "u_le" : [0.00],
            "t_mid" : [0.0365],
            "u_mid" : [0.20],
            "t_te" : [0.008],
            "u_te" : [1.00],

            "orientation" : -1,
        }
        # Case - Fitted to blade
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
                     round((np.pi/180.) * 27., 4),
                     round((np.pi/180.) * 25., 4),
                     round((np.pi/180.) * 10., 4)
                 ],
            "ratioX" : [
                     0.65,
                     0.65,
                     0.15,
                     0.24
                 ],
            "deltaY" : [
                     0.80,
                     0.75,
                     0.75,
                     0.53
                 ],
            "offX" : [
                     0.125,
                     0.11,
                     0.11,
                     0.005
                 ],
            "offY" : [
                     0.065,
                     0.07,
                     0.035
                 ],

            "spanwiseCuts_td" : [0.00, 1.00,],
            "t_le" : [0.020,0.02],
            "u_le" : [0.00],
            "t_mid" : [0.04,0.03],
            "u_mid" : [0.50],
            "t_te" : [0.01],
            "u_te" : [1.00],

            "adjustRadius" : True,
            "orientation" : 1,

        }
        ## Case: of_ExtInterf_0
        #self.configRunner = {
        #    "label" : "ru",
        #    "regChannel" : 1,
        #    "nBlades" : 15,
        #    
        #    "spanwiseCuts_mp" : [0.00, 0.33,  0.66, 1.00,],
        #    "alpha_1" : [
        #             round((np.pi/180.) * 90., 4),
        #             round((np.pi/180.) * 75., 4),
        #             round((np.pi/180.) * 52., 4)
        #         ],
        #    "alpha_2" : [
        #             round((np.pi/180.) * 45., 4),
        #             round((np.pi/180.) * 31., 4),
        #             round((np.pi/180.) * 32., 4),
        #             round((np.pi/180.) * 10., 4)
        #         ],
        #    "ratioX" : [
        #             0.65,
        #             0.70,
        #             0.35,
        #             0.22
        #         ],
        #    "deltaY" : [
        #             0.80,
        #             0.55,
        #             0.90,
        #             0.55
        #         ],
        #    "offX" : [
        #             0.125,
        #             0.125,
        #             0.0
        #         ],
        #    "offY" : [
        #             0.065,
        #             0.085,
        #             0.035
        #         ],

        #    "spanwiseCuts_td" : [0.00, 1.00,],
        #    "t_le" : [0.020,0.018],
        #    "u_le" : [0.00],
        #    "t_mid" : [0.04,0.03],
        #    "u_mid" : [0.50],
        #    "t_te" : [0.01],
        #    "u_te" : [1.00],

        #    "adjustRadius" : True,
        #    "orientation" : 1,

        #}
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

def createOFCase(container, stateLbl, indiv, h_inlet):

    
    bV = container.cptr_bV()
    dC = container.cptr_dC()

    dtOO.lVHOstateHandler().makeState(stateLbl+"_"+str(indiv))
    
    #
    # draft tube
    #

    rmsh = dtOO.readMOABMesh()
    rmsh.jInit(dtOO.jsonPrimitive('{"label" : "dt_mesh", "_fileName" : "saugrohr.cgns"}'), None, None, None, None, None)
    rmsh.makeGrid()
    
    bV.push_back( rmsh )

    ob = dtOO.bVOFaceToPatchRule()
    ob.thisown = False
    ob.jInit(
      dtOO.jsonPrimitive(
        '{'
          '"_patchRule" : ['
            '":meshSet_2d_0::dt_mesh_wall:",'
            '":meshSet_2d_1::dt_mesh_central:",'
            '":meshSet_2d_2::dt_mesh_vent:",'
            '":meshSet_2d_3::dt_mesh_outlet:",'
            '":meshSet_2d_4::dt_mesh_inlet:"'
          '],'
          '"_regRule" : ['
            '":meshSet_3d_0::dt_mesh:"'
          ']'
        '}'
      ),
      rmsh
    )
    ob.postUpdate()
    
    ob = dtOO.bVOOrientCellVolumes()
    ob.thisown = False
    ob.jInit(
        dtOO.jsonPrimitive('{"_positive" : true}'), rmsh
    )
    ob.postUpdate()

    #ob = dtOO.bVOWriteMSH()
    #ob.thisown = False
    #ob.jInit(
    #        dtOO.jsonPrimitive('{"_filename" : "dt_mesh.msh", "_saveAll" : true}'),
    #  None, None, None, None, None, rmsh
    #)
    #ob.postUpdate()
    
     
    from dtOOPythonApp.builder import (
      ofOpenFOAMCase_turboMachine,
      ofOpenFOAMCase_setupWrapper
    )

    container = ofOpenFOAMCase_turboMachine(
      label = "of",
      bVs = [
        bV["gv_mesh"], 
        bV["ru_mesh"], 
        bV["dt_mesh"],
      ],
      dictRule = \
          ofOpenFOAMCase_setupWrapper.controlDict(
            application = "simpleFoam",
            endTime = 2000,
            # Patches where Q and PT is tracked
            QPatches = ['gv_mesh_inlet', 'gv_mesh_outlet', 
                        'ru_mesh_inlet', 'ru_mesh_outlet',
                        'dt_mesh_inlet', 'dt_mesh_outlet',
                        'gv_mesh_suction', 'gv_mesh_pressure',
                        'ru_mesh_suction', 'ru_mesh_pressure'],
            PTPatches = ['gv_mesh_inlet', 'gv_mesh_outlet',
                        'ru_mesh_inlet', 'ru_mesh_outlet',
                         'dt_mesh_inlet', 'dt_mesh_outlet',
                        'gv_mesh_suction', 'gv_mesh_pressure',
                        'ru_mesh_suction', 'ru_mesh_pressure'],
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
          ## meshInlet
          ofOpenFOAMCase_setupWrapper.cylindricalInletRuleString(
            "gv_mesh_inlet",
            ["U"],
            [ [-2.92*2.7,-7.23*2.7,0], ]
          ),
          ofOpenFOAMCase_setupWrapper.inletRuleString(
            "gv_mesh_inlet",
            ["p", "k", "omega",],
            [ [0], [0.1, 0.10], [0.032*h_inlet, 0.1] ]
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
            planes = 40,
            planesBl = 15,
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
          ofOpenFOAMCase_setupWrapper.mixingPlaneRuleString(
            "ru_mesh_outlet", "dt_mesh_inlet",
            ["U", "p", "k", "omega",],
            axis = dtOO.dtVector3(0,0,1),
            origin = dtOO.dtPoint3(0,0,0),
            stackAxis = "R",
            discretization = "userDefined",
            planes = 40,
            planesBl = 15,
            gradingIf = "false"
          ),
          
          # draft tube
          ofOpenFOAMCase_setupWrapper.wallRuleString(
            "dt_mesh_vent",
            ["omega", "U", "p", "k", "nut"]
          ),
          ofOpenFOAMCase_setupWrapper.wallRuleString(
            "dt_mesh_central",
            ["omega", "U", "p", "k", "nut"]
          ),
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
    
    dC["of"].runCurrentState()
    
def run(*args, **kwargs):
    from dtOOPythonApp.vis import dtOOInParaVIEW

    machine = ConfigExtInterf()
    configM, configGV, configRu, configL = machine.getConfig()
    hubCurves, shroudCurves = machine.getCurves()

    generate = radMeridional.radMeridional()

    generate.createMeridional(configM, hubCurves, shroudCurves)
    #generate.createBlade(configGV)
    generate.createBlade(configRu)

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
    
    #bV["gv_mesh"].makeGrid()
    #bV["ru_mesh"].makeGrid()
            
    #stateLbl = "test"
    #indiv = "0" 
                  
    #createOFCase(cc, stateLbl, indiv)

    return cc, rr

if __name__ == "__main__":
    
    machine = ConfigExtInterf()
    configM, configGV, configRu, configL = machine.getConfig()
    hubCurves, shroudCurves = machine.getCurves()
    
    generate = radMeridional.radMeridional()
    
    generate.createMeridional(configM, hubCurves, shroudCurves)
    #generate.createLayerRegion(configL)
    generate.createBlade(configGV)
    generate.createBlade(configRu)
    
    container = generate.getContainer()
    #bV = container.cptr_bV()
     
    #container = generate.getContainer()
    #from dtOOPythonApp.tools import dtOO2OCC
    #
    #lab = "ru"
    #
    #curve = dtOO2OCC.analyticCurve_analyticGeometry(
    #        container.cptr_aG()["debug_interface_1_radMeridionalContour"]
    #    ).approx(uInts=np.linspace(0.0, 1.0, 501))

    #surface = dtOO.analyticSurface(
    #    dtOO.surfaceOfRevolution_curveRotateConstructOCC(
    #        curve.ptrConstDtCurve(),
    #        dtOO.dtPoint3(0.0, 0.0, 0.0),
    #        dtOO.dtVector3(0.0, 0.0, 1.0)
    #    ).result()
    #)

    #print(type(surface))
    #
    #dtOO2OCC.WriteSTEP(
    #  dtOO2OCC.TopoDS([surface]),
    #  "interface.stp"
    #)
    
    #bV["ru_mesh"].makeGrid()
    #bV["gv_mesh"].makeGrid()
    #bV["meshLayers"].makeGrid()

    stateLbl = "fittedBlade"
    indiv = "0"
    
    createOFCase(container, stateLbl, indiv, 0.36)

    # ------------------- EOF ------------------- #
