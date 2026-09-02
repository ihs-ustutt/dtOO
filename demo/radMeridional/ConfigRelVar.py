import numpy as np
import math
import random
import ast
import csv
import copy
import importlib
import traceback
import radMeridional
import dtOOPythonSWIG as dtOO

class ConfigOptimization():

    def __init__(self, configMeas):
        
        print("Setting up input dictionary")
        
        # setting dict items as class variables
        for key, value in configMeas.items():
            setattr(self, key, value)
        
        # calculation point offsets for the channel curves
        self.dx_hub0, self.dz_hub0 = self.deltas_LengthAndAngle(self.angle_hub0, self.l_hub0)
        self.dx_hub1, self.dz_hub1 = self.deltas_LengthAndAngle(self.angle_hub1, self.l_hub1)
        self.dx_shroud0, self.dz_shroud0 = self.deltas_LengthAndAngle(self.angle_shroud0, self.l_shroud0)
        self.dx_shroud1, self.dz_shroud1 = self.deltas_LengthAndAngle(self.angle_shroud1, self.l_shroud1)
        
        # build hub and shroud curves
        self.hubCurves = self.buildHubCurves()
        self.shroudCurves = self.buildShroudCurves()
 
        print("Finished")

    def buildHubCurves(self):
        
        return [
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(+(self.d_inlet/2 + self.l_inExt), +0.00, +self.h_inlet)
                  << dtOO.dtPoint3(+self.d_inlet/2, +0.00, +self.h_inlet),
                1
              ).result()
            ),
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(+self.d_inlet/2, +0.00, +self.h_inlet)
                  << dtOO.dtPoint3(+self.d_inlet/2-self.dx_hub0, +0.00, +self.h_inlet-self.dz_hub0)
                  << dtOO.dtPoint3(+self.d_outHub/2+self.dx_hub1, +0.00, -self.h_hub+self.dz_hub1+self.h_inlet)
                  << dtOO.dtPoint3(+self.d_outHub/2, +0.00, -self.h_hub+self.h_inlet),
                2
              ).result()
            ),
          ]
    
    def buildShroudCurves(self):
        
        return [
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(+(self.d_inlet/2 + self.l_inExt), +0.00, +0.00)
                  << dtOO.dtPoint3(+self.d_inlet/2, +0.00, +0.00),
                1
              ).result()
            ),
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(+self.d_inlet/2, +0.00, +0.00)
                  << dtOO.dtPoint3(+self.d_inlet/2-self.dx_shroud0, +0.00, +0.00-self.dz_shroud0)
                  << dtOO.dtPoint3(+self.d_outShroud/2+self.dx_shroud1, +0.00, -self.h_shroud+self.dz_shroud1)
                  << dtOO.dtPoint3(+self.d_outShroud/2, +0.00, -self.h_shroud),
                2
              ).result()
            ),
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(+self.d_outShroud/2, +0.00, -self.h_shroud)
                  << dtOO.dtPoint3(+self.d_outShroud/2, +0.00, -0.54),
                1
              ).result()
            ),
          ]

    def deltas_LengthAndAngle(self, angle, length):

        dx = np.cos(angle)*length
        dz = np.sin(angle)*length

        return dx, dz
    
    def getCurves(self):
        # returns hub and shroud curves
        return self.hubCurves, self.shroudCurves

class Config():
    
    def __init__(self):

        # 
        # specifies configs
        #
       
        # config for hub and shroud curves
        self.configMeas = {
            "d_inlet" : 2.58,
            "l_inExt" : 0.27,
            "d_outHub" : 0.4,
            "d_outShroud" : 1.885,

            "l_hub0" : 0.38,
            "angle_hub0" : round(0 * np.pi/180, 4),
            "l_hub1" : 0.38,
            "angle_hub1" : round(65 * np.pi/180, 4),

            "l_shroud0" : 0.13,
            "angle_shroud0" : round(0 * np.pi/180, 4),
            "l_shroud1" : 0.18,
            "angle_shroud1" : round(90 * np.pi/180, 4),

            "h_inlet" : 0.36,
            "h_hub" : 0.68,
            "h_shroud" : 0.38,
        }
        
        # config for meridional contour -> specifies interfaces
        self.configMeridional = {
            "label" : "radMeridionalContour",
            "interface_hub" : [[1, 0.00],
                               [1, 1.0],],                # [curve, percent]
            "interface_shroud" : [[1, 0.00],
                                  [2, 0.5],],
            "interface_curvature" : [[0.0, 0.5, 1],
                                     [0.0, 0.5, -1],],
        }

        # config for guide vane blade
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

        # config for runner blade
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

        # config for generation of layer region
        self.configLayer = {
            "label" : "radMeridionalContour",
            "nSlices" : 15,
            "layer_thickness" : 0.0775,
            "layer_supports" : [0.5],
        }
        
        #
        # input values for variation and their variation:
        #


        self.varList = [
            ["alpha_1", 0.05], ["alpha_2", 0.05],
            ["ratioX", 0.05], ["deltaY", 0.05],
            ["t_le",0.05], ["t_mid", 0.05], ["t_te", 0.05],
        ]
        #self.varList = [
        #    ["l_hub0", 0.5], ["l_hub1", 0.5], ["angle_hub1", 0.1],
        #    ["l_shroud0", 0.5], ["l_shroud1", 0.5],  
        #    ["alpha_1", 0.05], ["alpha_2", 0.05],
        #    ["ratioX", 0.1], ["deltaY", 0.1],
        #    ["t_le",0.1], ["t_mid", 0.1], ["t_te", 0.1],
        #]
    
    def getConfig(self):
        # returns config dictionaries
        return (
                copy.deepcopy(self.configMeas), copy.deepcopy(self.configMeridional), 
                copy.deepcopy(self.configGuideVane), copy.deepcopy(self.configRunner), 
                copy.deepcopy(self.configLayer),
            )

    def getVarList(self):
        # returns varList
        return self.varList

def createOFCase(container, stateLbl, indiv, h_inlet, h_shroud):
   
        
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
    # function for running the file in paraview
    from dtOOPythonApp.vis import dtOOInParaVIEW
    
    config = Config()
    configMeas, configMeridional, configGuideVane, configRunner, configLayer = config.getConfig()
    varList = config.getVarList()
    
    # individual which will be recreated
    target_individual = 3
    
    stateLbl = "relVar"
    evalFolder = "ofCase_eval"
    
    # open csv which saves the varied parameters
    with open("./"+evalFolder+"/"+stateLbl+".csv", "r", newline="") as f:
        reader = list(csv.reader(f))
        row = reader[target_individual+1]


    # skip first column (individual index) and the last (case created)
    values = row[1:-1]
    
    # updating the configs so they contain the parmeters form the specified individual
    for i, param in enumerate(varList):
        value = values[i]
        
        if param[0] in configMeas:
            # detect list
            if value.startswith("["):
                configMeas[param[0]] = ast.literal_eval(value)
            else:
                configMeas[param[0]] = float(value)
 
        elif param[0] in configRunner:
            # detect list
            if value.startswith("["):
                configRunner[param[0]] = ast.literal_eval(value)
            else:
                configRunner[param[0]] = float(value)
    print("configMeas:")
    for p in configMeas:
        print(p," : ",configMeas[p])
    print("configRunner")
    for p in configRunner:
        print(p," : ",configRunner[p])
    
    # generate hub and shroud curves
    machine = ConfigOptimization(configMeas)
    hubCurves, shroudCurves = machine.getCurves()
    
    importlib.reload(radMeridional)
    
    # create radMeridional object
    generate = radMeridional.radMeridional()

    # create meridional channel
    generate.createMeridional(configMeridional, hubCurves, shroudCurves)

    ## create guide vane
    #generate.createBlade(configGuideVane)

    # create runner
    generate.createBlade(configRunner)
 
    # paraview plotting
    cc = generate.getContainer()
    
    bV = cc.cptr_bV()
    bV["ru_mesh"].makeGrid()

    #createOFCase(cc, stateLbl, target_individual, configMeas["h_inlet"], configMeas["h_shroud"])

    rr = dtOOInParaVIEW( cc )
    return cc, rr

def round_sig(x, sig=4):
    # rounding to significant position
    if x == 0:
        return 0
    return round(x, sig - int(np.floor(np.log10(abs(x)))) - 1)

def varConfig(config, paramStr, var):
    
    #
    # varies the config dict 
    #

    # checks if the varied parameter is in the dict
    if paramStr in config:

        # if the varied paramStreter is a list, all list items are changed
        #  with the same variation
        if isinstance(config[paramStr], list):
            for i in range(len(config[paramStr])):
                config[paramStr][i] = round_sig(config[paramStr][i] + config[paramStr][i]*var)
            

        # non list items are varied like this
        else:
            config[paramStr] = round_sig(config[paramStr] + config[paramStr]*var)
    
    return config

def appendRow(row, varList, config):
    
    #
    # appending the row for the cfg file
    #

    # iterating over parameters that are varied
    for param in varList:

        # when the parameter string is in the config file the value will
        #  be written in the row
        if param[0] in config:
            if isinstance(config[param[0]], list):
                row.append(str(config[param[0]]))
            else:
                row.append(config[param[0]])
    
    return row


if __name__ == "__main__":
    
    # create Config object and return the specified varList
    config = Config()
    varList = config.getVarList()
    
    stateLbl = "relVar"
     
    # number of iterations
    nIt = 4

    # activates optimization
    optiOn = True 
    
    # writer for the csv file
    evalFolder = "ofCase_eval"
    with open("./"+evalFolder+"/"+stateLbl+".csv", "w", newline="") as f:
        writer = csv.writer(f)

        # header (column names)
        header = ["individual"] + [param[0] for param in varList] + ["case created"]
        writer.writerow(header)
        
        # iterating over individuals
        for individual in range(nIt):
            
            # getting the original config dicts from the Config object
            configMeas, configMeridional, configGuideVane, configRunner, configLayer = config.getConfig()
            
            # first individual is made from specified values -> no variation
            if individual != 0 and optiOn == True:
                
                # iterating over the parameters which are changed
                for param in varList:
                    
                    # extracting values
                    paramStr = param[0]
                    variation = param[1]

                    # variation is +- of the specified value
                    var = random.uniform(-variation, variation)
                    
                    # apply the variation to the configs
                    configMeas = varConfig(configMeas, paramStr, var)
                    configRunner = varConfig(configRunner, paramStr, var)
            
            # saving parameters of the individual in row
            row = [individual]
            row = appendRow(row, varList, configMeas)
            row = appendRow(row, varList, configRunner)
            
            print("Building Individual No. " + str(individual))
            
            # generate geometry
            try:
                machine = ConfigOptimization(configMeas) 
                hubCurves, shroudCurves = machine.getCurves()
                
                generate = radMeridional.radMeridional()
                generate.createMeridional(configMeridional, hubCurves, shroudCurves)
                #if not (individual != 0 and optiOn == True):
                generate.createBlade(configGuideVane)
                generate.createBlade(configRunner)

                container = generate.getContainer()
                

                print("Sucess")
                row.append("success")
                writer.writerow(row)
                f.flush()
            except:
                error_msg = traceback.format_exc()
                print("Failed:\n", error_msg)
                row.append(str(error_msg)) 
                writer.writerow(row)
                f.flush()
                if individual == 0:
                    break
                continue






