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
  vectorDtPoint3,
  vectorHandlingConstDtCurve,
  vec3dSurfaceTwoD,
  vec3dTwoDInMap3dTo3d
)
from dtOOPythonApp.builder import scaOneD_scaCurve2dOneDPointConstruct 
from typing import List
import logging

import numpy as np

class vec3dTwoDInMap3dTo3d_approximateAndFullExtendMeanplane(dtBundleBuilder):
  """Approximate and extend mean plane in m-direction.

  An in v-direction (should correspond to m-direction) constant line at each 
  entry of segVs_ is extracted from the mean plane. On this spline every entry 
  in segUs_ (u-direction should correspond spanwise direction) is 
  reparameterized in the channel. The figure shows two (symbol x and o) 
  reparameterized splines consisting each of three points ( [ (phir10, m10), 
  (phir11, m11), (phir12, m12) ] and [ (phir20, m20), (phir21, m21), 
  (phir22, m22) ], respectively ). Additionally, one intermediate point is 
  calculated between last point on the mean plane and the straight extension
  to the outlet. This intermediate point can be shifted by ratioBladeExt in
  phir-direction.

         m                                                  
         ^                                                
         |                                                
  m21/m22|...............o--------------o                 
         |              /:              :                 
  m11/m12+.......x-----/-:-------x      :                 
         |      /:    /  :       :      :                 
         |     / :   /   :       :      :                 
         |    /  :  /    :       :      :                 
      m20|.. /...:.o     :       :      :                 
      m10|..x    : :     :       :      :                 
         |  :    : :     :       :      :                 
         |  :    : :     :       :      :                 
         |--+----+-+-----+-------+------+------>phir 
            :    : phir20:       phir12 phir22            
            :    phir11  phir21                             
            phir10                                          
            <--X--->      
               |phiStartMean          
                                 <--X--->                 
                                    |phiEndMean

  If the distance in phir-direction of two following points is greater than 
  critU_ * (channel_phir_max-channel_phir_min), all points with a coordinate
  greater than critU_ are changed to negative values by subtractin 1.0. This 
  supports mean planes intersecting phir = 0.0.

  From all first (points phir10 and phir20 in figure) and last (points phir12
  and phir22 in figure) control points an average is calculated. It is shown
  in the figure as phiStartMean and phiEndMean (marker X) at the bottom. Each
  reparameterized spline is then extended to the min and max value in 
  m-direction. The ratioIn and ratioOut blending factors control the shifting 
  towards the phiMean values. A blending of 1.0 results in an extended 
  meanplane that starts at phiStartMean and ends at phiEndMean at all 
  segVs_ positions (or, put differently, in spanwise direction).

  Attributes
  ----------
  label_: str
    Label.
  channel_: map3dTo3d
    Channel for reparameterization.
  meanplane_: map2dTo3d
    Mean plane that is approximated and extended. 
  ratioIn_: scaOneD 
    Ratios of inlet extension.
  ratioOut_: scaOneD 
    Ratios of outlet extension.
  ratioBladeExt_: scaOneD 
    Ratios of blade extension.
  segUs_: List[float]
    Segment positions in u direction on meanplane.
  segVs_: List[float]
    Segment positions in v direction on meanplane.
  critU_: float
    Critical fraction of u direction.


  Examples
  --------

  >>> from rotatingMap2dTo3d_hubShroudCreate import (
  ...   rotatingMap2dTo3d_hubShroudCreate
  ... )

  >>> from dtOOPythonSWIG import (
  ...   dtBundle,
  ...   dtPoint3,
  ...   dtVector3
  ... )

  >>> container = dtBundle()
  >>> container = rotatingMap2dTo3d_hubShroudCreate(
  ...   "simple",
  ...   shroudPoints = [
  ...     dtPoint3(1.00, 0.00, 1.00),
  ...     dtPoint3(1.00, 0.00, 0.00),
  ...   ],
  ...   hubPoints = [
  ...     dtPoint3(0.50, 0.00, 1.00),
  ...     dtPoint3(0.50, 0.00, 0.00),
  ...   ],
  ...   order = 1,
  ...   rotVector = dtVector3(0, 0, 1)
  ... ).buildExtract( container )


  >>> from analyticSurface_threePointMeanplane import (
  ...   analyticSurface_threePointMeanplane,
  ...   analyticSurface_threePointMeanplaneFromRatio
  ... )
  >>> from scaOneD_scaCurve2dOneDPointConstruct import (
  ...   scaOneD_scaCurve2dOneDPointConstruct
  ... )
    
  >>> container = analyticSurface_threePointMeanplaneFromRatio(
  ...   "meanplane",
  ...   spanwiseCuts = [
  ...     0.00,  
  ...     1.00,
  ...   ],
  ...   alphaOne = scaOneD_scaCurve2dOneDPointConstruct(
  ...     [
  ...       dtPoint2(0.00, (np.pi/180.) * 80.0),  
  ...       dtPoint2(1.00, (np.pi/180.) * 70.0),
  ...     ],
  ...     1
  ...   )(),
  ...   alphaTwo = scaOneD_scaCurve2dOneDPointConstruct(
  ...     [
  ...       dtPoint2(0.00, (np.pi/180.) * 50.0),  
  ...       dtPoint2(1.00, (np.pi/180.) * 15.0),
  ...     ],
  ...     1
  ...   )(),
  ...   ratioX = scaOneD_scaCurve2dOneDPointConstruct(
  ...     [
  ...       dtPoint2(0.00, 0.50),
  ...       dtPoint2(1.00, 0.50),  
  ...     ],
  ...     1
  ...   )(),
  ...   deltaY = scaOneD_scaCurve2dOneDPointConstruct(
  ...     [
  ...       dtPoint2(0.00, 0.50),
  ...       dtPoint2(1.00, 0.50),  
  ...     ],
  ...     1
  ...   )(),
  ...   offX = scaOneD_scaCurve2dOneDPointConstruct(
  ...     [
  ...       dtPoint2(0.00, 0.2),  
  ...       dtPoint2(1.00, 0.2),
  ...     ],
  ...     1
  ...   )(),
  ...   offY = scaOneD_scaCurve2dOneDPointConstruct(
  ...     [
  ...       dtPoint2(0.00, 0.5),  
  ...       dtPoint2(1.00, 0.5),
  ...     ],
  ...     1
  ...   )(),
  ...   originOnLengthPercent = 0.5
  ... ).buildExtract( container )

  """
  def __init__( 
    self, 
    label: str, 
    channel: map3dTo3d,
    meanplane: map2dTo3d,
    ratioIn: scaOneD = scaOneD_scaCurve2dOneDPointConstruct(
      [
        dtPoint2(0.00, 0.50),
        dtPoint2(1.00, 0.50)
      ],
      1
    )(),
    ratioOut: scaOneD = scaOneD_scaCurve2dOneDPointConstruct(
      [
        dtPoint2(0.00, 0.50),
        dtPoint2(1.00, 0.50)
      ], 
      1
    )(),
    ratioBladeExt: scaOneD = scaOneD_scaCurve2dOneDPointConstruct(
      [
      dtPoint2(0.00, 0.00),
      dtPoint2(1.00, 0.00)
      ], 
      1
    )(),
    segUs: List[float] = [0.0, 0.5, 1.0],
    segVs: List[float] = [0.0, 0.5, 1.0],
    critU: float = 0.5
  ) -> None:
    """Constructor.
  
    Parameters
    ----------
    label: str
      Label.
    channel: map3dTo3d
      Channel for reparameterization.
    meanplane: map2dTo3d
      Mean plane that is approxiamted and extended. 
    ratioIn: scaOneD, default = 0.5 
      Ratios of inlet extension.
    ratioOut: scaOneD, default = 0.5
      Ratios of outlet extension.
    ratioBladeExt: scaOneD 
      Ratios of blade extension.
    segUs: List[float], default = [0.0, 0.5, 1.0]
      Segment positions in u direction on meanplane.
    segVs: List[float], default = [0.0, 0.5, 1.0]
      Segment positions in v direction on meanplane.
    critU: float, default = 0.5
      Critical fraction of u direction.

    Returns
    -------
    None
  
    """
    logging.info( "Initializing %s ..." % (label) )
    super(
      vec3dTwoDInMap3dTo3d_approximateAndFullExtendMeanplane, self
    ).__init__()
    self.label_ = label
    self.channel_ = map3dTo3d.MustDownCast( channel )
    self.meanplane_ = map2dTo3d.MustDownCast( meanplane )
    self.ratioIn_ = ratioIn
    self.ratioOut_ = ratioOut 
    self.ratioBladeExt_ = ratioBladeExt
    self.segUs_ = segUs
    self.segVs_ = segVs
    self.critU_ = critU

  @staticmethod
  def shiftCurve( pL: List[dtPoint3], critU ) -> List[dtPoint3]:
    """Build part.

    Parameters
    ----------
    pL: List[dtPoint3]
      List of points to be checked.
    critU: float
      Critical distance in first parameter direction.

    Returns
    -------
    List[dtPoint3]
      Shifted points.

    """
    xMax = -1.0E+99
    xMin = +1.0E+99
    for p in pL:
      if p.x() > xMax:
        xMax = p.x()
      if p.x() < xMin:
        xMin = p.x()

    logging.info("Curve between %f <--> %f " % (xMin, xMax))
    if (xMax - xMin)>critU:
      logging.info("Detect critical distance / critU = %f" % (critU))
    else:
      return pL

    pLTwin = []
    for p in pL:
      if p.x() > 0.5:
        pLTwin.append( dtPoint3( p.x()-1.0, p.y(), p.z() ) )
      else:
        pLTwin.append( dtPoint3( p.x(), p.y(), p.z() ) )


    for p,pTwin in zip(pL,pLTwin):
      logging.info(
        "Shift point : (%f, %f, %f) -> (%f, %f, %f)" 
        % 
        (
          p.x(), p.y(), p.z(), pTwin.x(), pTwin.y(), pTwin.z()
        )
      )
    return pLTwin


  def build(self) -> None:
    """Build part.

    Parameters
    ----------
    None

    Returns
    -------
    None

    """
    logging.info( "Building %s ..." % (self.label_) )
    cc = vectorHandlingConstDtCurve()
    for segV in self.segVs_:
      ratioIn = self.ratioIn_(segV)[0]
      ratioOut = self.ratioOut_(segV)[0]
      ratioBladeExt = self.ratioBladeExt_(segV)[0]
      logging.info(
        "segV = %f, ratioIn = %f, ratioBladeExt = %f, ratioOut = %f" 
        %
        (
          segV, ratioIn, ratioBladeExt, ratioOut
        )
      )
      seg = self.meanplane_.segmentConstVPercent(segV)
      pL = []
      for segU in self.segUs_:
        rP = self.channel_.reparamInVolume( seg.getPointPercent(segU) ) 
        logging.info(
          "seg = (%f , %f) / Coordinate ( %f , %f , %f )"
          %
          ( segU, segV, rP[0], rP[1], rP[2] )
        )
        pL.append( rP )

      # shift curve if necessary
      pL = self.shiftCurve( pL, self.critU_ )
      
      #
      # calculate blade distance in parameter space
      #
      delta_phirms = dtVector3(
        pL[-1][0] - pL[0][0],
        pL[-1][1] - pL[0][1],
        pL[-1][2] - pL[0][2]
      )

      logging.info(
        "delta_phirms : (%f %f %f)"
        %
        (
          delta_phirms.x(), delta_phirms.y(), delta_phirms.z()
        )
      )

      #
      # inset points at begining
      #
      pL.insert(
        0, 
        dtPoint3(
          pL[0].x(), 
          self.channel_.getVMin(), 
          pL[0].z()
        )
      )

      #
      # insert intermediate point
      #
      pL.append(
        dtPoint3(
          pL[-1][0] + ratioBladeExt * delta_phirms[0],
          pL[-1][1] + 0.5 * ( self.channel_.getVMax()-pL[-1][1] ), 
          pL[-1][2] + 0.5 * delta_phirms[2],
        )
      ) 

      #
      # insert points at end
      #
      pL.append(
        dtPoint3(
          pL[-1].x(),
          self.channel_.getVMax(), 
          pL[-1].z()
        )
      ) 


      #
      # create spline
      #
      pV = vectorDtPoint3()      
      for p in pL:
        pV.append( p )
      cc.append( bSplineCurve_pointConstructOCC( pV, 2 ).result() )
      logging.info(
        "[ %f ] ( %f -> %f, %f -> %f, %f -> %f )" 
        % 
        (
          segV, 
          pL[0][0], pL[-1][0],
          pL[0][1], pL[-1][1],
          pL[0][2], pL[-1][2],
        )
      )

    #
    # average input and output
    #
    phiStart = np.zeros(cc.size(), float)
    phiEnd = np.zeros(cc.size(), float)
    count = 0
    for curve in cc:
      phiStart[ count ] = curve.controlPoint( 0 ).x()
      phiEnd[ count ] = curve.controlPoint( curve.nControlPoints()-1 ).x()
      count = count + 1
    phiStartMean = np.mean( phiStart )
    phiEndMean = np.mean( phiEnd )
    logging.info("mean(phi) = [ %f, %f]" % (phiStartMean,phiEndMean))

    count = 0
    for curve in cc:
      ratioIn = self.ratioIn_(self.segVs_[count])[0]
      ratioOut = self.ratioOut_(self.segVs_[count])[0]
      curve.setControlPoint( 
        0, 
        dtPoint3(
          ratioIn * phiStartMean + curve.controlPoint( 0 ).x() * (1.-ratioIn),
          curve.controlPoint( 0 ).y(),
          curve.controlPoint( 0 ).z()
        )
      )
      curve.setControlPoint( 
        curve.nControlPoints()-1,
        dtPoint3(
          ratioOut * phiEndMean 
          + 
          curve.controlPoint( curve.nControlPoints()-1 ).x() * (1.-ratioOut),
          curve.controlPoint( curve.nControlPoints()-1 ).y(),
          curve.controlPoint( curve.nControlPoints()-1 ).z()
        )
      )
      count = count + 1

    #
    # create surface
    #
    theRef = vec3dTwoDInMap3dTo3d(
      vec3dSurfaceTwoD(
        bSplineSurface_skinConstructOCC(cc).result()
      ),
      self.channel_
    )
    theRef.setLabel(self.label_)
    self.appendAnalyticGeometry( theRef.clone() )
    return
