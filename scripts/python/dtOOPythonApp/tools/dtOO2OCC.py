from dtOOPythonSWIG import labeledVectorHandlingAnalyticGeometry
from dtOOPythonSWIG import labeledVectorHandlingAnalyticFunction
from dtOOPythonSWIG import analyticGeometry
from dtOOPythonSWIG import map2dTo3d
from dtOOPythonSWIG import analyticSurface
from dtOOPythonSWIG import dtOCCSurface
from dtOOPythonSWIG import vectorDtPoint3
from dtOOPythonSWIG import vectorHandlingConstDtCurve
from dtOOPythonSWIG import bSplineCurve_pointConstructOCC
from dtOOPythonSWIG import bSplineSurface_skinConstructOCC
from dtOOPythonSWIG import vec3dSurfaceTwoD

from OCC.Core.STEPControl import STEPControl_Writer
from OCC.Core.BRepBuilderAPI import BRepBuilderAPI_MakeFace
from OCC.Core.Precision import precision
from OCC.Core.TopoDS import TopoDS_Shape

from typing import List
import logging

class dtOO2OCC:
  """Writes a STEP file.

  Attributes
  ----------
  None

  Examples
  --------

  >>> from dtOOPythonSWIG import dtPoint3
  >>> from dtOOPythonSWIG import bSplineCurve_pointConstructOCC
  >>> from dtOOPythonSWIG import bSplineSurface_skinConstructOCC
  >>> from dtOOPythonSWIG import analyticSurface

  Create an analyticGeometry:

  >>> aS = analyticSurface( 
  ...   bSplineSurface_skinConstructOCC(
  ...     bSplineCurve_pointConstructOCC(
  ...       dtPoint3(0,0,0), dtPoint3(1,0,0)
  ...     ).result(),
  ...     bSplineCurve_pointConstructOCC(
  ...       dtPoint3(0,1,0), dtPoint3(1,1,0)
  ...     ).result()
  ...   ).result()
  ... )

  Write the geometry to a STEP file:

  >>> dtOO2OCC.WriteSTEP( 
  ...   dtOO2OCC.TopoDS( 
  ...     [ 
  ...       aS,
  ...     ] 
  ...   ), 
  ...   "writeSTEP.stp" 
  ... )

  """
  class analyticSurface_analyticGeometry:
    """Appoximates a map2dTo3d by an analyticSurface.

    Parameters
    ----------
    m2d_: map2dTo3d
      AnalyticGeometry to be approximated.
    approx_: analyticSurface
      Approximation of the geometry.

    Examples
    --------
  
    >>> from dtOOPythonSWIG import dtPoint3
    >>> from dtOOPythonSWIG import bSplineCurve_pointConstructOCC
    >>> from dtOOPythonSWIG import bSplineSurface_skinConstructOCC
    >>> from dtOOPythonSWIG import analyticSurface
  
    Create an analyticGeometry:
  
    >>> aS = analyticSurface( 
    ...   bSplineSurface_skinConstructOCC(
    ...     bSplineCurve_pointConstructOCC(
    ...       dtPoint3(0,0,0), dtPoint3(1,0,0)
    ...     ).result(),
    ...     bSplineCurve_pointConstructOCC(
    ...       dtPoint3(0,1,0), dtPoint3(1,1,0)
    ...     ).result()
    ...   ).result()
    ... )
  
    Approximate the geometry by an analyticSurface:
  
    >>> approx = dtOO2OCC.analyticSurface_analyticGeometry(aS).approx()

    Check if type of approximation is an analyticSurface:

    >>> approx.virtualClassName()
    'analyticSurface'
    
    """
    import numpy as np

    def __init__( self, surf_aG : analyticGeometry ) -> None:
      """Constructor.

      Parameters
      ----------
      surf_aG: analyticGeometry
        AnalyticGeometry to be approximated.
      """
      self.m2d_ : map2dTo3d = map2dTo3d.MustConstDownCast( surf_aG.clone() )
      self.approx_ : analyticSurface = None
  
    def approx( 
      self, 
      uInts: List[float] = np.linspace(0.0, 1.0, 101),
      vInts: List[float] = np.linspace(0.0, 1.0,  21),
      uOrder: int = 2,
      vOrder: int = 2,
      constructFirstDim: int = 0
    ):
      """Create approximation.

      The approximation is created by constructing points in the first 
      direction. Afterwards the points are connected by a BSpline curve. All
      appximated curves are the connected by a BSpline surface. The orders for
      the curves and the surface can be adjusted by `uOrder` and `vOrder`.
      Currently, the order for the first approximation direction is fixed to
      the first parameter direction of the self.m2d_ direction. The position
      of the points that are used to create the approxiamtion can be modified
      by `uInts` and `vInts`.

      Parameters
      ----------
      uInts: List[float]
        Parameter coordinates in first direction for approximation.
      vInts: List[float]
        Parameter coordinates in second direction for approximation.
      uOrder: int
        Order of approximation for first direction.
      vOrder: int
        Order of approximation for second direction.
      constructFirstDim: int
        Switch to change the first spline direction of the approximation.

      Returns
      -------
      map2dTo3d
        Approximation of the geometry.

      Raises
      ------
      ValueError
        If constructFirstDim!=0.
      """
      if constructFirstDim == 0:
        cc = vectorHandlingConstDtCurve()
        for uu in uInts:
          pp = vectorDtPoint3() 
          for vv in vInts:
            pp.push_back( self.m2d_.getPointPercent(uu, vv) )
          cc.push_back(
            bSplineCurve_pointConstructOCC(pp, uOrder).result()
          )
        self.approx_ = analyticSurface(
          bSplineSurface_skinConstructOCC(cc, vOrder, vOrder, 0).result()
        )
      else:
        raise ValueError("constructFirstDim!=0")
  
      return self.approx_

  def TopoDS( 
    shapes : labeledVectorHandlingAnalyticGeometry 
  ) -> List[ TopoDS_Shape ]:
    """Converts a labeledVectorHandlingAnalyticGeometry to List[TopoDS_Shape].

    analyticGeometries are converted to TopoDS_Shapes. Currently, the 
    conversion is only supported for analyticSurfaces.

    Attributes
    ----------
    shapes: labeledVectorHandlingAnalyticGeometry
      Shapes to be converted.

    Returns
    -------
    List[TopoDS_Shape]
      Converted shapes.

    Raises
    ------
    ValueError                                                                  
      If shape is not an analyticSurface.
    """
    cShapes = []
    for aShape in shapes:
      logging.info("Processing %s" % ( aShape.getLabel() ) )
      if analyticSurface.Is( aShape ):
        logging.info( "  > analyticSurface" )
        occ_ref = dtOCCSurface.ConstDownCast( 
          analyticSurface.ConstDownCast( aShape ).ptrConstDtSurface()
        ).OCCRef()
        brep = BRepBuilderAPI_MakeFace()
        brep.Init(occ_ref.getOCC(), True, precision.Confusion())
        cShapes.append( brep.Shape() )
      else:
        ValueError("Unknown Type")

    return cShapes

  def TopoDSFunction(
    shapes : labeledVectorHandlingAnalyticFunction
  ) -> List[ TopoDS_Shape ]:
    """Converts a labeledVectorHandlingAnalyticGeometry to List[TopoDS_Shape].

    analyticGeometries are converted to TopoDS_Shapes. Currently, the
    conversion is only supported for analyticFunctions.

    Attributes
    ----------
    shapes: labeledVectorHandlingAnalyticGeometry
      Shapes to be converted.

    Returns
    -------
    List[TopoDS_Shape]
      Converted shapes.

    Raises
    ------
    ValueError
      If shape is not an analyticSurface.
    """
    cShapes = []
    for aShape in shapes:
      logging.info("Processing %s" % ( aShape.getLabel() ) )
      if vec3dSurfaceTwoD.Is( aShape ):
        logging.info( "  > analyticSurface" )
        occ_ref = dtOCCSurface.ConstDownCast(
          vec3dSurfaceTwoD.ConstDownCast( aShape ).constPtrDtSurface()
        ).OCCRef()
        brep = BRepBuilderAPI_MakeFace()
        brep.Init(occ_ref.getOCC(), True, precision.Confusion())
        cShapes.append( brep.Shape() )
      else:
        ValueError("Unknown Type")

    return cShapes

  def WriteSTEP( shapes: List[ TopoDS_Shape ], fname : str ) -> None:
    """Write the STEP file.

    Attributes
    ----------
    shapes: List[ TopoDS_Shape ]
      Shapes to write.
    fname: str
      File name of STEP file.

    Returns
    -------
    None
    """
    ss = STEPControl_Writer()
    for aShape in shapes:
      ss.Transfer(aShape, 0)
    ss.Write(fname) 

