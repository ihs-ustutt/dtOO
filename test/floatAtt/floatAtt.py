"""
Import

>>> from dtOOPythonSWIG import *

Create ``staticPropertiesHandler`` and initialize:

>>> staticPropertiesHandler.getInstance().jInit(
...   jsonPrimitive(
...     '{'                                                                                                                                                                              
...       '"option" : ['                                                        
...         '{"name" : "reparamOnFace_precision", "value" : "1.e-06"},'         
...         '{"name" : "reparamInVolume_precision","value" : "1.e-06"},'        
...         '{"name" : "invY_precision", "value" : "1.e-04"},'                  
...         '{"name" : "xyz_resolution", "value" : "1.e-08"},'                  
...         '{"name" : "XYZ_resolution", "value" : "1.e-07"},'                  
...         '{"name" : "uvw_resolution", "value" : "1.e-04"},'                  
...         '{"name" : "logLevel", "value" : "99"},'
...         '{"name" : "map2dTo3d_deltaPer", "value" : "1.e-8"}'
...       ']'                                                                   
...     '}' 
...   )
... )

Initialize a log file:

>>> logMe.initLog("floatAtt.log")
'floatAtt.log'

Create the function 

.. math::

    ff: (u,v) \\rightarrow (u,v,u^2+v^2)

as a ``vec3dMuParserTwoD``-object:

>>> ff = vec3dMuParserTwoD("u,v,u*u+v*v","u","v")

Set the ranges for :math:`u` and :math:`v`:

>>> for i in [0,1,]:
...   ff.setMin(i,-100.0)
...   ff.setMax(i,+100.0)

Create the infinite space :math:`im` as parameter space for the function :math:`ff`:

>>> im = infinityMap3dTo3d()

Define an ``analyticGeometry`` as a composition of :math:`ff` and :math:`im`:

>>> mm = vec3dTwoDInMap3dTo3d(ff,im)

Define the point :math:`p0` that should be reparameterized:

>>> p0 = dtPoint3(0,0,0)

Define a ``pointGeometryDist`` object that provides the connection between the
``analyticGeometry`` :math:`mm` and the point :math:`p0`; ``pointGeometryDist``
is a strategy for ``floatAtt`:

>>> att = pointGeometryDist(p0, mm)

Minimize the distance between geometry and point using a gradient-based 
minimization technique; the attributes of the minimizer are the strategy, the
initial guess, the step size, the desired precision, and the maximum number of
iterations:

>>> gradMin = gslGradMinFloatAttr(
...   att, 
...   [0.01,.99], 
...   [0.001,0.001], 
...   1.e-8,
...   100
... )

Convergence check:

>>> gradMin.perform()
True

Minimize the distance using a gradient-free minimization technique:

>>> min = gslMinFloatAttr(
...   att, 
...   [0.01,.99], 
...   [0.001,0.001], 
...   1.e-8,
...   100
... )

Convergence check:

>>> min.perform()
True
"""
