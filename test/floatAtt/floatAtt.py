"""
>>> from dtOOPythonSWIG import *

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

>>> logMe.initLog("floatAtt.log")
'floatAtt.log'

>>> ff = vec3dMuParserTwoD("u,v,u*u+v*v","u","v")

>>> for i in [0,1,]:
...   ff.setMin(i,-100.0)
...   ff.setMax(i,+100.0)

>>> im = infinityMap3dTo3d()
>>> mm = vec3dTwoDInMap3dTo3d(ff,im)
>>> p0 = dtPoint3(0,0,0)
>>> att = pointGeometryDist(p0, mm)

>>> gradMin = gslGradMinFloatAttr(
...   att, 
...   [0.01,.99], 
...   [0.001,0.001], 
...   1.e-8,
...   100
... )

>>> gradMin.perform()
True

>>> min = gslMinFloatAttr(
...   att, 
...   [0.01,.99], 
...   [0.001,0.001], 
...   1.e-8,
...   100
... )

>>> min.perform()
True
"""
