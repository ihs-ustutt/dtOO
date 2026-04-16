import numpy as np
import math
import random
import ast
import csv
import copy
import importlib
import radMeridionalOptimization
import dtOOPythonSWIG as dtOO

class ConfigDraftTube():

    def __init__(self):

        print("Setting up input dictionary")

        self.config = {
            # number of blades
            "nRuBlades" : 15,
            "nGvBlades" : 24,

            # meridional channel
            "d_inlet" : 2.58,
            "l_inExt" : 0.27,
            "d_outHub" : 0.4,
            "d_outShroud" : 1.865,

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
            
            # layers and interfaces
            "layer_thickness" : 0.0166,
            "layer_supports" : [0.5],
            "interface_hub" : [[1, 0.00],
                               [1, 0.90],],                # [curve, percent]
            "interface_shroud" : [[1, 0.00],
                                  [2, 0.4],],
            "interface_curvature" : [[0.0, 0.5, 1],
                                     [0.0, 0.5, -1],],

            # guide vane
            "spanwiseCuts" : [0.00, 1.00,],
            "gv_alpha_1" : [round((np.pi/180.) * -55.0, 4)],
            "gv_alpha_2" : [round((np.pi/180.) * -16.0, 4)],
            "gv_ratioX" : [0.5],
            "gv_deltaY" : [0.12],
            "gv_offX" : [-0.046],
            "gv_offY" : [0.077],
            
            "gv_t_le" : [0.01],
            "gv_u_le" : [0.00],
            "gv_t_mid" : [0.03],
            "gv_u_mid" : [0.20],
            "gv_t_te" : [0.01],
            "gv_u_te" : [0.80],
            
            # runner
            "spanwiseCuts_mp" : [0.00, 0.33,  0.66, 1.00,],
            "ru_alpha_1" : [
                    round((np.pi/180.) * 90., 4),
                    round((np.pi/180.) * 75., 4),
                    round((np.pi/180.) * 52., 4)
                ],
            ## of_newCoarse_0 :
            #"ru_alpha_2" : [
            #        round((np.pi/180.) * 45., 4), 
            #        round((np.pi/180.) * 31., 4), 
            #        round((np.pi/180.) * 32., 4), 
            #        round((np.pi/180.) * 10., 4) 
            #    ],
            # of_newCoarse_bladeAngle_0
            "ru_alpha_2" : [
                    round((np.pi/180.) * 46., 4), 
                    round((np.pi/180.) * 32., 4), 
                    round((np.pi/180.) * 33., 4), 
                    round((np.pi/180.) * 11., 4) 
                ],
            #of_newCoarse_bladeAngle_1
            #"ru_alpha_2" : [
            #        round((np.pi/180.) * 44., 4), 
            #        round((np.pi/180.) * 30., 4), 
            #        round((np.pi/180.) * 31., 4), 
            #        round((np.pi/180.) * 10., 4) 
            #    ],
            "ru_ratioX" : [
                    0.65,
                    0.70,
                    0.35,
                    0.22
                ],
            "ru_deltaY" : [
                    0.80,
                    0.55,
                    0.90,
                    0.55
                ],
            "ru_offX" : [
                    0.125,
                    0.125,
                    0.0
                ],
            "ru_offY" : [
                    0.065,
                    0.085,
                    0.035
                ],
            
            "spanwiseCuts_td" : [0.00, 1.00,],
            "ru_t_le" : [0.020,0.018],
            "ru_u_le" : [0.00],
            "ru_t_mid" : [0.04,0.03],
            "ru_u_mid" : [0.50],
            "ru_t_te" : [0.02],
            "ru_u_te" : [0.80],
        }
        
        for key, value in self.config.items():
            setattr(self, key, value)
        
        self.dx_hub0, self.dz_hub0 = self.deltas_LengthAndAngle(self.angle_hub0, self.l_hub0)
        self.dx_hub1, self.dz_hub1 = self.deltas_LengthAndAngle(self.angle_hub1, self.l_hub1)
        self.dx_shroud0, self.dz_shroud0 = self.deltas_LengthAndAngle(self.angle_shroud0, self.l_shroud0)
        self.dx_shroud1, self.dz_shroud1 = self.deltas_LengthAndAngle(self.angle_shroud1, self.l_shroud1)

        self.hubCurves = self.buildHubCurves()
        self.shroudCurves = self.buildShroudCurves()

        #self.config["hubCurves"] = hubCurves
        #self.config["shroudCurves"] = shroudCurves
        
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
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(+self.d_outHub/2, +0.00, -self.h_hub+self.h_inlet)
                  << dtOO.dtPoint3(+self.d_outHub/2, +0.00, -self.h_hub+self.h_inlet-0.22),
                1
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
                  << dtOO.dtPoint3(+self.d_outShroud/2, +0.00, -self.h_shroud-0.16),
                1
              ).result()
            ),
          ]

    def deltas_LengthAndAngle(self, angle, length):

        dx = np.cos(angle)*length
        dz = np.sin(angle)*length

        return dx, dz
    
    def getConfig(self):
        return copy.deepcopy(self.config)

    def getCurves(self):
        return self.hubCurves, self.shroudCurves

def run(*args, **kwargs):
    from dtOOPythonApp.vis import dtOOInParaVIEW

    machine = ConfigDraftTube()
    config = machine.getConfig()
    hubCurves, shroudCurves = machine.getCurves()

    importlib.reload(radMeridionalOptimization)
    cc = radMeridionalOptimization.radMeridionalOptimization(
            config, hubCurves, shroudCurves,  "newCoarse_bladeAngle", 1,  True
        ).create()
    rr = dtOOInParaVIEW( cc )
    return cc, rr

def round_sig(x, sig=4):
    if x == 0:
        return 0
    return round(x, sig - int(np.floor(np.log10(abs(x)))) - 1)

if __name__ == "__main__":

    machine = ConfigDraftTube()
    
    hubCurves, shroudCurves = machine.getCurves()
    
    stateLbl = "fittingLayers_bladeAngle0"
    
    # input values which will be optimized:
    varList = [
        "l_hub0", "l_hub1", "angle_hub1",
        "l_shroud0", "l_shroud1", 

        #"gv_alpha_1", "gv_alpha_2",
        #"gv_ratioX", "gv_deltaY",
        #"gv_offX","gv_offY",
        #"gv_t_le", "gv_t_mid", "gv_t_te",
        
        "ru_alpha_1", "ru_alpha_2",
        "ru_ratioX", "ru_deltaY",
        "ru_t_le", "ru_t_mid", "ru_t_te",
        ]
    
    variation = 0.1
    # number of iterations
    nIt = 1

    # activates optimization
    optiOn = False
    
    evalFolder = "ofCase_eval"
    with open("./"+evalFolder+"/"+stateLbl+".csv", "w", newline="") as f:
        writer = csv.writer(f)

        # header (column names)
        header = ["individual"] + varList 
        writer.writerow(header)
        
        # iterating over individuals
        for individual in range(nIt):
            
            # geting the initiated config file
            config = machine.getConfig()
            
            # first individual is made from specified values
            if individual != 0 and optiOn == True:
                
                # iterating over the parameters which are changed
                for param in varList:
                    # variation is +- of the specified value
                    var = random.uniform(-variation, variation)
                    
                    # if the varied parameter is a list, all list items are changed
                    #  with the same variation
                    if isinstance(config[param], list):
                        for i in range(len(config[param])):
                            config[param][i] = round_sig(config[param][i] + config[param][i]*var)
                        
                        #row.append(str(config[param]))

                    # non list items are varied like this
                    else:
                        config[param] = round_sig(config[param] + config[param]*var)
                        #row.append(config[param])
            
            row = [individual]
            for param in varList:
                if isinstance(config[param], list):
                    row.append(str(config[param]))
                else:
                    row.append(config[param])
            writer.writerow(row)
            
            print("Building Individual No. " + str(individual))
            #try:
            radMeridionalOptimization.radMeridionalOptimization(
                    config, hubCurves, shroudCurves, stateLbl, individual,  True
                ).create()
            print("Sucess")
            #except:
            #    print("Failed")
            #    continue

    
    """
    target_individual = 4

    with open("./"+evalFolder+"/"+stateLbl+".csv", "r", newline="") as f:
        reader = list(csv.reader(f))
        row = reader[target_individual+1]


    # skip first column (individual index)
    values = row[1:]

    for i, param in enumerate(varList):
        value = values[i]

        # detect list
        if value.startswith("["):
            config[param] = ast.literal_eval(value)
        else:
            config[param] = float(value)

    for param in config:
        print(param +" = "+ str(config[param]))
    """





