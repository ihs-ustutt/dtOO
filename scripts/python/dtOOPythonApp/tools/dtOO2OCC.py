from dtOOPythonSWIG import labeledVectorHandlingAnalyticGeometry
from dtOOPythonSWIG import labeledVectorHandlingAnalyticFunction
from dtOOPythonSWIG import analyticGeometry
from dtOOPythonSWIG import map2dTo3d
from dtOOPythonSWIG import map1dTo3d
from dtOOPythonSWIG import analyticCurve
from dtOOPythonSWIG import dtOCCCurve
from dtOOPythonSWIG import analyticSurface
from dtOOPythonSWIG import dtOCCSurface
from dtOOPythonSWIG import vectorDtPoint3
from dtOOPythonSWIG import vectorHandlingConstDtCurve
from dtOOPythonSWIG import bSplineCurve_pointConstructOCC
from dtOOPythonSWIG import bSplineSurface_skinConstructOCC
from dtOOPythonSWIG import vec3dSurfaceTwoD

from OCC.Core.STEPControl import (
    STEPControl_Writer,
    STEPControl_GeometricCurveSet,
)
from OCC.Core.BRep import BRep_Tool
from OCC.Core.BRepAlgoAPI import BRepAlgoAPI_Cut
from OCC.Core.gp import gp_Pnt, gp_Trsf
from OCC.Core.BRepCheck import BRepCheck_Analyzer
from OCC.Core.BRepBuilderAPI import BRepBuilderAPI_Transform
from OCC.Core.BRepBuilderAPI import BRepBuilderAPI_MakeFace
from OCC.Core.BRepBuilderAPI import BRepBuilderAPI_MakeEdge
from OCC.Core.BRepBuilderAPI import BRepBuilderAPI_MakeWire
from OCC.Core.BRepBuilderAPI import BRepBuilderAPI_Sewing
from OCC.Core.BRepBuilderAPI import BRepBuilderAPI_MakeSolid
from OCC.Core.Precision import precision
from OCC.Core.TopoDS import TopoDS_Shell
from OCC.Core.TopoDS import TopoDS_Solid
from OCC.Core.TopoDS import TopoDS_Shape
from OCC.Core.TopoDS import TopoDS_Face
from OCC.Core.TopoDS import TopoDS_Wire

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

    class analyticCurve_analyticGeometry:
        import numpy as np

        def __init__( self, surf_aG : analyticGeometry ) -> None:

          self.m1d_ : map1dTo3d = map1dTo3d.MustConstDownCast( surf_aG.clone() )
          self.approx_ : analyticCurve = None


        def approx( 
          self, 
          uInts: List[float] = np.linspace(0.0, 1.0, 101),
          uOrder: int = 2,
          constructFirstDim: int = 0
        ):
          if constructFirstDim == 0:
            pp = vectorDtPoint3() 
            for uu in uInts:
              
              pp.push_back( self.m1d_.getPointPercent(uu) )
            
            self.approx_ = analyticCurve(
                bSplineCurve_pointConstructOCC(pp, uOrder).result()
            )
          else:
            raise ValueError("constructFirstDim!=0")

          return self.approx_

    def TopoDS( 
        shapes : labeledVectorHandlingAnalyticGeometry 
        ) -> List[ TopoDS_Shape ]:
        """Converts a labeledVectorHandlingAnalyticGeometry to List[TopoDS_Shape].

        analyticGeometries are converted to TopoDS_Shapes. Currently, the 
        conversion is supported for analyticCurves and analyticSurfaces.

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
          
          # conversion of analyticSurface objects
          if analyticSurface.Is( aShape ):
            logging.info( "  > analyticSurface" )
            occ_ref = dtOCCSurface.ConstDownCast( 
              analyticSurface.ConstDownCast( aShape ).ptrConstDtSurface()
            ).OCCRef()
            brep = BRepBuilderAPI_MakeFace()
            brep.Init(occ_ref.getOCC(), True, precision.Confusion())
            
            cShapes.append( brep.Shape() )

          # conversion of analyticCurve objects
          elif analyticCurve.Is( aShape ):
            logging.info( "  > analyticCurve" )
            occ_ref = dtOCCCurve.ConstDownCast( 
              analyticCurve.ConstDownCast( aShape ).ptrConstDtCurve()
            ).OCCRef()
            brep = BRepBuilderAPI_MakeEdge()
            brep.Init(occ_ref.getOCC())

            cShapes.append( brep.Shape() )

          else:
            ValueError("Unknown Type")

        return cShapes

    def TopoDSFunction(
        shapes : labeledVectorHandlingAnalyticFunction
        ) -> List[ TopoDS_Shape ]:
        """Converts a labeledVectorHandlingAnalyticFunction to List[TopoDS_Shape].

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
    
    @staticmethod
    def makeTopoDS_FaceAndEdges(
            face: TopoDS_Face, 
            edgesTrim: list,
            edgesHole: list = None,
        ) -> TopoDS_Wire:
        """Creates a trimmed surface from an underlying surface and edge loops.

        The surface is trimmed through the edge loop handed over by edgesTrim.
        If edgesHole is defined a hole is created in this surface.
        From the edges a wire is defined through the method createWire().
        The edge sequence in the list has to be consistent in terms of 
        connectivity and direction.
        
        Attributes
        ----------
        face: TopoDS_Face
          Underlying Surface.
        edgesTrim: list[TopoDS_Edge]
          Edges through which the surface is trimmed.
        edgesHole: list[TopoDS_Edge]
          Edges through which the hole in the surface is created.
        
        Returns
        -------
        TopoDS_Face
          Trimmed surface.
        """
        
        # Get the geometric surface underlying the face
        surface = BRep_Tool.Surface(face)
        
        # Create the wire which trimms the surface
        wire_outer = dtOO2OCC.createWire(edgesTrim) 
    
        # Build a new face on that surface, bounded by the wire
        brepF = BRepBuilderAPI_MakeFace(
            surface,
            wire_outer,
            False
        )
        
        # Subtract the hole if it exists
        if edgesHole != None:
            wire_inner = dtOO2OCC.createWire(edgesHole)
            brepF.Add(wire_inner)

        return brepF.Face()
    
    @staticmethod
    def createWire(edges: list) -> TopoDS_Wire:
        """Creates a wire from a list of edges.

        The edge sequence in the list has to be consistent in terms of 
        connectivity and direction.
        
        Attributes
        ----------
        edgesTrim: list[TopoDS_Edge]
          Edges which form the wire.
        
        Returns
        -------
        TopoDS_Wire
          Wire.
        """
        
        # Create the wire builder
        brepW = BRepBuilderAPI_MakeWire()
        
        # Loop over edge list, add them to the wire
        for e in edges:
            brepW.Add(e)
        
        return brepW.Wire()
    
    def createShell(faces: list) -> TopoDS_Shell:
        """ Create shells of connected faces through sewing
        
        Attributes
        ----------
        faces: list[TopoDS_Shape]
          Faces
        
        Returns
        -------
        TopoDS_Shell
          Shell resulting from the faces.
        """
        sewing = BRepBuilderAPI_Sewing()

        for face in faces:
            sewing.Add(face)

        sewing.Perform()

        return sewing.SewedShape()
    
    def createSolid(shell: TopoDS_Shell) -> TopoDS_Solid:
        """ Create solid from a shell which encloses a volume
        
        Attributes
        ----------
        shell: TopoDS_Shell
          Shell.
        
        Returns
        -------
        TopoDS_Solid
          Solid body enclosed by the shell.
        """

        solid = BRepBuilderAPI_MakeSolid()

        solid.Add(shell)

        return solid.Solid()

    def scale(geom: TopoDS_Shape, factor: float) -> TopoDS_Shape:
        """ Scale a geometry object by a factor
        
        Attributes
        ----------
        geom: TopoDS_Shape
          Geometry.
        factor: float
          Scaling factor
        
        Returns
        -------
        TopoDS_Shape
          Scaled geometry.
        """
        
        scale = gp_Trsf()
        scale.SetScale(gp_Pnt(0.0, 0.0, 0.0), factor)

        transform = BRepBuilderAPI_Transform(geom, scale, True)

        return transform.Shape()

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
        for i, aShape in enumerate(shapes):
          status = ss.Transfer(aShape, 0)
          #status = ss.Transfer(
          #  aShape,
          #  STEPControl_GeometricCurveSet
          #)
          
          #print(
          #  f"Shape {i}: "
          #  f"Null={aShape.IsNull()}, "
          #  f"TransferStatus={status}"
          #)

        status = ss.Write(fname)

        #print(f"WriteStatus={status}")

