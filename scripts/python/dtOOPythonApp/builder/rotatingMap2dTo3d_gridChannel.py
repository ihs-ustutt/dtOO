from dtOOPythonApp.tools.dtBundleTools import dtBundleBuilder

from dtOOPythonSWIG import (
  analyticGeometry,
  map2dTo3d,
  partRotatingMap2dTo3d,
  dtVector3
)

import logging

class rotatingMap2dTo3d_gridChannel(dtBundleBuilder):
  """Create a grid channel.

  The grid channel is created by rotating the channelSide in the range:
  
    $[ -0.5*2*pi/numberOfSections, 0.5*2*pi/numberOfSections ]$

  The grid channel is then represented as a rotatingMap2dTo3d within the 
  predefined angle.

  Attributes
  ----------
  label_: str
    Label.
  channelSide_: map2dTo3d
    Channel side of the grid channel.
  numberOfSections_: int
    Number of periodic sections.
  rotVector_: dtVector3
    Rotation vector.

  Examples
  --------

  >>> from dtOOPythonSWIG import dtPoint3
  >>> from dtOOPythonSWIG import bSplineCurve_pointConstructOCC
  >>> from dtOOPythonSWIG import bSplineSurface_skinConstructOCC
  >>> from dtOOPythonSWIG import analyticSurface
  >>> import numpy as np

  Create an analyticSurface as channel side:

  >>> channelSide = analyticSurface( 
  ...   bSplineSurface_skinConstructOCC(
  ...     bSplineCurve_pointConstructOCC(
  ...       dtPoint3(1,0,0), dtPoint3(2,0,0)
  ...     ).result(),
  ...     bSplineCurve_pointConstructOCC(
  ...       dtPoint3(1,0,1), dtPoint3(2,0,1)
  ...     ).result()
  ...   ).result()
  ... )

  Create a simple channel:

  >>> builder = rotatingMap2dTo3d_gridChannel(
  ...   "simple", channelSide, 8
  ... )

  Build channel:

  >>> builder.build()
  
  Check label of first analyticGeometry:

  >>> builder.lVH_aG().labels()[0]
  'simple'
  """

  def __init__( 
    self, 
    label: str, 
    channelSide: analyticGeometry,
    numberOfSections: int,
    rotVector: dtVector3 = dtVector3(0,0,1)
  ) -> None:
    """Constructor.
  
    Parameters
    ----------
    label: str
      Label.
    channelSide: map2dTo3d
      Channel side of the grid channel.
    numberOfSections: int
      Number of periodic sections.
    rotVector: dtVector3
      Rotation vector.
 
    Returns
    -------
    None
  
    """
    logging.info( "Initializing %s ..." % (label) )
    super(rotatingMap2dTo3d_gridChannel, self).__init__()
    self.label_ = label
    self.channelSide_ = map2dTo3d.MustDownCast( channelSide )
    self.numberOfSections_ = numberOfSections
    self.rotVector_ = rotVector

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

    deltaPer = 0.5/self.numberOfSections_ 
    theRef = partRotatingMap2dTo3d(
      self.rotVector_, self.channelSide_,
      -deltaPer, +deltaPer
    )
    theRef.setLabel(self.label_)
    self.appendAnalyticGeometry( theRef )

    return
