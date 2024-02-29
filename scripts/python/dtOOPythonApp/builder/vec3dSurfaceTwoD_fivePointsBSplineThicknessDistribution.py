from dtOOPythonApp.tools.dtBundleTools import dtBundleBuilder

from dtOOPythonSWIG import (
  dtPoint3,
  vectorDtPoint3,
  vectorHandlingConstDtCurve,
  bSplineCurve_pointConstructOCC,
  bSplineCurve_curveConnectConstructOCC,
  bSplineSurface_skinConstructOCC,
  vec3dSurfaceTwoD,
  scaOneD
)

from typing import List
import logging

class vec3dSurfaceTwoD_fivePointsBSplineThicknessDistribution(
  dtBundleBuilder
):
  """Create a thickness distribution defined in 5 points.

  The thickness disctribution is created in 5 points (x in figure), but the 
  first and last points are fixed. The inner points le, mid and te
  have the coordinates (uLe,tLe), (uMid, tMid) and (uTe, tTe), respectively.

        t
        ^
        |                   (mid)
  tMid -+      (le)         x
   tLe -+      x
        |
        |                              (te)
   tTe -+                              x
        |
        |
   0.0 -x------+------------+----------+-------x->u
        |      |            |          |       |
        0.0    uLe          uMid       uTe    1.0

  The 5 points form one B-Spline with order orderC_ and the mirrored points
  at the u-axis form a second spline. In order to create a symmetric 
  distribution the splines are then connected afterwards. Within each 
  spanwise cut an equivalent spline is created and forms a B-Spline surface
  with order = [orderMinS_, orderMaxS_]. The surface can be iterated with
  the parameter numIter_.
     
  Attributes
  ---------
  label_: str
    Label.
  spanwiseCuts_: List[float]
    Creation cuts in spanwise direction.
  tLe_: scaOneD
    Coordinate tLe.
  uLe_: scaOneD
    Coordinate uLe.
  tMid_: scaOneD
    Coordinate tMid.
  uMid_: scaOneD
    Coordinate uMid.
  tTe_: scaOneD
    Coordinate tTe.
  uTe_: scaOneD
    Coordinate uTe.
  orderC_: int, default = 2
    Splines' order within each spanwise cut.
  orderMinS_: int, default = 2
    Minimum order of resulting surface.
  orderMaxS_: int, default = 2
    Maximum order of resulting surface.
  numIter_: int, default = 0
    Number of iterations when creating the surface.

  Examples
  --------
  >>> from dtOOPythonSWIG import dtPoint2
  >>> from scaOneD_scaCurve2dOneDPointConstruct import (
  ...   scaOneD_scaCurve2dOneDPointConstruct
  ... )

  Create a thickness distribtuin by 3 spanwise cuts:

  >>> builder = vec3dSurfaceTwoD_fivePointsBSplineThicknessDistribution(
  ...   "simple",
  ...   spanwiseCuts = [
  ...     0.00,  
  ...     0.50,  
  ...     1.00,
  ...   ],
  ...   tLe = scaOneD_scaCurve2dOneDPointConstruct(
  ...     [
  ...       dtPoint2(0.00, 0.05),  
  ...       dtPoint2(0.50, 0.05),  
  ...       dtPoint2(1.00, 0.05),
  ...     ]
  ...   )(),
  ...   uLe = scaOneD_scaCurve2dOneDPointConstruct(
  ...     [
  ...       dtPoint2(0.00, 0.00),  
  ...       dtPoint2(0.50, 0.00),  
  ...       dtPoint2(1.00, 0.00),
  ...     ]
  ...   )(),
  ...   tMid = scaOneD_scaCurve2dOneDPointConstruct(
  ...     [
  ...       dtPoint2(0.00, 0.07),  
  ...       dtPoint2(0.50, 0.07),  
  ...       dtPoint2(1.00, 0.07),
  ...     ]
  ...   )(),
  ...   uMid = scaOneD_scaCurve2dOneDPointConstruct(
  ...     [
  ...       dtPoint2(0.00, 0.50),  
  ...       dtPoint2(0.50, 0.50),  
  ...       dtPoint2(1.00, 0.50),
  ...     ]
  ...   )(),
  ...   tTe = scaOneD_scaCurve2dOneDPointConstruct(
  ...     [
  ...       dtPoint2(0.00, 0.01),  
  ...       dtPoint2(0.50, 0.01),  
  ...       dtPoint2(1.00, 0.01),
  ...     ]
  ...   )(),
  ...   uTe = scaOneD_scaCurve2dOneDPointConstruct(
  ...     [
  ...       dtPoint2(0.00, 0.80),  
  ...       dtPoint2(0.50, 0.80),  
  ...       dtPoint2(1.00, 0.80),
  ...     ]
  ...   )()
  ... )

  Create thickness distribution:

  >>> builder.build()

  Check label of first analyticFunction:

  >>> builder.lVH_aF().labels()[0]
  'simple'

  Check 3 different points:

  >>> pp = builder.lVH_aF()[0](0.0,0.0)
  >>> [ pp[0], pp[1], pp[2] ]
  [0.0, 0.0, 0.0]
  >>> pp = builder.lVH_aF()[0](0.5,0.5)
  >>> [ pp[0], pp[1], pp[2] ]
  [0.04000000000000001, 0.0625, 0.5]
  >>> pp = builder.lVH_aF()[0](1.0,1.0)
  >>> [ pp[0], pp[1], pp[2] ]
  [0.060000000000000005, 0.25, 1.0]

  """
  def __init__(
    self,
    label: str,
    spanwiseCuts: List[float],
    tLe: scaOneD,
    uLe: scaOneD,
    tMid: scaOneD,
    uMid: scaOneD,
    tTe: scaOneD,
    uTe: scaOneD,
    orderC: int = 2,
    orderMinS: int = 2,
    orderMaxS: int = 2,
    numIter: int = 0
  ) -> None:
    """Constructor.

    Parameters
    ----------
    label_: str
      Label.
    spanwiseCuts_: List[float]
      Creation cuts in spanwise direction.
    tLe_: scaOneD
      Coordinate tLe.
    uLe_: scaOneD
      Coordinate uLe.
    tMid_: scaOneD
      Coordinate tMid.
    uMid_: scaOneD
      Coordinate uMid.
    tTe_: scaOneD
      Coordinate tTe.
    uTe_: scaOneD
      Coordinate uTe.
    orderC_: int, default = 2
      Splines' order within each spanwise cut.
    orderMinS_: int, default = 2
      Minimum order of resulting surface.
    orderMaxS_: int, default = 2
      Maximum order of resulting surface.
    numIter_: int, default = 0
      Number of iterations when creating the surface.

    Returns
    -------
    None
    """
    logging.info( "Initializing %s ..." % (label) )
    super(
      vec3dSurfaceTwoD_fivePointsBSplineThicknessDistribution, self
    ).__init__()

    self.label_ = label
    self.spanwiseCuts_ = spanwiseCuts
    self.tLe_ = tLe 
    self.uLe_ = uLe
    self.tMid_ = tMid
    self.uMid_ = uMid
    self.tTe_ = tTe
    self.uTe_ = uTe
    self.orderC_ = orderC
    self.orderMinS_ = orderMinS
    self.orderMaxS_ = orderMaxS
    self.numIter_ = numIter
    return
  
  def build(self) -> None:
    """Build part.
  
    Parameters
    ----------
    None
  
    Returns
    -------
    None
    """
    CC = vectorHandlingConstDtCurve()
    for (sC) in self.spanwiseCuts_:
      cc = vectorHandlingConstDtCurve()
      pp = vectorDtPoint3()
      pp.push_back( dtPoint3( 0.0,               0.0,               sC ) )
      pp.push_back( dtPoint3( self.tLe_(sC)[0],  self.uLe_(sC)[0],  sC ) )
      pp.push_back( dtPoint3( self.tMid_(sC)[0], self.uMid_(sC)[0], sC ) )
      pp.push_back( dtPoint3( self.tTe_(sC)[0],  self.uTe_(sC)[0],  sC ) )
      pp.push_back( dtPoint3( 0.0,                1.0,              sC ) )

      cc.push_back( 
        bSplineCurve_pointConstructOCC(pp, self.orderC_).result() 
      )

      pp = vectorDtPoint3()
      pp.push_back( dtPoint3( 0.0,               0.0,               sC ) )
      pp.push_back( dtPoint3(-self.tLe_(sC)[0],  self.uLe_(sC)[0],  sC ) )
      pp.push_back( dtPoint3(-self.tMid_(sC)[0], self.uMid_(sC)[0], sC ) )
      pp.push_back( dtPoint3(-self.tTe_(sC)[0],  self.uTe_(sC)[0],  sC ) )
      pp.push_back( dtPoint3( 0.0,               1.0,               sC ) )

      cc.push_back( 
        bSplineCurve_pointConstructOCC(pp, self.orderC_).result() 
      )
      
      CC.push_back( 
        bSplineCurve_curveConnectConstructOCC(cc[0], cc[1]).result() 
      )

    theRef = vec3dSurfaceTwoD(
      bSplineSurface_skinConstructOCC(
        CC,self.orderMinS_,self.orderMaxS_,self.numIter_
      ).result()
    ).clone()
    theRef.setLabel(self.label_)
    self.appendAnalyticFunction( theRef )

    return
