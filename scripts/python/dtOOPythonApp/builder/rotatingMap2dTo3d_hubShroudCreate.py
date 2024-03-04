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

from dtOOPythonApp.tools.dtBundleTools import dtBundleBuilder

from dtOOPythonSWIG import (
  dtPoint3,
  vectorDtPoint3,
  dtVector3,
  vectorHandlingConstDtCurve,
  bSplineCurve_pointConstructOCC,
  analyticSurface,
  bSplineSurface_skinConstructOCC,
  rotatingMap2dTo3d
)

from typing import List
import logging

class rotatingMap2dTo3d_hubShroudCreate(dtBundleBuilder):
  """Create a channel by skinning hub and shroud.

  Attributes
  ----------
  label_: str
    Label.
  hubPoints_: List[dtPoint3]:
    Hubs' points.
  shroudPoints_: List[dtPoint3]:
    Shrouds' points.
  order_: int
    Splines' order for hub and shroud.
  rotVector_: dtVector3
    Channel's rotation vector.

  Examples
  --------
  >>> from dtOOPythonSWIG import (
  ...   dtPoint3, 
  ...   dtVector3
  ... )

  Create a simple channel from 2 points for hub and shroud lines. The order of
  the spline is 1 and the channel rotates around the negative z-axis:

  >>> builder = rotatingMap2dTo3d_hubShroudCreate(
  ...   "simple",
  ...   shroudPoints = [
  ...     dtPoint3(2.00, 0.00, 2.00),  
  ...     dtPoint3(2.00, 0.00, 0.00),  
  ...   ],
  ...   hubPoints = [
  ...     dtPoint3(0.50, 0.00, 2.00),  
  ...     dtPoint3(0.50, 0.00, 0.00),  
  ...   ],
  ...   order = 1,
  ...   rotVector = dtVector3(0, 0, -1)
  ... )

  Build channel:

  >>> builder.build()

  Check label of first analyticFunction:

  >>> builder.lVH_aG().labels()[0]
  'simple'
  """
  def __init__( 
    self, 
    label: str, 
    hubPoints: List[dtPoint3], 
    shroudPoints: List[dtPoint3], 
    order: int,
    rotVector: dtVector3 = dtVector3(0.0,0.0,1.0)
  ) -> None:
    """Constructor.

    Parameters
    ----------
    label: str
      Label.
    hubPoints: List[dtPoint3]:
      Hubs' points.
    shroudPoints: List[dtPoint3]:
      Shrouds' points.
    order: int
      Splines' order for hub and shroud.
    rotVector: dtVector3
      Channel's rotation vector.
 
    Returns
    -------
    None
    """
    logging.info("Initializing %s ..." % (label))
    super(rotatingMap2dTo3d_hubShroudCreate, self).__init__()
    self.label_ = label
    self.hubPoints_ = vectorDtPoint3()
    self.shroudPoints_ = vectorDtPoint3()
    for pp in hubPoints:
      self.hubPoints_.append( pp )
    for pp in shroudPoints:
      self.shroudPoints_.append( pp )
    self.order_ = order
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
    logging.info("Building %s ..." % (self.label_))
    vh = vectorHandlingConstDtCurve()
    vh.push_back( 
      bSplineCurve_pointConstructOCC(self.hubPoints_, self.order_).result() 
    )
    vh.push_back(
      bSplineCurve_pointConstructOCC(self.shroudPoints_, self.order_).result() 
    )
    
    ref = rotatingMap2dTo3d(
      self.rotVector_,
      analyticSurface(
        bSplineSurface_skinConstructOCC(vh).result()
      )
    )
    ref.setLabel(self.label_)
 
    self.appendAnalyticGeometry( ref )

    return
