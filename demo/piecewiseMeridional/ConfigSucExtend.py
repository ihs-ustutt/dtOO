import numpy as np
import copy
import importlib
import radMeridional
import dtOOPythonSWIG as dtOO

class ConfigSucExtend():

    def __init__(self):
        
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

        h_inlet = 0.36
        h_hub = 0.68
        h_shroud = 0.38
        
        dx_hub0, dz_hub0 = self.deltas_LengthAndAngle(angle_hub0, l_hub0)
        dx_hub1, dz_hub1 = self.deltas_LengthAndAngle(angle_hub1, l_hub1)
        dx_shroud0, dz_shroud0 = self.deltas_LengthAndAngle(angle_shroud0, l_shroud0)
        dx_shroud1, dz_shroud1 = self.deltas_LengthAndAngle(angle_shroud1, l_shroud1)

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
                  << dtOO.dtPoint3(+d_inlet/2-dx_hub0, +0.00, +h_inlet-dz_hub0)
                  << dtOO.dtPoint3(+d_outHub/2+dx_hub1, +0.00, -h_hub+dz_hub1+h_inlet)
                  << dtOO.dtPoint3(+d_outHub/2, +0.00, -h_hub+h_inlet),
                2
              ).result()
            ),
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(+d_outHub/2, +0.00, -h_hub+h_inlet)
                  << dtOO.dtPoint3(+d_outHub/2, +0.00, -1.54),
                1
              ).result()
            ),
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(+d_outHub/2, +0.00, -1.54)
                  << dtOO.dtPoint3(+0.28, +0.00, -1.84),
                1
              ).result()
            ),
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(+0.28, +0.00, -1.84)
                  << dtOO.dtPoint3(+0.28, +0.00, -2.67),
                1
              ).result()
            ),
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(+0.28, +0.00, -2.67)
                  << dtOO.dtPoint3(+0.00, +0.00, -2.67),
                1
              ).result()
            ),
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(+0.00, +0.00, -2.67)
                  << dtOO.dtPoint3(+0.00, +0.00, -3.17),
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
                  << dtOO.dtPoint3(+d_inlet/2-dx_shroud0, +0.00, +0.00-dz_shroud0)
                  << dtOO.dtPoint3(+d_outShroud/2+dx_shroud1, +0.00, -h_shroud+dz_shroud1)
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
                  << dtOO.dtPoint3(+1.15, +0.00, -3.17),
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
                                     [0.4, 0.5, -1],],
        }
        self.configGuideVane = {
            "label" : "gv",
            "regChannel" : 0,
            "nBlades" : 24,
            
            "spanwiseCuts_mp" : [0.00, 1.00,],
            "alpha_1" : [round((np.pi/180.) * -55.0, 4)],
            "alpha_2" : [round((np.pi/180.) * -16.0, 4)],
            "ratioX" : [0.5],
            "deltaY" : [0.12],
            "offX" : [-0.046],
            "offY" : [0.077],

            "spanwiseCuts_td" : [0.00, 1.00,],
            "t_le" : [0.01],
            "u_le" : [0.00],
            "t_mid" : [0.03],
            "u_mid" : [0.20],
            "t_te" : [0.01],
            "u_te" : [0.80],

            "adjustRadius" : False,
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
            "u_te" : [0.80],

            "adjustRadius" : True,
        }
        self.configLayer = {
            "label" : "radMeridionalContour",
            "nSlices" : 15,
            "layer_thickness" : 0.2,
            "layer_supports" : [0.5],
        }
 

    def deltas_LengthAndAngle(self, angle, length):

        dx = np.cos(angle)*length
        dz = np.sin(angle)*length

        return dx, dz
    
    def getConfig(self):
        return self.configMeridional, self.configGuideVane, self.configRunner, self.configLayer

    def getCurves(self):
        return self.hubCurves, self.shroudCurves

def createOFCase(container, bV, dC, stateLbl, indiv):

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

    machine = ConfigSucExtend()
    configM, configGV, configRu, configL = machine.getConfig()
    hubCurves, shroudCurves = machine.getCurves()

    generate = radMeridional.radMeridional()

    generate.createMeridional(configM, hubCurves, shroudCurves)
    generate.createBlade(configGV)
    generate.createBlade(configRu)
    generate.createLayerRegion(configL)

    cc = generate.getContainer()
    
    rr = dtOOInParaVIEW( cc )
    
    #bV, dC = generate.getbVAnddC()
    #        
    #stateLbl = "test"
    #indiv = "0" 
    #              
    #createOFCase(cc, bV, dC, stateLbl, indiv)

    return cc, rr

if __name__ == "__main__":
    machine = ConfigSucExtend()
    configM, configGV, configRu, configL = machine.getConfig()
    hubCurves, shroudCurves = machine.getCurves()
    
    generate = radMeridional.radMeridional()

    generate.createMeridional(configM, hubCurves, shroudCurves)
    generate.createBlade(configGV)
    generate.createBlade(configRu)
    generate.createLayerRegion(configL)
    
    container = generate.getContainer()
    bV, dC = generate.getbVAnddC()
     
    stateLbl = "sucExtend"
    indiv = "0"

    createOFCase(container, bV, dC, stateLbl, indiv)

    # ------------------- EOF ------------------- #


