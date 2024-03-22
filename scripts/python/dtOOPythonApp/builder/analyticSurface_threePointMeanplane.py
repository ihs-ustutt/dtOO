#------------------------------------------------------------------------------
#  dtOO < design tool Object-Oriented >
#    
#    Copyright (C) 2024 A. Tismer.
#------------------------------------------------------------------------------
#License
#    This file is part of dtOO.
#
#    dtOO is distributed in the hope that it will be useful, but WITHOUT
#    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
#    dtOO root directory for more details.
#
#    You should have received a copy of the License along with dtOO.
#
#------------------------------------------------------------------------------

from dtOOPythonApp.tools import dtBundleBuilder
from .scaOneD_scaCurve2dOneDPointConstruct import (
  scaOneD_scaCurve2dOneDPointConstruct
)

from dtOOPythonSWIG import (
  dtPoint2,
  dtVector3,
  dtVector2,
  vectorDtPoint2,
  scaOneD,
  scaCurve2dOneD,
  bSplineCurve2d_pointConstructOCC,
  bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder,
  jsonPrimitive,
  addConstCoordinate,
  translate,
  vectorHandlingConstDtCurve,
  bSplineSurface_skinConstructOCC,
  vec3dCurveOneD,
  vec3dSurfaceTwoD
)

from typing import List, Union
import logging
import numpy as np

class analyticSurface_threePointMeanplane(dtBundleBuilder):
  """Create a meanplane by skinning three point meanlines.

  Attributes
  ----------
  label_: str
    Label.
  spanwiseCuts_: List[float]
    Spanwise cuts for creation.
  alphaOne_: scaOneD
    Inlet angle versus spanwise coordinate.
  alphaTwo_: scaOneD
    Outlet angle versus spanwise coordinate.
  deltaX_: scaOneD
    Projected length in x (typically phi*r) direction.
  deltaY_: scaOneD
    Projected length in y (typically m) direction.
  offX_: scaOneD
    Offset in x direction.
  offY_: scaOneD
    Offset in y direction.
  targetLength_: scaOneD
    Target length of mean plane.
  targetLengthTolerance_: float
    Tolerance for length iteration. 
  originOnLengthPercent_: float
    Position of origin.
  skinOrderMin_: int
    Minimum order for skining.
  skinOrderMax_: int
    Maximum order for skining.
  skinNIter_: int
    Number of iterations for skining.

  Examples
  --------
  >>> from scaOneD_scaCurve2dOneDPointConstruct import (
  ...   scaOneD_scaCurve2dOneDPointConstruct
  ... )
  >>> from dtOOPythonSWIG import dtPoint2
 
  Create simple mean plane:

  >>> builder = analyticSurface_threePointMeanplane(
  ...  "simple",
  ...  [
  ...    0.00, 
  ...    0.50,
  ...    1.00,
  ...  ],
  ...  scaOneD_scaCurve2dOneDPointConstruct(
  ...    [
  ...      dtPoint2(0.00, (np.pi/180.) * 70.0),  
  ...      dtPoint2(0.50, (np.pi/180.) * 80.0),  
  ...      dtPoint2(1.00, (np.pi/180.) * 90.0),
  ...    ]
  ...  )(),
  ...  scaOneD_scaCurve2dOneDPointConstruct(
  ...    [
  ...      dtPoint2(0.00, (np.pi/180.) * 45.0),  
  ...      dtPoint2(0.50, (np.pi/180.) * 50.0),  
  ...      dtPoint2(1.00, (np.pi/180.) * 60.0),
  ...    ]
  ...  )(),
  ...  scaOneD_scaCurve2dOneDPointConstruct(
  ...    [
  ...      dtPoint2(0.00, 0.20),  
  ...      dtPoint2(0.50, 0.20),  
  ...      dtPoint2(1.00, 0.20),
  ...    ]
  ...  )(),
  ...  scaOneD_scaCurve2dOneDPointConstruct(
  ...    [
  ...      dtPoint2(0.00, 0.50),  
  ...      dtPoint2(0.50, 0.50),  
  ...      dtPoint2(1.00, 0.50),
  ...    ]
  ...  )(),
  ...  scaOneD_scaCurve2dOneDPointConstruct(
  ...    [
  ...      dtPoint2(0.00, 0.00),  
  ...      dtPoint2(0.50, 0.00),  
  ...      dtPoint2(1.00, 0.00),
  ...    ]
  ...  )(),
  ...  scaOneD_scaCurve2dOneDPointConstruct(
  ...    [
  ...      dtPoint2(0.00, 0.00),  
  ...      dtPoint2(0.50, 0.00),  
  ...      dtPoint2(1.00, 0.00),
  ...    ]
  ...  )()
  ... )

  Build mean plane:

  >>> builder.build()

  Check label of first analyticFunction:

  >>> builder.lVH_aF().labels()[0]
  'simple'
  """
  def __init__( 
    self, 
    label: str, 
    spanwiseCuts: List[float],
    alphaOne: scaOneD, 
    alphaTwo: scaOneD, 
    deltaX: scaOneD, 
    deltaY: scaOneD,
    offX: scaOneD, 
    offY: scaOneD,
    targetLength: Union[scaOneD, None] = None,
    targetLengthTolerance: float = None,
    originOnLengthPercent: float = None,
    skinOrderMin: int = 2,
    skinOrderMax: int = 2,
    skinNIter: int = 0
  ) -> None:
    """Constructor.
  
    Parameters
    ----------
    label: str
      Label.
    spanwiseCuts: List[float]
      Spanwise cuts for creation.
    alphaOne: scaOneD
      Inlet angle versus spanwise coordinate.
    alphaTwo: scaOneD
      Outlet angle versus spanwise coordinate.
    deltaX: scaOneD
      Projected length in x (typically phi*r) direction.
    deltaY: scaOneD
      Projected length in y (typically m) direction.
    offX: scaOneD
      Offset in x direction.
    offY: scaOneD
      Offset in y direction.
    targetLength: scaOneD
      Target length of mean plane.
    targetLengthTolerance: float
      Tolerance for length iteration. 
    originOnLengthPercent: float
      Position of origin.
    skinOrderMin: int
      Minimum order for skining.
    skinOrderMax: int
      Maximum order for skining.
    skinNIter: int
      Number of iterations for skining.

    Returns
    -------
    None
    """
    logging.info( "Initializing %s ..." % (label) )
    super(analyticSurface_threePointMeanplane, self).__init__()
    self.label_ = label
    self.spanwiseCuts_ = spanwiseCuts
    self.alphaOne_ = alphaOne
    self.alphaTwo_ = alphaTwo
    self.deltaX_ = deltaX
    self.deltaY_ = deltaY
    self.offX_ = offX
    self.offY_ = offY
    self.targetLength_ = targetLength
    self.targetLengthTolerance_ = targetLengthTolerance
    self.originOnLengthPercent_ = originOnLengthPercent
    self.skinOrderMin_ = skinOrderMin
    self.skinOrderMax_ = skinOrderMax
    self.skinNIter_ = skinNIter

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
    cL = vectorHandlingConstDtCurve()
    for yPos in self.spanwiseCuts_:
      jP = jsonPrimitive()\
        .appendReal( "alphaOne", self.alphaOne_(yPos)[0] )\
        .appendReal( "alphaTwo", self.alphaTwo_(yPos)[0] )\
        .appendReal( "deltaX", self.deltaX_(yPos)[0] )\
        .appendReal( "deltaY", self.deltaY_(yPos)[0] )
      if self.targetLength_ is not None:     
        jP = jP.appendReal("targetLength", self.targetLength_(yPos)[0])
        if self.targetLengthTolerance_ is not None:     
          jP = jP.appendReal(
            "targetLengthTolerance", self.targetLengthTolerance_
          )
      if self.originOnLengthPercent_ is not None:     
        jP = jP.appendReal(
          "originOnLengthPercent", self.originOnLengthPercent_
        )
      res = bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder().buildPart(
        jP
      )
      addC = addConstCoordinate(
        jsonPrimitive()\
          .appendReal("_cc", yPos)\
          .appendDtVector3("_vv", dtVector3(0,0,1))
      )
      transM = translate(
        jsonPrimitive()\
          .appendDtVector2(
            "_v2",
            dtVector2(
              self.offX_(yPos)[0],
              self.offY_(yPos)[0]
            )
          )\
          .appendDtVector3(
            "_v3",
            dtVector3(
              self.offX_(yPos)[0],
              self.offY_(yPos)[0],
              0.0
            )
          )
      )
      res = transM.applyAnalyticFunction(
        addC.applyAnalyticFunction( res[0] )
      ).clone()

      cL.push_back(
        vec3dCurveOneD.ConstDownCast(
          res
        ).ptrConstDtCurve()
      )
    res = vec3dSurfaceTwoD( 
      bSplineSurface_skinConstructOCC(
        cL, 
        self.skinOrderMin_, 
        self.skinOrderMax_, 
        self.skinNIter_
      ).result() 
    )
    res.setLabel( self.label_ )
    self.appendAnalyticFunction( res )
     
    return

class analyticSurface_threePointMeanplaneFromRatio(
  analyticSurface_threePointMeanplane
):
  """Create a meanplane by skinning three point meanlines.

  Attributes
  ----------
  None

  Examples
  --------
  >>> from scaOneD_scaCurve2dOneDPointConstruct import (
  ...   scaOneD_scaCurve2dOneDPointConstruct
  ... )
  >>> from dtOOPythonSWIG import dtPoint2
 
  Create simple mean plane:

  >>> builder = analyticSurface_threePointMeanplaneFromRatio(
  ...  "simple",
  ...  [
  ...    0.00, 
  ...    0.50,
  ...    1.00,
  ...  ],
  ...  scaOneD_scaCurve2dOneDPointConstruct(
  ...    [
  ...      dtPoint2(0.00, (np.pi/180.) * 70.0),  
  ...      dtPoint2(0.50, (np.pi/180.) * 80.0),  
  ...      dtPoint2(1.00, (np.pi/180.) * 90.0),
  ...    ]
  ...  )(),
  ...  scaOneD_scaCurve2dOneDPointConstruct(
  ...    [
  ...      dtPoint2(0.00, (np.pi/180.) * 45.0),  
  ...      dtPoint2(0.50, (np.pi/180.) * 50.0),  
  ...      dtPoint2(1.00, (np.pi/180.) * 60.0),
  ...    ]
  ...  )(),
  ...  scaOneD_scaCurve2dOneDPointConstruct(
  ...    [
  ...      dtPoint2(0.00, 0.20),  
  ...      dtPoint2(0.50, 0.20),  
  ...      dtPoint2(1.00, 0.20),
  ...    ]
  ...  )(),
  ...  scaOneD_scaCurve2dOneDPointConstruct(
  ...    [
  ...      dtPoint2(0.00, 0.50),  
  ...      dtPoint2(0.50, 0.50),  
  ...      dtPoint2(1.00, 0.50),
  ...    ]
  ...  )(),
  ...  scaOneD_scaCurve2dOneDPointConstruct(
  ...    [
  ...      dtPoint2(0.00, 0.00),  
  ...      dtPoint2(0.50, 0.00),  
  ...      dtPoint2(1.00, 0.00),
  ...    ]
  ...  )(),
  ...  scaOneD_scaCurve2dOneDPointConstruct(
  ...    [
  ...      dtPoint2(0.00, 0.00),  
  ...      dtPoint2(0.50, 0.00),  
  ...      dtPoint2(1.00, 0.00),
  ...    ]
  ...  )()
  ... )

  Build mean plane:

  >>> builder.build()

  Check label of first analyticFunction:

  >>> builder.lVH_aF().labels()[0]
  'simple'
  """

  def __init__( 
    self, 
    label: str, 
    spanwiseCuts: List[float],
    alphaOne: scaOneD, 
    alphaTwo: scaOneD, 
    ratioX: scaOneD, 
    deltaY: scaOneD,
    offX: scaOneD, 
    offY: scaOneD,
    targetLength: Union[scaOneD, None] = None,
    targetLengthTolerance: float = None,
    originOnLengthPercent: float = None,
    skinOrderMin: int = 2,
    skinOrderMax: int = 2,
    skinNIter: int = 0
  ) -> None:
    """Constructor.
 
    Instead of deltaX the parameter ratioX is given. Within the constructor
    the corresponding deltaX is calculated and a piecewise linear function
    is created. The function contains a point in each spanwise cut.

    Parameters
    ----------
    label: str
      Label.
    spanwiseCuts: List[float]
      Spanwise cuts for creation.
    alphaOne: scaOneD
      Inlet angle versus spanwise coordinate.
    alphaTwo: scaOneD
      Outlet angle versus spanwise coordinate.
    deltaX: scaOneD
      Projected length in x (typically phi*r) direction.
    deltaY: scaOneD
      Projected length in y (typically m) direction.
    offX: scaOneD
      Offset in x direction.
    offY: scaOneD
      Offset in y direction.
    targetLength_: scaOneD
      Target length of mean plane.
    targetLengthTolerance: float
      Tolerance for length iteration. 
    originOnLengthPercent: float
      Position of origin.
    skinOrderMin: int
      Minimum order for skining.
    skinOrderMax: int
      Maximum order for skining.
    skinNIter: int
      Number of iterations for skining.

    Returns
    -------
    None
    """
   
    logging.info( "Initializing %s ..." % (label) )
    deltaXPoints = list()
    for spanwiseCut in spanwiseCuts:
      deltaXPoints.append(
        dtPoint2(
          spanwiseCut,
          ( 1.0 - ratioX( spanwiseCut )[0] )
          *
          (
            ( deltaY( spanwiseCut )[0] )
            /
            np.tan( 
              np.max( 
                [ alphaOne( spanwiseCut )[0] , alphaTwo( spanwiseCut )[0]  ] 
              ) 
            )
          )
          +
          ratioX( spanwiseCut )[0] 
          *
          (
            ( deltaY( spanwiseCut )[0] )
            /
            np.tan( 
              np.min( 
                [ alphaOne( spanwiseCut )[0] , alphaTwo( spanwiseCut )[0]  ] 
              ) 
            )
          )
        )
      )
    super(analyticSurface_threePointMeanplaneFromRatio, self).__init__(
      label, 
      spanwiseCuts,
      alphaOne, 
      alphaTwo, 
      scaOneD_scaCurve2dOneDPointConstruct(deltaXPoints,1)(), 
      deltaY,
      offX, 
      offY,
      targetLength,
      targetLengthTolerance,
      originOnLengthPercent
    )

