import numpy as np
import dtOOPythonSWIG as dtOO
import foamlib as fl
import sys
import subprocess
import time
#from utils import * 
import os
import shutil
import traceback
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

import pyDtOO as pd


import numpy as np
import copy
import radMeridional
import ofCase

class Config():
    """Configure the case variables 

    Attributes
    ----------
    x: List
      List of optimization parameters.
    h_inlet: float
      Inlet height.
    hubCurves: List[analyticCurve]
      List of hub curves.
    shroudCurves: List[analyticCurve]
      List of shroud curves.
    ConfigMerdional: dict
      Configuration dictionary for interfaces.
    ConfigRunner: dict
      Configuration dictionary for runner geometries.
    ConfigGuideVane: dict
      Configuration dictionary for guide vane geometries.

    Examples
    --------

    Parametrizes the hub and shroud contours and creates curve lists.
    Creates the configuration dictionaries as input for the geometry generation.
    Through the methods :meth:`getConfig` and :meth:`getCurves` the Configuration
    Dictionaries as well as the inlet height ``h_inlet`` and the hub and shroud 
    curves can be returned.

    The optimization parameters can be handed to this class through the ``List``
    ``x``.
    
    """
    def __init__(self, x):

        print("Setting up input dictionary")
        
        #
        # Parameters defining the hub and shroud curves
        #
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

        #self.h_inlet = 0.18
        self.h_inlet = 0.36
        h_hub = 0.68
        h_shroud = 0.38

        dx_hub1 = np.cos(angle_hub1)*l_hub1
        dz_hub1 = np.sin(angle_hub1)*l_hub1
        
        # hub curves
        self.hubCurves = [
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(+(d_inlet/2 + l_inExt), +0.00, +self.h_inlet)
                  << dtOO.dtPoint3(+d_inlet/2, +0.00, +self.h_inlet),
                1
              ).result()
            ),
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(+d_inlet/2, +0.00, +self.h_inlet)
                  << dtOO.dtPoint3(+d_inlet/2-l_hub0, +0.00, +self.h_inlet)
                  << dtOO.dtPoint3(+d_outHub/2+dx_hub1, +0.00, -h_hub+dz_hub1+self.h_inlet)
                  << dtOO.dtPoint3(+d_outHub/2, +0.00, -h_hub+self.h_inlet),
                2
              ).result()
            ),
          ]
        
        # shroud curves
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
            #"alpha_1" : [
            #         round((np.pi/180.) * 90., 4),
            #         round((np.pi/180.) * 75., 4),
            #         round((np.pi/180.) * 52., 4)
            #     ],
            #"alpha_2" : [
            #         round((np.pi/180.) * 45., 4),
            #         round((np.pi/180.) * 27., 4),
            #         round((np.pi/180.) * 25., 4),
            #         round((np.pi/180.) * 10., 4)
            #     ],
            #"ratioX" : [
            #         0.65,
            #         0.65,
            #         0.15,
            #         0.24
            #     ],
            #"deltaY" : [
            #         0.80,
            #         0.75,
            #         0.75,
            #         0.53
            #     ],
            "alpha_1" : [
                     round((np.pi/180.) * x[0], 4),
                     round((np.pi/180.) * x[1], 4),
                     round((np.pi/180.) * x[2], 4)
                 ],
            "alpha_2" : [
                     round((np.pi/180.) * x[3], 4),
                     round((np.pi/180.) * x[4], 4),
                     round((np.pi/180.) * x[5], 4),
                     round((np.pi/180.) * x[6], 4)
                 ],
            "ratioX" : [
                     x[7],
                     x[8],
                     x[9],
                     x[10]
                 ],
            "deltaY" : [
                     x[11],
                     x[12],
                     x[13],
                     x[14]
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
    
    def getConfig(self):
        return self.configMeridional, self.configGuideVane, self.configRunner, self.h_inlet

    def getCurves(self):
        return self.hubCurves, self.shroudCurves

    
def runRadMeridional(x, state, solver_launcher=None):
    history = {}
    try:
        #
        # Geometry Generation
        #
        # Configure the geometry parameters
        geomConf = Config(x)
        configM, configGV, configRu, h_inlet = geomConf.getConfig()
        hubCurves, shroudCurves = geomConf.getCurves()
        
        # Build the geometry model
        model = radMeridional.radMeridional(state)
        state = model.state_
        model.createMeridional(configM, hubCurves, shroudCurves)
        model.createBlade(configGV)
        model.createBlade(configRu)
        cc = model.getContainer()
        
        #
        # Simulation
        #
        sim = ofCase.ofCase(state)    
        # Create the open foam case
        start_time = time.time()
        sim.createOFCase(cc, h_inlet)
        end_time = time.time()
        logging.info(f"Proxy: finished meshing for state {state}. Time: {end_time - start_time}. Start CFD...")
        # Run the simulation
        sim.simulate(solver_launcher)
        # Evaluation
        fit, dh_ru, M_z, eta = sim.evaluate()
        fit_extra = {'dh_ru':dh_ru.tolist(), 'M_z': float(M_z), 'eta': eta.tolist()}
        
        logging.info(f"STATE: {state}. SUCCESSFULLY.")
        #sim.delete_case()

    except Exception as e:
        logging.warning("Proxy: Exception in radMeridional: \n")
        logging.exception(e)
        #fit = ofCase.ofCase.failedFitness()
        #fit_extra = {'dh_ru': 1e6, 'M_z': 1e6, 'eta': 1e6}
        raise
    history = sim.get_history() if "sim" in locals() else history
    if type(fit) == np.ndarray:
        fit = float(fit[0]) #serializable
    return fit, fit_extra, state, history

if __name__ == "__main__":

    state = "testState"
    x = [
            90.,
            75.,
            52.,
            
            45.,
            27.,
            25.,
            10.,
            
            0.65,
            0.65,
            0.15,
            0.24,
            
            0.80,
            0.75,
            0.75,
            0.53,
        ]

    fit, fit_extra, state, history = runRadMeridional(x, state)

    print("fit :")
    print(fit)
    print("fit_extra :")
    print(fit_extra)

    # ------------------- EOF ------------------- #
