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

"""
This example creates a curve in OpenCASCADE via
`pythonOCC <https://github.com/tpaviot/pythonocc-core>`_. The curve is then
transferred to `dtOO`.

Import `dtOO`:

>>> import dtOOPythonSWIG as dtOO

Import modules of `pythonOCC`:

>>> from OCC.Core.TColgp import TColgp_Array1OfPnt
>>> from OCC.Core.TColStd import (
...   TColStd_Array1OfReal, TColStd_Array1OfInteger
... )
>>> from OCC.Core.gp import gp_Pnt
>>> from OCC.Core.Geom import Geom_BSplineCurve
>>> from OCC.Core.Geom import Geom_SurfaceOfRevolution

Define number of points, order of the curve, and the length of the `knots` and 
`mults` vector:

>>> nP = 3
>>> order = 2
>>> lKnotsAndMults = nP - (order + 1)

Create `pole` vector that contains the control points, the `knots` vector, and
the `mults` vector. All vectors are objects of OpenCASCADE:

>>> poles = TColgp_Array1OfPnt(1, nP)
>>> knots = TColStd_Array1OfReal(1, lKnotsAndMults + 2)
>>> mults = TColStd_Array1OfInteger(1, lKnotsAndMults + 2)

Set the control points to the vector:

>>> poles.SetValue(1, gp_Pnt(0.0, 0.0, 0.0))
>>> poles.SetValue(2, gp_Pnt(0.0, 0.5, 0.5))
>>> poles.SetValue(3, gp_Pnt(0.0, 1.0, 0.0))

Set ascending values to the `knots` vector:

>>> for ii in range(knots.Length()):
...  knots.SetValue(ii+1, ii)

Set multiplicity of the knots to the `knots` vector; first and last knot have
the multiplicity `order+1`:

>>> mults.Init(1)
>>> mults.SetValue(1, order + 1)
>>> mults.SetValue(lKnotsAndMults + 2, order + 1)

Create the B-Spline curve by calling an OpenCASCADE constructor via 
`pythonOCC`:

>>> curve_occ = Geom_BSplineCurve(poles, knots, mults, order)

The object `curve_occ` is a subclass of `Geom_Curve` from OpenCASCADE and 
can be used directly as the underlying object in an `analyticCurve`. The 
class `dtOCCCurveBase` wraps the binding to OpenCASCADE and stores the 
pointer of type `Geom_Curve`. With the builder `geomCurve_baseConstructOCC`
an object of type `dtCurve` is created:

>>> curve_dtCurve = dtOO.geomCurve_baseConstructOCC( 
...   dtOO.dtOCCCurveBase( curve_occ ) 
... ).result()

Finally, the `dtCurve` object is then used as the underlying curve of an
`analyticCurve` object:

>>> curve_analyticGeometry = dtOO.analyticCurve( curve_dtCurve )

This object can be used to perform operations in `dtOO`. E.g. the coordinates
of a point can be extracted with

>>> p3 = curve_analyticGeometry.getPointPercent(0.5)

and printed:

>>> print( "%f %f %f" % ( p3.x(), p3.y(), p3.z()) )
0.000000 0.500000 0.250000

The complete procedure can also be gone in reverse order to extract the 
underlying `OpenCASCADE` object of an `dtOO` object. As an example a surface
of revolution is constructed with the already created `dtOO` curve object
`curve_dtCurve,`:

>>> surface_dtSurface = dtOO.surfaceOfRevolution_curveRotateConstructOCC(
...   curve_dtCurve, dtOO.dtPoint3(0,0,0), dtOO.dtVector3(1,1,1)
... ).result()

Then, the created surface is casted to the `OpenCASCADE` wrapper in `dtOO`:

>>> surface_dtOCCSurface = dtOO.dtOCCSurface.DownCast( surface_dtSurface )

The wrapper stores a pointer to the `OpenCASCADE` object of type 
`Geom_Surface`:

>>> surface_occ = surface_dtOCCSurface.OCCRef().getOCC()

In the case at hand, the surface has the type `Geom_SurfaceOfRevolution`; 
therefore, the check should be true:

>>> surface_occ.IsInstance( "Geom_SurfaceOfRevolution" )
True

The type can then also be casted:

>>> surface_occSurfaceOfRevoltuion = Geom_SurfaceOfRevolution.DownCast( surface_occ )
"""
