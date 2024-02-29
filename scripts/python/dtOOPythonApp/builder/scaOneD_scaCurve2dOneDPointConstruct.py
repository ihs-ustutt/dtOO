from dtOOPythonApp.tools.analyticFunctionTools import scaOneDBuilder

from dtOOPythonSWIG import (
  dtPoint2,
  vectorDtPoint2,
  scaOneD,
  scaCurve2dOneD,
  bSplineCurve2d_pointConstructOCC
)

from typing import List
import logging

class scaOneD_scaCurve2dOneDPointConstruct(scaOneDBuilder):
  """Create a scaOneD function from points and order.

  Attributes
  ----------
  points_: List[dtPoint2]
    Spline's control points.
  order_: int
    Spline's order.

  Examples
  --------
  >>> from dtOOPythonSWIG import dtPoint2

  Create a curve from 3 points and order 2:

  >>> builder = scaOneD_scaCurve2dOneDPointConstruct(
  ...   [
  ...     dtPoint2(0.00, 0.00),
  ...     dtPoint2(0.50, 0.50),
  ...     dtPoint2(1.00, 1.00),  
  ...   ],
  ...   2
  ... )

  Create curve:

  >>> theFun = builder()
 
  Get function value at 0.5:

  >>> theFun(0.5)[0]
  0.5
  """
  def __init__( 
    self, 
    points: List[dtPoint2], order: int = 2
  ) -> None:
    """Constructor.

    Parameters
    ----------
    points: List[dtPoint2]
      Spline's control points.
    order: int
      Spline's order.

    Returns
    -------
    None
    """
    logging.info( "Initializing ..." )
    self.points_ = points
    self.order_ = order

  def build( self ) -> scaCurve2dOneD:
    """Build part.
  
    Parameters
    ----------
    None
  
    Returns
    -------
    scaCurve2dOneD
      Created function.
    """
    v2d = vectorDtPoint2()
    for point in self.points_:
      v2d.push_back( point )
    
    return scaCurve2dOneD( 
      bSplineCurve2d_pointConstructOCC(v2d, self.order_).result() 
    )
