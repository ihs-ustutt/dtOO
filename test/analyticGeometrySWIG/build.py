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
