import numpy as np
import importlib
import radMeridional
import dtOOPythonSWIG as dtOO
from dtOOPythonApp.vis import dtOOInParaVIEW

class ConfigSucExtend():

    def __init__(self):

        print("")
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
            "layer_supports" : [0.5],
            "interface_hub" : [[1, 0.00],
                               [1, 0.7],],                # [curve, percent]
            "interface_shroud" : [[1, 0.00],
                                  [2, 0.5],],
            "interface_curvature" : [[0.0, 0.5, 1],
                                     [0.4, 0.5, -1],],
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
            "ru_alpha_2" : [
                    round((np.pi/180.) * 45., 4),
                    round((np.pi/180.) * 31., 4),
                    round((np.pi/180.) * 32., 4),
                    round((np.pi/180.) * 10., 4)
                ],
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
                  << dtOO.dtPoint3(+self.d_outHub/2, +0.00, -1.54),
                1
              ).result()
            ),
            dtOO.analyticCurve(
              dtOO.bSplineCurve_pointConstructOCC(
                dtOO.vectorDtPoint3()
                  << dtOO.dtPoint3(+self.d_outHub/2, +0.00, -1.54)
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
                  << dtOO.dtPoint3(+1.15, +0.00, -3.17),
                1
              ).result()
            )
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

    machine = ConfigSucExtend()
    config = machine.getConfig()
    hubCurves, shroudCurves = machine.getCurves()

    importlib.reload(radMeridional)
    cc = radMeridional.radMeridional(config, hubCurves, shroudCurves).create()
    rr = dtOOInParaVIEW( cc )
    return cc, rr


if __name__ == "__main__":
    machine = ConfigSucExtend()
    config = machine.getConfig()
    hubCurves, shroudCurves = machine.getCurves()

    radMeridional.radMeridional(
                config, hubCurves, shroudCurves, "SucExtend", 0,  True
            ).create()



