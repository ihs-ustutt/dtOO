import sys

from dtOOPythonSWIG import *
import numpy as np

rphiz = dtPoint3(np.sqrt(2),45./180.*np.pi,0.0)
xyz = dtPoint3(1.0,1.0,0.0)

print("xYz_rPhiZ.applyDtPoint3")
if analyticGeometry.inXYZTolerance(
    xYz_rPhiZ(
      jsonPrimitive()\
        .appendDtPoint3("_origin", dtPoint3(0.0,0.0,0.0))\
        .appendDtVector3("_rotAxis", dtVector3(0.0,0.0,1.0))\
        .appendDtVector3("_refAxis", dtVector3(1.0,0.0,0.0))
    ).applyDtPoint3( rphiz ),
    xyz ):
  print("-> OK")
else:
  print("-> FAIL")
  sys.exit(-1)

print("xYz_rPhiZ.retractDtPoint3")
if analyticGeometry.inXYZTolerance(
    xYz_rPhiZ(
      jsonPrimitive()\
        .appendDtPoint3("_origin", dtPoint3(0.0,0.0,0.0))\
        .appendDtVector3("_rotAxis", dtVector3(0.0,0.0,1.0))\
        .appendDtVector3("_refAxis", dtVector3(1.0,0.0,0.0))
    ).retractDtPoint3( xyz ),
    rphiz ):
  print("-> OK")
else:
  print("-> FAIL")
  sys.exit(-1)