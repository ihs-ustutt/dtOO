from dtOOPythonApp.tools.dtBundleTools import dtBundleBuilder

from dtOOPythonSWIG import (
  map2dTo3d,
  map3dTo3d,
  scaOneD,
  map1dTo3d,
  dtPoint3 ,
  dtPoint2,
  dtVector3,
  bSplineCurve_pointConstructOCC,
  bSplineSurface_skinConstructOCC,
  bSplineSurface_exchangeSurfaceConstructOCC,
  bSplineSurface_bSplineCurveFillConstructOCC,
  vectorDtPoint3,
  vectorHandlingConstDtCurve,
  labeledVectorHandlingAnalyticFunction,
  vec3dSurfaceTwoD,
  vec3dCurveOneD,
  vec3dTwoDInMap3dTo3d,
  vec3dOneDInMap3dTo3d,
  dtLinearAlgebra,
  analyticGeometry,
  analyticSurface,
  analyticCurve,
  vec3dTwoD,
  vec3dThreeDInMap3dTo3d,
  dtOCCBSplineSurface,
  dtSurface,
  analyticCurve
)
from dtOOPythonApp.builder import scaOneD_scaCurve2dOneDPointConstruct
from typing import List
import logging

import numpy as np

class vec3dSurfaceTwoD_inOutFeMeanplane(dtBundleBuilder):


    def __init__(
        self,
        label: str,
        channel: analyticGeometry,
        curves: labeledVectorHandlingAnalyticFunction
      ) -> None:


        logging.info( "Initializing %s ..." % (label) )
        super(
          vec3dSurfaceTwoD_inOutFeMeanplane, self
        ).__init__()
        self.label_ = label
        self.channel_ = channel
        self.aF_ = curves
    
    def build(self) -> None:

        channel = map3dTo3d.MustDownCast(self.channel_) 
        channelVW = channel.segmentConstUPercent(0)

        mpCurveList = [["in", 0], ["out", 1]]
        for oc in range(len(mpCurveList)):
            
            offC = vec3dCurveOneD.MustDownCast(
                    self.aF_[self.label_+"_meshBlockCurve_"+mpCurveList[oc][0]+"1"]
                )
        
            interfacePoints = vectorDtPoint3()
            
            for uu in [0.00, 0.33, 0.66, 1.00]:
            #n = 4
            #for i in range(n):
         
                #uu = i/(n-1)
                meanLineLength = channelVW.segmentConstVPercent(uu).length()
                print(uu, ", ", meanLineLength)
                vInterf = np.floor(meanLineLength * 1e5) / 1e5 * mpCurveList[oc][1]
         
                p1 = offC.YdtPoint3Percent(uu)
           
                interfacePoints.append(
                    dtPoint3(
                        p1.x(),
                        vInterf,
                        p1.z()
                    )
                )
            
            interfaceCurve = vec3dCurveOneD(
                    bSplineCurve_pointConstructOCC(interfacePoints,2).result()
                ) << self.label_+"_meshBlockCurve_"+mpCurveList[oc][0]+"2"
            
            self.appendAnalyticFunction(interfaceCurve)
            self.aF_.push_back(interfaceCurve)

            #vhc = vectorHandlingConstDtCurve()
            r = 2
            for i in range(r):
                if mpCurveList[oc][0] == "in":
                    c0 = i
                    c1 = i+1
                    s = i
                elif mpCurveList[oc][0] == "out":
                    c0 = r-i
                    c1 = r-1-i
                    s = r-1-i
                
                surf = vec3dSurfaceTwoD(
                    bSplineSurface_exchangeSurfaceConstructOCC(
                        bSplineSurface_skinConstructOCC(
                            vec3dCurveOneD.MustDownCast(
                                self.aF_[self.label_+"_meshBlockCurve_"+mpCurveList[oc][0]+str(c0)]
                            ).ptrConstDtCurve(),
                            vec3dCurveOneD.MustDownCast(
                                self.aF_[self.label_+"_meshBlockCurve_"+mpCurveList[oc][0]+str(c1)]
                            ).ptrConstDtCurve()
                        ).result()
                    ).result()
                )

                self.appendAnalyticFunction(
                        surf << self.label_+"_fe_meanplane_"+mpCurveList[oc][0]+str(s)
                    )
                

