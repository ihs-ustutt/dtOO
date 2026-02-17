"""
Import dtOO:

>>> from dtOOPythonSWIG import *

Create an analyticCurve:

>>> aC = analyticCurve( 
...   bSplineCurve_pointConstructOCC(
...     dtPoint3(0.0,0.0,0.0),
...     dtPoint3(1.0,1.0,1.0)
...   ).result() 
... )

Create a labeledVectorHandlingAnalyticGeometry:

>>> lvhAG = labeledVectorHandlingAnalyticGeometry()

Append a clone of the object to the labeledVectorHandlingAnalyticGeometry; the 
overloaded `lshift` operator clones the original object and renames it:

>>> lvhAG.push_back( aC << "myAnalyticCurve")

Destroy object:

>>> del aC

Check that object is still in the vector; it is a clone so it has to be still 
there:

>>> lvhAG[0].getLabel()
'myAnalyticCurve'

Create a vec3dCurveOneD:

>>> v3d = vec3dCurveOneD( 
...   bSplineCurve_pointConstructOCC(
...     dtPoint3(0.0,0.0,0.0),
...     dtPoint3(1.0,1.0,1.0)
...   ).result() 
... )

Create a labeledVectorHandlingAnalyticFunction:

>>> lvhAF = labeledVectorHandlingAnalyticFunction()

Append a clone:

>>> lvhAF.push_back( v3d << "myVec3dCurveOneD")

Destroy object:

>>> del v3d

Check also for labeledVectorHandlingAnalyticFunction:

>>> lvhAF[0].getLabel()
'myVec3dCurveOneD'
"""
