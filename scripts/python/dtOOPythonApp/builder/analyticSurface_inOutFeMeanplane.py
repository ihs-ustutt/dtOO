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
  labeledVectorHandlingAnalyticGeometry,
  vec3dSurfaceTwoD,
  vec3dCurveOneD,
  vec3dMuParserOneD,
  vec3dTwoDInMap3dTo3d,
  vec3dOneDInMap3dTo3d,
  dtLinearAlgebra,
  analyticGeometry,
  analyticSurface,
  analyticCurve,
  vec3dTwoD,
  vec3dOneD,
  vec3dThreeDInMap3dTo3d,
  dtOCCBSplineSurface,
  dtSurface,
  analyticCurve,
  trans4SidedFace
)
from dtOOPythonApp.builder import scaOneD_scaCurve2dOneDPointConstruct
from typing import List
import logging

import numpy as np

class analyticSurface_inOutFeMeanplane(dtBundleBuilder):


    def __init__(
        self,
        prefix: str,
        label: str,
        channel: analyticGeometry,
        curves: labeledVectorHandlingAnalyticGeometry
      ) -> None:


        logging.info( "Initializing %s ..." % (label) )
        super(
          analyticSurface_inOutFeMeanplane, self
        ).__init__()
        self.prefix_ = prefix
        self.label_ = label
        self.channel_ = map3dTo3d.MustDownCast(channel)
        self.aG_ = curves
    
    def build(self) -> None:
 
        mpCurveList = [["in", 0], ["out", 1]]
        for oc in range(len(mpCurveList)):
            
            # current offset curve
            offC = map1dTo3d.MustDownCast(
                    self.aG_["xyz_"+self.label_+"_meshBlockCurve_"+mpCurveList[oc][0]+"1"]
                )
            
            # v coordinate at outlet or inlet
            vChannel = self.channel_.v_percent(mpCurveList[oc][1])
            
            interfPoints = vectorDtPoint3()
            
            # Calculating Points and Curves at hub and shroud
            # shroud: uu = 1 and hub: uu = 0 along meshBlockCurve
            # shroud first and hub second so interfPoints is filled so the 
            #  resulting curve from it will point from shroud to hub
            for uu in [1, 0]:
                logging.info( "Meanplane extention curve at: %slet , v = %d" %(mpCurveList[oc][0], uu)  )
                
                # getting the point on the offset curve and reparamtrizing it in channel
                #  returns the uvw coordinates of the point in the channel
                pCurve_uvw = self.channel_.reparamInVolume(offC.getPointPercent(uu)) 
                
                # shifting the point on the offset curve to the current interface
                pChannel_uvw = dtPoint3(pCurve_uvw.x(), vChannel, pCurve_uvw.z())
                interfPoints.append( pChannel_uvw )
                
                # creating hub and shroud curves
                # directions have to be reversed at hub and shroud
                # at hub
                if uu == 0:
                    hsCurve = bSplineCurve_pointConstructOCC(
                            vectorDtPoint3()
                              << pChannel_uvw
                              << pCurve_uvw,
                            1
                        ).result()
                # at shroud
                elif uu == 1:
                    hsCurve = bSplineCurve_pointConstructOCC(
                            vectorDtPoint3()
                              << pCurve_uvw
                              << pChannel_uvw,
                            1
                        ).result()
                # parametrizung uvw hub/shroud curves in the xyz channel 
                hsCurveInChannel = vec3dOneDInMap3dTo3d(
                        vec3dOneD.MustConstDownCast(
                            vec3dCurveOneD(
                                hsCurve
                            ).clone()
                        ), 
                        self.channel_
                    )
                # pushing it into aG_
                # naming convention hsCurve_u0_in
                # hs -> hub or shroud
                # u0 -> u coordinate on curve 0 -> hub, 1 -> shroud
                # in / out at inlet or outlet
                self.aG_.push_back(
                        hsCurveInChannel << "hsCurve_"+"u"+str(uu)+"_"+str(mpCurveList[oc][0])
                    )
            
            # exception if the interface curve goes trough the u coordinate of 100% (or 0%)
            # checking the percentage of the u coordinate
            u1 = self.channel_.percent_u(interfPoints[0].x())
            u2 = self.channel_.percent_u(interfPoints[1].x())
            
            # if the u coordinates of the start and end points of the interface curve are
            #  far apart (blade stretches over u = 100% of the channel) 
            if abs(u1 - u2) > 0.50:
                logging.info("WARNING: Blade %s extends over 0.5 of channel u" % self.label_)
                logging.info("uShroud = %f, uHub = %f" %(u1, u2))
                # shroud point is at a greater u coordinate
                if u1 > u2:
                    logging.info("Changing shroud point to extend over u = 1 of channel")
                    # calculating the negative u coordinate
                    u1 =  u1 - 1
                    logging.info("uShroud = %f" % u1)
                    # changing the point to contain negative u coordinate
                    pChannel_uvw = dtPoint3(
                            self.channel_.u_percent(u1), 
                            interfPoints[0].y(), 
                            interfPoints[0].z()
                        )
                    interfPoints[0] = pChannel_uvw
                else:
                    logging.info("Changing hub point to extend over u = 100% of channel")
                    # other way round if hub point has the greater u value
                    u2 = u2 - 1
                    logging.info("uHub = %f" % u2)
                    pChannel_uvw = dtPoint3(
                            self.channel_.u_percent(u2), 
                            interfPoints[1].y(), 
                            interfPoints[1].z()
                        )
                    interfPoints[1] = pChannel_uvw
                
            # creating the interface curve
            interfCurve = bSplineCurve_pointConstructOCC(
                    interfPoints,
                    1
                ).result()
            
            # mapping it to the channel and pushing it into aG_
            interfCurveInChannel = vec3dOneDInMap3dTo3d(
                    vec3dOneD.MustConstDownCast(
                        vec3dCurveOneD(
                            interfCurve
                        ).clone()
                    ),
                    self.channel_
                )
            self.aG_.push_back(
                    interfCurveInChannel << "interfCurve_"+str(mpCurveList[oc][0])
                ) 
            
            # creating the meanplane face at the outlet as a trans4SidedFace
            self.aG_.set(
              trans4SidedFace(
                map1dTo3d.MustDownCast( self.aG_["hsCurve_u0_"+mpCurveList[oc][0]] ),
                map1dTo3d.MustDownCast( self.aG_["xyz_"+self.label_+"_meshBlockCurve_"+mpCurveList[oc][0]+"1"] ),
                map1dTo3d.MustDownCast( self.aG_["hsCurve_u1_"+mpCurveList[oc][0]] ),
                map1dTo3d.MustDownCast( self.aG_["interfCurve_"+mpCurveList[oc][0]] )
              ) << self.label_+"_fe_meanplane_"+mpCurveList[oc][0]+str(1)
            )
            # appending it to the geometry
            self.appendAnalyticGeometry(
                    self.aG_[self.label_+"_fe_meanplane_"+mpCurveList[oc][0]+str(1)],
                    self.prefix_+"_"+self.label_+"_fe_meanplane_"+mpCurveList[oc][0]+str(1)
                )

        #
        # add debug faces and lines
        #
        if self.debug():
            for jj in ["interfCurve_*", "hsCurve_*"]:
                for iNum in self.aG_.getIndices(jj):
                    ii = self.aG_.getLabel( iNum )
                    self.appendAnalyticGeometry(
                        self.aG_[ii],
                        "debug_"+ii
                    )

