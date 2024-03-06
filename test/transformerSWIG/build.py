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

import sys

from dtOOPythonSWIG import *
import numpy as np

#
# rPhiZ
#
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

#
# rotate
#
xyz = dtPoint3(1.0,0.0,0.0)
xyz_rot = dtPoint3(np.sqrt(1./2.),np.sqrt(1./2.),0.0)
angle = 45./180.*np.pi
print("rotate.applyDtPoint3")
if analyticGeometry.inXYZTolerance(
    rotate(
      jsonPrimitive()\
        .appendDtPoint3("_origin", dtPoint3(0.0,0.0,0.0))\
        .appendDtVector3("_rotVector", dtVector3(0.0,0.0,1.0))\
        .appendReal("_angle", angle)
    ).applyDtPoint3( xyz ),
    xyz_rot ):
  print("-> OK")
else:
  print("-> FAIL")
  sys.exit(-1)

print("rotate.retractDtPoint3")
if analyticGeometry.inXYZTolerance(
    rotate(
      jsonPrimitive()\
        .appendDtPoint3("_origin", dtPoint3(0.0,0.0,0.0))\
        .appendDtVector3("_rotVector", dtVector3(0.0,0.0,1.0))\
        .appendReal("_angle", -angle)
    ).retractDtPoint3( xyz_rot ),
    xyz ):
  print("-> OK")
else:
  print("-> FAIL")
  sys.exit(-1)
