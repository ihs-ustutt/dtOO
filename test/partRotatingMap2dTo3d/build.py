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


def angleBetween(p1, p2):
  def cart2pol(x, y):
    rho = np.sqrt(x**2 + y**2)
    phi = np.arctan2(y, x)
    return(rho, phi)
 
  p1rp =  cart2pol(p1.x(), p1.y())
  p2rp =  cart2pol(p2.x(), p2.y())

  return np.abs(p2rp[1] - p1rp[1])

bss = bSplineSurface_skinConstructOCC(
  bSplineCurve_pointConstructOCC(
    dtPoint3(0,1,0), 
    dtPoint3(0,1,1)
  ).result(), 
  bSplineCurve_pointConstructOCC(
    dtPoint3(0,2,0), 
    dtPoint3(0,2,1)
  ).result() 
).result()

prm = partRotatingMap2dTo3d(
  dtVector3(0,0,1), 
  analyticSurface(bss), 
  -0.125,
  0.125 
)
sprm = prm.segmentConstVPercent(0.50)
print(
  "Angle = %f ( should be 90 )"
  %
  (
    angleBetween(
      sprm.getPointPercent(0.00,0.00),
      sprm.getPointPercent(1.00,0.00)
    )/np.pi*180.
  )
)

if analyticGeometry.inXYZTolerance(
     np.abs(
       angleBetween(
         sprm.getPointPercent(0.00,0.00),
         sprm.getPointPercent(1.00,0.00)
       )/np.pi*180.
       -
       90.
     )
   ):
  print("-> OK")
else:
  print("-> FAIL")
  sys.exit(-1)
