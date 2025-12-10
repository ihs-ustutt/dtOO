from dtOOPythonApp.tools.dtBundleTools import dtBundleBuilder

from dtOOPythonSWIG import (
  dtPoint3,
  vectorDtPoint3,
  dtCurve,
#  dtVector3,
#  vectorHandlingConstDtCurve,
  bSplineCurve_pointConstructOCC,
  analyticCurve,
#  bSplineSurface_skinConstructOCC,
#  map1dTo3d
)

from typing import List
import logging

class draftTubeCenterline(dtBundleBuilder):
  """Creates a 2d Spline by connecting a number of points

  Attributes
  ----------
  label_: str
    Label.
  splinePoints_: List[dtPoint3]:
    suction line points.
  order_: int
    Splines' order for suction line.

  Examples
  --------
  >>> from dtOOPythonSWIG import (
  ...   dtPoint3,
  ...   dtVector3
  ... )

  Create a simple suction line from 2 points. The order of
  the spline is 1:

  >>> builder = surgeLine(
  ...   "simple",
  ...   splinePoints = [
  ...     dtPoint3(2.00, 0.00, 2.00),
  ...     dtPoint3(2.00, 0.00, 0.00),
  ...   ],
  ...   order = 1,
  ... )

  Build line:

  >>> builder.build()

  Check label of first analyticFunction:

  >>> builder.lVH_aG().labels()[0]
  'simple'

  """

  def __init__(
      self,
      label: str,
      splinePoints: List[dtPoint3],
      order: int,
    ) -> None:

    """Constructor.

    Parameters
    ----------
    label: str
      Label.
    splinePoints: List[dtPoint3]:
      splines' points.
    order: int
      Splines' order for hub and shroud.

    Returns
    -------
    None
    """

    logging.info("Initializing %s ..." % (label))
    super(draftTubeCenterline, self).__init__()
    self.label_ = label
    self.splinePoints_ = vectorDtPoint3()
    for pp in splinePoints:
      self.splinePoints_.append( pp )

    self.order_ = order

  def build(self) -> None:

    """Build line.

    Parameters
    ----------
    None

    Returns
    -------
    None
    """

    logging.info("Building %s ..." % (self.label_))

    res = bSplineCurve_pointConstructOCC(self.splinePoints_, self.order_).result()

    ref = analyticCurve(
      res
    )
    ref.setLabel(self.label_)

    self.appendAnalyticGeometry( ref )

    #store the created dtCurve

    self.spline_dtCurve_object_ = res

    return


  def get_dtCurve(self) -> dtCurve:

    return self.spline_dtCurve_object_
