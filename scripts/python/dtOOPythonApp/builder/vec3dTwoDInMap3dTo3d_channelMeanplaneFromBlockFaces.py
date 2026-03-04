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
  bSplineSurface_bSplineCurveFillConstructOCC,
  vectorDtPoint3,
  vectorHandlingConstDtCurve,
  vec3dSurfaceTwoD,
  vec3dCurveOneD,
  vec3dTwoDInMap3dTo3d,
  vec3dOneDInMap3dTo3d,
  dtLinearAlgebra,
  analyticSurface,
  analyticCurve,
  vec3dTwoD
)
from dtOOPythonApp.builder import scaOneD_scaCurve2dOneDPointConstruct
from typing import List
import logging

import numpy as np

class vec3dTwoDInMap3dTo3d_channelMeanplaneFromBlockFaces(dtBundleBuilder):


    def __init__(
        self,
        label: str,
        channel: map3dTo3d,
        mpFaces: List[map2dTo3d]
      ) -> None:


        logging.info( "Initializing %s ..." % (label) )
        super(
          vec3dTwoDInMap3dTo3d_channelMeanplaneFromBlockFaces, self
        ).__init__()
        self.label_ = label
        self.channel_ = map3dTo3d.MustDownCast( channel )
        self.mpFaces_ = mpFaces

    
    def build(self) -> None:
        
        self.blockExtention(self.mpFaces_[0], 0)
        #self.blockExtention(self.mpBlocks_[0], 0)

        for i, face in enumerate(self.mpFaces_):
            self.appendAnalyticGeometry(
                    face,
                    "TEST_"+self.label_+"_meanplaneFace_"+ str(i)
                )


    def blockExtention(self, blockSurf, uu):
        
        f = 1
        off = 0.5 
        
        print("Classes for surface segments:") 
        print(blockSurf.virtualClassName())

        print( vec3dTwoDInMap3dTo3d.MustDownCast( blockSurf ).refToVec3dTwoD().virtualClassName() )
        blockBiLin = vec3dTwoD.MustDownCast( vec3dTwoDInMap3dTo3d.MustDownCast( blockSurf ).refToVec3dTwoD() )
        #p0 = blockBiLin.YdtPoint3Percent(uu,0.0)
        #p1 = blockBiLin.YdtPoint3Percent(uu,1.0)
        #print( "%f, %f, %f" % (p0.x(), p0.y(), p0.z()) )
        #print( "%f, %f, %f" % (p1.x(), p1.y(), p1.z()) )
        
        
        blockPoints = vectorDtPoint3()
        offsetPoints = vectorDtPoint3()
        outletPoints = vectorDtPoint3()

        supports = [0.00,  1.00]
        for vv in supports:
            print("")
            print("vv = ",vv)
            blockPoint = blockBiLin.YdtPoint3Percent(uu,vv)
            blockPoints.append(blockPoint)
            
            print("blockPoint[0] = ", blockPoint[0])
            print("blockPoint[1] = ", blockPoint[1])
            print("blockPoint[2] = ", blockPoint[2])

            tangentVecs = blockBiLin.DYdtVector3(blockBiLin.x_percent(uu, vv))
            
            for vec in tangentVecs:
                print("")
                print( "%f, %f, %f" % (vec.x(), vec.y(), vec.z()) )

            meanTangent = dtLinearAlgebra.normalize(tangentVecs[0])
             
            offsetPoint = dtPoint3(
                    (blockPoint[0] + meanTangent[0]*f*off),
                    (blockPoint[1] + meanTangent[1]*f*off),
                    (blockPoint[2] + meanTangent[2]*f*off)
                )
            offsetPoint = self.channel_.reparamInVolume(offsetPoint)
            offsetPoints.append(offsetPoint)

            print("offsetPoint[0] = ", offsetPoint[0])
            print("offsetPoint[1] = ", offsetPoint[1])
            print("offsetPoint[2] = ", offsetPoint[2])
            tangentCurve = bSplineCurve_pointConstructOCC(
                vectorDtPoint3()
                  << blockPoint
                  << offsetPoint,
                1
            ).result()
            tangentCurve = vec3dOneDInMap3dTo3d(
                vec3dCurveOneD(
                    tangentCurve
                ),
                vec3dTwoDInMap3dTo3d.MustDownCast( blockSurf ).refToMap3dTo3d()
            )
            self.appendAnalyticGeometry(
                tangentCurve,
                "TEST_"+self.label_+"_tangentCurve"
            )




        # plotting resulting curves
        blockCurve = bSplineCurve_pointConstructOCC(blockPoints,1).result() 
        blockCurve = vec3dOneDInMap3dTo3d(
            vec3dCurveOneD(
                blockCurve
            ),
            vec3dTwoDInMap3dTo3d.MustDownCast( blockSurf ).refToMap3dTo3d()            
        )
        self.appendAnalyticGeometry(
            blockCurve,
            "TEST_"+self.label_+"_blockCurve"
        )
        
        offsetCurve = bSplineCurve_pointConstructOCC(offsetPoints,1).result() 
        offsetCurve = vec3dOneDInMap3dTo3d(
            vec3dCurveOneD(
                offsetCurve
            ),
            vec3dTwoDInMap3dTo3d.MustDownCast( blockSurf ).refToMap3dTo3d()
        )
        self.appendAnalyticGeometry(
            offsetCurve,
            "TEST_"+self.label_+"_offsetCurve"
        )

        """
        blockOffsetPoints = vectorDtPoint3()
        channelBlockPoints = vectorDtPoint3()
        channelOutletPoints  = vectorDtPoint3()
        supports = [0.00, 0.5,  1.00]
        for vv in supports:
            
            segV = blockSurf.segmentConstVPercent(vv)
            
            
            blockPoint = segV.getPoint(
                segV.u_percent(uPercent)
            )
            blockTangent = dtLinearAlgebra.normalize(
                segV.firstDerUPercent(uPercent)
            )
            
            print("")
            print("blockPoint[0] = ", blockPoint[0])
            print("blockPoint[1] = ", blockPoint[1])
            print("blockPoint[2] = ", blockPoint[2])

            
            rP = self.channel_.reparamInVolume( blockPoint )
            logging.info(
                "Coordinate ( %f , %f , %f )"
                %
                ( rP[0], rP[1], rP[2] )
            )
            channelBlockPoints.append(rP)
            
            blockOffset = dtPoint3(
                    (blockPoint[0] + blockTangent[0]*f*off),
                    (blockPoint[1] + blockTangent[1]*f*off),
                    (blockPoint[2] + blockTangent[2]*f*off)
                )
            blockOffset = self.channel_.reparamInVolume(blockOffset)

            #blockOffset = dtPoint3(
            #        (rP[0] + blockTangent[0]*f*off),
            #        (rP[1] + blockTangent[1]*f*off),
            #        (rP[2] + blockTangent[2]*f*off)
            #    )
            
            blockOffsetLine = bSplineCurve_pointConstructOCC(
                    vectorDtPoint3()
                      << rP 
                      << blockOffset 
                      ,1
                ).result()
            blockOffsetLine = vec3dOneDInMap3dTo3d(
              vec3dCurveOneD(
                blockOffsetLine
              ),
              self.channel_
            )
            self.appendAnalyticGeometry(
                blockOffsetLine,
                "TEST_"+self.label_+"_tangent_vv"+str(vv)
            )
            
            blockOffsetPoints.append(
                   blockOffset
                )
 

            outletPoint = dtPoint3(
                        blockOffset[0], self.channel_.getVMax(), blockOffset[2]
                    )
            channelOutletPoints.append(outletPoint)
            #delta = np.abs(self.channel_.getVMax() - rP[1])
            #print("dist = ", dist)
        
        
        hubCurve = bSplineCurve_pointConstructOCC(
            vectorDtPoint3()
                << channelOutletPoints[0]
                << blockOffsetPoints[0]
                << channelBlockPoints[0],
                2
            ).result()
        shroudCurve = bSplineCurve_pointConstructOCC(
            vectorDtPoint3()
                << channelOutletPoints[-1]
                << blockOffsetPoints[-1]
                << channelBlockPoints[-1],
                2
            ).result()

        outletCurve = bSplineCurve_pointConstructOCC(channelOutletPoints,2).result()
        intermediateCurve = bSplineCurve_pointConstructOCC(blockOffsetPoints,2).result()
        blockCurve = bSplineCurve_pointConstructOCC(channelBlockPoints,2).result()
        
        outletSurf = bSplineSurface_bSplineCurveFillConstructOCC(
                hubCurve,
                blockCurve,
                shroudCurve,
                outletCurve
            ).result()

        outletSurf = vec3dTwoDInMap3dTo3d(
          vec3dSurfaceTwoD(
            outletSurf
          ),
          self.channel_
        )

        self.appendAnalyticGeometry(
                outletSurf,
                "TEST_"+self.label_+"_otherOutletSurf"
            )


        cc = vectorHandlingConstDtCurve()
        cc.append(outletCurve)
        cc.append(intermediateCurve)
        cc.append(blockCurve)
        
        i = 0
        for c in cc:
            c = vec3dOneDInMap3dTo3d(
              vec3dCurveOneD(
                c
              ),
              self.channel_
            )
            self.appendAnalyticGeometry(
                c,
                "TEST_"+self.label_+"_extentCurve_"+str(i)
            )
            i = i+1

        theRef = vec3dTwoDInMap3dTo3d(
          vec3dSurfaceTwoD(
            bSplineSurface_skinConstructOCC(
                cc
            ).result()
          ),
          self.channel_
        )

        
        self.appendAnalyticGeometry(
                theRef,
                "TEST_"+self.label_+"_outletSurf"
                )
        """        
