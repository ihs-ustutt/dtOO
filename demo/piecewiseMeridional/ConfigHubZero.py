import numpy as np
import importlib
import radMeridional
import dtOOPythonSWIG as dtOO
from dtOOPythonApp.vis import dtOOInParaVIEW

class ConfigHubZero():

    def __init__(self):

        print("Setting up input dictionary")

        self.config = {
            "nRuBlades" : 15,
            "nGvBlades" : 24,
            "d_inlet" : 2.58,
            "l_inExt" : 0.27,
            "d_outHub" : 0.4,
            "d_outShroud" : 1.865,

            "l_hub0" : 0.38,
            "angle_hub0" : 0 * np.pi/180,
            "l_hub1" : 0.38,
            "angle_hub1" : 65 * np.pi/180,

            "l_shroud0" : 0.13,
            "angle_shroud0" : 0 * np.pi/180,
            "l_shroud1" : 0.18,
            "angle_shroud1" : 90 * np.pi/180,

            "h_inlet" : 0.36,
            "h_hub" : 0.68,
            "h_shroud" : 0.38,
            
            "label" : "radMeridionalContour",
            "layer_thickness" : 0.2,
            "layer_supports" : [0.33, 0.66],
            "interface_hub" : [[1, 0.00],
                               [1, 0.7],],                # [curve, percent]
            "interface_shroud" : [[1, 0.00],
                                  [2, 0.5],],
            "interface_curvature" : [[0.0, 0.5, 1],
                                     [0.4, 0.5, -1],],
        }
        for key, value in self.config.items():
            setattr(self, key, value)
        
        self.dx_hub0, self.dz_hub0 = self.deltas_LengthAndAngle(self.angle_hub0, self.l_hub0)
        self.dx_hub1, self.dz_hub1 = self.deltas_LengthAndAngle(self.angle_hub1, self.l_hub1)
        self.dx_shroud0, self.dz_shroud0 = self.deltas_LengthAndAngle(self.angle_shroud0, self.l_shroud0)
        self.dx_shroud1, self.dz_shroud1 = self.deltas_LengthAndAngle(self.angle_shroud1, self.l_shroud1)

        hubCurves = self.buildHubCurves()
        shroudCurves = self.buildShroudCurves()

        self.config["hubCurves"] = hubCurves
        self.config["shroudCurves"] = shroudCurves
        
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
                  << dtOO.dtPoint3(+0.00, +0.00, -self.h_hub+self.h_inlet),
                1
              ).result()
            ),
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(+0.00, +0.00, -self.h_hub+self.h_inlet)
                  << dtOO.dtPoint3(+0.00, +0.00, -2.55),
                1
              ).result()
            )
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
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(+self.d_outShroud/2, +0.00, -0.54)
                  << dtOO.dtPoint3(+1.15, +0.00, -2.55),
                1
              ).result()
            )
          ]

    def deltas_LengthAndAngle(self, angle, length):

        dx = np.cos(angle)*length
        dz = np.sin(angle)*length

        return dx, dz

    def run(self):
        importlib.reload(radMeridional)
        cc = radMeridional.radMeridional(self.config).create()
        rr = dtOOInParaVIEW( cc )
        return cc, rr


