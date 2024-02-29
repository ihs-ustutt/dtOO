from dtOOPythonApp.tools.dtBundleTools import dtBundleBuilder

from dtOOPythonSWIG import analyticFunction
from dtOOPythonSWIG import vectorHandlingConstAnalyticFunction
from dtOOPythonSWIG import vec3dTransVolThreeD_skinBSplineSurfaces
from dtOOPythonSWIG import vec3dSurfaceTwoD
from dtOOPythonSWIG import bSplineSurface_bSplineSurfaceSplitConstructOCC

from typing import List
import logging

class vec3dThreeD_skinAndSplit(dtBundleBuilder):
  """Create vec3dThreeD function by skining two BSpline surfaces with split.

  Attributes
  ----------
  label_: str
    Label.
  aFOne_: vec3dSurfaceTwoD
    First BSpline surface.
  aFTwo_: vec3dSurfaceTwoD
    Second BSpline surface.
  splitDim_: int
    Dimension where surface is splitted before skinned.
  splits_: List[List[float]]
    Positions for splitting.

  Examples
  --------
  
  >>> from dtOOPythonSWIG import dtPoint3
  >>> from dtOOPythonSWIG import bSplineCurve_pointConstructOCC
  >>> from dtOOPythonSWIG import bSplineSurface_skinConstructOCC
  >>> from dtOOPythonSWIG import vec3dSurfaceTwoD

  Create first analyticFunction:

  >>> aFOne = vec3dSurfaceTwoD( 
  ...   bSplineSurface_skinConstructOCC(
  ...     bSplineCurve_pointConstructOCC(
  ...       dtPoint3(0,0,0), dtPoint3(1,0,0)
  ...     ).result(),
  ...     bSplineCurve_pointConstructOCC(
  ...       dtPoint3(0,1,0), dtPoint3(1,1,0)
  ...     ).result()
  ...   ).result()
  ... )

  Create second analyticFunction:

  >>> aFTwo = vec3dSurfaceTwoD( 
  ...   bSplineSurface_skinConstructOCC(
  ...     bSplineCurve_pointConstructOCC(
  ...       dtPoint3(0,0,1), dtPoint3(1,0,1)
  ...     ).result(),
  ...     bSplineCurve_pointConstructOCC(
  ...       dtPoint3(0,1,1), dtPoint3(1,1,1)
  ...     ).result()
  ...   ).result()
  ... )

  Initialize builder:

  >>> builder = vec3dThreeD_skinAndSplit("unitCube", aFOne, aFTwo)

  Build volume:

  >>> builder.build()

  Check label of first analyticFunction:

  >>> builder.lVH_aF().labels()[0]
  'unitCube'

  """
  def __init__( 
    self, 
    label: str, 
    aFOne: analyticFunction,
    aFTwo: analyticFunction,
    splitDim: int = 0,
    splits: List[List[float]] = [[]]
  ) -> None:
    """Constructor.
  
    Parameters
    ----------
    label: str
      Label.
    aFOne: analyticFunction
      First BSpline surface.
    aFTwo_: analyticFunction
      Second BSpline surface.
    splitDim: int
      Dimension where surface is splitted before it is skinned.
    splits: List[List[float]]
      Positions for splitting.
  
    Returns
    -------
    None
  
    """
    logging.info( "Initializing %s ..." % (label) )
    super(vec3dThreeD_skinAndSplit, self).__init__()
    self.label_ = label
    self.aFOne_ = vec3dSurfaceTwoD.MustDownCast( aFOne )
    self.aFTwo_ = vec3dSurfaceTwoD.MustDownCast( aFTwo )
    self.splitDim_ = splitDim
    self.splits_ = splits
    # adjust splits to corresponding parameter space
    if splits != [[]]:
      for split in self.splits_:
        if self.splitDim_ == 0:
          split[0] = self.aFOne_.x_percent( split[0], 0.0 )[0]
          split[1] = self.aFOne_.x_percent( split[1], 0.0 )[0]
        elif self.splitDim_ == 1:
          split[0] = self.aFOne_.x_percent( 0.0, split[0] )[1]
          split[1] = self.aFOne_.x_percent( 0.0, split[1] )[1]

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

    if self.splits_ == [[]]: 
      vh_aF = vectorHandlingConstAnalyticFunction()
      vh_aF.push_back( self.aFOne_ )
      vh_aF.push_back( self.aFTwo_ )
      theRef = vec3dTransVolThreeD_skinBSplineSurfaces( vh_aF ).result()
      theRef.setLabel( self.label_ )
      self.appendAnalyticFunction( theRef )
    else:
      cc = 0
      for split in self.splits_:
        logging.info( 
          "Split between %f <--> %f in direction %d" 
          % 
          (split[0], split[1], self.splitDim_) 
        )
        vh_aF = vectorHandlingConstAnalyticFunction()
        vh_aF.push_back( 
          vec3dSurfaceTwoD(
            bSplineSurface_bSplineSurfaceSplitConstructOCC(
              self.aFOne_.constPtrDtSurface(), 
              self.splitDim_, 
              split[0], 
              split[1]
            ).result()
          ).clone()
        )
        vh_aF.push_back( 
          vec3dSurfaceTwoD(
            bSplineSurface_bSplineSurfaceSplitConstructOCC(
              self.aFTwo_.constPtrDtSurface(), 
              self.splitDim_, 
              split[0], 
              split[1]
            ).result()
          ).clone()
        )
        theRef = vec3dTransVolThreeD_skinBSplineSurfaces( vh_aF ).result()
        theRef.setLabel( self.label_+"_"+str(cc) )
        self.appendAnalyticFunction( theRef )
        cc = cc + 1
    return
