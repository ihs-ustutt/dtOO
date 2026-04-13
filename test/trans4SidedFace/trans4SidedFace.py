"""
Create a four sided face with a transfinite interpolation of the interior
face.

Import `dtOO`:

>>> from dtOOPythonSWIG import *

Create a log file:

>>> logMe.initLog("trans4SidedFace.log")
'trans4SidedFace.log'

Define `staticProperties`:

>>> staticPropertiesHandler.getInstance().jInit(
...   jsonPrimitive(
...     '{'
...       '"option" : ['
...         '{"name" : "reparamOnFace_precision", "value" : "1.e-05"},'
...         '{"name" : "reparamInVolume_precision","value" : "1.e-05"},'
...         '{"name" : "invY_precision", "value" : "1.e-04"},'
...         '{"name" : "xyz_resolution", "value" : "1.e-05"},'
...         '{"name" : "XYZ_resolution", "value" : "1.e-04"},'
...         '{"name" : "uvw_resolution", "value" : "1.e-04"},'
...         '{"name" : "point_render_diameter", "value" : "0.0025"},'
...         '{"name" : "vector_render_size", "value" : "0.0030"},'
...         '{"name" : "function_render_resolution_u", "value" : "201"},'
...         '{"name" : "function_render_resolution_v", "value" : "201"},'
...         '{"name" : "function_render_resolution_w", "value" : "201"},'
...         '{"name" : "geometry_render_resolution_u", "value" : "21"},'
...         '{"name" : "geometry_render_resolution_v", "value" : "21"},'
...         '{"name" : "geometry_render_resolution_w", "value" : "21"},'
...         '{"name" : "ompNumThreads", "value" : "2"},'
...         '{"name" : "map1dTo3d_deltaPer", "value" : "0.01"},'
...         '{"name" : "map2dTo3d_deltaPer", "value" : "0.01"},'
...         '{"name" : "map3dTo3d_deltaPer", "value" : "0.01"},'
...         '{"name" : "logLevel", "value" : "99"}'
...       ']'
...     '}'
...   )
... )

Create a `dtBundle` object:

>>> container = dtBundle()

Reference to `analyticGeometry` container:

>>> aG = container.cptr_aG()

Create first curve as a B-Spline curve with order 2; the curve is labeled as 
`s3_0` and a clone is appended to the vector:

>>> aG.set(
...   analyticCurve( 
...     bSplineCurve_pointConstructOCC(
...       vectorDtPoint3()
...       << dtPoint3( +0.00, +0.00, +0.00 )
...       << dtPoint3( +0.50, -0.30, +0.00 )
...       << dtPoint3( +1.00, +0.00, +0.00 ),
...       2
...     ).result() 
...   ) << "s3_0"
... )

Create a `muParser` function as second curve:

>>> mup = vec3dMuParserOneD("1,alpha,0", "alpha")

Set minimum and maximum parametric coordinate value for `alpha`:

>>> mup.setMin(0.0); mup.setMax(1.0)

Append curve to container:

>>> aG.set( vec3dOneDInMap3dTo3d(mup, infinityMap3dTo3d()) << "s3_1" )
 
Append third B-Spline curve:

>>> aG.set( 
...   analyticCurve( 
...     bSplineCurve_pointConstructOCC(
...       vectorDtPoint3()
...       << dtPoint3( +1.00, +1.00, +0.00 )
...       << dtPoint3( +0.50, +0.90, +0.00 )
...       << dtPoint3( +0.00, +1.00, +0.00 ),
...       2
...     ).result() 
...   ) << "s3_2" 
... )

Append fourth B-Spline curve:

>>> aG.set(
...   analyticCurve(
...     bSplineCurve_poleWeightKnotMultOrderConstructOCC(
...       vectorDtPoint3()
...       << dtPoint3( +0.00, +1.00, +0.00 )
...       << dtPoint3( +0.30, +0.80, +0.00 )
...       << dtPoint3( -0.30, +0.25, +0.00 )
...       << dtPoint3( +0.00, +0.00, +0.00 ),
...       vectorReal([1.0, 4.0, 1.0, 1.0]),
...       vectorReal([0.0, 0.5, 1.0]),
...       vectorInt([3,1,3]), # [k+1, ... , k+1] 
...       2,
...       False
...     ).result()
...   ) << "s3_3"
... )

Create object of `trans4SidedFace`; the internal of the face is interpolated 
by a transfinite interpolation based on the four bounding curves:

>>> aG.set(
...   trans4SidedFace(
...     map1dTo3d.MustDownCast( aG["s3_0"] ), 
...     map1dTo3d.MustDownCast( aG["s3_1"] ), 
...     map1dTo3d.MustDownCast( aG["s3_2"] ), 
...     map1dTo3d.MustDownCast( aG["s3_3"] )
...   ) << "aS_0" 
... )
"""
