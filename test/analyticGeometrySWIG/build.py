import sys

from dtOOPythonSWIG import *

aC = analyticCurve( 
  bSplineCurve_pointConstructOCC(
    dtPoint3(0.0,0.0,0.0),
    dtPoint3(1.0,1.0,1.0)
  ).result() 
)
if analyticGeometry.inXYZTolerance(
  aC.getPointPercent(0.5),
  dtPoint3(0.5,0.5,0.5)
):
  print("-> OK")
else:
  print("-> FAIL")
  sys.exit(-1)